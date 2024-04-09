import sys
sys.path.append("/data1/wzheng/projects/caffe-quant-master-16bit-floor/python/")
import numpy as np
from caffe.proto import caffe_pb2
import copy
from google.protobuf import text_format

multi_add_coef = 2
activation_complexity = 2
non_linear_complexity = 6
def infer_shape_recursive(net_params, shape_dict, layer_dict, blob_dict, complexity_dict, model_size_dict, current_layer_name):
    current_layer = layer_dict[current_layer_name]
    current_layer_type = current_layer.type
    current_bottom = current_layer.bottom
    current_top = current_layer.top

    for bottom_name in current_bottom:
        for bottom_layer_name in blob_dict[bottom_name]:
            if ((current_top[0] == current_bottom[0]) or (bottom_layer_name in shape_dict.keys() and bottom_layer_name in complexity_dict.keys() and bottom_layer_name in model_size_dict.keys())):
                pass
            else:
                infer_shape_recursive(net_params, shape_dict, layer_dict, blob_dict, complexity_dict, model_size_dict, bottom_layer_name)

    if current_layer_type == "Input":
        input_shape = np.array([0, 0, 0])
        input_shape[0] = current_layer.input_param.shape._values[0].dim._values[1]  # C
        input_shape[1] = current_layer.input_param.shape._values[0].dim._values[2]  # H
        input_shape[2] = current_layer.input_param.shape._values[0].dim._values[3]  # W
        assert(len(current_top) == 1)
        shape_dict[current_top[0]] = input_shape
        complexity_dict[current_layer_name] = 0
        model_size_dict[current_layer_name] = 0
    elif current_layer_type == "Buffer":
        assert (len(current_bottom) == 1)
        bottom_shape = shape_dict[current_bottom[0]]
        current_shape = copy.deepcopy(bottom_shape)
        assert (len(current_top) == 1)
        shape_dict[current_top[0]] = current_shape
        complexity_dict[current_layer_name] = 0
        model_size_dict[current_layer_name] = 0
    elif current_layer_type == "Slice":
        bottom_shape = shape_dict[current_bottom[0]]
        assert (len(current_bottom) == 1)
        assert (current_layer.slice_param.axis==1) #only support slice channel
        assert (bottom_shape[0] % len(current_top) == 0)
        current_shape = np.array([0, 0, 0])
        current_shape[0] = bottom_shape[0]/len(current_top)
        current_shape[1] = bottom_shape[1]
        current_shape[2] = bottom_shape[2]
        for top_blob_name in current_top:
            shape_dict[top_blob_name] = current_shape
        complexity_dict[current_layer_name] = 0
        model_size_dict[current_layer_name] = 0
    elif current_layer_type =="Convolution" or current_layer_type =="ConvolutionRistretto":
        assert (len(current_bottom) == 1)
        bottom_shape = shape_dict[current_bottom[0]]
        kernel_size = current_layer.convolution_param.kernel_size[0]

        if len(current_layer.convolution_param.dilation) == 0:
            dilation = 1
        else:
            dilation = current_layer.convolution_param.dilation[0]

        if len(current_layer.convolution_param.pad._values) == 0:
            pad = 0
        else:
            pad = current_layer.convolution_param.pad[0]

        if len(current_layer.convolution_param.stride._values) == 0:
            stride = 1
        else:
            stride = current_layer.convolution_param.stride[0]

        group = current_layer.convolution_param.group
        output_channel = current_layer.convolution_param.num_output

        current_shape = np.array([0,0,0])
        current_shape[0] = output_channel

        current_shape[1] = (bottom_shape[1] + 2 * pad - (kernel_size*dilation-(dilation-1))) / stride + 1
        current_shape[2] = (bottom_shape[2] + 2 * pad - (kernel_size*dilation-(dilation-1))) / stride + 1

        assert(len(current_top)==1)
        shape_dict[current_top[0]] = current_shape
        complexity_dict[current_layer_name] = kernel_size*kernel_size*bottom_shape[0]*current_shape[0]*current_shape[1]*current_shape[2]*multi_add_coef/group
        model_size_dict[current_layer_name] = kernel_size*kernel_size*bottom_shape[0]*current_shape[0]/group
    elif current_layer_type =="Deconvolution" or current_layer_type =="DeconvolutionRistretto":
        assert (len(current_bottom) == 1)
        bottom_shape = shape_dict[current_bottom[0]]
        kernel_size = current_layer.convolution_param.kernel_size._values[0]
        if len(current_layer.convolution_param.pad._values) == 0:
            pad = 0
        else:
            pad = current_layer.convolution_param.pad._values[0]

        if len(current_layer.convolution_param.stride._values) == 0:
            stride = 1
        else:
            stride = current_layer.convolution_param.stride._values[0]

        group = current_layer.convolution_param.group
        output_channel = current_layer.convolution_param.num_output

        current_shape = np.array([0,0,0])
        current_shape[0] = output_channel
        current_shape[1] = (bottom_shape[1] - 1) * stride + kernel_size - 2 * pad
        current_shape[2] = (bottom_shape[2] - 1) * stride + kernel_size - 2 * pad
        assert(len(current_top)==1)
        shape_dict[current_top[0]] = current_shape
        complexity_dict[current_layer_name] = kernel_size*kernel_size*current_shape[0]*bottom_shape[0]*bottom_shape[1]*bottom_shape[2]*multi_add_coef/group
        model_size_dict[current_layer_name] = kernel_size*kernel_size*bottom_shape[0]*current_shape[0]/group
    elif current_layer_type == "Pooling":
        assert (len(current_bottom) == 1)
        bottom_shape = shape_dict[current_bottom[0]]
        kernel_size = current_layer.pooling_param.kernel_size
        pad = current_layer.pooling_param.pad
        stride = current_layer.pooling_param.stride
        global_pooling = current_layer.pooling_param.global_pooling
        current_shape = np.array([0,0,0])
        if global_pooling:
            current_shape[0] = bottom_shape[0]
            current_shape[1] = 1
            current_shape[2] = 1
        else:
            current_shape[0] = bottom_shape[0]
            current_shape[1] = np.ceil((bottom_shape[1] + 2 * pad - kernel_size) / np.float(stride)) + 1
            current_shape[2] = np.ceil((bottom_shape[2] + 2 * pad - kernel_size) / np.float(stride)) + 1
            if pad > 0 and (current_shape[1]-1)*np.float(stride) >= bottom_shape[1] + pad:
                current_shape[1] -= 1
            if pad > 0 and (current_shape[2] - 1) * np.float(stride) >= bottom_shape[2] + pad:
                current_shape[2] -= 1
        assert (len(current_top) == 1)
        shape_dict[current_top[0]] = current_shape
        complexity_dict[current_layer_name] = kernel_size*kernel_size*current_shape[0]*current_shape[1]*current_shape[2]
        model_size_dict[current_layer_name] = 0
    elif current_layer_type == "InnerProduct" or current_layer_type == "InnerProductRistretto":
        assert (len(current_bottom) == 1)
        output_num = current_layer.inner_product_param.num_output
        current_shape = np.array([0, 0, 0])
        current_shape[0] = output_num
        current_shape[1] = 1
        current_shape[2] = 1
        bottom_shape = shape_dict[current_bottom[0]]
        assert (len(current_top) == 1)
        shape_dict[current_top[0]] = current_shape
        complexity_dict[current_layer_name] = bottom_shape[0]*bottom_shape[1]*bottom_shape[2]*current_shape[0]*current_shape[1]*current_shape[2]*multi_add_coef
        model_size_dict[current_layer_name] = bottom_shape[0]*bottom_shape[1]*bottom_shape[2]*current_shape[0]*current_shape[1]*current_shape[2]
    elif current_layer_type == "Flatten":
        assert (len(current_bottom) == 1)
        bottom_shape = shape_dict[current_bottom[0]]
        current_shape = np.array([0, 0, 0])
        current_shape[0] = bottom_shape[0]*bottom_shape[1]*bottom_shape[2]
        current_shape[1] = 1
        current_shape[2] = 1
        assert (len(current_top) == 1)
        shape_dict[current_top[0]] = current_shape
        complexity_dict[current_layer_name] = 0
        model_size_dict[current_layer_name] = 0
    elif current_layer_type == "PixelShuffle":
        upscale_factor = current_layer.pixel_shuffle_param.upscale_factor
        assert (len(current_bottom) == 1)
        bottom_shape = shape_dict[current_bottom[0]]
        current_shape = np.array([0, 0, 0])
        current_shape[0] = bottom_shape[0]/upscale_factor/upscale_factor
        current_shape[1] = bottom_shape[1]*upscale_factor
        current_shape[2] = bottom_shape[2]*upscale_factor
        assert (len(current_top) == 1)
        shape_dict[current_top[0]] = current_shape
        complexity_dict[current_layer_name] = 0
        model_size_dict[current_layer_name] = 0
    elif current_layer_type == "Eltwise" or current_layer_type == "EltwiseRistretto":
        assert (len(current_bottom) > 1)
        bottom_shape = shape_dict[current_bottom[0]]
        current_shape = copy.deepcopy(bottom_shape)
        assert (len(current_top) == 1)
        shape_dict[current_top[0]] = current_shape
        complexity_dict[current_layer_name] = current_shape[0]*current_shape[1]*current_shape[2]*(len(current_bottom)-1)
        model_size_dict[current_layer_name] = 0
    elif current_layer_type == "Concat":
        assert (len(current_bottom) > 1)
        concat_axis = current_layer.concat_param.axis
        assert(concat_axis<3)
        current_shape = copy.deepcopy(shape_dict[current_bottom[0]])

        for i in range(1, len(current_bottom)):
            for j in range(0,3):
                if j == concat_axis-1:
                    continue
                #print current_shape[j], shape_dict[current_bottom[i]][j]
                # if(current_shape[j] != shape_dict[current_bottom[i]][j]):
                #     print("debug")
                assert (current_shape[j] == shape_dict[current_bottom[i]][j])
            current_shape[concat_axis-1] += shape_dict[current_bottom[i]][concat_axis-1]
        assert (len(current_top) == 1)
        shape_dict[current_top[0]] = current_shape
        complexity_dict[current_layer_name] = 0
        model_size_dict[current_layer_name] = 0
    elif current_layer_type == "Softmax":
        assert (len(current_bottom) == 1)
        current_shape = copy.deepcopy(shape_dict[current_bottom[0]])
        assert (len(current_top) == 1)
        shape_dict[current_top[0]] = current_shape
        complexity_dict[current_layer_name] = current_shape[0]*current_shape[1]*current_shape[2]*non_linear_complexity
        model_size_dict[current_layer_name] = 0
    elif current_layer_type == "Scale":
        bottom_shape = shape_dict[current_bottom[0]]
        current_shape = copy.deepcopy(bottom_shape)
        assert (len(current_top) == 1)
        shape_dict[current_top[0]] = current_shape
        complexity_dict[current_layer_name] = current_shape[0] * current_shape[1] * current_shape[2] * activation_complexity * 2  # a little bit more complex then ReLU
        channel_shared = current_layer.prelu_param.channel_shared
        if channel_shared:
            model_size_dict[current_layer_name] = 1
        else:
            model_size_dict[current_layer_name] = current_shape[0]
    elif current_layer_type == "ShuffleChannel":
        assert (len(current_bottom) == 1)
        bottom_shape = shape_dict[current_bottom[0]]
        current_shape = copy.deepcopy(bottom_shape)
        assert (len(current_top) == 1)
        shape_dict[current_top[0]] = current_shape
        complexity_dict[current_layer_name] = current_shape[0] * current_shape[1] * current_shape[2]  # consider the worst implementation which will copy all feature map into a new memory space
        model_size_dict[current_layer_name] = 0
    elif current_layer_type == "ReLU":
        assert (len(current_bottom) == 1)
        bottom_shape = shape_dict[current_bottom[0]]
        current_shape = copy.deepcopy(bottom_shape)
        assert (len(current_top) == 1)
        shape_dict[current_top[0]] = current_shape
        complexity_dict[current_layer_name] = current_shape[0]*current_shape[1]*current_shape[2]*activation_complexity
        model_size_dict[current_layer_name] = 0
    elif current_layer_type == "BatchNorm":
        assert (len(current_bottom) == 1)
        bottom_shape = shape_dict[current_bottom[0]]
        current_shape = copy.deepcopy(bottom_shape)
        assert (len(current_top) == 1)
        shape_dict[current_top[0]] = current_shape
        complexity_dict[current_layer_name] = 0
        model_size_dict[current_layer_name] = 0
    elif current_layer_type == "Dropout":
        assert (len(current_bottom) == 1)
        bottom_shape = shape_dict[current_bottom[0]]
        current_shape = copy.deepcopy(bottom_shape)
        assert (len(current_top) == 1)
        shape_dict[current_top[0]] = current_shape
        complexity_dict[current_layer_name] = 0
        model_size_dict[current_layer_name] = 0
    else:
       print("layer type " + current_layer_type + " layer name " + current_layer_name + " is not impelemented")

#if
def calculate_in_place_layers(shape_dict, layer_dict, complexity_dict, model_size_dict):

    for current_layer_name in layer_dict.keys():
        current_layer = layer_dict[current_layer_name]
        current_layer_type = current_layer.type
        current_bottom = current_layer.bottom
        current_top = current_layer.top

        if current_layer_type == "ReLU" and current_top[0] not in shape_dict.keys():
            assert (len(current_bottom) == 1)
            bottom_shape = shape_dict[current_bottom[0]]
            current_shape = copy.deepcopy(bottom_shape)
            assert (len(current_top) == 1)
            shape_dict[current_top[0]] = current_shape
            complexity_dict[current_layer_name] = current_shape[0]*current_shape[1]*current_shape[2]*activation_complexity
            model_size_dict[current_layer_name] = 0
        elif current_layer_type == "Dropout" and current_top[0] not in shape_dict.keys():
            assert (len(current_bottom) == 1)
            bottom_shape = shape_dict[current_bottom[0]]
            current_shape = copy.deepcopy(bottom_shape)
            assert (len(current_top) == 1)
            shape_dict[current_top[0]] = current_shape
            complexity_dict[current_layer_name] = 0
            model_size_dict[current_layer_name] = 0
        else:
            pass

def infer_complexity(net_prototxt,last_tops):
    shape_dict = dict()
    layer_dict = dict()
    blob_dict = dict()
    complexity_dict = dict()
    model_size_dict = dict()

    net_param = caffe_pb2.NetParameter()
    text_format.Merge(open(net_prototxt).read(), net_param)
    layer_info = net_param.layer._values

    for item in layer_info:
        layer_dict[item.name] = item
        for top_blob_name in item.top:
            if top_blob_name in blob_dict.keys():
                blob_dict[top_blob_name].append(item.name)
            else:
                blob_dict[top_blob_name] = []
                blob_dict[top_blob_name].append(item.name)

    for last_top in last_tops:
        infer_shape_recursive(net_param, shape_dict, layer_dict, blob_dict, complexity_dict, model_size_dict, last_top)
    calculate_in_place_layers(shape_dict, layer_dict, complexity_dict, model_size_dict)

    total_complexity = 0
    for item in complexity_dict.keys():
        total_complexity += complexity_dict[item]
    total_model_size = 0
    for item in model_size_dict.keys():
        total_model_size += model_size_dict[item]

    print(total_complexity)
    print(total_model_size)
    print(complexity_dict)
    print(shape_dict)
    return total_complexity, total_model_size, complexity_dict, model_size_dict, shape_dict






if __name__ == "__main__":
    #infer_complexity("/data/wzheng/projects/caffe-quant-master/examples/car_ground_cls_improved/classification_sm_res_deploy.prototxt", ["prob"])
    infer_complexity("./models/deploy_hcnet_nobn_nogroup.prototxt",["fc"])
    #infer_complexity("/data/wzheng/projects/caffe-quant-master/examples/car_type_proj/minieye_classification_delpoy.prototxt", ["prob"])
    #infer_complexity("/data/wzheng/projects/caffe-quant-master/examples/car_regression_proj/regression_baseline_deploy.prototxt")
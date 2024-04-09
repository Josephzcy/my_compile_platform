import sys
sys.path.append('/data/wzheng/projects/caffe-quant-master/python')

import os
import copy
import cv2
import caffe
from caffe.proto import caffe_pb2
import copy
from google.protobuf import text_format

import numpy as np
import infer_complexity

in_place_layers = ["ReLU"]

#caffe.set_mode_cpu()
caffe.set_mode_gpu()
caffe.set_device(0)

def modified_round(data): #numpy round is different from C round, np.round(1.5)=2, np.round(2.5)=2, round is to select the nearest even number
    data = np.array(data)
    ori_shape = data.shape
    data = data.reshape((data.size,))
    data_round = np.floor(data)
    diff = data - data_round
    for idx in range(0, data.size):
        if diff[idx]<0.5:
            None
        if diff[idx]>0.5:
            data_round[idx] = data_round[idx] + 1
        if diff[idx] == 0.5 and data[idx] > 0:
            data_round[idx] = data_round[idx] + 1
        if diff[idx] == 0.5 and data[idx] < 0:
            None

    data_round = np.int32(data_round)
    data_round = data_round.reshape(ori_shape)
    # data = np.array(data)
    # data_round = np.floor(data)
    return data_round

def float_to_fix(float_input,bit_width, bit_fl, unsigned_or_not):
    if unsigned_or_not == False:
        max_val = pow(2,bit_width-1)-1
        min_val = -pow(2,bit_width-1)
        #right_shift = pow(2, -bit_fl)
        left_shift = pow(2, bit_fl)
        #float_input_left = np.int32(np.round(float_input*np.float(left_shift)))######
        float_input_left = modified_round(float_input * left_shift) ######
        float_input_left = np.maximum(np.minimum(max_val, float_input_left), min_val)
    else:
        max_val = pow(2,bit_width)-1
        min_val = 0
        #right_shift = pow(2, -bit_fl)
        left_shift = pow(2, bit_fl)
        #float_input_left = np.int32(np.round(float_input*np.float(left_shift)))######
        float_input_left = modified_round(float_input * left_shift)  ######
        float_input_left = np.maximum(np.minimum(max_val, float_input_left), min_val)
    return float_input_left


def record_blob(blob, file_path, bw_layer_out, fl_layer_out, unsigned_or_not):
    data_fix = float_to_fix(blob.data, bw_layer_out, fl_layer_out, unsigned_or_not)
    shape = blob.shape
    with open(file_path,"w") as filer:
        filer.write("%d\n" % bw_layer_out)
        filer.write("%d\n" % fl_layer_out)
        filer.write("%d\n" % int(unsigned_or_not))
        filer.write("%d\n"%len(shape))
        for i in range(0,len(shape)):
            filer.write("%d\n"%shape[i])
        data_fix.tofile(filer, "\t", format="%d")
    return


def record_ConvolutionRistretto(file_path, layer_proto, weights):
    kernel_size = 1
    if layer_proto.convolution_param.kernel_size._values[0] > 1:
        kernel_size = layer_proto.convolution_param.kernel_size._values[0]
    stride = 1
    if layer_proto.convolution_param.stride._values[0] > 1:
        stride = layer_proto.convolution_param.stride._values[0]
    pad = 0
    if layer_proto.convolution_param.pad._values[0] > 0:
        pad = layer_proto.convolution_param.pad._values[0]
    group = layer_proto.convolution_param.group

    bw_layer_in = layer_proto.quantization_param.bw_layer_in
    bw_layer_out = layer_proto.quantization_param.bw_layer_out
    bw_params = layer_proto.quantization_param.bw_params
    fl_layer_in = layer_proto.quantization_param.fl_layer_in
    fl_layer_out = layer_proto.quantization_param.fl_layer_out
    fl_params = layer_proto.quantization_param.fl_params
    unsigned_in = int(layer_proto.quantization_param.unsigned_in)
    unsigned_out = int(layer_proto.quantization_param.unsigned_out)


    w = weights[0].data
    if len(weights) == 1:  #if there is no bias, using 0 filling
        b = np.zeros(shape=(weights[0].data.shape[0], ),dtype=np.float32)
    else:
        b = weights[1].data
    w_fix = float_to_fix(w, bw_params, fl_params, False)
    b_fix = float_to_fix(b, bw_params, fl_params, False)

    with open(file_path,"w") as filer:
        filer.write("name:%s\n"% layer_proto.name)
        filer.write("type:%s\n"% layer_proto.type)
        filer.write("kernel_size:%d\n"% kernel_size)
        filer.write("stride:%d\n"% stride)
        filer.write("pad:%d\n"% pad)
        filer.write("group:%d\n"% group)
        filer.write("bw_layer_in:%d\n"% bw_layer_in)
        filer.write("bw_layer_out:%d\n"% bw_layer_out)
        filer.write("bw_params:%d\n"% bw_params)
        filer.write("fl_layer_in:%d\n"% fl_layer_in)
        filer.write("fl_layer_out:%d\n"% fl_layer_out)
        filer.write("fl_params:%d\n"% fl_params)
        filer.write("unsigned_in:%d\n" % unsigned_in)
        filer.write("unsigned_out:%d\n" % unsigned_out)
        filer.write("weights_n:%d\n"% w_fix.shape[0])
        filer.write("weights_c:%d\n"% w_fix.shape[1])
        filer.write("weights_h:%d\n"% w_fix.shape[2])
        filer.write("weights_w:%d\n"% w_fix.shape[3])
        w_fix.tofile(filer, "\t", "%d")
        filer.write("\n")
        b_fix.tofile(filer, "\t", "%d")
    return


def record_InnerProductRistretto(file_path, layer_proto, weights):
    bw_layer_in = layer_proto.quantization_param.bw_layer_in
    bw_layer_out = layer_proto.quantization_param.bw_layer_out
    bw_params = layer_proto.quantization_param.bw_params
    fl_layer_in = layer_proto.quantization_param.fl_layer_in
    fl_layer_out = layer_proto.quantization_param.fl_layer_out
    fl_params = layer_proto.quantization_param.fl_params
    unsigned_in = int(layer_proto.quantization_param.unsigned_in)
    unsigned_out = int(layer_proto.quantization_param.unsigned_out)

    w = weights[0].data
    b = weights[1].data
    w_fix = float_to_fix(w, bw_params, fl_params, False)
    b_fix = float_to_fix(b, bw_params, fl_params, False)

    with open(file_path,"w") as filer:
        filer.write("name:%s\n"% layer_proto.name)
        filer.write("type:%s\n"% layer_proto.type)
        filer.write("bw_layer_in:%d\n"% bw_layer_in)
        filer.write("bw_layer_out:%d\n"% bw_layer_out)
        filer.write("bw_params:%d\n"% bw_params)
        filer.write("fl_layer_in:%d\n"% fl_layer_in)
        filer.write("fl_layer_out:%d\n"% fl_layer_out)
        filer.write("fl_params:%d\n"% fl_params)
        filer.write("unsigned_in:%d\n" % unsigned_in)
        filer.write("unsigned_out:%d\n" % unsigned_out)
        filer.write("weights_out:%d\n"% w_fix.shape[0])
        filer.write("weights_in:%d\n"% w_fix.shape[1])
        w_fix.tofile(filer, "\t", "%d")
        filer.write("\n")
        b_fix.tofile(filer, "\t", "%d")
    return

def record_Pooling(file_path, layer_proto):
    kernel_size = 1
    if layer_proto.pooling_param.kernel_size > 1:
        kernel_size = layer_proto.pooling_param.kernel_size
    stride = 1
    if layer_proto.pooling_param.stride > 1:
        stride = layer_proto.pooling_param.stride
    pad = 0
    if layer_proto.pooling_param.pad > 0:
        pad = layer_proto.pooling_param.pad
    pool = layer_proto.pooling_param.pool
    global_pool = layer_proto.pooling_param.global_pooling
    bw_layer_in = layer_proto.quantization_param.bw_layer_in
    bw_layer_out = layer_proto.quantization_param.bw_layer_out
    bw_params = layer_proto.quantization_param.bw_params
    fl_layer_in = layer_proto.quantization_param.fl_layer_in
    fl_layer_out = layer_proto.quantization_param.fl_layer_out
    fl_params = layer_proto.quantization_param.fl_params
    unsigned_in = int(layer_proto.quantization_param.unsigned_in)
    unsigned_out = int(layer_proto.quantization_param.unsigned_out)

    with open(file_path,"w") as filer:
        filer.write("name:%s\n"% layer_proto.name)
        filer.write("type:%s\n"% layer_proto.type)
        filer.write("kernel_size:%d\n"% kernel_size)
        filer.write("stride:%d\n"% stride)
        filer.write("pad:%d\n"% pad)
        filer.write("bw_layer_in:%d\n"% bw_layer_in)
        filer.write("bw_layer_out:%d\n"% bw_layer_out)
        filer.write("bw_params:%d\n"% bw_params)
        filer.write("fl_layer_in:%d\n"% fl_layer_in)
        filer.write("fl_layer_out:%d\n"% fl_layer_out)
        filer.write("fl_params:%d\n"% fl_params)
        filer.write("unsigned_in:%d\n" % unsigned_in)
        filer.write("unsigned_out:%d\n" % unsigned_out)
        if pool == 0:
            filer.write("method:MAX\n")
        elif pool == 1:
            filer.write("method:AVE\n")
        else:
            raise Exception("pooling method is not supported in record_Pooling")
        if global_pool:
            filer.write("global_pool:1\n")
        else:
            filer.write("global_pool:0\n")
    return

def record_Concat(file_path, layer_proto):
    axis = 1
    bw_layer_in = layer_proto.quantization_param.bw_layer_in
    bw_layer_out = layer_proto.quantization_param.bw_layer_out
    bw_params = layer_proto.quantization_param.bw_params
    fl_layer_in = layer_proto.quantization_param.fl_layer_in
    fl_layer_out = layer_proto.quantization_param.fl_layer_out
    fl_params = layer_proto.quantization_param.fl_params
    unsigned_in = int(layer_proto.quantization_param.unsigned_in)
    unsigned_out = int(layer_proto.quantization_param.unsigned_out)
    if layer_proto.concat_param.axis > 1:
        axis = layer_proto.pooling_param.kernel_size
    with open(file_path,"w") as filer:
        filer.write("name:%s\n"% layer_proto.name)
        filer.write("type:%s\n"% layer_proto.type)
        filer.write("axis:%d\n"% axis)
        filer.write("bw_layer_in:%d\n"% bw_layer_in)
        filer.write("bw_layer_out:%d\n"% bw_layer_out)
        filer.write("bw_params:%d\n"% bw_params)
        filer.write("fl_layer_in:%d\n"% fl_layer_in)
        filer.write("fl_layer_out:%d\n"% fl_layer_out)
        filer.write("fl_params:%d\n"% fl_params)
        filer.write("unsigned_in:%d\n" % unsigned_in)
        filer.write("unsigned_out:%d\n" % unsigned_out)
    return

def record_EltwiseRistretto(file_path, layer_proto):
    bw_layer_in = layer_proto.quantization_param.bw_layer_in
    bw_layer_out = layer_proto.quantization_param.bw_layer_out
    bw_params = layer_proto.quantization_param.bw_params
    fl_layer_in = layer_proto.quantization_param.fl_layer_in
    fl_layer_out = layer_proto.quantization_param.fl_layer_out
    fl_params = layer_proto.quantization_param.fl_params
    unsigned_in = int(layer_proto.quantization_param.unsigned_in)
    unsigned_out = int(layer_proto.quantization_param.unsigned_out)
    with open(file_path,"w") as filer:
        filer.write("name:%s\n"% layer_proto.name)
        filer.write("type:%s\n"% layer_proto.type)
        filer.write("operation_type:%d\n"% layer_proto.eltwise_param.operation)
        filer.write("bw_layer_in:%d\n"% bw_layer_in)
        filer.write("bw_layer_out:%d\n"% bw_layer_out)
        filer.write("bw_params:%d\n"% bw_params)
        filer.write("fl_layer_in:%d\n"% fl_layer_in)
        filer.write("fl_layer_out:%d\n"% fl_layer_out)
        filer.write("fl_params:%d\n"% fl_params)
        filer.write("unsigned_in:%d\n" % unsigned_in)
        filer.write("unsigned_out:%d\n" % unsigned_out)
    return

def record_Input(file_path, layer_proto):
    shape = layer_proto.input_param.shape[0].dim
    bw_layer_in = layer_proto.quantization_param.bw_layer_in
    bw_layer_out = layer_proto.quantization_param.bw_layer_out
    bw_params = layer_proto.quantization_param.bw_params
    fl_layer_in = layer_proto.quantization_param.fl_layer_in
    fl_layer_out = layer_proto.quantization_param.fl_layer_out
    fl_params = layer_proto.quantization_param.fl_params
    unsigned_in = int(layer_proto.quantization_param.unsigned_in)
    unsigned_out = int(layer_proto.quantization_param.unsigned_out)
    with open(file_path,"w") as filer:
        filer.write("name:%s\n"% layer_proto.name)
        filer.write("type:%s\n"% layer_proto.type)
        filer.write("input_channel:%d\n"% shape[1])
        filer.write("input_height:%d\n"% shape[2])
        filer.write("input_width:%d\n"% shape[3])
        filer.write("bw_layer_in:%d\n"% bw_layer_in)
        filer.write("bw_layer_out:%d\n"% bw_layer_out)
        filer.write("bw_params:%d\n"% bw_params)
        filer.write("fl_layer_in:%d\n"% fl_layer_in)
        filer.write("fl_layer_out:%d\n"% fl_layer_out)
        filer.write("fl_params:%d\n"% fl_params)
        filer.write("unsigned_in:%d\n" % unsigned_in)
        filer.write("unsigned_out:%d\n" % unsigned_out)
    return

def record_ReLU(file_path, layer_proto):
    bw_layer_in = layer_proto.quantization_param.bw_layer_in
    bw_layer_out = layer_proto.quantization_param.bw_layer_out
    bw_params = layer_proto.quantization_param.bw_params
    fl_layer_in = layer_proto.quantization_param.fl_layer_in
    fl_layer_out = layer_proto.quantization_param.fl_layer_out
    fl_params = layer_proto.quantization_param.fl_params
    unsigned_in = int(layer_proto.quantization_param.unsigned_in)
    unsigned_out = int(layer_proto.quantization_param.unsigned_out)
    with open(file_path,"w") as filer:
        filer.write("name:%s\n"% layer_proto.name)
        filer.write("type:%s\n"% layer_proto.type)
        filer.write("bw_layer_in:%d\n"% bw_layer_in)
        filer.write("bw_layer_out:%d\n"% bw_layer_out)
        filer.write("bw_params:%d\n"% bw_params)
        filer.write("fl_layer_in:%d\n"% fl_layer_in)
        filer.write("fl_layer_out:%d\n"% fl_layer_out)
        filer.write("fl_params:%d\n"% fl_params)
        filer.write("unsigned_in:%d\n" % unsigned_in)
        filer.write("unsigned_out:%d\n" % unsigned_out)
    return

def record_Buffer(file_path, layer_proto):
    bw_layer_in = layer_proto.quantization_param.bw_layer_in
    bw_layer_out = layer_proto.quantization_param.bw_layer_out
    bw_params = layer_proto.quantization_param.bw_params
    fl_layer_in = layer_proto.quantization_param.fl_layer_in
    fl_layer_out = layer_proto.quantization_param.fl_layer_out
    fl_params = layer_proto.quantization_param.fl_params
    unsigned_in = int(layer_proto.quantization_param.unsigned_in)
    unsigned_out = int(layer_proto.quantization_param.unsigned_out)
    with open(file_path,"w") as filer:
        filer.write("name:%s\n"% layer_proto.name)
        filer.write("type:%s\n"% layer_proto.type)
        filer.write("bw_layer_in:%d\n"% bw_layer_in)
        filer.write("bw_layer_out:%d\n"% bw_layer_out)
        filer.write("bw_params:%d\n"% bw_params)
        filer.write("fl_layer_in:%d\n"% fl_layer_in)
        filer.write("fl_layer_out:%d\n"% fl_layer_out)
        filer.write("fl_params:%d\n"% fl_params)
        filer.write("unsigned_in:%d\n" % unsigned_in)
        filer.write("unsigned_out:%d\n" % unsigned_out)
    return

def record_Flatten(file_path, layer_proto):
    bw_layer_in = layer_proto.quantization_param.bw_layer_in
    bw_layer_out = layer_proto.quantization_param.bw_layer_out
    bw_params = layer_proto.quantization_param.bw_params
    fl_layer_in = layer_proto.quantization_param.fl_layer_in
    fl_layer_out = layer_proto.quantization_param.fl_layer_out
    fl_params = layer_proto.quantization_param.fl_params
    unsigned_in = int(layer_proto.quantization_param.unsigned_in)
    unsigned_out = int(layer_proto.quantization_param.unsigned_out)
    with open(file_path,"w") as filer:
        filer.write("name:%s\n"% layer_proto.name)
        filer.write("type:%s\n"% layer_proto.type)
        filer.write("bw_layer_in:%d\n"% bw_layer_in)
        filer.write("bw_layer_out:%d\n"% bw_layer_out)
        filer.write("bw_params:%d\n"% bw_params)
        filer.write("fl_layer_in:%d\n"% fl_layer_in)
        filer.write("fl_layer_out:%d\n"% fl_layer_out)
        filer.write("fl_params:%d\n"% fl_params)
        filer.write("unsigned_in:%d\n" % unsigned_in)
        filer.write("unsigned_out:%d\n" % unsigned_out)
    return

def record_net_struct(path, net, shape_dict, net_proto_def):
    if not os.access(path, os.W_OK):
        os.mkdir(path)
    with open(path+"/net_struct_def.txt", "w") as f:
        for layer in net_proto_def:
            if len(layer.bottom)>0:
                if layer.bottom[0] == layer.top[0] and layer.type in in_place_layers: #skip layers can be skipped, since their effect are already considered in other layers
                    print("!!!warning: layer ", layer.name, " is in-place and skipped")
                    continue
                if layer.bottom[0] == layer.top[0] and layer.type not in in_place_layers:
                    raise Exception("Layer %s is not allowed in place: %s", layer.name, layer.top[0])

            f.write("%s\t"% layer.name)
            f.write("%s\t"%layer.type)
            f.write("%d\t"% len(layer.bottom))
            for bottom in layer.bottom:
                assert (len(shape_dict[bottom]) == 3)
                f.write(("%s\t%d\t%d\t%d\t") % (bottom, shape_dict[bottom][0], shape_dict[bottom][1], shape_dict[bottom][2]))
            f.write("%d\t" % len(layer.top))
            for top in layer.top:
                assert (len(shape_dict[top]) == 3)
                f.write(("%s\t%d\t%d\t%d\t") % (top, shape_dict[top][0], shape_dict[top][1], shape_dict[top][2]))
            f.write("\n")

            file_path = path + "/param_" + layer.name + ".txt"
            if layer.type == "ConvolutionRistretto":
                record_ConvolutionRistretto(file_path, layer, net.params[layer.name])
            elif layer.type == "DeconvolutionRistretto":
                record_ConvolutionRistretto(file_path, layer, net.params[layer.name])
            elif layer.type == "InnerProductRistretto":
                record_InnerProductRistretto(file_path, layer, net.params[layer.name])
            elif layer.type == "EltwiseRistretto":
                record_EltwiseRistretto(file_path, layer)
            elif layer.type == "Pooling":
                record_Pooling(file_path, layer)
            elif layer.type == "Concat":
                record_Concat(file_path, layer)
            elif layer.type == "ReLU":
                record_ReLU(file_path, layer)
            elif layer.type == "Flatten":
                record_Flatten(file_path, layer)
            elif layer.type == "Input":
                record_Input(file_path, layer)
            elif layer.type == "Buffer":
                record_Buffer(file_path, layer)
            else:
                None
    return


def init_model(net_proto, net_weights):
    net = caffe.Net(net_proto, net_weights, caffe.TEST)
    return net

def forward_cls_reg_gray(net, caffe_proto, img_path, bottom_layer, top_layer, resize_w=-1, resize_h=-1, mean_val=128.0, scale = 0.0078125):
    record_path = img_path[0:img_path.rfind(".")]
    if not os.access(record_path, os.W_OK):
        os.mkdir(record_path)
    img_cv = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)
    img = copy.deepcopy(img_cv)
    if resize_h>0 and resize_w>0:
        img = cv2.resize(img, dsize=(resize_w, resize_h), interpolation=cv2.INTER_LINEAR)
    img = (img - mean_val)*scale
    img = np.expand_dims(img, axis=0)
    net.blobs[bottom_layer].data[...] = img
    net.forward()
    result = net.blobs[top_layer[0]].data
    output_blobs = net.blobs
    for key in output_blobs.keys():
        file_path = record_path + "/top_" + key + ".txt"
        #assert (caffe_proto.layer._values[1].type == "ConvolutionRistretto" or caffe_proto.layer._values[1].type == "Convolution" )
        #bw_layer_out = caffe_proto.layer._values[1].quantization_param.bw_layer_out
        #fl_layer_out = caffe_proto.layer._values[1].quantization_param.fl_layer_out
        #unsigned_out = caffe_proto.layer._values[1].quantization_param.unsigned_out
        find_blob = False
        for layer in caffe_proto.layer:
            for top_name in layer.top:
                if top_name == key:
                    bw_layer_out = layer.quantization_param.bw_layer_out
                    fl_layer_out = layer.quantization_param.fl_layer_out
                    unsigned_out = layer.quantization_param.unsigned_out
                    find_blob = True
                    break
            if find_blob == True:
                break
        record_blob(output_blobs[key], file_path, bw_layer_out, fl_layer_out, unsigned_out)
    return result

def generate_samples(net_prototxt, model_path, bottom_layer, top_layer, param_folder, test_sample_path, record_param_or_not):
    caffenet = init_model(net_prototxt, model_path)
    caffe_proto = caffe_pb2.NetParameter()
    text_format.Merge(open(net_prototxt).read(), caffe_proto)
    total_complexity, total_model_size, complexity_dict, model_size_dict, shape_dict = \
        infer_complexity.infer_complexity(net_prototxt, top_layer)
    if record_param_or_not == True:
        record_net_struct(param_folder, caffenet, shape_dict, caffe_proto.layer)
    result = forward_cls_reg_gray(caffenet, caffe_proto, test_sample_path, bottom_layer, top_layer, resize_w=320, resize_h=192, mean_val=0.0, scale=1.0/255.0)
    print(result)


if __name__ == "__main__":
    #print(modified_round(1.5),modified_round(2.5),modified_round(-1.5),modified_round(-2.5), np.floor(-0.4)); exit(0)
    #net_prototxt = "../examples/quantization_lane_seg/HCSegNetSmall_relu.prototxt_quantized.prototxt_remove_inplace_add_buffer.prototxt"
    net_prototxt = "../examples/quantization_lane_seg/HCSegNetSmall.prototxt_quantized_16bit.prototxt"
    model_path = "../examples/quantization_lane_seg/HCSegNetSmall.caffemodel"
    bottom_layer = "data"
    top_layer=["ConvNd_31"]
    param_folder = "./test_imgs/lane_segmentation/16bit/net_params"
    test_sample_path = "./test_imgs/lane_segmentation/16bit/5.bmp"
    generate_samples(net_prototxt, model_path, bottom_layer, top_layer, param_folder, test_sample_path, True)
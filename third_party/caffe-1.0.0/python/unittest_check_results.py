import numpy as np
import copy
#from unittest_generate_samples import modified_round
segment_string = "######################################################"
def print_check_res(correct_or_not, layer_name):
    if correct_or_not:
        print(layer_name + " is same with caffe")
    else:
        print(layer_name + " is different from caffe!!!!!!!!!!!!!!!!!!")

def max_min_val(bw, unsigned_flag):
    if unsigned_flag == 0:
        max_val = pow(2, bw - 1) - 1
        min_val = -pow(2, bw - 1)
    else:
        max_val = pow(2, bw) - 1
        min_val = 0
    return min_val, max_val

def simulate_binary(dec_num):
    element_binary = bin(dec_num)
    if dec_num > 0:
        element_binary = '+' + element_binary
    element_binary_long = '0000000000000000000000000000000000000000000000000000000000000000' + element_binary[3:]
    return element_binary_long

def simulate_round(data, left_shift):
    data = np.array(data)
    ori_shape = data.shape
    if left_shift>=0:
        data_round = (data << left_shift)
    else:
        data_round = (data >> (-left_shift))
        data = data.reshape(data.size)
        data_round = data_round.reshape(data_round.size)
        for idx in range(0, data.size):
            element = data[idx]
            scale = np.float32(pow(2, -left_shift))
            element_binary = simulate_binary(element)
            if element_binary[left_shift] == '1' and element>0:
                data_round[idx] += 1
            elif element_binary[left_shift] == '0' and element % scale!=0 and element<0:
                data_round[idx] += 1
            else:
                None
        data = data.reshape(ori_shape)
        data_round = data_round.reshape(ori_shape)
    # data = np.array(data)
    # if left_shift >= 0:
    #     data_round = (data << left_shift)
    # else:
    #     data_round = (data >> (-left_shift))
    return data_round

def preprocess_bottom(bottom, bw_layer_in, fl_layer_in, unsigned_flag_in, fl_last_layer_out):
    bottom = simulate_round(bottom, fl_layer_in-fl_last_layer_out)
    #bottom = np.int32(modified_round((bottom*pow(2, -fl_last_layer_out)*pow(2, fl_layer_in))))
    min_val, max_val = max_min_val(bw_layer_in, unsigned_flag_in)
    bottom = np.maximum(np.minimum(bottom, max_val), min_val)
    return bottom

def postprocess_top(top, bw_layer_out, fl_layer_out, unsigned_flag_out, fl_layer_in, fl_params):
    min_val, max_val = max_min_val(bw_layer_out, unsigned_flag_out)
    top = simulate_round(top, fl_layer_out-fl_params-fl_layer_in)
    #top = np.int32(modified_round(top * pow(2, fl_layer_out) * pow(2, -(fl_layer_in+fl_params))))
    top = np.maximum(np.minimum(top, max_val), min_val)
    return top

################################################################################
#basic blob operation
def load_blob_data(blob_file):
    with open(blob_file) as filer:
        bw_bit = int(filer.readline())
        fl_bit = int(filer.readline())
        unsigned_or_not = int(filer.readline())
        dim_num = int(filer.readline())
        shape = np.zeros(shape=(dim_num,), dtype=np.int)
        size = 1
        for i in range(0, dim_num):
            shape[i] = int(filer.readline())
            size *= shape[i]
        str_line = filer.readline()
        data_line = np.fromstring(str_line, dtype=np.int, count=size, sep="\t")
        data = data_line
        if dim_num == 1:
            data = data_line.reshape((shape[0],))
        elif dim_num == 2:
            data = data_line.reshape((shape[0],shape[1]))
        elif dim_num == 3:
            data = data_line.reshape((shape[0], shape[1], shape[2]))
        elif dim_num == 4:
            data = data_line.reshape((shape[0], shape[1], shape[2], shape[3]))
        else:
            raise Exception("only support 1, 2, 3, 4 dimensional blobs")
    return data, bw_bit, fl_bit, unsigned_or_not

def record_blob(blob, bw_bit, fl_bit, unsigned_or_not, file_path):
    shape = blob.shape
    with open(file_path,"w") as filer:
        filer.write("%d\n" % bw_bit)
        filer.write("%d\n" % fl_bit)
        filer.write("%d\n" % int(unsigned_or_not))
        filer.write("%d\n"%len(shape))
        for i in range(0, len(shape)):
            filer.write("%d\n"%shape[i])
        blob.tofile(filer, "\t", format="%d")
    return

def compare_blob_file(file1, file2, res_file):
    blob1, bw_bit1, fl_bit1, unsigned_or_not1 = load_blob_data(file1)
    blob2, bw_bit2, fl_bit2, unsigned_or_not2 = load_blob_data(file2)
    shape1 = blob1.shape
    shape2 = blob2.shape
    if bw_bit1 != bw_bit1:
        print (res_file + " cannot be generated due to shape difference (compare_blob_file): ", bw_bit1, bw_bit2)
        print (segment_string)
        return False
    if fl_bit1 != fl_bit1:
        print (res_file + " cannot be generated due to shape difference (compare_blob_file): ", fl_bit1, fl_bit2)
        print (segment_string)
        return False
    if unsigned_or_not1 != unsigned_or_not2:
        print (res_file + " cannot be generated due to shape difference (compare_blob_file): ", unsigned_or_not1, unsigned_or_not2)
        print (segment_string)
        return False
    if shape1 != shape2:
        print (res_file + " cannot be generated due to shape difference (compare_blob_file): ", shape1, shape2)
        print (segment_string)
        return False
    else:
        shape_num = len(shape1)
        if shape_num > 4:
            print (res_file + " cannot be generated due to dimension not supported (compare_blob_file), shape="+str(shape1))
            print (segment_string)
            return False
        else:
            diff = blob1-blob2
            difference_max = diff.max()
            difference_min = diff.min()
            diff_list = list()
            diff_list_idx = list()
            diff_count = 0
            if shape_num == 1:
                for s0 in range(0, shape1[0]):
                    if diff[s0] != 0:
                        diff_list_idx.append([s0])
                        diff_list.append([blob1[s0], blob2[s0]])
                        diff_count+=1
            elif shape_num == 2:
                for s0 in range(0, shape1[0]):
                    for s1 in range(0, shape1[1]):
                        if diff[s0,s1] != 0:
                            diff_list_idx.append([s0,s1])
                            diff_list.append([blob1[s0,s1], blob2[s0,s1]])
                            diff_count+=1
            elif shape_num == 3:
                for s0 in range(0, shape1[0]):
                    for s1 in range(0, shape1[1]):
                        for s2 in range(0, shape1[2]):
                            if diff[s0,s1,s2] != 0:
                                diff_list_idx.append([s0,s1])
                                diff_list.append([blob1[s0,s1,s2], blob2[s0,s1,s2]])
                                diff_count+=1
            elif shape_num == 4:
                for s0 in range(0, shape1[0]):
                    for s1 in range(0, shape1[1]):
                        for s2 in range(0, shape1[2]):
                            for s3 in range(0, shape1[3]):
                                if diff[s0,s1,s2,s3] != 0:
                                    diff_list_idx.append([s0,s1,s2,s3])
                                    diff_list.append([blob1[s0,s1,s2,s3], blob2[s0,s1,s2,s3]])
                                    diff_count+=1
            else:
                None

            with open(res_file, "w") as filer:
                filer.write("difference_num:%d\n"%diff_count)
                filer.write("difference_max:%d\n"%(difference_max))
                filer.write("difference_min:%d\n"%(difference_min))
                filer.write("difference listed below:\n")
                for idx, diff_list_ele in zip(diff_list_idx, diff_list):
                    filer.write("idx:\t"+str(idx)+":\tdifference:\t"+str(diff_list_ele)+"\n")
            res = False
            if diff_count == 0:
                res = True
    return res

####################################################################################
#python convolution implementation
def load_ConvolutionRistretto_param(param_file):
    with open(param_file) as filer:
        name = filer.readline()
        name = name[name.rfind(":")+1:]
        type = filer.readline()
        type = type[type.rfind(":")+1:]
        kernel_size = filer.readline()
        kernel_size = int(kernel_size[kernel_size.rfind(":")+1:])
        stride = filer.readline()
        stride = int(stride[stride.rfind(":")+1:])
        pad = filer.readline()
        pad = int(pad[pad.rfind(":")+1:])
        group = filer.readline()
        group = int(group[group.rfind(":")+1:])
        bw_layer_in = filer.readline()
        bw_layer_in = int(bw_layer_in[bw_layer_in.rfind(":")+1:])
        bw_layer_out = filer.readline()
        bw_layer_out = int(bw_layer_out[bw_layer_out.rfind(":")+1:])
        bw_params = filer.readline()
        bw_params = int(bw_params[bw_params.rfind(":")+1:])
        fl_layer_in = filer.readline()
        fl_layer_in = int(fl_layer_in[fl_layer_in.rfind(":")+1:])
        fl_layer_out = filer.readline()
        fl_layer_out = int(fl_layer_out[fl_layer_out.rfind(":")+1:])
        fl_params = filer.readline()
        fl_params = int(fl_params[fl_params.rfind(":")+1:])
        unsigned_in = filer.readline()
        unsigned_in = int(unsigned_in[unsigned_in.rfind(":") + 1:])
        unsigned_out = filer.readline()
        unsigned_out = int(unsigned_out[unsigned_out.rfind(":") + 1:])
        shape0 = filer.readline()
        shape0 = int(shape0[shape0.rfind(":")+1:])
        shape1 = filer.readline()
        shape1 = int(shape1[shape1.rfind(":")+1:])
        shape2 = filer.readline()
        shape2 = int(shape2[shape2.rfind(":")+1:])
        shape3 = filer.readline()
        shape3 = int(shape3[shape3.rfind(":")+1:])

        weights_str = filer.readline()
        weights_line = np.fromstring(weights_str, dtype=np.int, count=shape0*shape1*shape2*shape3, sep="\t")
        weights = weights_line.reshape((shape0, shape1, shape2, shape3))

        bias_str = filer.readline()
        bias_line = np.fromstring(bias_str, dtype=np.int, count=shape0, sep="\t")
        bias = bias_line.reshape((shape0,))
    return pad, stride, group, weights, bias, bw_layer_out, fl_layer_out, bw_layer_in, fl_layer_in, bw_params, fl_params, unsigned_in, unsigned_out

def simulate_ConvolutionRistretto(bottom_file, param_file, simulate_top_file):
    pad, stride, group, weights, bias, bw_layer_out, fl_layer_out, bw_layer_in, fl_layer_in, bw_params, fl_params, unsigned_in, unsigned_out \
        = load_ConvolutionRistretto_param(param_file) #load parameters from file
    bottom, bw_last_layer, fl_last_layer, unsigned_flag_last_layer= load_blob_data(bottom_file) #load bottom data from file
    bottom = preprocess_bottom(bottom, bw_layer_in, fl_layer_in, unsigned_in, fl_last_layer)

    #padding data using 0
    bottom_pad = np.zeros(shape=(bottom.shape[0],bottom.shape[1],bottom.shape[2]+pad*2,bottom.shape[3]+pad*2), dtype=np.int)
    bottom_pad[:,:,pad:bottom.shape[2]+pad, pad:bottom.shape[3]+pad] = bottom

    assert (weights.shape[2] == weights.shape[3])
    kernel_size = weights.shape[2]
    N = bottom.shape[0] #number of input images
    M = bottom.shape[1] #number of input channels
    C = weights.shape[0] #number of output channels
    H = int((bottom_pad.shape[2] - kernel_size) / stride + 1)
    W = int((bottom_pad.shape[3] - kernel_size) / stride + 1)

    top_blob = np.zeros(shape=(N, C, H, W), dtype=np.int)
    bias_shifted = np.zeros(shape=(C,1), dtype=np.int)
    for idx in range(0, C):
        bias_shifted[idx] = (bias[idx]<< fl_layer_in)

    assert(M%group==0 and C%group==0)  #N and C must be divided group in caffe implementation, but in our implementation, C does not need to be divided by group
    mg = int(M/group)
    cg = int(C/group)
    for n in range(0,N):
        for g in range(0,group):
            for c in range(0,cg):
                for h in range(0,H):
                    for w in range(0,W):
                        channel_idx = c+cg*g
                        conv_data = bottom_pad[n, mg*g:mg*(g+1), h*stride:h*stride+kernel_size, w*stride:w*stride+kernel_size]
                        channel_weights = weights[channel_idx, :, :, :]
                        conv_res = np.multiply(conv_data, channel_weights)
                        top_blob[n, channel_idx, h, w] = np.sum(conv_res)+bias_shifted[channel_idx]
                        # if n==0 and channel_idx==19 and h==12 and w==33:
                        #     print (top_blob[n, channel_idx, h, w])

    top_blob = postprocess_top(top_blob, bw_layer_out, fl_layer_out, unsigned_out, fl_layer_in, fl_params)
    record_blob(top_blob, bw_layer_out, fl_layer_out, unsigned_out, simulate_top_file)
    return

#deconvolution has the same parameters with convolution and thus share the same function of load_ConvolutionRistretto_param
def simulate_DeconvolutionRistretto(bottom_file, param_file, simulate_top_file):
    pad, stride, group, weights, bias, bw_layer_out, fl_layer_out, bw_layer_in, fl_layer_in, bw_params, fl_params, unsigned_in, unsigned_out \
        = load_ConvolutionRistretto_param(param_file) #load parameters from file
    bottom, bw_last_layer, fl_last_layer, unsigned_flag_last_layer= load_blob_data(bottom_file) #load bottom data from file
    bottom = preprocess_bottom(bottom, bw_layer_in, fl_layer_in, unsigned_in, fl_last_layer)

    assert (weights.shape[2] == weights.shape[3])
    kernel_size = weights.shape[2]
    N = bottom.shape[0] #number of input images
    M = bottom.shape[1] #number of input channels
    H = bottom.shape[2]
    W = bottom.shape[3]
    C = weights.shape[1]*group  # number of output channels per group

    top_H = (H - 1) * stride + kernel_size - pad * 2
    top_W = (W - 1) * stride + kernel_size - pad * 2
    top_pad_H = top_H + 2 * pad
    top_pad_W = top_W + 2 * pad
    top_blob_pad = np.zeros(shape=(N, C, top_pad_H, top_pad_W), dtype=np.int)

    bias_shifted = np.zeros(shape=(C,1), dtype=np.int)
    for idx in range(0, C):
        bias_shifted[idx] = (bias[idx]<< fl_layer_in)

    assert(M%group==0)  #M and C must be divided group in caffe implementation, but in our implementation, C does not need to be divided by group
    mg = int(M/group)
    cg = int(C/group)
    for n in range(0,N):
        for g in range(0,group):
            for h in range(0, H):
                for w in range(0, W):
                    back_conv = np.zeros(shape=(1, cg, kernel_size, kernel_size), dtype=np.int)
                    for m in range(mg*g, mg*(g+1)):
                        weights_tmp = weights[m:m+1,:,:,:]
                        back_conv = back_conv + weights_tmp * bottom[n,m,h,w]
                    top_blob_pad[n:n+1, cg*g:cg*(g+1), h*stride:h*stride+kernel_size, w*stride:w*stride+kernel_size] = \
                        top_blob_pad[n:n+1, cg*g:cg*(g+1), h*stride:h*stride+kernel_size, w*stride:w*stride+kernel_size] + back_conv

    top_blob = top_blob_pad[:,:,pad:(top_H+pad), pad:(top_W+pad)]
    #forward bias for each channel
    for c in range(0, C):
        top_blob[:, c, :, :] = top_blob[:, c, :, :] + bias_shifted[c]

    top_blob = postprocess_top(top_blob, bw_layer_out, fl_layer_out, unsigned_out, fl_layer_in, fl_params)
    record_blob(top_blob, bw_layer_out, fl_layer_out, unsigned_out, simulate_top_file)
    return


####################################################################################
#python fc implementation
def load_InnerProductRistretto_param(param_file):
    with open(param_file) as filer:
        name = filer.readline()
        name = name[name.rfind(":") + 1:]
        type = filer.readline()
        type = type[type.rfind(":") + 1:]
        bw_layer_in = filer.readline()
        bw_layer_in = int(bw_layer_in[bw_layer_in.rfind(":")+1:])
        bw_layer_out = filer.readline()
        bw_layer_out = int(bw_layer_out[bw_layer_out.rfind(":")+1:])
        bw_params = filer.readline()
        bw_params = int(bw_params[bw_params.rfind(":")+1:])
        fl_layer_in = filer.readline()
        fl_layer_in = int(fl_layer_in[fl_layer_in.rfind(":")+1:])
        fl_layer_out = filer.readline()
        fl_layer_out = int(fl_layer_out[fl_layer_out.rfind(":")+1:])
        fl_params = filer.readline()
        fl_params = int(fl_params[fl_params.rfind(":")+1:])
        unsigned_in = filer.readline()
        unsigned_in = int(unsigned_in[unsigned_in.rfind(":") + 1:])
        unsigned_out = filer.readline()
        unsigned_out = int(unsigned_out[unsigned_out.rfind(":") + 1:])
        weights_out = filer.readline()
        weights_out = int(weights_out[weights_out.rfind(":")+1:])
        weights_in= filer.readline()
        weights_in = int(weights_in[weights_in.rfind(":") + 1:])

        weights_str = filer.readline()
        weights_line = np.fromstring(weights_str, dtype=np.int, count=weights_in*weights_out, sep="\t")
        weights = weights_line.reshape((weights_out, weights_in))

        bias_str = filer.readline()
        bias_line = np.fromstring(bias_str, dtype=np.int, count=weights_out, sep="\t")
        bias = bias_line.reshape((weights_out,))
    return weights, bias, bw_layer_out, fl_layer_out, bw_layer_in, fl_layer_in, bw_params, fl_params, unsigned_in, unsigned_out

def simulate_InnerProductRistretto(bottom_file, param_file, simulate_top_file):
    weights, bias, bw_layer_out, fl_layer_out, bw_layer_in, fl_layer_in, bw_params, fl_params, unsigned_in, unsigned_out= \
        load_InnerProductRistretto_param(param_file) #load parameters from file

    bottom, bw_last_layer, fl_last_layer, unsigned_flag_last_layer= load_blob_data(bottom_file) #load bottom data from file
    bottom = preprocess_bottom(bottom, bw_layer_in, fl_layer_in, unsigned_in, fl_last_layer)

    weights_out = weights.shape[0]
    weights_in = weights.shape[1]

    top_blob = np.zeros(shape=(bottom.shape[0], weights_out), dtype=np.int)
    bias_shifted = np.zeros(shape=(weights_out,1), dtype=np.int)
    for idx in range(0, weights_out):
        bias_shifted[idx] = (bias[idx]<<fl_layer_in)
    for n in range(0, bottom.shape[0]):
        for idx in range(0, weights_out):
            weight_channel = weights[idx, :]
            data = bottom[n,:].reshape((weights_in,))
            assert (data.shape == weight_channel.shape)
            tmp = np.multiply(weight_channel,data)
            top_blob[n,idx] = np.sum(tmp)
            top_blob[n,idx] = (top_blob[n,idx]+bias_shifted[idx])
    top_blob = postprocess_top(top_blob, bw_layer_out, fl_layer_out, unsigned_out, fl_layer_in, fl_params)
    record_blob(top_blob, bw_layer_out, fl_layer_out, unsigned_out, simulate_top_file)
    return


####################################################################################
#python eltwise implementation
def load_EltwiseRistretto_param(param_file):
    with open(param_file) as filer:
        name = filer.readline()
        name = name[name.rfind(":") + 1:]
        type = filer.readline()
        type = type[type.rfind(":") + 1:]
        operation = filer.readline()
        operation = int(operation[operation.rfind(":")+1:])
        bw_layer_in = filer.readline()
        bw_layer_in = int(bw_layer_in[bw_layer_in.rfind(":")+1:])
        bw_layer_out = filer.readline()
        bw_layer_out = int(bw_layer_out[bw_layer_out.rfind(":")+1:])
        bw_params = filer.readline()
        bw_params = int(bw_params[bw_params.rfind(":")+1:])
        fl_layer_in = filer.readline()
        fl_layer_in = int(fl_layer_in[fl_layer_in.rfind(":")+1:])
        fl_layer_out = filer.readline()
        fl_layer_out = int(fl_layer_out[fl_layer_out.rfind(":")+1:])
        fl_params = filer.readline()
        fl_params = int(fl_params[fl_params.rfind(":")+1:])
        unsigned_in = filer.readline()
        unsigned_in = int(unsigned_in[unsigned_in.rfind(":") + 1:])
        unsigned_out = filer.readline()
        unsigned_out = int(unsigned_out[unsigned_out.rfind(":") + 1:])
    return operation,bw_layer_out, fl_layer_out, bw_layer_in, fl_layer_in, unsigned_in, unsigned_out

def simulate_EltwiseRistretto(bottom_files, param_file, simulate_top_file):
    operation, bw_layer_out, fl_layer_out, bw_layer_in, fl_layer_in, unsigned_in, unsigned_out = \
        load_EltwiseRistretto_param(param_file) #load parameters from file
    assert (len(bottom_files) == 2)
    bottoms = list()
    for bottom_file in bottom_files:
        bottom, bw_last_layer, fl_last_layer, unsigned_flag_last_layer = load_blob_data(bottom_file)  # load bottom data from file
        bottom = preprocess_bottom(bottom, bw_layer_in, fl_layer_in, unsigned_in, fl_last_layer)
        bottoms.append(bottom) #load bottom data from file

    assert (bottoms[0].shape == bottoms[1].shape)

    top_blob = np.zeros(shape=bottoms[0].shape, dtype=np.int)
    if len(bottoms[0].shape) == 4:
        for d0 in range(0, bottoms[0].shape[0]):
            for d1 in range(0, bottoms[0].shape[1]):
                for d2 in range(0, bottoms[0].shape[2]):
                    for d3 in range(0, bottoms[0].shape[3]):
                        top_blob[d0,d1,d2,d3] = bottoms[0][d0,d1,d2,d3]+bottoms[1][d0,d1,d2,d3]

        top_blob = postprocess_top(top_blob, bw_layer_out, fl_layer_out, unsigned_out, fl_layer_in, 0)
        record_blob(top_blob, bw_layer_out, fl_layer_out, unsigned_out, simulate_top_file)
    else:
        raise Exception("Do not support dimensions in eltwise ristretto layer")

        record_blob(top_blob, bw_layer_out, fl_layer_out, unsigned_out, simulate_top_file)
    return

####################################################################################
#python pooling implementation
def load_Pooling_param(param_file):
    with open(param_file) as filer:
        name = filer.readline()
        name = name[name.rfind(":")+1:]
        type = filer.readline()
        type = type[type.rfind(":")+1:]
        kernel_size = filer.readline()
        kernel_size = int(kernel_size[kernel_size.rfind(":")+1:])
        stride = filer.readline()
        stride = int(stride[stride.rfind(":")+1:])
        pad = filer.readline()
        pad = int(pad[pad.rfind(":")+1:])
        bw_layer_in = filer.readline()
        bw_layer_in = int(bw_layer_in[bw_layer_in.rfind(":")+1:])
        bw_layer_out = filer.readline()
        bw_layer_out = int(bw_layer_out[bw_layer_out.rfind(":")+1:])
        bw_params = filer.readline()
        bw_params = int(bw_params[bw_params.rfind(":")+1:])
        fl_layer_in = filer.readline()
        fl_layer_in = int(fl_layer_in[fl_layer_in.rfind(":")+1:])
        fl_layer_out = filer.readline()
        fl_layer_out = int(fl_layer_out[fl_layer_out.rfind(":")+1:])
        fl_params = filer.readline()
        fl_params = int(fl_params[fl_params.rfind(":")+1:])
        unsigned_in = filer.readline()
        unsigned_in = int(unsigned_in[unsigned_in.rfind(":") + 1:])
        unsigned_out = filer.readline()
        unsigned_out = int(unsigned_out[unsigned_out.rfind(":") + 1:])
        pooling_method = filer.readline()
        pooling_method = pooling_method[pooling_method.rfind(":")+1:].strip("\r\n")
        pooling_global = filer.readline()
        pooling_global = int(pooling_global[pooling_global.rfind(":")+1:])
    return kernel_size, pad, stride, pooling_method, pooling_global, unsigned_in, unsigned_out, fl_layer_in, fl_layer_out, bw_layer_out, bw_layer_in

def simulate_Pooling(bottom_file, param_file, simulate_top_file):
    kernel_size, pad, stride, pooling_method, pooling_global, unsigned_in, unsigned_out, fl_layer_in, fl_layer_out, bw_layer_out, bw_layer_in= \
        load_Pooling_param(param_file) #load parameters from file
    bottom, bw_last_layer, fl_last_layer, unsigned_flag_last_layer = load_blob_data(bottom_file)  # load bottom data from file
    bottom = preprocess_bottom(bottom, bw_layer_in, fl_layer_in, unsigned_in, fl_last_layer)
    #padding data using 0
    bottom_pad = np.zeros(shape=(bottom.shape[0],bottom.shape[1],bottom.shape[2]+pad*2,bottom.shape[3]+pad*2), dtype=np.int)
    bottom_pad[:,:,pad:bottom.shape[2]+pad, pad:bottom.shape[3]+pad] = bottom


    if pooling_global == 1:
        top_blob = np.zeros(shape=(bottom_pad.shape[0], bottom_pad.shape[1], 1, 1), dtype=np.int)
        if pooling_method == "MAX":
            for s0 in bottom_pad.shape[0]:
                for s1 in bottom_pad.shape[1]:
                    top_blob[s0,s1,0,0] = np.max(bottom_pad[s0,s1,:,:])

        elif pooling_method == "AVE":
            dominator = bottom_pad.shape[2]*bottom_pad.shape[3]
            for s0 in bottom_pad.shape[0]:
                for s1 in bottom_pad.shape[1]:
                    top_blob[s0, s1, 0, 0] = np.sum(bottom_pad[s0,s1,:,:]) / dominator

        else:
            raise Exception("not supported pooling method (in global pooling): " + pooling_method)

    else:  #normal pooling
        N = bottom.shape[0]  # number of input images
        C = bottom.shape[1]  # number of channels
        pooled_height = np.int(np.ceil((bottom_pad.shape[2] - kernel_size) / np.float(stride))) + 1
        pooled_width = np.int(np.ceil((bottom_pad.shape[3] - kernel_size) / np.float(stride))) + 1
        if (pooled_height-1)*stride>=bottom.shape[2]+pad:
            pooled_height -= 1
        if (pooled_width-1)*stride>=bottom.shape[3]+pad:
            pooled_width -= 1
        top_blob = np.zeros(shape=(N, C, pooled_height, pooled_width), dtype=np.int)
        for n in range(0, N):
            for c in range(0, C):
                for h in range(0, pooled_height):
                    for w in range(0, pooled_width):
                        h_start = h*stride
                        h_end = min(h*stride+kernel_size, bottom_pad.shape[2])
                        w_start = w*stride
                        w_end = min(w*stride+kernel_size, bottom_pad.shape[3])
                        pooled_data = bottom_pad[n,c, h_start:h_end, w_start:w_end]

                        if pooling_method == "MAX":
                            top_blob[n,c,h,w] = np.max(pooled_data)
                        elif pooling_method == "AVE":
                            top_blob[n,c,h,w] = np.sum(pooled_data) / pooled_data.size
                        else:
                            raise Exception("not supported pooling method (in normal pooling): " + pooling_method)

    top_blob = postprocess_top(top_blob, bw_layer_out, fl_layer_out, unsigned_out, fl_layer_in, 0)
    record_blob(top_blob, bw_layer_out, fl_layer_out, unsigned_out, simulate_top_file)
    record_blob(top_blob, bw_layer_out, fl_layer_out, unsigned_out, simulate_top_file)
    return

####################################################################################
#python concat implementation
def load_Concat_param(param_file):
    with open(param_file) as filer:
        name = filer.readline()
        name = name[name.rfind(":")+1:]
        type = filer.readline()
        type = type[type.rfind(":")+1:]
        axis = filer.readline()
        axis = int(axis[axis.rfind(":")+1:])
        bw_layer_in = filer.readline()
        bw_layer_in = int(bw_layer_in[bw_layer_in.rfind(":")+1:])
        bw_layer_out = filer.readline()
        bw_layer_out = int(bw_layer_out[bw_layer_out.rfind(":")+1:])
        bw_params = filer.readline()
        bw_params = int(bw_params[bw_params.rfind(":")+1:])
        fl_layer_in = filer.readline()
        fl_layer_in = int(fl_layer_in[fl_layer_in.rfind(":")+1:])
        fl_layer_out = filer.readline()
        fl_layer_out = int(fl_layer_out[fl_layer_out.rfind(":")+1:])
        fl_params = filer.readline()
        fl_params = int(fl_params[fl_params.rfind(":")+1:])
        unsigned_in = filer.readline()
        unsigned_in = int(unsigned_in[unsigned_in.rfind(":") + 1:])
        unsigned_out = filer.readline()
        unsigned_out = int(unsigned_out[unsigned_out.rfind(":") + 1:])
    return axis, unsigned_in, unsigned_out, fl_layer_in, fl_layer_out, bw_layer_out, bw_layer_in

def simulate_Concat(bottom_files, param_file, simulate_top_file):
    axis, unsigned_in, unsigned_out, fl_layer_in, fl_layer_out, bw_layer_out, bw_layer_in = load_Concat_param(param_file) #load parameters from file
    bottoms = list()
    shapes = list()
    for bottom_file in bottom_files:
        bottom, bw_last_layer, fl_last_layer, unsigned_flag_last_layer = load_blob_data(bottom_file)  # load bottom data from file
        bottom = preprocess_bottom(bottom, bw_layer_in, fl_layer_in, unsigned_in, fl_last_layer)
        bottoms.append(bottom)
        shapes.append(bottom.shape)

    if len(shapes[0]) == 4 and axis == 1:
        axis_1_dim = 0
        for shape in shapes:
            axis_1_dim += shape[1]
        top_blob = np.zeros(shape=(shapes[0][0], axis_1_dim, shapes[0][2],shapes[0][3]), dtype=np.int)
        idx  = 0
        for bottom in bottoms:
            top_blob[:, idx:idx+bottom.shape[1], :, :] = bottom
            idx += bottom.shape[1]

    else:
        raise Exception("Concat mode is not implemented")

    top_blob = postprocess_top(top_blob, bw_layer_out, fl_layer_out, unsigned_out, fl_layer_in, 0)
    record_blob(top_blob, bw_layer_out, fl_layer_out, unsigned_out, simulate_top_file)
    record_blob(top_blob, bw_layer_out, fl_layer_out, unsigned_out, simulate_top_file)
    return


####################################################################################
#python concat implementation
def load_Flatten_param(param_file):
    with open(param_file) as filer:
        name = filer.readline()
        name = name[name.rfind(":")+1:]
        type = filer.readline()
        type = type[type.rfind(":")+1:]
        bw_layer_in = filer.readline()
        bw_layer_in = int(bw_layer_in[bw_layer_in.rfind(":")+1:])
        bw_layer_out = filer.readline()
        bw_layer_out = int(bw_layer_out[bw_layer_out.rfind(":")+1:])
        bw_params = filer.readline()
        bw_params = int(bw_params[bw_params.rfind(":")+1:])
        fl_layer_in = filer.readline()
        fl_layer_in = int(fl_layer_in[fl_layer_in.rfind(":")+1:])
        fl_layer_out = filer.readline()
        fl_layer_out = int(fl_layer_out[fl_layer_out.rfind(":")+1:])
        fl_params = filer.readline()
        fl_params = int(fl_params[fl_params.rfind(":")+1:])
        unsigned_in = filer.readline()
        unsigned_in = int(unsigned_in[unsigned_in.rfind(":") + 1:])
        unsigned_out = filer.readline()
        unsigned_out = int(unsigned_out[unsigned_out.rfind(":") + 1:])
    return unsigned_in, unsigned_out, fl_layer_in, fl_layer_out, bw_layer_in, bw_layer_out

####################################################################################
#python flatten implementation, no parameters for this layer
def simulate_Flatten(bottom_file, param_file, simulate_top_file):
    unsigned_in, unsigned_out, fl_layer_in, fl_layer_out, bw_layer_in, bw_layer_out = load_Flatten_param(param_file)
    bottom, bw_last_layer, fl_last_layer, unsigned_flag_last_layer = load_blob_data(bottom_file)  # load bottom data from file
    bottom = preprocess_bottom(bottom, bw_layer_in, fl_layer_in, unsigned_in, fl_last_layer)

    top_blob = np.reshape(bottom, (1, bottom.size))

    top_blob = postprocess_top(top_blob, bw_layer_out, fl_layer_out, unsigned_out, fl_layer_in, 0)
    record_blob(top_blob, bw_layer_out, fl_layer_out, unsigned_out, simulate_top_file)
    record_blob(top_blob, bw_layer_out, fl_layer_out, unsigned_out, simulate_top_file)
    return

####################################################################################
#python concat implementation
def load_ReLU_param(param_file):
    with open(param_file) as filer:
        name = filer.readline()
        name = name[name.rfind(":")+1:]
        type = filer.readline()
        type = type[type.rfind(":")+1:]
        bw_layer_in = filer.readline()
        bw_layer_in = int(bw_layer_in[bw_layer_in.rfind(":")+1:])
        bw_layer_out = filer.readline()
        bw_layer_out = int(bw_layer_out[bw_layer_out.rfind(":")+1:])
        bw_params = filer.readline()
        bw_params = int(bw_params[bw_params.rfind(":")+1:])
        fl_layer_in = filer.readline()
        fl_layer_in = int(fl_layer_in[fl_layer_in.rfind(":")+1:])
        fl_layer_out = filer.readline()
        fl_layer_out = int(fl_layer_out[fl_layer_out.rfind(":")+1:])
        fl_params = filer.readline()
        fl_params = int(fl_params[fl_params.rfind(":")+1:])
        unsigned_in = filer.readline()
        unsigned_in = int(unsigned_in[unsigned_in.rfind(":") + 1:])
        unsigned_out = filer.readline()
        unsigned_out = int(unsigned_out[unsigned_out.rfind(":") + 1:])
    return unsigned_in, unsigned_out, fl_layer_in, fl_layer_out, bw_layer_in, bw_layer_out
####################################################################################
#python ReLU implementation, no parameters for this layer
def simulate_ReLU(bottom_file, param_file, simulate_top_file):
    unsigned_in, unsigned_out, fl_layer_in, fl_layer_out, bw_layer_in, bw_layer_out = load_ReLU_param(param_file)
    bottom, bw_last_layer, fl_last_layer, unsigned_flag_last_layer = load_blob_data(bottom_file)  # load bottom data from file
    bottom = preprocess_bottom(bottom, bw_layer_in, fl_layer_in, unsigned_in, fl_last_layer)

    top_blob = copy.deepcopy(bottom)
    top_blob = np.maximum(top_blob, 0)

    top_blob = postprocess_top(top_blob, bw_layer_out, fl_layer_out, unsigned_out, fl_layer_in, 0)
    record_blob(top_blob, bw_layer_out, fl_layer_out, unsigned_out, simulate_top_file)
    record_blob(top_blob, bw_layer_out, fl_layer_out, unsigned_out, simulate_top_file)
    return

blob_layer_map = dict()
class layer_def:
    def __init__(self):
        self.name = ""  #name is the same with top
        self.type = ""  #layers' type
        self.bottom = list() # the bottom bolb of this layer
        self.bottom_shape = list()
        self.top = list() #top blobs of this layer
        self.top_shape = list()
        return

def load_net_def(net_struct_def_file):
    net_struct_def = list()
    with open(net_struct_def_file) as filer:
        lines = filer.readlines()
        for line in lines:
            layer = layer_def()
            line_strs = line.strip("\n").split("\t")
            layer.name = line_strs[0]
            layer.type = line_strs[1]
            bottom_num = int(line_strs[2])
            for i in range(0, bottom_num):
                bottom_blob_name = line_strs[3 + i * 4]
                layer.bottom.append(bottom_blob_name)
                if bottom_blob_name in blob_layer_map.keys():
                    if layer.name not in blob_layer_map[bottom_blob_name]:
                        blob_layer_map[bottom_blob_name].append(layer.name)
                else:
                    blob_layer_map[bottom_blob_name] = []
                    blob_layer_map[bottom_blob_name].append(layer.name)
                layer.bottom_shape.append([int(line_strs[3 + i * 4 + 1]), int(line_strs[3 + i * 4 + 2]), int(line_strs[3 + i * 4 + 3])])
            top_num = int(line_strs[3+bottom_num*4])
            for i in range(0, top_num):
                top_blob_name = line_strs[4 + bottom_num * 4 + i * 4]
                layer.top.append(top_blob_name)
                layer.top_shape.append([int(line_strs[4 + bottom_num * 4 + i*4 + 1]), int(line_strs[4 + bottom_num * 4 + i * 4  + 2]), int(line_strs[4 + bottom_num * 4 + i * 4  + 3])])
            net_struct_def.append(layer)

    return net_struct_def

def simulation_using_net_def(param_folder, img_output_folder):
    net_struct_def = load_net_def(param_folder+"/net_struct_def.txt")
    for layer in net_struct_def:
        if layer.type == "ConvolutionRistretto":
            assert (len(layer.bottom)==1)
            param_file = param_folder+"/param_"+layer.name+".txt"
            img_bottom_file = img_output_folder+"/top_"+layer.bottom[0]+".txt"
            img_top_file = img_output_folder+"/top_"+layer.top[0]+".txt"
            simulate_top_file = img_output_folder+"/sim_"+layer.top[0]+".txt"
            compare_top_file = img_output_folder+"/com_"+layer.top[0]+".txt"
            simulate_ConvolutionRistretto(img_bottom_file, param_file, simulate_top_file)
            checked_res = compare_blob_file(simulate_top_file, img_top_file, compare_top_file)
            print_check_res(checked_res, layer.name)
        elif layer.type == "DeconvolutionRistretto":
            assert (len(layer.bottom)==1)
            param_file = param_folder+"/param_"+layer.name+".txt"
            img_bottom_file = img_output_folder+"/top_"+layer.bottom[0]+".txt"
            img_top_file = img_output_folder+"/top_"+layer.top[0]+".txt"
            simulate_top_file = img_output_folder+"/sim_"+layer.top[0]+".txt"
            compare_top_file = img_output_folder+"/com_"+layer.top[0]+".txt"
            simulate_DeconvolutionRistretto(img_bottom_file, param_file, simulate_top_file)
            checked_res = compare_blob_file(simulate_top_file, img_top_file, compare_top_file)
            print_check_res(checked_res, layer.name)
        elif layer.type == "Pooling":
            assert (len(layer.bottom) == 1)
            param_file = param_folder+"/param_"+layer.name+".txt"
            img_bottom_file = img_output_folder+"/top_"+layer.bottom[0]+".txt"
            img_top_file = img_output_folder+"/top_"+layer.top[0]+".txt"
            simulate_top_file = img_output_folder+"/sim_"+layer.top[0]+".txt"
            compare_top_file = img_output_folder+"/com_"+layer.top[0]+".txt"
            simulate_Pooling(img_bottom_file, param_file, simulate_top_file)
            checked_res = compare_blob_file(simulate_top_file, img_top_file, compare_top_file)
            print_check_res(checked_res, layer.name)
        elif layer.type == "InnerProductRistretto":
            assert (len(layer.bottom) == 1)
            param_file = param_folder+"/param_"+layer.name+".txt"
            img_bottom_file = img_output_folder+"/top_"+layer.bottom[0]+".txt"
            img_top_file = img_output_folder+"/top_"+layer.top[0]+".txt"
            simulate_top_file = img_output_folder+"/sim_"+layer.top[0]+".txt"
            compare_top_file = img_output_folder+"/com_"+layer.top[0]+".txt"
            simulate_InnerProductRistretto(img_bottom_file, param_file, simulate_top_file)
            checked_res = compare_blob_file(simulate_top_file, img_top_file, compare_top_file)
            print_check_res(checked_res, layer.name)
        elif layer.type == "EltwiseRistretto":
            assert (len(layer.bottom) > 1)
            param_file = param_folder + "/param_" + layer.name + ".txt"
            img_bottom_files = list()
            for idx in range(0, len(layer.bottom)):
                img_bottom_file = img_output_folder + "/top_" + layer.bottom[idx] + ".txt"
                img_bottom_files.append(img_bottom_file)

            img_top_file = img_output_folder + "/top_" + layer.top[0] + ".txt"
            simulate_top_file = img_output_folder + "/sim_" + layer.top[0] + ".txt"
            compare_top_file = img_output_folder + "/com_" + layer.top[0] + ".txt"
            simulate_EltwiseRistretto(img_bottom_files, param_file, simulate_top_file)
            checked_res = compare_blob_file(simulate_top_file, img_top_file, compare_top_file)
            print_check_res(checked_res, layer.name)
        elif layer.type == "Concat":
            assert (len(layer.bottom) > 1)
            param_file = param_folder + "/param_" + layer.name + ".txt"
            img_bottom_files = list()
            for idx in range(0, len(layer.bottom)):
                img_bottom_file = img_output_folder + "/top_" + layer.bottom[idx] + ".txt"
                img_bottom_files.append(img_bottom_file)

            img_top_file = img_output_folder + "/top_" + layer.top[0] + ".txt"
            simulate_top_file = img_output_folder + "/sim_" + layer.top[0] + ".txt"
            compare_top_file = img_output_folder + "/com_" + layer.top[0] + ".txt"
            simulate_Concat(img_bottom_files, param_file, simulate_top_file)
            checked_res = compare_blob_file(simulate_top_file, img_top_file, compare_top_file)
            print_check_res(checked_res, layer.name)
        elif layer.type == "Flatten":
            assert (len(layer.bottom) == 1)
            param_file = param_folder + "/param_" + layer.name + ".txt"
            img_bottom_file = img_output_folder + "/top_" + layer.bottom[0] + ".txt"
            img_top_file = img_output_folder + "/top_" + layer.top[0] + ".txt"
            simulate_top_file = img_output_folder + "/sim_" + layer.top[0] + ".txt"
            compare_top_file = img_output_folder + "/com_" + layer.top[0] + ".txt"
            simulate_Flatten(img_bottom_file, param_file, simulate_top_file)
            checked_res = compare_blob_file(simulate_top_file, img_top_file, compare_top_file)
            print_check_res(checked_res, layer.name)
        elif layer.type == "ReLU":
            assert (len(layer.bottom) == 1)
            param_file = param_folder + "/param_" + layer.name + ".txt"
            img_bottom_file = img_output_folder + "/top_" + layer.bottom[0] + ".txt"
            img_top_file = img_output_folder + "/top_" + layer.top[0] + ".txt"
            simulate_top_file = img_output_folder + "/sim_" + layer.top[0] + ".txt"
            compare_top_file = img_output_folder + "/com_" + layer.top[0] + ".txt"
            simulate_ReLU(img_bottom_file, param_file, simulate_top_file)
            checked_res = compare_blob_file(simulate_top_file, img_top_file, compare_top_file)
            print_check_res(checked_res, layer.name)
        elif layer.type == "Buffer":
            pass
        else:
            print("do not support checking layer: " + layer.name)


if __name__ == "__main__":
    param_folder = "./test_imgs/lane_segmentation/16bit/net_params/"
    img_output_folder = "./test_imgs/lane_segmentation/16bit/5/"
    simulation_using_net_def(param_folder, img_output_folder)


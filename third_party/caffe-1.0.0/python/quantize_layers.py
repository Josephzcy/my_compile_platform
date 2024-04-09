"""
Copyright (c) 2018-present, minieye.
All rights reserved.

This source code is licensed under the BSD-style license found in the
LICENSE file in the root directory of this source tree. An additional grant
of patent rights can be found in the PATENTS file in the same directory.
"""

import math
import numpy as np
import sys
import os
import cv2
import copy
sys.path.append("/data1/wzheng/projects/caffe-quant-master/python/")
import caffe
from caffe.proto import caffe_pb2 as pb2
from google.protobuf import text_format

caffe.set_device(1)
caffe.set_mode_gpu()

input_unsigned = dict()
output_unsigned = dict()
input_bw = dict()
input_fl = dict()
output_bw = dict()
output_fl = dict()
param_bw = dict()
param_fl = dict()

predefined_input_bw = 8
predefined_output_bw = 8
predefined_param_bw = 16
unsigned_layers = ['Input', 'Data', 'Pooling', 'Concat', 'Flatten']
signed_layers = ['InnerProduct', 'Eltwise', 'Deconvolution', 'Python']
def build_converter(opts):
    return {
        'Python': unchange,
        'Data': unchange,
        'Input': unchange,
        'Softmax': unchange,
        'Pooling': normal,
        'Concat': normal,
        'Dropout': normal,
        'Flatten': normal,
        'Slice': normal,
        'ReLU': normal,
        'Hardtanh': normal,
        'InnerProduct': inner_product,
        'Convolution': convolution,
        'Deconvolution': deconvolution,
        'Eltwise': eltwise
    }

def gci(filepath):
    def __gci(filepath):
        files = os.listdir(filepath)
        for fi in files:
            fi_d = os.path.join(filepath, fi)
            if os.path.isdir(fi_d):
                __gci(fi_d)
            else:
                if os.path.splitext(fi_d)[1] == '.jpg':
                    file_list.append(os.path.join(filepath,fi_d))

    file_list = []
    __gci(filepath)
    return file_list

def add_quantized_information(caffe_float_layer):
    caffe_fix_layer = caffe_float_layer
    caffe_fix_layer.type = caffe_fix_layer.type+"Ristretto"
    caffe_fix_layer.quantization_param.unsigned_in = input_unsigned[caffe_fix_layer.name]
    caffe_fix_layer.quantization_param.unsigned_out = output_unsigned[caffe_fix_layer.name]
    caffe_fix_layer.quantization_param.bw_layer_in = input_bw[caffe_fix_layer.name]
    caffe_fix_layer.quantization_param.fl_layer_in = input_fl[caffe_fix_layer.name]
    caffe_fix_layer.quantization_param.bw_layer_out = output_bw[caffe_fix_layer.name]
    caffe_fix_layer.quantization_param.fl_layer_out = output_fl[caffe_fix_layer.name]
    caffe_fix_layer.quantization_param.bw_params = param_bw[caffe_fix_layer.name]
    caffe_fix_layer.quantization_param.fl_params = param_fl[caffe_fix_layer.name]
    if len(caffe_fix_layer.param._values) == 2:
        caffe_fix_layer.param[0].lr_mult = 0
        caffe_fix_layer.param[0].decay_mult = 0
        caffe_fix_layer.param[1].lr_mult = 0
        caffe_fix_layer.param[1].decay_mult = 0
    if len(caffe_fix_layer.param._values) == 1:
        caffe_fix_layer.param[0].lr_mult = 0
        caffe_fix_layer.param[0].decay_mult = 0

    caffe_fix_layer.propagate_down._values=[]
    for i in range(0, len(caffe_float_layer.bottom)):
        caffe_fix_layer.propagate_down._values.append(0)
    return caffe_fix_layer


def convolution(caffe_float_layer):
    caffe_fix_layer = add_quantized_information(caffe_float_layer)
    return caffe_fix_layer

def deconvolution(caffe_float_layer):
    caffe_fix_layer = add_quantized_information(caffe_float_layer)
    return caffe_fix_layer

def inner_product(caffe_float_layer):
    caffe_fix_layer = add_quantized_information(caffe_float_layer)
    return caffe_fix_layer

def eltwise(caffe_float_layer):
    caffe_fix_layer = add_quantized_information(caffe_float_layer)
    return caffe_fix_layer

def normal(caffe_float_layer):
    caffe_fix_layer = caffe_float_layer
    caffe_fix_layer.quantization_param.unsigned_in = input_unsigned[caffe_fix_layer.name]
    caffe_fix_layer.quantization_param.unsigned_out = output_unsigned[caffe_fix_layer.name]
    caffe_fix_layer.quantization_param.bw_layer_in = input_bw[caffe_fix_layer.name]
    caffe_fix_layer.quantization_param.fl_layer_in = input_fl[caffe_fix_layer.name]
    caffe_fix_layer.quantization_param.bw_layer_out = output_bw[caffe_fix_layer.name]
    caffe_fix_layer.quantization_param.fl_layer_out = output_fl[caffe_fix_layer.name]
    caffe_fix_layer.quantization_param.bw_params = param_bw[caffe_fix_layer.name]
    caffe_fix_layer.quantization_param.fl_params = param_fl[caffe_fix_layer.name]
    caffe_fix_layer.propagate_down._values=[]
    for i in range(0, len(caffe_float_layer.bottom)):
        caffe_fix_layer.propagate_down._values.append(0)
    return caffe_fix_layer

def unchange(caffe_float_layer):
    return caffe_float_layer

def convert_caffe(typename, caffe_layer):
    converter = build_converter(typename)
    #print typename
    if typename not in converter:
        raise ValueError("Unknown layer type: {}, known types: {}".format(
            typename, converter.keys()))
    return converter[typename](caffe_layer)

def init_range_dict(net_proto):
    max_val = 1000000
    input_range = dict()
    output_range = dict()
    param_range = dict()
    for layer in net_proto.layer._values:
        layer_name = layer.name
        input_range[layer_name] = list()
        output_range[layer_name] = list()
        param_range[layer_name] = list()
        input_range[layer_name].append(max_val)
        input_range[layer_name].append(-max_val)
        output_range[layer_name].append(max_val)
        output_range[layer_name].append(-max_val)
        param_range[layer_name].append(max_val)
        param_range[layer_name].append(-max_val)
    return input_range, output_range, param_range

def find_layer_range(net, net_proto, input_range, output_range, param_range):
    for layer in net_proto.layer:
        layer_name = layer.name
        if layer_name in net.params.keys():
            #for weights
            max_val = max(net.params[layer_name][0].data.flatten())
            min_val = min(net.params[layer_name][0].data.flatten())
            if min_val < param_range[layer_name][0]:
                param_range[layer_name][0] = min_val
            if max_val > param_range[layer_name][1]:
                param_range[layer_name][1] = max_val
            #for bias
            if len(net.params[layer_name])>1:
                max_val = max(net.params[layer_name][1].data.flatten())
                min_val = min(net.params[layer_name][1].data.flatten())
                if min_val < param_range[layer_name][0]:
                    param_range[layer_name][0] = min_val
                if max_val > param_range[layer_name][1]:
                    param_range[layer_name][1] = max_val

    max_val = dict()
    min_val = dict()
    for layer in net_proto.layer:
        if layer.type == "ReLU" and layer.top[0] == layer.bottom[0]:#in-place ReLU
            layer_name = layer.bottom[0]
            max_val[layer_name] = -1000000
            min_val[layer_name] = 1000000
            max_tmp = max(net.blobs[layer_name].data.flatten())
            min_tmp = min(net.blobs[layer_name].data.flatten())
            if min_tmp < min_val[layer_name]:
                min_val[layer_name] = min_tmp
            if max_tmp > max_val[layer_name]:
                max_val[layer_name] = max_tmp
        else:
            layer_name = layer.name
            max_val[layer_name] = -1000000
            min_val[layer_name] = 1000000
            max_tmp = max(net.blobs[layer_name].data.flatten())
            min_tmp = min(net.blobs[layer_name].data.flatten())
            if min_tmp < min_val[layer_name]:
                min_val[layer_name] = min_tmp
            if max_tmp > max_val[layer_name]:
                max_val[layer_name] = max_tmp

    for layer in net_proto.layer:
        layer_name = layer.name
        top_names = layer.top._values
        bottom_names = layer.bottom._values
        for top_name in top_names:
            if min_val[top_name] < output_range[layer_name][0]:
                output_range[layer_name][0] = min_val[top_name]
            if max_val[top_name] > output_range[layer_name][1]:
                output_range[layer_name][1] = max_val[top_name]
        for bottom_name in bottom_names:
            if min_val[bottom_name] < input_range[layer_name][0]:
                input_range[layer_name][0] = min_val[bottom_name]
            if max_val[bottom_name] > input_range[layer_name][1]:
                input_range[layer_name][1] = max_val[bottom_name]



def cal_bit_from_range(bw, float_range, unsigned_flag=False):  #treat all layers as signed numbers
    min_val, max_val = float_range
    if unsigned_flag == False:
        pow_f = math.log(max(abs(max_val),abs(min_val)), 2)
        pow_i = math.ceil(pow_f)
        fl = bw - 1 - pow_i
    else:
        pow_f = math.log(abs(max(max_val,0.000001)), 2)
        pow_i = math.ceil(pow_f)
        fl = bw - pow_i
    return int(fl)

def run_forward(net, img_lst, net_proto, input_range, output_range, param_range):
    idx = 0
    for img_path in img_lst:
        img = cv2.imread(img_path)
        img_input = img.astype(float) / 255.0
        img_input = img_input.transpose(2, 0, 1)
        net.blobs['data'].data[...] = img_input
        net.forward()
        find_layer_range(net, net_proto, input_range, output_range, param_range)
        print("processing ", idx, "th image:",  img_path)
        idx+=1
        # if idx>2:
        #     break
    return net

def quantize_net(prototxt_path, model_path, img_lst,prototxt_path_quantized):
    warning_info = []
    net = caffe.Net(prototxt_path, model_path, caffe.TEST)
    net_proto = pb2.NetParameter()
    protofile = open(prototxt_path, 'r')
    text_format.Merge(protofile.read(),net_proto)
    protofile.close()

    input_range, output_range, param_range = init_range_dict(net_proto)
    run_forward(net, img_lst, net_proto, input_range, output_range, param_range)

    for layer in net_proto.layer:
        input_bw[layer.name] = predefined_input_bw
        output_bw[layer.name] = predefined_output_bw
        param_bw[layer.name] = predefined_param_bw
        param_fl[layer.name] = cal_bit_from_range(param_bw[layer.name], param_range[layer.name], False)
        #only the first convolution layer allow negative input
        if layer.type == "Convolution":
            for layer_bottom in net_proto.layer:
                if layer_bottom.name == layer.bottom[0]:
                    break
            if layer_bottom.type == 'Data' or layer_bottom.type == 'Input':
                input_unsigned[layer.name] = False
                output_unsigned[layer.name] = False
            else:
                input_unsigned[layer.name] = True
                output_unsigned[layer.name] = True
            input_fl[layer.name] = cal_bit_from_range(input_bw[layer.name], input_range[layer.name], input_unsigned[layer.name])
            output_fl[layer.name] = cal_bit_from_range(output_bw[layer.name], output_range[layer.name], output_unsigned[layer.name])
        elif layer.type == "ReLU": #special layers (unsigned or signed, in-place or not in-place)
            input_fl[layer.name] = output_fl[layer.bottom[0]]
            output_fl[layer.name] = input_fl[layer.name]
            input_unsigned[layer.name] = output_unsigned[layer.bottom[0]]
            output_unsigned[layer.name] = input_unsigned[layer.name]
        elif layer.type in signed_layers: #must be signed
            input_unsigned[layer.name] = False
            output_unsigned[layer.name] = False
            input_fl[layer.name] = cal_bit_from_range(input_bw[layer.name], input_range[layer.name], input_unsigned[layer.name])
            output_fl[layer.name] = cal_bit_from_range(output_bw[layer.name], output_range[layer.name], output_unsigned[layer.name])
        elif layer.type in unsigned_layers: #unsigned layers
            #input and output of pooling, concat, faltten are unsigned
            input_unsigned[layer.name] = True
            output_unsigned[layer.name] = True
            input_fl[layer.name] = cal_bit_from_range(input_bw[layer.name], input_range[layer.name], input_unsigned[layer.name])
            output_fl[layer.name] = cal_bit_from_range(output_bw[layer.name], output_range[layer.name], output_unsigned[layer.name])
        else:
            raise Exception("!!!warning: " + layer.name + " " + layer.type + " not defined signed or unsigned")


        print("##################################"+layer.name+"####################################\n")
        print("input:", input_bw[layer.name], input_fl[layer.name], input_range[layer.name][0], input_range[layer.name][1], "\n")
        print("output:", output_bw[layer.name], output_fl[layer.name], output_range[layer.name][0], output_range[layer.name][1], "\n")
        print("params:", param_bw[layer.name], param_fl[layer.name], param_range[layer.name][0], param_range[layer.name][1], "\n")

    for warning in warning_info:
        print(warning)

    for i in range(0, len(net_proto.layer._values)):
        typename = net_proto.layer._values[i].type
        net_proto.layer._values[i] = convert_caffe(typename, net_proto.layer._values[i])

    new_proto_file = open(prototxt_path_quantized, "w")
    new_proto_file.write(str(net_proto))
    new_proto_file.close()

#if layer_top_name!=layer_name, change layer top name to layer name
def regularize_prototxt(prototxt_path):
    net_proto = pb2.NetParameter()
    protofile = open(prototxt_path, 'r')
    text_format.Merge(protofile.read(),net_proto)
    protofile.close()
    for layer in net_proto.layer:
        assert (len(layer.top) == 1)
        if layer.top[0]!=layer.name:
            old_top_name = layer.top[0]
            layer.top[0] = layer.name
            for (i, layer_tmp) in enumerate(net_proto.layer):
                if layer_tmp.name == layer.name:
                    continue
                for (j,layer_bottom) in enumerate(layer_tmp.bottom):
                    if layer_bottom == old_top_name:
                        net_proto.layer[i].bottom[j] = layer.name

    protofile = open(prototxt_path, "w")
    protofile.write(str(net_proto))
    protofile.close()


def remove_inplace_append_buffer(prototxt_path,net_buffer_prototxt):
    net_proto = pb2.NetParameter()
    protofile = open(prototxt_path, 'r')
    text_format.Merge(protofile.read(),net_proto)
    protofile.close()

    #replace in-place ReLU and make sure there is no in-place layers
    for layer in net_proto.layer:
        if layer.type == "ReLU" and layer.bottom[0] == layer.top[0]:
            layer.top[0] = layer.bottom[0]+"_relu"
            idx = 0
            for layer_top in net_proto.layer:
                if layer_top.name == layer.name or len(layer_top.bottom)==0:
                    idx += 1
                    continue
                if layer.bottom[0] in layer_top.bottom:
                    for bottom_idx in range(0, len(layer_top.bottom)):
                        if layer_top.bottom[bottom_idx] == layer.bottom[0]:
                            net_proto.layer[idx].bottom[bottom_idx] = layer.top[0]
                idx += 1

    net_buffer_proto = copy.deepcopy(net_proto)
    net_buffer_proto.layer._values=[]
    for layer in net_proto.layer:
        net_buffer_proto.layer._values.append(layer)
        top_layer_idx = 0
        branch_idx = 0
        for layer_top in net_proto.layer:
            top_bottom_idx = 0
            for top_bottom in layer_top.bottom:
                if top_bottom == layer.top[0]:
                    layer_buffer = pb2.LayerParameter()
                    layer_buffer.type = "Buffer"
                    layer_buffer.name = layer.name + ("_buffer_%03d" % branch_idx)
                    layer_buffer.bottom.append(layer.top[0])
                    layer_buffer.top.append(layer_buffer.name)
                    net_proto.layer[top_layer_idx].bottom[top_bottom_idx] = layer_buffer.top[0]
                    net_buffer_proto.layer._values.append(layer_buffer)
                    branch_idx += 1
                top_bottom_idx += 1
            top_layer_idx += 1

    new_proto_file = open(net_buffer_prototxt, "w")
    new_proto_file.write(str(net_buffer_proto))
    new_proto_file.close()

if __name__ == "__main__":
    image_path = '/data1/ymli/datasets/lane_basler_nanjing_2W_small/images/train/nanjing_499'
    image_list = gci(image_path)
    image_list.sort()
    # quantize_net("/data1/wzheng/projects/caffe-quant-master/examples/quantization_lane_seg/HCSegNetSmall.prototxt",
    #              "/data1/wzheng/projects/caffe-quant-master/examples/quantization_lane_seg/HCSegNetSmall.caffemodel",
    #              image_list,
    #              "/data1/wzheng/projects/caffe-quant-master/examples/quantization_lane_seg/HCSegNetSmall_quantized.prototxt")

    float_prototxt = "../examples/quantization_lane_seg/HCSegNetSmall.prototxt"
    float_model = "../examples/quantization_lane_seg/HCSegNetSmall.caffemodel"
    quantize_prototxt = float_prototxt+"_quantized.prototxt"
    quantize_refined_prototxt = quantize_prototxt+"_remove_inplace_add_buffer.prototxt"
    regularize_prototxt(float_prototxt)
    quantize_net(float_prototxt,float_model,image_list,quantize_prototxt)
    remove_inplace_append_buffer(quantize_prototxt, quantize_refined_prototxt)
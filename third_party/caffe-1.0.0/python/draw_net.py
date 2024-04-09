#!/usr/bin/env python
"""
Draw a graph of the net architecture.
"""
from argparse import ArgumentParser, ArgumentDefaultsHelpFormatter
from google.protobuf import text_format

import caffe
import caffe.draw
from caffe.proto import caffe_pb2
import infer_complexity
import os

def parse_args():
    """Parse input arguments
    """

    parser = ArgumentParser(description=__doc__,
                            formatter_class=ArgumentDefaultsHelpFormatter)

    parser.add_argument('input_net_proto_file',
                        help='Input network prototxt file')
    parser.add_argument('output_image_file',
                        help='Output image file')
    parser.add_argument('--rankdir',
                        help=('One of TB (top-bottom, i.e., vertical), '
                              'RL (right-left, i.e., horizontal), or another '
                              'valid dot option; see '
                              'http://www.graphviz.org/doc/info/'
                              'attrs.html#k:rankdir'),
                        default='LR')

    args = parser.parse_args()
    return args

def load_profiler_file(profiler_path):
    profiler_dict = dict()
    if os.access(profiler_path, os.R_OK) == False:
        return
    with open(profiler_path, "r") as profiling_file:
        lines = profiling_file.readlines()
        for line in lines:
            strs = line.split(" ")
            if strs[0] in profiler_dict.keys():
                profiler_dict[strs[0]] += np.double(strs[1])
            else:
                profiler_dict[strs[0]] = np.double(strs[1])
    total_time = 0
    for key_word in profiler_dict.keys():
        total_time += profiler_dict[key_word]
    for key_word in profiler_dict.keys():
        profiler_dict[key_word] /= total_time
    profiler_dict["###all_layers_processing_time###"] = total_time/(len(lines)/(len(profiler_dict.keys())-1)+0.00000001)
    return profiler_dict


def main():
    args = parse_args()
    net = caffe_pb2.NetParameter()
    text_format.Merge(open(args.input_net_proto_file).read(), net)
    print('Drawing net to %s' % args.output_image_file)
    infer_complexity.infer_complexity(args.input_net_proto_file)
    caffe.draw.draw_net_to_file(net, args.output_image_file, args.rankdir)


if __name__ == '__main__':
    input_net_prototxt = "/data1/wzheng/projects/caffe-quant-seg-sparse-master/examples/quantization_for_7020_8bit/LDW/road_net_4_tasks/models/roadNet.prototxt"
    output_image_file = "/data1/wzheng/projects/caffe-quant-seg-sparse-master/examples/quantization_for_7020_8bit/LDW/road_net_4_tasks/models/roadNet.png"
    #input_net_prototxt = "/data/wzheng/projects/caffe-quant-master/examples/car_type_proj_improved/classification_sm_res_regulized_deploy.prototxt"
    #input_net_prototxt = "/data/wzheng/projects/caffe-quant-master/examples/car_regression_simplenet/train_val_deploy.prototxt"
    #input_net_prototxt = "/data/wzheng/projects/caffe-quant-master/examples/car_regression_proj/regression_baseline_deploy.prototxt"
    #input_net_prototxt = "/data/wzheng/projects/caffe-quant-master/examples/mtcnn_side_person/stage_1/12net.prototxt"
    #input_net_prototxt = "/data/wzheng/projects/caffe-quant-master/examples/mtcnn_side_person/stage_1_24_online/net_deploy_stage_1_group.prototxt"
    #input_net_prototxt = "/data/wzheng/projects/caffe-quant-master/examples/car_regression_proj_wczhang_quantiize/ristretto_16/wczhang_normOk1_sm_res_quantized_deploy.prototxt"
    #output_image_file = "/data/wzheng/projects/caffe-quant-master/examples/mtcnn_side_person/stage_1_24_online/stage_1_group_24.png"
    last_tops = ["Permute_8", "Permute_6", "Permute_4", "Addmm_1"]
    profiler_file = " "#""//data/wzheng/projects/caffe-quant-master/models/vgg/layer_profiler_VGG16.txt"
    profiler_dict = load_profiler_file(profiler_file)
    #last_tops = ["conv3"]
    net = caffe_pb2.NetParameter()
    text_format.Merge(open(input_net_prototxt).read(), net)
    print("Drawing net to %s" % output_image_file)
    total_complexity, total_model_size, complexity_dict, model_size_dict, shape_dict  \
        = infer_complexity.infer_complexity(input_net_prototxt, last_tops)
    caffe.draw.draw_net_to_file(net, output_image_file, complexity_dict, model_size_dict, shape_dict, profiler_dict)

import sys
import copy

sys.path.append('/data/wzheng/projects/caffe-quant-master/python')

import cv2
import caffe
import numpy as np
import random

import regression_benchmark

caffe.set_mode_gpu()
caffe.set_device(0)

def init_model(net_proto, net_weights):
    net = caffe.Net(net_proto, net_weights, caffe.TEST)
    return net

def visualize_paint(img_color, x, y, res_w, res_h, R, G, B):
    x[0] = x[0]
    y[0] = y[0]
    x[1] = x[1]
    y[1] = y[1]
    x[2] = res_w - x[2]
    y[2] = res_h - y[2]
    x[3] = x[3]
    y[3] = res_h - y[3]

    img_color[y[0] - 2:y[0] + 3, x[0] - 2:x[0] + 3, 2] = R
    img_color[y[0] - 2:y[0] + 3, x[0] - 2:x[0] + 3, 1] = G
    img_color[y[0] - 2:y[0] + 3, x[0] - 2:x[0] + 3, 0] = B

    img_color[y[1] - 2:y[1] + 3, x[1] - 2:x[1] + 3, 2] = R
    img_color[y[1] - 2:y[1] + 3, x[1] - 2:x[1] + 3, 1] = G
    img_color[y[1] - 2:y[1] + 3, x[1] - 2:x[1] + 3, 0] = B

    img_color[y[2] - 2:y[2] + 3, x[2] - 2:x[2] + 3, 2] = R
    img_color[y[2] - 2:y[2] + 3, x[2] - 2:x[2] + 3, 1] = G
    img_color[y[2] - 2:y[2] + 3, x[2] - 2:x[2] + 3, 0] = B

    img_color[y[3] - 2:y[3] + 3, x[3] - 2:x[3] + 3, 2] = R
    img_color[y[3] - 2:y[3] + 3, x[3] - 2:x[3] + 3, 1] = G
    img_color[y[3] - 2:y[3] + 3, x[3] - 2:x[3] + 3, 0] = B

def visualize_paint_14pts(img_color, x, y, R, G, B):

    for i in xrange(14):
        img_color[y[i] - 2:y[i] + 3, x[i] - 2:x[i] + 3, 2] = R
        img_color[y[i] - 2:y[i] + 3, x[i] - 2:x[i] + 3, 1] = G
        img_color[y[i] - 2:y[i] + 3, x[i] - 2:x[i] + 3, 0] = B



def visualize_res(img_path, res, img_label, res_w, res_h):

    img_color = cv2.imread(img_path, cv2.IMREAD_COLOR)
    img_color = cv2.resize(img_color, dsize=(res_w, res_h), interpolation=cv2.INTER_LINEAR)
    x = np.zeros(shape=(4),dtype=np.int)
    y = np.zeros(shape=(4),dtype=np.int)
    x_label = np.zeros(shape=(4),dtype=np.int)
    y_label = np.zeros(shape=(4),dtype=np.int)
    for i in range(0,4):
        x[i] = round(res[0,i*2+1]*res_w)
        y[i] = round(res[0,i*2+2]*res_h)
    for i in range(0, 4):
        x_label[i] = round(img_label[i * 2 + 1] * res_w)
        y_label[i] = round(img_label[i * 2 + 2] * res_h)
    visualize_paint(img_color, x, y, res_w, res_h, 255, 0, 0)
    visualize_paint(img_color, x_label, y_label, res_w, res_h, 0, 255, 0)
    print res
    print img_label
    print abs(res[0,0]-img_label[0])/img_label[0]

    cv2.imshow("res", img_color)
    cv2.waitKey(-1)

def visualize_res_14pts(img_path, res, img_label, res_w, res_h):

    img_color = cv2.imread(img_path, cv2.IMREAD_COLOR)
    img_color = cv2.resize(img_color, dsize=(res_w, res_h), interpolation=cv2.INTER_LINEAR)
    x = np.zeros(shape=(14),dtype=np.int)
    y = np.zeros(shape=(14),dtype=np.int)
    x_label = np.zeros(shape=(14),dtype=np.int)
    y_label = np.zeros(shape=(14),dtype=np.int)
    for i in range(0,14):
        x[i] = round((res[0,i*2+0]+0.5)*res_w)
        y[i] = round((res[0,i*2+1]+0.5)*res_h)
    for i in range(0, 14):
        x_label[i] = round((img_label[i * 2 + 0]+0.5) * res_w)
        y_label[i] = round((img_label[i * 2 + 1]+0.5) * res_h)
    visualize_paint_14pts(img_color, x, y, 255, 0, 0)
    visualize_paint_14pts(img_color, x_label, y_label, 0, 255, 0)
    print res
    print img_label
    print abs(res[0,0]-img_label[0])/img_label[0]

    cv2.imshow("res", img_color)
    cv2.waitKey(-1)


def predict_gray(net, img_path, resize_w=-1, resize_h=-1, mean_val=128.0):

    img_cv = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)
    img = copy.deepcopy(img_cv)
    if resize_h>0 and resize_w>0:
        img = cv2.resize(img, dsize=(resize_w, resize_h), interpolation=cv2.INTER_LINEAR)
    img = img - mean_val
    img = np.expand_dims(img, axis=0)
    net.blobs['Data1'].data[...] = img
    result = copy.deepcopy(net.forward()["InnerProduct2"])
    return result

def load_label_lst(file_lst, img_root_path, shuffle=True):
    label_file = open(file_lst, "r")
    info_lines = label_file.readlines()
    if shuffle is True:
        random.shuffle(info_lines)
    img_paths = list()
    img_labels = list()
    for info_line in info_lines:
        strs = info_line.split("\t")
        label_str = info_line[len(strs[0])+1:-1]
        img_paths.append(img_root_path+"/"+strs[0])
        img_labels.append(np.fromstring(label_str,sep=" ", dtype=np.float))
    label_file.close()
    return img_paths, img_labels

def record_res(result_path, paths, labels, ress):
    result_file = open(result_path, "w")
    for path, label, res in zip(paths, labels, ress):
        line = path
        for i in xrange(len(label)):
            line += ("\t%f" % label[i])
        for i in xrange(res.shape[1]):
            line += ("\t%f" % res[0,i])
        line += "\n"
        result_file.write(line)
    result_file.close()


if __name__ == "__main__":
    caffenet = init_model("/data/wzheng/projects/caffe-quant-master/examples/car_regression_proj/regression_baseline_deploy.prototxt", \
                          "/data/wzheng/projects/caffe-quant-master/examples/car_regression_proj/models/box_car_regression_iter_240000.caffemodel")
    img_paths, img_labels = load_label_lst("/data/wzheng/data/car_regression_80/box_80_128_14pts_test.txt", \
                            "/data/wzheng/data/car_regression_80/box_80_img_test", shuffle=False)
    res_lst = list()
    label_lst = list()
    path_lst = list()
    i = 0
    for img_path, img_label in zip(img_paths, img_labels):
        result = predict_gray(caffenet, img_path, resize_w=80, resize_h=80)
        #visualize_res_14pts(img_path, result, img_label, 400, 400)
        res_lst.append(result)
        label_lst.append(img_label)
        path_lst.append(img_path)
        i +=1
        if i%1000 == 0:
            print "processed %d images" % i
    record_res("regression_caffe_baseline_result.txt", path_lst, label_lst, res_lst)
    regression_benchmark.evaluate()

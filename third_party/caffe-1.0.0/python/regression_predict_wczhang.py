import sys
import copy

# sys.path.append('/data/wczhang/open_tools/caffe-new-quant/python')
sys.path.append('/data/wzheng/projects/caffe-quant-master/python')


import cv2
import caffe
import numpy as np
import random

import regression_benchmark

# mean_value = 137.11#109.82637
mean_value = 117.38

caffe.set_mode_gpu()
caffe.set_device(3)

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

def visualize_res_14pts(img_path, res, img_label, res_w, res_h,res_path='',res_str=''):

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
    error = abs(res[0,-1]-img_label[-1])/img_label[-1]
    print error
    if error > 0:
        # names = img_path.split("/")
        # imagename = res_path + res_str + "_"+names[-1]
        # cv2.imwrite(imagename, img_color)

        xw = ((x[1] + x[2] + x[3] + x[4])/4.0 - (x[0]+x[9]+x[8]+x[7])/4.0)/res_w
        xerror = abs(xw -img_label[-1])/img_label[-1]
        print xerror,img_path
        cv2.imshow("res", img_color)
        cv2.waitKey(-1)



def predict_gray(net, img_path, resize_w=-1, resize_h=-1, mean_val=128.0,scale=1.0):

    img_cv = cv2.imread(img_path, cv2.IMREAD_GRAYSCALE)
    # img_cv = cv2.imread(img_path,-1)
    # img_cv = cv2.cvtColor(img_cv,cv2.COLOR_BGR2GRAY)
    # img_cv = img_cv.astype(np.float32)
    img = copy.deepcopy(img_cv)
    if resize_h>0 and resize_w>0:
       img = cv2.resize(img, dsize=(resize_w, resize_h), interpolation=cv2.INTER_LINEAR)
    # print img-mean_val
    img = (img - mean_val) * scale
    # print img
    img = np.expand_dims(img, axis=0)
    net.blobs['Data1'].data[...] = img
    result = copy.deepcopy(net.forward()["fc2"])#["InnerProduct2"])#["fc2"])#
    # result = copy.deepcopy(net.forward()["InnerProduct2"])#["fc2"])#
    return result

def load_label_lst(file_lst, img_root_path, shuffle=True):
    label_file = open(file_lst, "r")
    info_lines = label_file.readlines()
    if shuffle is True:
        random.shuffle(info_lines)
    img_paths = list()
    img_labels = list()
    for info_line in info_lines:
        # print info_line
        strs = info_line.split("\t")
        label_str = info_line[len(strs[0])+1:-len(strs[-1])-1]
        # print label_str
        # break
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


def load_orilabel_lst(file_lst, img_root_path, shuffle=True):
    label_file = open(file_lst, "r")
    info_lines = label_file.readlines()
    if shuffle is True:
        random.shuffle(info_lines)
    img_paths = list()
    img_labels = list()
    car_width = list()
    side = list()
    for info_line in info_lines:
        # print info_line
        strs = info_line.split("\t")
        label_str = info_line[len(strs[0])+1:-len(strs[-1])-1-len(strs[-2])-1 -len(strs[-3])-1]
        # print label_str
        # break
        img_paths.append(img_root_path+"/"+strs[0])
        img_labels.append(np.fromstring(label_str,sep=" ", dtype=np.float))
        car_width.append(float(strs[-4]) * target_size)
        side.append(int(strs[-2]))
    label_file.close()
    return img_paths, img_labels, car_width, side



def computeDiff(res, img_label):

    x = np.zeros(shape=(14),dtype=np.float32)
    y = np.zeros(shape=(14),dtype=np.float32)
    x_label = np.zeros(shape=(14),dtype=np.float32)
    y_label = np.zeros(shape=(14),dtype=np.float32)
    for i in range(0,14):
        x[i] = res[0,i*2+0]
        y[i] = res[0,i*2+1]
    for i in range(0, 14):
        x_label[i] = img_label[i * 2 + 0]
        y_label[i] = img_label[i * 2 + 1]

    error = abs(res[0,-1]-img_label[-1])/img_label[-1]

    xw = ((x[1] + x[2] + x[3] + x[4])/4.0 - (x[0]+x[9]+x[8]+x[7])/4.0)
    xw_r = ((x_label[1] + x_label[2] + x_label[3] + x_label[4])/4.0 - (x_label[0]+x_label[9]+x_label[8]+x_label[7])/4.0)
    xerror = abs(xw - xw_r)/xw_r

    return error, xerror

def computeYDiff(res, img_label):

    x = np.zeros(shape=(14),dtype=np.float32)
    y = np.zeros(shape=(14),dtype=np.float32)
    x_label = np.zeros(shape=(14),dtype=np.float32)
    y_label = np.zeros(shape=(14),dtype=np.float32)
    for i in range(0,14):
        x[i] = res[0,i*2+0]
        y[i] = res[0,i*2+1]
    for i in range(0, 14):
        x_label[i] = img_label[i * 2 + 0]
        y_label[i] = img_label[i * 2 + 1]

    bw = x_label[1] - x_label[0]
    bh = y_label[4] - y_label[1]

    ty = (y_label[5] + y_label[6])/2.0
    error = abs((y[5]+y[6]) - (y_label[5] + y_label[6]))/2.0*80

    return bh/bw, ty, error


target_size = 80

if __name__ == "__main__":
    # caffenet = init_model("regression_baseline_deploy.prototxt",# "box_car_regression_iter_120000_batch4_80_gray_aug.caffemodel")
    #                      "models/box_car_regression_iter_1000000.caffemodel")
    # caffenet = init_model("net22/wczhang_classification_sm_res_deploy.prototxt", \
    #                     "net22/good_model/cls80_iter_1200000.caffemodel")
    # caffenet = init_model("more4net22/wczhang_classification_sm_res_deploy.prototxt", \
    #                       "more4net22/model/cls80_iter_560000.caffemodel")

    # caffenet = init_model("banlance4net22/wczhang_classification_sm_res_deploy.prototxt", \
    #                       "banlance4net22/model/cls80_iter_2925000.caffemodel")

    # caffenet = init_model("banlance4net22/wczhang_classification_sm_res_deploy.prototxt", \
    #                       "banlance4net22/fromHead1/cls80_iter_2705000.caffemodel")
    #
    # caffenet = init_model("banlance4net22/wczhang_classification_sm_res_deploy.prototxt", \
    #                       "banlance4net22/ft/cls80_iter_2495000.caffemodel")

#----------------------------------above mean_value is 117.38
    # caffenet = init_model("banlance4net22/wczhang_classification_sm_res_deploy.prototxt", \
    #                       "banlance4net22/smodel/cls80_iter_2930000.caffemodel")

    # caffenet = init_model("normNet/wczhang_norm_sm_res_deploy.prototxt", \
    #                       "normNet/model/norm80_iter_2865000.caffemodel")

    # caffenet = init_model("normNet/wczhang_normOk1_sm_res_deploy.prototxt", \
    #                       "normNet/ok1model/norm80_iter_1780000.caffemodel")

    # caffenet = init_model("normNet/wczhang_normOk1_sm_res_deploy.prototxt", \
    #                       "normNet/outOk1model/norm80_iter_2545000.caffemodel")

    caffenet = init_model("/data/wzheng/projects/caffe-quant-master/examples/car_regression_proj_wczhang_quantiize/ristretto_16/wczhang_normOk1_sm_res_quantized_deploy.prototxt", \
                          "/data/wzheng/projects/caffe-quant-master/examples/car_regression_proj_wczhang_quantiize/ristretto_16/models/norm80_quantized_finetune_iter_1.caffemodel")
    #/data/wzheng/projects/caffe-quant-master/examples/car_regression_proj_wczhang_quantiize/ristretto_16/   norm80_iter_2545000.caffemodel norm80_quantized_finetune_iter_1000.caffemodel wczhang_normOk1_sm_res_quantized_deploy.prototxt
    # caffenet = init_model("tNormNet/classification_sm_res_regulized_deploy.prototxt", \
    #                       "tNormNet/model/norm80_iter_1700000.caffemodel")


    # img_paths, img_labels = load_label_lst("../box_regression/new_80_128_test_tag.txt", \
    #                                        "../box_regression/new_128_test", shuffle=False)
    # img_paths, img_labels = load_label_lst("../box_regression/128_tag_test.txt", \
    #                                        "../box_regression/128_img_test", shuffle=False)
    # img_paths, img_labels = load_label_lst("../box_regression/80_128_tag_test.txt", \
    #                          "../box_regression/80_img_test", shuffle=False)
    # img_paths, img_labels = load_label_lst("../box_regression/bak_aug80_128_tag.txt", \
    #                       "../box_regression/bak_aug80_img", shuffle=False)
    # img_paths, img_labels = load_label_lst("/data/wzheng/data/car_regression_80/box_80_128_14pts_aug.txt", \
    #                         "/data/wzheng/data/car_regression_80/box_80_img_aug", shuffle=False)

    # img_paths, img_labels = load_label_lst("../box_regression/stage1_train_test.txt", \
    #                                        "../box_regression/stage1_train_test80", shuffle=False)  # 9375 stage1_train_test80


    # img_paths, img_labels = load_label_lst("../box_regression/more_aug80_tag_new_correct.txt", \
    #                                        "../box_regression/more_aug80_img_new", shuffle=False)  # 38w stage1_train_test80

    img_paths, img_labels = load_label_lst("/data/wczhang/regression/box_regression/80_128_tag_test.txt", \
                           "/data/wczhang/regression/box_regression/80_img_test", shuffle=False)  # 631

    # img_paths, img_labels = load_label_lst("/data/wczhang/regression/box_regression/stage2_box_test_day_tag.txt", \
    #                                        "/data/wczhang/regression/box_regression/stage2_box_test_day80",
    #                                        shuffle=False)  # 273 day

    # img_paths, img_labels = load_label_lst("/data/wczhang/regression/box_regression/stage2_box_test_rain_tag.txt", \
    #                                        "/data/wczhang/regression/box_regression/stage2_box_test_rain80",
    #                                        shuffle=False)  # 297 rain

    # img_paths, img_labels, car_widths, sides = load_orilabel_lst("../box_regression/twice_reg_test_tag.txt", \
    #                                        "../box_regression/twice_reg_test",
    #                                        shuffle=False)  # 3K+

    #
    # img_paths, img_labels = load_label_lst("/data/wczhang/regression/box_regression/stage2_box_test_tag.txt", \
    #                                       "/data/wczhang/regression/box_regression/stage2_box_test",
    #                                       shuffle=False)  # 3k+ day  normal

    # img_paths, img_labels = load_label_lst("../night_box/night_box_toTest1_tag.txt", \
    #                                        "../night_box/night_box_toTest1",
    #                                        shuffle=False)  # 300 night  normal
    # img_paths, img_labels = load_label_lst("../night_box/night_box_toTest2_tag.txt", \
    #                                        "../night_box/night_box_toTest2",
    #                                        shuffle=False)  # 299 night  normal


    # img_paths, img_labels = load_label_lst("../box_regression/stage2_box_test_scale_tag.txt", \
    #                                        "../box_regression/stage2_box_test_scale",
    #                                        shuffle=False)  # 3k+ day  with 0.05y

    # img_paths, img_labels, car_widths, sides = load_orilabel_lst("../box_regression/stage2_truth_test_tag.txt", \
    #                                        "../box_regression/stage2_truth_test",
    #                                        shuffle=False)  # 3k+ day

    # img_paths, img_labels = load_label_lst("../../old_regression/box_regression/new_80_128_test_tag.txt", \
    #                          "../../old_regression/box_regression/new_80_test", shuffle=False)  # 11909

    # img_paths, img_labels, car_widths, sides = load_orilabel_lst("../box_regression/new_ori_test_tag.txt", \
    #                                       "../box_regression/new_ori80_test", shuffle=False)  # 7490

    # img_paths, img_labels, car_widths, sides = load_orilabel_lst("../box_regression/new_ori_pcls_test_tag.txt", \
    #                                        "../box_regression/new_ori80_pcls_test", shuffle=False)  # 7490 with pred cls

    # img_paths, img_labels, car_widths, sides = load_orilabel_lst("../box_regression/new_ori_truth_test_tag.txt", \
    #                                        "../box_regression/new_ori80_truth_test", shuffle=False)  # 7490 with truth

    #img_paths, img_labels, car_widths, sides = load_orilabel_lst("../box_regression/new_ori_reg_test_tag.txt", \
     #                                                            "../box_regression/new_ori80_reg_test",
      #                                                           shuffle=False)  # 7490 with reg res
    res_lst = list()
    label_lst = list()
    path_lst = list()
    i = 0

    fw = open('80_models1_ori_res.txt','w')
    yerrors = np.zeros((100,),np.float32)

    res_path = 'reg_res/y/'
    twoerr = 0
    yout = 0
    yen = 0
    cwen = 0
    cwout = 0
    # for img_path, img_label, car_width, side in zip(img_paths, img_labels, car_widths, sides):
    for img_path, img_label in zip(img_paths, img_labels):
        result = predict_gray(caffenet, img_path, resize_w=target_size, resize_h=target_size, mean_val=128.0,scale=0.0078125)
        result /= target_size
        # img_label /= target_size  # for test data
        # visualize_res_14pts(img_path, result, img_label, 400, 400)
        res_lst.append(result)
        label_lst.append(img_label)
        path_lst.append(img_path)

        error = abs(result[0, -1] - img_label[-1]) / img_label[-1]
        hwratio,ty,yerror = computeYDiff(result, img_label)

        if yerror > 3.50:
        #     # print "yeror",yerror
            yen = yen + 1
            # if ty > 0.5:
            #     yout = yout + 1
        #         res_path = 'reg_res/yout/'
        #         res_str = str(ty)
        #         res_str = res_str[0:6]
        #     else:
        #     res_path = 'reg_res/y/'
        #     res_str = str(yerror)
        #     res_str = res_str[0:6]
        #     visualize_res_14pts(img_path, result, img_label, 400, 400,res_path,res_str)

        if error > 0.1:
            cwen = cwen + 1
            minx = 1.0
            maxx = -1.0
            for i in range(0, 14):
                if img_label[i * 2 + 0] < minx:
                    minx = img_label[i * 2 + 0]
                if img_label[i * 2 + 0] > maxx:
                    maxx = img_label[i * 2 + 0]
            # print "yeror",yerror
            if minx < -0.5 or maxx > 0.5:
                cwout = cwout + 1
            #     res_path = 'reg_res/cwout/'
            #     res_str = str(maxx)
            #     if minx < -0.5:
            #         res_str = str(minx)
            #     res_str = res_str[0:6]
            # else:
            #     res_path = 'reg_res/carw/'
            #     res_str = str(error)
            #     res_str = res_str[0:6]
            # visualize_res_14pts(img_path, result, img_label, 400, 400)#, res_path, res_str)
        # if error > 0.1:
        #     visualize_res_14pts(img_path, result, img_label, 400, 400)
        if error > 0.1 and yerror >= 3.5:
            twoerr = twoerr + 1
        #     paths = img_path.split('/')
        #     print paths[-1]
        # yerror = computeYDiff(result,img_label)
        # print yerror
        yerrors[int(round(yerror))] = yerrors[int(round(yerror))] + 1
        # err, xerr = computeDiff(result,img_label)
        fw.write(str(hwratio)+'\t'+str(error)+'\t'+str(yerror)+'\n')
        i +=1
        if i%1000 == 0:
            print "processed %d images" % i
    record_res("regression_caffe_baseline_result.txt", path_lst, label_lst, res_lst)
    regression_benchmark.evaluate()
    fw.close()

    yerrors /= len(res_lst)
    resY = [np.sum(yerrors[0:3]),np.sum(yerrors[0:4]),np.sum(yerrors[0:5]),np.sum(yerrors[0:6])]
    print resY
    # print yerrors
    # print "2px:",np.sum(yerrors[0:3])
    # print "3px:",np.sum(yerrors[0:4])
    # print "4px:", np.sum(yerrors[0:5])
    # print "5px:", np.sum(yerrors[0:6])


    print len(res_lst)
    print twoerr
    print yen
    print yout
    print cwen
    print cwout

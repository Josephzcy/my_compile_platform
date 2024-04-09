
import cv2 as cv
import os
import shutil
import numpy as np
import sys
sys.path.append("/data/wzheng/projects/caffe-quant-master/python/")
import caffe
from scipy.io import loadmat
import lmdb
import random
# import h5py
# import struct
# Please do not change the parameters without permision!!!  PICTURE and MAT

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
        label = np.fromstring(label_str,sep=" ", dtype=np.float)
        #label = label[0:label.shape[0]-1]
        img_labels.append(label)
    label_file.close()
    return img_paths, img_labels

def get_img_datum(image_fn, color=True, resize_w=-1, resize_h=-1):
    if color is True:
        img = cv.imread(image_fn, cv.IMREAD_COLOR)
        if resize_h>0 and resize_w>0:
            img = cv.resize(img, dsize=(resize_w, resize_h), interpolation=cv.INTER_LINEAR)
        img = img.transpose((2, 0, 1))
    else:
        img = cv.imread(image_fn, cv.IMREAD_GRAYSCALE)
        if resize_h>0 and resize_w>0:
            img = cv.resize(img, dsize=(resize_w, resize_h), interpolation=cv.INTER_LINEAR)
        img = np.expand_dims(img, axis=0)

    # img = np.expand_dims(img,axis=0)
    datum = caffe.io.array_to_datum(img)
    return datum

def get_gt_datum(label):  # load mat , data_name = 'lb'
    label = np.expand_dims(label, axis=1)
    label = np.expand_dims(label, axis=2)
    datum = caffe.io.array_to_datum(label)
    return datum

def create_dataset_from_lst(label_lst, img_root_path, img_db, label_db, shuffle=True, color=True, resize_w=-1, resize_h=-1):
    os.makedirs(img_db)
    os.makedirs(label_db)
    img_paths, img_labels = load_label_lst(label_lst, img_root_path, shuffle)
    #del_and_create(img_db)
    img_env_train = lmdb.Environment(img_db, map_size=10000000 * 256 * 256)
    img_txn_train = img_env_train.begin(write=True, buffers=True)

    #del_and_create(label_db)
    gt_env_train = lmdb.Environment(label_db, map_size=100000 * 256 * 256)
    gt_txn_train = gt_env_train.begin(write=True, buffers=True)

    key = 0
    for (img_fn, img_label) in zip(img_paths, img_labels):
        img_datum = get_img_datum(img_fn, color,resize_w, resize_h)
        gt_datum = get_gt_datum(img_label)

        key_put = '%010d' % key
        img_txn_train.put(key_put, img_datum.SerializeToString())
        gt_txn_train.put(key_put, gt_datum.SerializeToString())
        key += 1
        if key % 1000 == 0:
            print ("processed %10d files" % key)

    img_txn_train.commit()
    gt_txn_train.commit()
    img_env_train.close()
    gt_env_train.close()


if __name__ == '__main__':
    train_lst = "/data/wzheng/data/car_regression_80/box_80_128_14pts_aug.txt"
    val_lst = "/data/wzheng/data/car_regression_80/box_80_128_14pts_test.txt"
    img_db_train = "/data/wzheng/data/car_regression_80/img_gray_80_train"
    label_db_train = "/data/wzheng/data/car_regression_80/label_gray_80_train"
    img_db_val = "/data/wzheng/data/car_regression_80/img_gray_80_val"
    label_db_val = "/data/wzheng/data/car_regression_80/label_gray_80_val"
    img_root_path_train = "/data/wzheng/data/car_regression_80/box_80_img_aug"
    img_root_path_val = "/data/wzheng/data/car_regression_80/box_80_img_test"
    resize_w = 80
    resize_h = 80
    create_dataset_from_lst(train_lst, img_root_path_train, img_db_train, label_db_train, \
                            shuffle=True, color=False, resize_w=resize_w, resize_h=resize_h)
    create_dataset_from_lst(val_lst, img_root_path_val, img_db_val, label_db_val, \
                            shuffle=True, color=False, resize_w=resize_w, resize_h=resize_h)

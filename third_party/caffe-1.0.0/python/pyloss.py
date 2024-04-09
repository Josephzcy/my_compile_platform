import os
import sys

caffe_root = '/data/wzheng/projects/caffe-quant-master'
sys.path.insert(0, caffe_root + '/python')
import numpy as np
import caffe


class SmoothLossL1Layer(caffe.Layer):
    """
    Compute the Euclidean Loss in the same manner as the C++ EuclideanLossLayer
    to demonstrate the class interface for developing layers in Python.
    """

    def setup(self, bottom, top):
        # check input pair
        if len(bottom) != 2:
            raise Exception("Need two inputs to compute distance.")
        self.rx = [2, 4, 6, 8]
        self.lx = [0, 18, 16, 14]
        self.dy = [11, 13]
        self.cw = 28

    def reshape(self, bottom, top):
        # check input dimensions match
        if bottom[0].count != bottom[1].count:
            raise Exception("Inputs must have the same dimension.")
        # difference is shape of inputs
        self.diff = np.zeros_like(bottom[0].data, dtype=np.float32)
        # loss output is scalar
        top[0].reshape(1)

    def forward(self, bottom, top):
        # print self.diff.shape
        # print np.reshape(bottom[0].data,self.diff.shape)
        # print np.reshape(bottom[1].data,self.diff.shape)
        tempb0 = np.reshape(bottom[0].data, self.diff.shape)
        tempb1 = np.reshape(bottom[1].data, self.diff.shape)
        self.diff = tempb0 - tempb1
        # print self.diff
        idx = (np.abs(self.diff) < 1.0)
        # idx[:,self.cw] = False # move car width
        # print idx
        # print np.sum((self.diff[idx]**2) * 0.5)
        idx1 = np.abs(self.diff) >= 1.0
        # idx1[:,self.diff.shape[1]-1] = False # move car width
        # print idx1
        # print np.sum( np.abs(self.diff[idx1]) - 0.5)
        # print tempb0[:,self.rx] - tempb0[:,self.lx]
        # print tempb1[:,self.rx] - tempb1[:,self.lx]
        a = np.sum(np.abs(1 - (tempb0[:, self.rx] - tempb0[:, self.lx]) / (tempb1[:, self.rx] - tempb1[:, self.lx])))
        # print tempb0[:,self.cw]
        # print tempb1[:,self.cw]
        b = np.sum(1.0 * np.abs(1 - tempb0[:, self.cw] / tempb1[:, self.cw]))
        c = np.sum(np.abs(np.sum(tempb0[:, self.dy]) - np.sum(tempb1[:, self.dy])) / 4)
        # print idx.shape
        # print tempb1[:,self.dy]
        # print a
        # print b
        # print c
        top[0].data[...] = (np.sum((self.diff[idx] ** 2) * 0.5) + np.sum(np.abs(self.diff[idx1]) - 0.5) + a + b + c) / \
                           bottom[0].num
        # print np.reshape(top[0].data,(1,1))
        # print dix

    def backward(self, top, propagate_down, bottom):

        idx = np.abs(self.diff) < 1.0
        idx1 = self.diff >= 1.0
        idx2 = self.diff <= -1.0
        # idx[:,self.cw] = False
        # idx1[:,self.cw] = False
        # idx2[:,self.cw] = False
        tempb0 = np.reshape(bottom[0].data, self.diff.shape)
        tempb1 = np.reshape(bottom[1].data, self.diff.shape)
        # print idx
        # print idx1
        # print idx2
        for i in range(2):
            if not propagate_down[i]:
                continue
            if i == 0:
                sign = 1
            else:
                sign = -1
            # compute smooth l1 d
            bottom[i].diff[idx] = sign * self.diff[idx] / bottom[i].num
            bottom[i].diff[idx1] = sign * 1.0 / bottom[i].num
            bottom[i].diff[idx2] = sign * (-1.0) / bottom[i].num
            # print bottom[i].diff
            bottom[i].diff[:, self.rx] += sign / bottom[i].num / (tempb1[:, self.rx] - tempb1[:, self.lx])
            bottom[i].diff[:, self.lx] += sign * (-1) / bottom[i].num / (tempb1[:, self.rx] - tempb1[:, self.lx])
            bottom[i].diff[:, self.cw] += sign / tempb1[:, self.cw] / bottom[i].num
            bottom[i].diff[:, self.dy] += sign / 4.0 / bottom[i].num
            # print bottom[i].diff
            # print dix


class EuclideanLossLayerPython(caffe.Layer):
    """
    Compute the Euclidean Loss in the same manner as the C++ EuclideanLossLayer
    to demonstrate the class interface for developing layers in Python.
    """

    def setup(self, bottom, top):
        # check input pair
        if len(bottom) != 2:
            raise Exception("Need two inputs to compute distance.")

    def reshape(self, bottom, top):
        # check input dimensions match
        if bottom[0].count != bottom[1].count:
            raise Exception("Inputs must have the same dimension.")
        # difference is shape of inputs
        self.diff = np.zeros_like(bottom[0].data, dtype=np.float32)
        # loss output is scalar
        top[0].reshape(1)

    def forward(self, bottom, top):
        self.diff[...] = bottom[0].data - bottom[1].data
        top[0].data[...] = np.sum(self.diff ** 2) / bottom[0].num / 2.

    def backward(self, top, propagate_down, bottom):
        for i in range(2):
            if not propagate_down[i]:
                continue
            if i == 0:
                sign = 1
            else:
                sign = -1
            bottom[i].diff[...] = sign * self.diff / bottom[i].num

import os
import sys

sys.path.append('/data/wzheng/projects/caffe-quant-master/python')
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
        self.rx = [2,4,6,8]
        self.lx = [0,18,16,14]
        self.dy = [11,13]
        self.cw = 28

    def reshape(self, bottom, top):
        # check input dimensions match
        if bottom[0].count != bottom[1].count:
            raise Exception("Inputs must have the same dimension.")
        # difference is shape of inputs
        self.diff = np.zeros_like(bottom[0].data, dtype=np.float32)
        self.derive_diff = np.zeros((bottom[0].num,6),dtype=np.float32)
        # loss output is scalar
        top[0].reshape(1)

    def forward(self, bottom, top):
        #print self.diff.shape
        #print abc
        #print np.reshape(bottom[0].data,self.diff.shape)
        #print np.reshape(bottom[1].data,self.diff.shape)
        tempb0 = np.reshape(bottom[0].data,self.diff.shape)
        tempb1 = np.reshape(bottom[1].data,self.diff.shape)
        self.diff = tempb0 - tempb1
        #print self.diff
        idx = (np.abs(self.diff) < 1.0)
        idx1 = np.abs(self.diff) >= 1.0
        #print idx1
        size = (bottom[0].num,6)
        derive_b0 = np.zeros(size,np.float32)
        derive_b1 = np.zeros(size,np.float32)
        #derive_diff = np.zeros(size,np.float32)

        derive_b0[:,0:4] = tempb0[:,self.rx] - tempb0[:,self.lx]
        derive_b1[:,0:4] = tempb1[:,self.rx] - tempb1[:,self.lx]
        derive_b0[:,4] = np.sum(tempb0[:,self.dy],1)/2.0
        derive_b1[:,4] = np.sum(tempb1[:,self.dy],1)/2.0
        derive_b0[:,5] = tempb0[:,self.cw]
        derive_b1[:,5] = tempb1[:,self.cw]
        #
        self.derive_diff = derive_b0 - derive_b1
        didx = (np.abs(self.derive_diff) < 1.0)
        didx1 = np.abs(self.derive_diff) >= 1.0
        #print tempb0[:,self.rx]
        #print tempb1[:,self.lx]
        #print tempb0[:,self.dy]
        #print tempb1[:,self.dy]
        #print derive_b0
        derive_cost = np.sum((self.derive_diff[didx]**2) * 0.5) + np.sum( np.abs(self.derive_diff[didx1]) - 0.5)
        top[0].data[...] = (np.sum((self.diff[idx]**2) * 0.5) + np.sum( np.abs(self.diff[idx1]) - 0.5) + derive_cost)/bottom[0].num 
        #print np.reshape(top[0].data,(1,1))
        #print dix

    def backward(self, top, propagate_down, bottom):
        
        tempb0 = np.reshape(bottom[0].data,self.diff.shape)
        tempb1 = np.reshape(bottom[1].data,self.diff.shape)
        # get derive 
        #size = (bottom[0].num,6)
        #derive_b0 = np.zeros(size,np.float32)
        #derive_b1 = np.zeros(size,np.float32)
        #derive_diff = np.zeros(size,np.float32)

        #derive_b0[:,0:4] = tempb0[:,self.rx] - tempb0[:,self.lx]
        #derive_b1[:,0:4] = tempb1[:,self.rx] - tempb1[:,self.lx]
        #derive_b0[:,4] = np.sum(tempb0[:,self.dy],1)/2.0
        #derive_b1[:,4] = np.sum(tempb1[:,self.dy],1)/2.0
        #derive_b0[:,5] = tempb0[:,self.cw]
        #derive_b1[:,5] = tempb1[:,self.cw]
        #
        #derive_diff = derive_b0 - derive_b1
        didx = (np.abs(self.derive_diff) < 1.0)
        didx1 = self.derive_diff >= 1.0
        didx2 = self.derive_diff <= -1.0

        idx = np.abs(self.diff) < 1.0
        idx1 = self.diff >= 1.0
        idx2 = self.diff <= -1.0
        #idx2[:,self.cw] = False
        #print idx
        #print idx1
        #print idx2
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
            #print bottom[i].diff
            temp = np.zeros_like(self.derive_diff,np.float32)
            temp[didx] = sign * self.derive_diff[didx] / bottom[i].num
            temp[didx1] = sign * 1.0 / bottom[i].num
            temp[didx2] = sign * (-1.0) / bottom[i].num

            bottom[i].diff[:,self.lx] = bottom[i].diff[:,self.lx] + (-1.0) * temp[:,0:4]
            bottom[i].diff[:,self.rx] = bottom[i].diff[:,self.rx] + temp[:,0:4]
            bottom[i].diff[:,self.dy[0]] = bottom[i].diff[:,self.dy[0]] + temp[:,4] * 0.5
            bottom[i].diff[:,self.dy[1]] = bottom[i].diff[:,self.dy[1]] + temp[:,4] * 0.5
            bottom[i].diff[:,self.cw] = bottom[i].diff[:,self.cw] + temp[:,5]
            #print dix
            


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
        top[0].data[...] = np.sum(self.diff**2) / bottom[0].num / 2.

    def backward(self, top, propagate_down, bottom):
        for i in range(2):
            if not propagate_down[i]:
                continue
            if i == 0:
                sign = 1
            else:
                sign = -1
            bottom[i].diff[...] = sign * self.diff / bottom[i].num

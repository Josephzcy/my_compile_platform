import sys
sys.path.append("../")
from caffe import layers as L,params as P,to_proto
train_img_lmdb="/data/wzheng/data/car_regression_80/img_gray_80_db/"
train_label_lmdb="/data/wzheng/data/car_regression_80/label_gray_80_db/"
train_val_proto='/data/wzheng/projects/caffe-quant-master/examples/car_regression_proj/regression_baseline_train_val.prototxt'

def create_net(train_img_lmdb,train_label_lmdb, batch_size, include_acc=False):
    data = L.Data(source=train_img_lmdb, backend=P.Data.LMDB, batch_size=batch_size, ntop=1,
        transform_param=dict(crop_size=80,mean_value=128,mirror=False), phase=0)
    label = L.Data(source=train_label_lmdb, backend=P.Data.LMDB, batch_size=batch_size, ntop=1, phase=0)

    conv1=L.Convolution(data, kernel_size=5, stride=1,num_output=16, pad=2,weight_filler=dict(type='xavier'))
    relu1=L.ReLU(conv1, in_place=True)
    pool1=L.Pooling(relu1, pool=P.Pooling.MAX, kernel_size=3, stride=2)

    conv2=L.Convolution(pool1, kernel_size=3, stride=1,num_output=32, pad=1,weight_filler=dict(type='xavier'))
    relu2=L.ReLU(conv2, in_place=True)
    pool2=L.Pooling(relu2, pool=P.Pooling.MAX, kernel_size=3, stride=2)

    fc3=L.InnerProduct(pool2, num_output=512,weight_filler=dict(type='xavier'))
    relu3=L.ReLU(fc3, in_place=True)

    #drop3 = L.Dropout(relu3, in_place=True)
    fc4 = L.InnerProduct(relu3, num_output=9,weight_filler=dict(type='xavier'))

    #loss = L.SoftmaxWithLoss(fc4, label)
    loss = L.EuclideanLoss(fc4, label)
    if include_acc:
        acc = L.Accuracy(fc4, label, phase=1)
        return to_proto(loss, acc)
    else:
        return to_proto(loss)

def write_net(train_img_lmdb, train_label_lmdb, batch_size):
    with open(train_val_proto, 'w') as f:
        f.write(str(create_net(train_img_lmdb, train_label_lmdb, batch_size=batch_size, include_acc=True)))

if __name__ == '__main__':
    write_net(train_img_lmdb, train_label_lmdb, 256)
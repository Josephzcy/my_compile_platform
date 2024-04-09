import matplotlib.pyplot as plt
import numpy as np
import re

TR_loss = re.compile('.*?\sTrain net output #1: loss_accuracy_1 = ([\d\.]+)')
VA_loss = re.compile('.*?\sTest net output #2: loss_accuracy_5 = ([\d\.]+)')

#log = open('/data/wzheng/projects/caffe-quant-master/examples/car_regression_proj/train_log.log').read()
log = open('/data/wzheng/projects/caffe-quant-master/examples/car_ground_cls/ristretto/finetune_log.log').read()
loss_tr = [float(x) for x in TR_loss.findall(log)]
loss_va = [float(x) for x in VA_loss.findall(log)]

tr_idx = np.arange(len(loss_tr))
va_idx = np.arange(len(loss_va))*50
base_loss = [0.001 for i in range(0,len(tr_idx))]

plt.figure(figsize=(8, 6))
plt.xlabel("iteration")
plt.ylabel("mse")

plt.plot(tr_idx, loss_tr, 'o', linestyle='-', color="g", label="Train loss")

plt.plot(va_idx, loss_va, '*', linestyle='-', color="r", label="Validation loss")
#plt.plot(tr_idx, base_loss,'x',linestyle='-',color='c',label="base 0.25")
'''
ls = []
print min(loss_va)
for i in range(5,len(loss_tr)+1):
    med = np.median(np.array(loss_tr[i-5:i]))
    mins = np.min(np.array(loss_tr[i-5:i]))
    amins = np.min(np.array(loss_tr[0:i]))
    sums = np.sum(np.array(loss_tr[i-5:i]))
    l = 5 * med / (sums - 5 * med) * (loss_va[i-1] -amins)/amins
    ls.append(l)
#print ls
'''

print max(loss_va)
# early stop
plt.xticks(np.arange(min(tr_idx), max(tr_idx) + 1, 50000))
ymax = min([max([max(loss_tr),max(loss_va),1])+0.1,1.0])
plt.yticks(np.arange(0, ymax, 0.01))
plt.ylim([0, ymax])
plt.show()


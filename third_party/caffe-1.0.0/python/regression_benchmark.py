import numpy as np
def load_benchmark_result(benchmark_file):
    f_benchmark = open(benchmark_file, "r")
    lines = f_benchmark.readlines()
    paths = list()
    labels = list()
    results = list()
    for line in lines:
        strs = line.split("\t")
        length = (len(strs)-1)/2
        label = np.zeros(shape=(length,), dtype=np.float)
        result = np.zeros(shape=(length,), dtype=np.float)
        for i in xrange(length):
            label[i] = np.float(strs[1+i])
            result[i] = np.float(strs[1+length+i])
        labels.append(label)
        results.append(result)
        paths.append(strs[0])
    f_benchmark.close()
    return paths, labels, results

def benchmark_car_width_with_width(percentage_diff, res_lst, label_lst):
    hit = 0
    for res, label in zip(res_lst, label_lst):
        assert (res.shape == label.shape)
        diff = abs(res[0]-label[0])/label[0]
        if diff < percentage_diff:
            hit += 1
    hit_rate = hit/np.float(len(res_lst))
    return hit_rate

def benchmark_car_width_with_4pts(percentage_diff, res_lst, label_lst):
    hit = 0
    for res, label in zip(res_lst, label_lst):
        assert (res.shape == label.shape)
        width_res = ((1-res[3]-res[1])+(1-res[5]-res[7]))/2
        width_label = ((1 - label[3] - label[1]) + (1 - label[5] - label[7])) / 2
        diff = abs(width_label-width_res)/width_label
        if diff < percentage_diff:
            hit += 1
    hit_rate = hit/np.float(len(res_lst))
    return hit_rate

def benchmark_car_width_with_width_14pts(percentage_diff, res_lst, label_lst):
    hit = 0
    for res, label in zip(res_lst, label_lst):
        assert (res.shape == label.shape)
        diff = abs(res[-1]-label[-1])/label[-1]
        if diff < percentage_diff:
            hit += 1
    hit_rate = hit/np.float(len(res_lst))
    return hit_rate

def benchmark_car_width_with_14pts(percentage_diff, res_lst, label_lst):
    hit = 0
    for res, label in zip(res_lst, label_lst):
        assert (res.shape == label.shape)
        width_res = (res[2]-res[0]+res[4]-res[18]+res[6]-res[16]+res[8]-res[14])/4
        width_label = (label[2]-label[0]+label[4]-label[18]+label[6]-label[16]+label[8]-label[14])/4
        diff = abs(width_label-width_res)/width_label
        if diff < percentage_diff:
            hit += 1
    hit_rate = hit/np.float(len(res_lst))
    return hit_rate

def benchmark_ground_with_14pts(pixel_diff, res_lst, label_lst):
    hit = 0
    for res, label in zip(res_lst, label_lst):
        assert (res.shape == label.shape)
        ground_res = (res[11]+res[13])/2.0
        ground_label = (label[11]+label[13])/2.0
        diff = abs((ground_label-ground_res)*80.0)
        if diff <= pixel_diff:
            hit += 1
    hit_rate = hit/np.float(len(res_lst))
    return hit_rate

def evaluate():
    benchmark_file = "regression_caffe_baseline_result.txt"
    path_lst, label_lst, res_lst = load_benchmark_result(benchmark_file)
    hit_rate_caffe_width = np.zeros(shape=(4,), dtype=np.float)
    hit_rate_caffe_4pts = np.zeros(shape=(4,), dtype=np.float)
    hit_rate_caffe_ground = np.zeros(shape=(4,), dtype=np.float)
    for i in xrange(4):
        #hit_rate_caffe_width[i] = benchmark_car_width_with_width(0.05*(i+1), res_lst, label_lst)
        #hit_rate_caffe_4pts[i] = benchmark_car_width_with_4pts(0.05 * (i + 1), res_lst, label_lst)
        hit_rate_caffe_width[i] = benchmark_car_width_with_width_14pts(0.05 * (i + 1), res_lst, label_lst)
        hit_rate_caffe_4pts[i] = benchmark_car_width_with_14pts(0.05 * (i + 1), res_lst, label_lst)
        hit_rate_caffe_ground[i] = benchmark_ground_with_14pts(i+2, res_lst, label_lst)

    print "predict width directly: ",hit_rate_caffe_width
    print "predict width with 4pts:", hit_rate_caffe_4pts
    print "predict ground with 2pts:", hit_rate_caffe_ground

if __name__=="__main__":
    evaluate()

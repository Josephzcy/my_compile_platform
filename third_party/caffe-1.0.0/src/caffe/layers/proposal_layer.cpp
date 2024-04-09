#include <algorithm>
#include <vector>

#include "caffe/layers/proposal_layer.hpp"

// debug too many vector variables
template <typename Dtype>
std::ostream& operator<<(std::ostream& o, std::vector<Dtype> const& v) {
  for (typename std::vector<Dtype>::const_iterator i = v.begin(); i != v.end(); ++i)
    o << *i << ' ';
  return o;
}

template <typename Dtype>
std::ostream& operator<<(std::ostream& o, caffe::Blob<Dtype> const& b) {
  int size = 1;
  for(int idx=0; idx<b.shape().size(); ++idx)
    size *= b.shape(idx);
  
  o << "blob shape" << " " << b.shape() << std::endl;
  int lastAxisNum = b.shape(b.shape().size()-1);
  int pageNum = size/lastAxisNum;
  for(int p=0; p<pageNum; ++p) {
    for(int i=0; i<lastAxisNum; ++i){
      o << *(b.cpu_data()+p*lastAxisNum+i) << ' ';
    }
    o << std::endl;
  }
  return o;
}

template <typename Dtype>
std::ostream& operator<<(std::ostream& o, caffe::Blob<Dtype>* b) {
  int size = 1;
  for(int idx=0; idx<b->shape().size(); ++idx)
    size *= b->shape(idx);
  
  o << "blob shape" << " " << b->shape() << std::endl;
  int lastAxisNum = b->shape(b->shape().size()-1);
  int pageNum = size/lastAxisNum;
  for(int p=0; p<pageNum; ++p) {
    for(int i=0; i<lastAxisNum; ++i){
      o << *(b->cpu_data()+p*lastAxisNum+i) << ' ';
    }
    o << std::endl;
  }
  return o;
}


namespace caffe {

void whctrs(float* whctr, const vector<float>& anchor) {
  // Return width, height, x center, and y center for an anchor (window).
  float w = anchor[2] - anchor[0] + 1,
      h = anchor[3] - anchor[1] + 1;
  whctr[0] = w,
  whctr[1] = h,
  whctr[2] = anchor[0] + 0.5 * (w - 1),
  whctr[3] = anchor[1] + 0.5 * (h - 1);
}

void mkanchors(vector<vector<float> >& ratio_anchors, const vector<float>& ws, 
    const vector<float>& hs, float x_ctr, float y_ctr) {
  // Given a vector of widths (ws) and heights (hs) around a center
  // (x_ctr, y_ctr), output a set of anchors (windows).
  for(int i = 0; i < ws.size(); ++i) {
    vector<float> anchor;
    anchor.push_back(x_ctr - 0.5 * (ws[i] - 1));
    anchor.push_back(y_ctr - 0.5 * (hs[i] - 1));
    anchor.push_back(x_ctr + 0.5 * (ws[i] - 1));
    anchor.push_back(y_ctr + 0.5 * (hs[i] - 1));
    ratio_anchors.push_back(anchor);
  }
}

void ratio_enum(vector<vector<float> >& anchors, const vector<float>& anchor, const vector<float>& ratios) {
  // Enumerate a set of anchors for each aspect ratio wrt an anchor.

  float whctr[4];
  whctrs(whctr, anchor);
  
  float w = whctr[0],
        h = whctr[1], 
        x_ctr = whctr[2], 
        y_ctr = whctr[3];
  
  float size = w * h;
  std::vector<float> size_ratios, ws, hs;
  for(int i = 0; i < ratios.size(); ++i) {
    size_ratios.push_back(size / ratios[i]);
    ws.push_back(round(sqrt(size_ratios[i])));
    hs.push_back(round(ws[i] * ratios[i]));
  }
  
  mkanchors(anchors, ws, hs, x_ctr, y_ctr);
}

void scale_enum(vector<vector<float> >& anchors, const vector<float>& anchor, const vector<int>& scales) {
  // Enumerate a set of anchors for each scale wrt an anchor.
  float whctr[4];
  whctrs(whctr, anchor);
  
  float w = whctr[0],
        h = whctr[1], 
        x_ctr = whctr[2], 
        y_ctr = whctr[3];
        
  vector<float> size_ratios, ws, hs;
  for(int scale : scales) {
    ws.push_back(w * scale);
    hs.push_back(h * scale);
  }
  
  mkanchors(anchors, ws, hs, x_ctr, y_ctr);
}

void generate_anchors(vector<vector<float> >& anchors, int base_size_, const vector<float>& ratios, const vector<int>& scales) {
  // Generate anchor (reference) windows by enumerating aspect ratios X
  // scales wrt a reference (0, 0, 15, 15) window.
  vector<float> base_anchor = {0, 0, base_size_-1, base_size_-1};
  vector<vector<float> > ratio_anchors;
  ratio_enum(ratio_anchors, base_anchor, ratios);
    
  for(vector<float> ratio_anchor : ratio_anchors) {
    vector<vector<float> > scale_anchors;
    scale_enum(scale_anchors, ratio_anchor, scales);
    
    for(vector<float> scale_anchor : scale_anchors)
      anchors.push_back(scale_anchor);
  }
}

template <typename Dtype>
void bbox_transform_inv(vector<vector<float> >& pred_boxes, const Blob<Dtype>& boxes, const Blob<Dtype>& deltas) {
  int proposalNum = boxes.shape(0);
  float width, height, ctr_x, ctr_y,
    pred_ctr_x, pred_ctr_y, pred_w, pred_h;

  for(int i=0; i<proposalNum; ++i) {
    width = *(boxes.cpu_data() + 4*i + 2) - *(boxes.cpu_data() + 4*i) + 1;
    height = *(boxes.cpu_data() + 4*i + 3) - *(boxes.cpu_data() + 4*i + 1) + 1;
    ctr_x = *(boxes.cpu_data() + 4*i) + 0.5 * width;
    ctr_y = *(boxes.cpu_data() + 4*i + 1) + 0.5 * height;
    
    pred_ctr_x = *(deltas.cpu_data() + 4*i) * width + ctr_x;
    pred_ctr_y = *(deltas.cpu_data() + 4*i + 1) * height + ctr_y;
    pred_w = exp(*(deltas.cpu_data() + 4*i + 2)) * width;
    pred_h = exp(*(deltas.cpu_data() + 4*i + 3)) * height;
    
    vector<float> pred_box = {pred_ctr_x-0.5*pred_w, pred_ctr_y-0.5*pred_h, 
        pred_ctr_x+0.5*pred_w, pred_ctr_y+0.5*pred_h};
    pred_boxes.push_back(pred_box);
  }
}

void clip_boxes(vector<vector<float> >& boxes, const float* im_shape) {
  // Clip boxes to image boundaries.
  for(int i=0; i<boxes.size(); ++i) {
    // x1 >= 0
    boxes[i][0] = std::max(std::min(boxes[i][0], im_shape[1] - 1), float(0.));
    // y1 >= 0
    boxes[i][1] = std::max(std::min(boxes[i][1], im_shape[0] - 1), float(0.));
    // x2 < im_shape[1]
    boxes[i][2] = std::max(std::min(boxes[i][2], im_shape[1] - 1), float(0.));
    // y2 < im_shape[0]
    boxes[i][3] = std::max(std::min(boxes[i][3], im_shape[0] - 1), float(0.));
  }
}

vector<int> filter_boxes(const vector<vector<float> >& boxes, float min_size) {
  // Remove all boxes with any side smaller than min_size.
  vector<int> keep;
  int w, h;
  for(int i=0; i<boxes.size(); ++i) {
    w = boxes[i][2] - boxes[i][0] + 1;
    h = boxes[i][3] - boxes[i][1] + 1;
    if(w >= min_size and h >= min_size)
      keep.push_back(i);
  }
  return keep;
}

bool cmpVec(vector<float> va, vector<float> vb) {
    return va[va.size()-1] > vb[vb.size()-1];    
}

template <typename Dtype>
void ProposalLayer<Dtype>::LayerSetUp(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  ProposalParameter proposal_param = this->layer_param_.proposal_param();
  feat_stride_ = proposal_param.feat_stride();
  base_size_ = proposal_param.base_size();
  pre_nms_topN_ = proposal_param.pre_nms_topn();
  min_size_ = proposal_param.min_size();
  
  scales_.clear();
  std::copy(proposal_param.scale().begin(),
      proposal_param.scale().end(),
      std::back_inserter(scales_));
  
  ratios_.clear();
  std::copy(proposal_param.ratio().begin(),
      proposal_param.ratio().end(),
      std::back_inserter(ratios_));
      
  generate_anchors(anchors_, base_size_, ratios_, scales_);
  
  // rois blob: holds R regions of interest, each is a 5-tuple
  // (n, x1, y1, x2, y2) specifying an image batch index n and a
  // rectangle (x1, y1, x2, y2)
  vector<int> vector_shape = {1, 5};
  top[0]->Reshape(vector_shape);

  // scores blob: holds scores for R regions of interest
  if(top.size() > 1)
    top[1]->Reshape(1, 1, 1, 1);

}

template <typename Dtype>
void ProposalLayer<Dtype>::Forward_cpu(const vector<Blob<Dtype>*>& bottom,
      const vector<Blob<Dtype>*>& top) {
  /*
  Algorithm:
  for each (H, W) location i
    generate A anchor boxes centered on cell i
    apply predicted bbox deltas at cell i to each of the A anchors
  clip predicted boxes to image
  remove predicted boxes with either height or width < threshold
  sort all (proposal, score) pairs by score from highest to lowest
  take top pre_nms_topN proposals before NMS
  apply NMS with threshold 0.7 to remaining proposals
  take after_nms_topN proposals after NMS
  return the top proposals (-> RoIs top, scores top)
  */
  
  CHECK_EQ(bottom[0]->shape(0), 1) << "Only single item batches are supported " <<
      bottom[0]->shape(0);
  
  CHECK(this->phase_ == TEST) << "Only test phase supported";
  
  // the first set of _num_anchors channels are bg probs
  // the second set are the fg probs, which we want
  
  int num_anchors = anchors_.size();
  int height = bottom[0]->shape(2), width = bottom[0]->shape(3);
  
  vector<int> shape_scores = {bottom[0]->shape(1)-num_anchors, height, width};  
  Blob<Dtype> scores;
  scores.Reshape(shape_scores);
  caffe_copy(shape_scores[0] * shape_scores[1] * shape_scores[2],
    bottom[0]->cpu_data() + num_anchors*bottom[0]->shape(2)*bottom[0]->shape(3), scores.mutable_cpu_data());

  const Blob<Dtype>* bbox_deltas = bottom[1];  
  float im_info[3] = {bottom[2]->cpu_data()[0], bottom[2]->cpu_data()[1], bottom[2]->cpu_data()[2]};
  
  // 1. Generate proposals from bbox deltas and shifted anchors
  // Enumerate all shifts
  Blob<Dtype> shifts;
  int K = height*width;
  vector<int> shape_shifts = {K, 4};
  shifts.Reshape(shape_shifts);
  
  for (int i=0; i<height; ++i){
    for (int j=0; j<width; ++j){
      shifts.mutable_cpu_data()[4*(i*width+j)] = j * feat_stride_;
      shifts.mutable_cpu_data()[4*(i*width+j)+1] = i * feat_stride_;
      shifts.mutable_cpu_data()[4*(i*width+j)+2] = j * feat_stride_;
      shifts.mutable_cpu_data()[4*(i*width+j)+3] = i * feat_stride_;
    }
  }
  
  // Enumerate all shifted anchors:
  //
  // add A anchors (1, A, 4) to
  // cell K shifts (K, 1, 4) to get
  // shift anchors (K, A, 4)
  // reshape to (K*A, 4) shifted anchors
  int A = num_anchors;  
  Blob<Dtype> anchors_gen;
  vector<int> shape_anchors_gen = {K * A, 4};
  anchors_gen.Reshape(shape_anchors_gen);
  
  for (int i=0; i<K; ++i){
    for (int j=0; j<A; ++j){
      anchors_gen.mutable_cpu_data()[4*(i*A+j)] = anchors_[j][0] + shifts.cpu_data()[4*i];
      anchors_gen.mutable_cpu_data()[4*(i*A+j)+1] = anchors_[j][1] + shifts.cpu_data()[4*i+1];
      anchors_gen.mutable_cpu_data()[4*(i*A+j)+2] = anchors_[j][2] + shifts.cpu_data()[4*i+2];
      anchors_gen.mutable_cpu_data()[4*(i*A+j)+3] = anchors_[j][3] + shifts.cpu_data()[4*i+3];
    }
  }
  
  // Reshape bbox_deltas and scores
  vector<int> shape_deltas_reshape = {height*width*A, 4};  
  Blob<Dtype> deltas_reshape;
  deltas_reshape.Reshape(shape_deltas_reshape);
  for(int h=0; h<height; ++h)
    for(int w=0; w<width; ++w)
      for(int a=0; a<A; ++a) {
        *(deltas_reshape.mutable_cpu_data()+4*(h*width*A + w*A + a)) = *(bbox_deltas->cpu_data()+(4*a)*width*height + h*width + w);
        *(deltas_reshape.mutable_cpu_data()+4*(h*width*A + w*A + a)+1) = *(bbox_deltas->cpu_data()+(4*a+1)*width*height + h*width + w);
        *(deltas_reshape.mutable_cpu_data()+4*(h*width*A + w*A + a)+2) = *(bbox_deltas->cpu_data()+(4*a+2)*width*height + h*width + w);
        *(deltas_reshape.mutable_cpu_data()+4*(h*width*A + w*A + a)+3) = *(bbox_deltas->cpu_data()+(4*a+3)*width*height + h*width + w);
      }

  vector<int> shape_scores_reshape = {height*width*A, 1};  
  Blob<Dtype> scores_reshape;
  scores_reshape.Reshape(shape_scores_reshape);
  for(int h=0; h<height; ++h)
    for(int w=0; w<width; ++w)
      for(int a=0; a<A; ++a)
        *(scores_reshape.mutable_cpu_data()+(h*width*A + w*A + a)) = *(scores.cpu_data()+ a*width*height + h*width + w);
      
  // Convert anchors into proposals via bbox transformations
  vector<vector<float> > proposals;
  bbox_transform_inv(proposals, anchors_gen, deltas_reshape);  
  
  // 2. clip predicted boxes to image
  clip_boxes(proposals, im_info);
  
  // 3. remove predicted boxes with either height or width < threshold
  // (NOTE: convert min_size to input image scale stored in im_info[2])
  vector<int> keep = filter_boxes(proposals, min_size_ * im_info[2]);
  
  vector<vector<float> > proposals_scores_keep;
  for(int idx : keep) {
    vector<float> score = proposals[idx];
    score.push_back(*(scores_reshape.cpu_data()+idx));
    proposals_scores_keep.push_back(score);
  }
  
  // 4. sort all (proposal, score) pairs by score from highest to lowest
  // 5. take top pre_nms_topN
  sort(proposals_scores_keep.begin(), proposals_scores_keep.end(), cmpVec);
  vector<vector<float> > proposals_scores_topN;
  
  if(proposals_scores_keep.size() > pre_nms_topN_)
    proposals_scores_topN.assign(proposals_scores_keep.begin(), proposals_scores_keep.begin() + pre_nms_topN_);
  else
    proposals_scores_topN.assign(proposals_scores_keep.begin(), proposals_scores_keep.end());
  
  // Output rois blob
  // Our RPN implementation only supports a single input image, so all
  // batch inds are 0
  if(proposals_scores_topN.size() == 0) {
    vector<int> shape_top0 = {1, 5};
    top[0]->Reshape(shape_top0);
    *(top[0]->mutable_cpu_data()) = 0;
    *(top[0]->mutable_cpu_data() + 1) = 0;
    *(top[0]->mutable_cpu_data() + 2) = 0;
    *(top[0]->mutable_cpu_data() + 3) = 16;
    *(top[0]->mutable_cpu_data() + 4) = 16;
  } else {
    vector<int> shape_top0 = {proposals_scores_topN.size(), 5};
    top[0]->Reshape(shape_top0);
    for(int i=0; i<proposals_scores_topN.size(); ++i){
      *(top[0]->mutable_cpu_data() + 5*i) = 0;
      *(top[0]->mutable_cpu_data() + 5*i + 1) = proposals_scores_topN[i][0];
      *(top[0]->mutable_cpu_data() + 5*i + 2) = proposals_scores_topN[i][1];
      *(top[0]->mutable_cpu_data() + 5*i + 3) = proposals_scores_topN[i][2];
      *(top[0]->mutable_cpu_data() + 5*i + 4) = proposals_scores_topN[i][3];
    }
  }
    
  // [Optional] output scores blob
  if(top.size() > 1) {
      vector<int> shape_top1 = {proposals_scores_topN.size(), 1};
      top[1]->Reshape(shape_top1);
      for(int i=0; i<proposals_scores_topN.size(); ++i)
        *(top[1]->mutable_cpu_data() + i) = proposals_scores_topN[i][4];
  }  

}

#ifdef CPU_ONLY
STUB_GPU(ProposalLayer);
#endif

INSTANTIATE_CLASS(ProposalLayer);
REGISTER_LAYER_CLASS(Proposal);

}  // namespace caffe

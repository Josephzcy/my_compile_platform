// Copyright 2021 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_CLASSIFICATION_H_
#define INCLUDE_OBJ_CNN_OPERATORS_CLASSIFICATION_H_

#include <obj_cnn/cv.h>
#include <obj_cnn/operator.h>

#include <memory>
#include <string>

namespace obj_cnn {

/*! General classification parse operator.

  Output bytes are chunks of groups of classification scores.

  Each group of classification scores describe scores of one object on
  different classifications or the classification value directly.

  If classification algorithm defines C classifications there will be C scores
  in a score chunk.

  Type of each score is defined by score_type_. It is "float32" by now.

  Each chunk describes one thing to be classified.

  ### Layout

  If a classification algorithm grade 3 types (ABC), and output 2 chunks
  describe 2 objects (XY).

  If "layout" is "chunk-first", output data will be:

      XA YA XB YB XC YC
      ----- ----- -----

  If "layout" is "classification-first", output data will be:

      XA XB XC YA YB YC
      -------- --------

  If layout is "result-positive-or-negative-of-chunks". Each chunk countains
  only one number v, if v is positive, result classification is 1, if v is
  negative result classification is 0.

  ### Option

  - chunks: required, number of chunks of scores.

  - classifications: required, number of scores in each chunk.

  - layout: default be "chunks-first", see *layout* section.

  - single-result: default be false. Turn this on to make this operator output
    a Classification instead of a list of Classification sized one when chunks
    is one.
*/
class ClassificationOperator : public OperatorUnary {
 private:
  int Operate(CvData const &data, CvData &output) override;

  std::string name() const override { return "Classification"; }

  int LoadOption(toml::value const &) override;

 public:
  ClassificationOperator();
  virtual ~ClassificationOperator();

  enum ScoreType {
    kFloat,
  };

  enum Layout {
    kClassificationFirst,
    kChunkFirst,
    kResultPositiveOrNegativeOfChunks,
  };

  /*! Describe array size with (meaningless) padding at begin and end. */
  struct PaddedSize {
    size_t begin;
    size_t size;
    size_t end;

    size_t total() const { return begin + size + end; }
  };

 private:
  ScoreType score_type_;
  Layout layout_;
  PaddedSize chunks_;
  PaddedSize classifications_;
  bool single_result_ = false;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_CLASSIFICATION_H_

// Copyright 2011 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATORS_NN_H_
#define INCLUDE_OBJ_CNN_OPERATORS_NN_H_

#include <string>
#include <vector>

#include "obj_cnn/nn.h"
#include "obj_cnn/operator.h"
#include "obj_cnn/operator_graph_context.h"

namespace obj_cnn {

class NnOperator : public Operator {
 public:
  int Operate(std::vector<CvData const *> const &input,
              std::vector<CvData *> const &output) override;

  std::string name() const override { return "Nn"; }

  int LoadOption(toml::value const &conf) override {
    LOGF(FATAL, "unreachable");
    // return nn_.Load(conf);
    return 1;
  }

  int LoadOptionWithContext(OperatorGraphContext *context,
                            toml::value const &conf) override {
    return nn_.Load(context, conf);
  }

  NnOperator();
  virtual ~NnOperator();

 private:
  NeuralNetwork nn_;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATORS_NN_H_

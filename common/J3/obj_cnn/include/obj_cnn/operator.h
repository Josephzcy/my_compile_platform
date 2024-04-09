// Copyright 2021 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATOR_H_
#define INCLUDE_OBJ_CNN_OPERATOR_H_

#include <cstdio>
#include <ostream>
#include <string>
#include <vector>

#include "toml/value.hpp"
#include "obj_cnn/cv.h"
#include "obj_cnn/operator_graph_context.h"

namespace obj_cnn {

/** Operators modifies CvData.
 */
class Operator {
 public:
  using InputDataList = std::vector<CvData const *>;
  using OutputDataList = std::vector<CvData *>;

  // Note, "output" is a const reference, output count of operator is decided by length
  // of actions outputs, this is a "input value".
  virtual int Operate(InputDataList const &input, OutputDataList const &output) = 0;

  // 用 RegisterOperator 来注册 Operator 时需要用此名字。否则
  // LoadOperatorPrototype 在创建出 Operator 的原型后会报告错误。
  //
  // 原则上，派生类 "{Name}Operator" 的重载应该返回 "{Name}"。
  virtual std::string name() const = 0;

  // 加载此 Operator 的配置。将未成功加载配置的 Operator 对象称作是
  // Operator 原 型。
  //
  // LoadOperatorPrototype
  // 依据构造工厂构建出的原型，需以此函数加载配置之后才能
  // 正常工作，并且再次工作之前不会再次加载配置。即这其实是构造过程的补充。
  virtual int LoadOption(toml::value const &) = 0;

  // 加载配置，并传入 OperatorGraphContext 的指针。算子之间可以通过 context 指针传递数据。
  //
  // 注意，这会导致算子耦合，尽可能使用图和图的组合来规避这种情况。
  virtual int LoadOptionWithContext(OperatorGraphContext *context, toml::value const &conf) { return LoadOption(conf); }

  // 输出此 Operator
  // 的调试描述字符串。派生类可以重载它，输出更详细的信息，从而 使得运行
  // Operator 的过程更透明。
  virtual std::ostream &DebugFormat(std::ostream &out) const { return out << "Operator" << name(); }

  virtual ~Operator();
};

// 一元 Operator，是 1 个输入 1 个输出的 Operator。
//
// 如果要派生出的 Operator 一定只有一个输入和一个输出，那么派生此子类可以避免检
// 查输入和输出参数的数目。
//
// OperatorBinary 也起到类似的作用。
class OperatorUnary : public Operator {
 public:
  virtual int Operate(CvData const &input, CvData &output) = 0;
  virtual ~OperatorUnary();

 private:
  int Operate(std::vector<CvData const *> const &input, std::vector<CvData *> const &output) override;
};

// 二元 Operator，是 2 个输入 1 个输出的 Operator。
class OperatorBinary : public Operator {
 public:
  virtual int Operate(CvData const &first_input, CvData const &second_input, CvData &output) = 0;
  virtual ~OperatorBinary();

 private:
  int Operate(std::vector<CvData const *> const &input, std::vector<CvData *> const &output) override;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATOR_H_

// Copyright 2021 MINIEYE

#ifndef INCLUDE_OBJ_CNN_OPERATOR_GRAPH_H_
#define INCLUDE_OBJ_CNN_OPERATOR_GRAPH_H_

#include <algorithm>
#include <cstddef>
#include <functional>
#include <map>
#include <memory>
#include <mutex>
#include <string>
#include <vector>

#include "obj_cnn/cv.h"
#include "obj_cnn/operator.h"
#include "obj_cnn/operator_graph_context.h"

namespace obj_cnn {

// 创建不同的 Operator 子类的工厂的基类。
class OperatorFactory {
 public:
  // 创建一个派生 Operator 的原型，即不加载配置的 Operator，相见 Operator
  // 的说 明。
  //
  // 当创建产生了错误，需要返回空指针，否则应该返回指向子类对象的指针。
  virtual std::shared_ptr<Operator> Create() = 0;

  virtual ~OperatorFactory();
};

// 一般而言 Operator 应该是可以平凡构造的，这样就可以用 DefaultOperatorFactory
// 模板为它创建默认的构造工厂。否则你可以实现自己的构造工厂。
template <typename OperatorT>
struct DefaultOperatorFactory : public OperatorFactory {
 public:
  // OperatorT 应该是 Operator 的派生类。
  static_assert(std::is_base_of<Operator, OperatorT>::value,
                "Template argument OperatorT of DefaultOperatorFactory should be a "
                "derived class of Operator");

  std::shared_ptr<Operator> Create() override { return std::make_shared<OperatorT>(); }
};

// 注册一个 Operator 类型，name 是 Operator 的名字，即需要与
//
//   std::string Operator::name() const;
//
// 返回值相等。
//
// factory 是创建此 Operator 的工厂对象的指针，不应当是空指针。
//
// 当为了同一个 Operator 以同一个 name 注册两次，靠后的注册会返回错误。当为了同
// 一个 Operator 以不同 name 注册，注册过程会成功，但 LoadOperatorPrototype 会
// 返回错误。
int RegisterOperator(std::string const &name, std::shared_ptr<OperatorFactory> factory);

// Register operator O, named "name" with default operator factory.
template <typename O>
int RegisterOperatorDefault(std::string const &name) {
  return RegisterOperator(name, std::make_shared<DefaultOperatorFactory<O>>());
}

// 创建名字为 name 的 Operator 的原型。当出现了错误时返回空指针。
std::shared_ptr<Operator> LoadOperatorPrototype(std::string const &name);

class OperatorGraph {
 public:
  OperatorGraph();

  // 加载配置。
  //
  // o  对 CvData 的配置：
  //    o  input 数组是输入 CvData 的名字列表。
  //    o  output 数组是输出 CvData 的名字列表。
  //    o  buffer 数组是 OperatorGraph 中间结果的名字列表。
  //    上述的名字两两不能重复，共同组成了 OperatorGraph
  //    的「数据名字空间」。
  //
  // o  operators 数组是计算需要的 Operator 的列表：
  //    o  name 是 Operator 类的名字，需要等于某个被注册的 Operator 的
  //    name()
  //       成员函数的返回值，OperatorGraph 需要以此查找 Operator。
  //    o  id 是此 Operator 实例的名字，一个 OperatorGraph
  //    可以实例化具有同一个
  //       name 的多个 Operator，以此可以使用不同配置的
  //       Operator。OperatorGraph 会以此索引 Operator 实例。
  //    o  其他配置选项会被 name 绑定的 Operator 解析，成为具体 Operator
  //    实例的
  //       配置。
  //
  // o  actions 数组是实际进行的计算的列表：
  //    o  operator-id 是此计算需要的 Operator 的 id。
  //    o  input 数组是此 Operator 输入的数据的名字列表。
  //    o  output 数组是此 Operator 输出的数据的名字列表。
  //    o  input 和 output
  //    的名字需要在「数据名字空间」中。同时可以设置为字符串，
  //       表示输入或输出只有一个。即 input 和 output
  //       会自动从字符串提升为字符 串数组。
  int LoadOption(toml::value const &option);

  // 在输入数据上运行 OperatorGraph。
  //
  // 它是线程安全的。
  int Run(std::vector<CvData> const &input_nodes, std::vector<CvData> &output);

  ~OperatorGraph() {}

  /// Action 包含如下内容的引用：
  ///
  /// 1. 待执行的 Operator
  /// 2. 待执行 Operator 需要的输入。
  /// 3. 待执行 Operator 会产生的输出。
  struct Action {
    size_t ioperator;

    /// 输入数据的地址的地址数组，不论 Run 时输入的 CvData 是什么，它的地址总是写入 input_nodes_ 数组，因此一个 Action
    /// 的输入数据的地址的地址是不变的。因此 inputs 数组可以在 LoadOption 阶段计算好。
    std::vector<const CvData *const *> inputs;

    // 类似 inputs 数组的情况，只是指针类型改变。
    std::vector<CvData *const *> outputs;
  };

  void DumpProfiling();

  toml::value const &misc() const { return misc_; }

 private:
  bool empty_;

  size_t input_count_;
  size_t output_count_;
  size_t buffer_count_;

  std::vector<const CvData *> input_nodes_;
  std::vector<CvData *> output_nodes_;

  std::vector<CvData> buffer_nodes_;
  // buffer_node_pointers_ hold pointers of elements in buffer_nodes_.
  std::vector<CvData *> buffer_node_pointers_;

  std::vector<std::shared_ptr<Operator>> operators_;
  std::vector<Action> actions_;

  // 对 LoadOption 和 Run 加锁。
  //
  // NOTE
  //
  // 我认为这是结构不良的。期待日后与集成过程拿出更好的方案。
  std::mutex mutex_;

  // 打开 profileing。
  bool use_profiling_;

  // 将 ProfilingRecord 列表追加写入此文件。
  std::string profiling_record_;

  struct ProfilingRecord;
  // 记录一次 Run 的用时情况。
  std::vector<ProfilingRecord> profiling_record_list_;

  // misc 配置文件里节的内容。
  toml::value misc_;

  std::shared_ptr<OperatorGraphContext> context_;
};

// 记录一个 Operator 的耗时。
struct OperatorGraph::ProfilingRecord {
  std::string name;
  uint64_t time_cost_us;
};

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_OPERATOR_GRAPH_H_

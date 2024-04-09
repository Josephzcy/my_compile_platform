// Copyright 2022 MINIEYE

#ifndef INCLUDE_OBJ_CNN_DEBUG_H_
#define INCLUDE_OBJ_CNN_DEBUG_H_

#include <string>
#include <vector>

namespace obj_cnn {

// 实现了 DebugStringTrait 的对象可以用 GetDebugString 和 GetDebugStringCstr
// 获取对应的调试字符串表示。
template <typename T>
struct DebugStringTrait {
  static std::string Get(T const &);
};

// 为类 Type 声明 DebugStringTrait。例：
//
//   struct User {
//     std::string name;
//     int age;
//   };
//
//   JCNN_DECLARE_DEBUG_STRING_TRAIT(User);
#define JCNN_DECLARE_DEBUG_STRING_TRAIT(Type) \
  template <>                                 \
  struct DebugStringTrait<Type> {             \
    static std::string Get(Type const &);     \
  }

// 实现 Type 作为 DebugStringTrait 需要的 Get 函数，例：
//
//   JCNN_DECLARE_DEBUG_STRING_IMPL(User, user) {
//     return "User";
//   }
#define JCNN_DECLARE_DEBUG_STRING_IMPL(Type, instance) \
  std::string DebugStringTrait<Type>::Get(Type const &(instance))

template <typename T>
std::string GetDebugString(T const &instance) {
  return DebugStringTrait<T>::Get(instance);
}

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_DEBUG_H_

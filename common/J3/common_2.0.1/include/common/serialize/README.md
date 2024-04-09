# simple-msgpack

Very simple JSON and MessagePack serializer.

## 用法

### 基本用法

```c++
#include "Packer.hpp"

Packer::Value v {          // map
  {"Name", "John Smith"},  // c string
  {"Sex", "Male"},
  {"Age", 20},             // int32_t
  {"Height", 1.80},        // double
  {"Children", {           // nested array
    "Alice Smith",
    "Bob Smith",
    "Carol Smith",
  }},
  {"Phone", {              // nested map
    {"Work", 87654321},
    {"Home", 12345678},
    {"Mobile", 123000111},
  }},
};

// json serialization
Packer::Buffer buf_json = v.to_json();
// print the json string
fwrite(buf_json.data(), 1, buf_json.size(), stdout);

// msgpack serialization
Packer::Buffer buf_msgpack = v.to_msgpack();
// e.g. send its data into a socket
sock.send(buf.data(), buf.size());
```

### 基本类型

目前支持的基本类型有:

* `std::nullptr_t`: 只有1个值: `nullptr` 。注意: 不能使用 `NULL`
* `bool`: 只有2个值: `true` 和 `false`
* integer: `int8_t`, `int16_t`, `int32_t`, `int64_t`
* unsigned integer: `uint8_t`, `uint16_t`, `uint32_t`, `uint64_t`
* float: `float`, `double`
* C string: `const char*`, `char*` 。注意: 必须以 `\0` 结尾
* C++ string: `const std::string`, `std::string`

### 复合类型: Array(数组)

* Array的语法是: `{elem1, elem2, ...}`
* 也可以直接传递 `std::vector<T>`
* Array的元素个数不限
* Array的元素类型不限，各个元素可以是不同的类型

### 复合类型: Map(关联数组，哈希表，字典)

* Map的语法是: `{{key1, val1}, {key2, val2}, ...}`
* 外层 `{}` 里面至少包含1个二级 `{}`
* 二级 `{}` 里面**有且仅有**2个元素，构成一个 **Key-value Pair** (键值对)
* 也可以直接传递 `std::map<K, V>`
* Key, value的类型不限，但是为了与其他语言兼容，key最好使用字符串类型，value可以是各种简单类型或复合类型

### 特殊类型: 二进制数据

二进制数据用 `Blob` 对象传递，构造函数是:

```c++
Blob(const char* data, size_t size);
```

例如:

```c++
std::vector<int8_t> jpg_data;

// create a Blob object
// constructor: Packer::Blob(const char* data, size_t size);
Packer::Blob jpg_blob(jpg_data.data(), jpg_data.size());

Packer::Value v {
  {"Name", "John Smith"},
  {"Photo", jpg_blob},  // use the Blob in initializer list
};
```

### 特殊类型: 序列化数据

如果我们有一些**已经序列化**的 JSON 或 msgpack 数据，可以把它嵌入到某个对象中，打包成一个整体来序列化。

这样做的好处是，这个整体序列化后的数据，只需经过一次 JSON 或 msgpack 反序列化操作就能解析出来，不需要先反序列化整体、再反序列化其中包含的 JSON 或 msgpack 数据。

```c++
Packer::Value child {
  {"Name", "Bob Smith"},
  {"Sex", "Male"},
  {"Age", 1},
};

Packer::Buffer child_data = child.to_msgpack();

// create a Pack object
// constructor: Packer::Pack(const char* data, size_t size);
Packer::Pack child_pack(child_data.data(), child_data.size());

Value v {
  {"Name", "John Smith"},
  {"children", {
    {"Bob", child_pack},  // use the Blob in initializer list
  }},
};
```

## 注意事项

### 临时对象的生命周期

在构造 `Value` 对象时，我们可以使用 `std::string`, `Packer::Blob`, `Packer::Pack` 等类型的临时对象。

由于 `Value` 并不拷贝这些临时对象的数据，而是在调用 `to_msgpack()` 等函数时才会尝试访问它们的数据，这就引出了一个问题： **临时对象的生命周期** 。

假如这些临时对象在 `Value` 之前析构，程序就有可能出现问题。因此，使用者要保证 **临时对象的生命周期不短于 Value 对象** 。

下面的用法是危险的，可能导致程序崩溃：

```c++
Value v{4, 5, 6, Blob(data, 5)};
Buffer buf = v.to_msgpack();
```

解释: 在第一个语句结束时，临时对象 `Blob(data, 5)` 已被销毁，第二个语句调用 `v.to_msgpack()` 需要访问 `Blob` 的内存，可能会触发 *Segment fault* 。

但是，还有一种安全的用法：声明一个 `Value` 对象，不把它赋给任何变量，而是直接调用 `to_msgpack` 方法将其序列化。

```c++
Buffer buf = Value{4, 5, 6, Blob(data, 5)}.to_msgpack();
```

在 `to_msgpack` 函数返回之前，临时对象 `Blob(data, 5)` 不会被销毁。

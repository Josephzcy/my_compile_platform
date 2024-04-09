// Copyright 2021 MINIEYE

#ifndef INCLUDE_OBJ_CNN_CV_H_
#define INCLUDE_OBJ_CNN_CV_H_

#include <cstdio>
#include <memory>
#include <ostream>
#include <string>
#include <vector>

#include "minieye_jsonhpp/json.hpp"
#include "obj_cnn/buffer.h"
#include "obj_cnn/image.h"
#include "obj_cnn/object_type.h"
#include "obj_cnn/interface.h"

#include "toml/value.hpp"

namespace obj_cnn {

class NdArray;

// CvDataType list all data types used in computer vision network IO, except
// network model data.
enum class CvDataType {
  kBuffer,
  kClassification,
  kConfidence,
  kEmpty,
  kImage,
  kList,
  kRectangle,
  kRectangle32F,
  kPoint,
  kPoint32F,
  kBoxSize,
  kBoxSize32F,
  kEllipse32F,
  kVehicleAttribute,
  kPedAttribute,
  kObjDetection,
  kNdArray,
};

template <>
char const *ToStr<CvDataType>(const CvDataType &);

// POD type to make a union.
//
// Convert to cv::Rect.
struct Rectangle {
  int x;
  int y;
  int width;
  int height;
};

// POD type to make a union.
//
// Convert to cv::Rect2f.
struct Rectangle32F {
  float x;
  float y;
  float width;
  float height;
};

// POD type to make a union.
//
// Convert to cv::Point2f.
struct Point32F {
  float x;
  float y;
};

// POD type to make a union.
//
// Convert to cv::Point.
struct Point {
  int x;
  int y;
};

// POD type to make a union.
//
// Convert to cv::Size.
struct BoxSize {
  int width;
  int height;
};

// POD type to make a union.
//
// Convert to cv::Size2f.
struct BoxSize32F {
  float width;
  float height;
};

// POD type to make a union.
struct Ellipse32F {
  Point32F point;
  BoxSize32F size;
  float angle;
};

// Base class for complex data, CvData will hold a pointer of this.
class CvDataComplex {
 public:
  virtual ~CvDataComplex() {}
};

class CvDataObjDetection;

// CvData is a sum type of types listed in CvDataType.
class CvData {
 public:
  using Self = CvData;

  // Create a empty CvData.
  CvData() : type_(CvDataType::kEmpty) {}

  CvData(const CvData &) = delete;
  CvData &operator=(const CvData &) = delete;

  CvData(CvData &&) = default;
  CvData &operator=(CvData &&) = default;

  // struct MakeRectangle32F {};
  // CvData(MakeRectangle32F, float x, float y, float w, float h);

  struct MakeConfidence {};
  CvData(MakeConfidence, float confidence);

  struct MakeClassification {};
  CvData(MakeClassification, int classification);

  struct MakeList {};
  CvData(MakeList, std::vector<CvData> &&list);

  struct MakeObjDetection {};
  CvData(MakeObjDetection, std::vector<ObjectDetectionTypedSet> &&list);

  struct MakeBuffer {};
  CvData(MakeBuffer, std::shared_ptr<BufferView> const &buffer);

  struct MakeNdArray {};
  CvData(MakeNdArray, std::shared_ptr<NdArray> const &array);

  CvDataType type() const { return type_; }

  // Check variants functions:
  bool IsBuffer() const { return type_ == CvDataType::kBuffer; }
  bool IsClassification() const { return type_ == CvDataType::kClassification; }
  bool IsConfidence() const { return type_ == CvDataType::kConfidence; }
  bool IsEmpty() const { return type_ == CvDataType::kEmpty; }
  bool IsImage() const { return type_ == CvDataType::kImage; }
  bool IsList() const { return type_ == CvDataType::kList; }
  bool IsRectangle() const { return type_ == CvDataType::kRectangle; }
  bool IsRectangle32F() const { return type_ == CvDataType::kRectangle32F; }
  bool IsPoint() const { return type_ == CvDataType::kPoint; }
  bool IsPoint32F() const { return type_ == CvDataType::kPoint32F; }
  bool IsBoxSize() const { return type_ == CvDataType::kBoxSize; }
  bool IsBoxSize32F() const { return type_ == CvDataType::kBoxSize32F; }
  bool IsEllipse32F() const { return type_ == CvDataType::kEllipse32F; }
  bool IsVehicleAttribute() const {
    return type_ == CvDataType::kVehicleAttribute;
  }
  bool IsPedAttribute() const { return type_ == CvDataType::kPedAttribute; }
  bool IsObjDetection() const { return type_ == CvDataType::kObjDetection; }
  bool IsNDArray() const { return type_ == CvDataType::kNdArray; }

  // Convert CvData into variants.
  //
  // int AsVariant(Variant &) const;

  int AsImage(Image &image) const;
  int AsClassification(int &c) const;
  int AsConfidence(float &c) const;
  int AsRectangle(cv::Rect &rect) const;
  int AsRectangle32F(cv::Rect2f &rect) const;
  int AsPoint(cv::Point &point) const;
  int AsPoint32F(cv::Point2f &point) const;
  int AsBoxSize(cv::Size &size) const;
  int AsBoxSize32F(cv::Size2f &size) const;
  int AsEllipse32F(Ellipse32F &ellipse) const;
  // int AsList(std::vector<CvData> &list) const;
  int AsBuffer(std::shared_ptr<BufferView> &buffer) const;
  int AsVehicleAttribute(VehicleAttribute *attribute) const;
  int AsPedAttribute(PedAttribute *attribute) const;

  int AsObjectDetectionTypedSet(
      std::vector<ObjectDetectionTypedSet> **objset) const;

  int AsObjectDetectionTypedSetShared(std::shared_ptr<CvDataObjDetection> *objset) const;

  int AsNdArray(std::shared_ptr<NdArray> &array) const;

  // 此类型是 List 时返回对应的指针，否则返回 nullptr。
  std::shared_ptr<std::vector<CvData>> AsListPtr() const;

  // 此类型是 List 是与 list 交换内存，返回 0，否则返回非 0。
  int AsListSwap(std::vector<CvData> &list) const;

  // Make a clone of this CvData, return error if this data do not allow clone internal.
  int Clone(CvData *that) const;

  static Self FromImage(Image const &image);
  static Self FromImage(Image &&image);
  static Self FromRectangle(cv::Rect rect);
  static Self FromRect(cv::Rect rect);
  static Self FromRectangle32F(cv::Rect2f rect);
  static Self FromBuffer(std::shared_ptr<BufferView> const &buffer);
  static Self FromClassification(int c);
  static Self FromConfidence(float c);
  // static Self FromList(std::vector<CvData> const &list);
  static Self FromListSwap(std::vector<CvData> &list);
  static Self FromListMove(std::vector<CvData> &&list);
  static Self FromListPtr(std::shared_ptr<std::vector<CvData>> list);
  static Self FromPoint(cv::Point point);
  static Self FromPoint32F(cv::Point2f point);
  static Self FromBoxSize(cv::Size point);
  static Self FromBoxSize32F(cv::Size2f point);
  static Self FromEllipse32F(Ellipse32F const &ellipse);
  static Self FromVehicleAttribute(VehicleAttribute const &attribute);
  static Self FromPedAttribute(PedAttribute const &attribute);
  static Self FromNdArray(std::shared_ptr<NdArray> const &array);
  static Self FromObjDetection(std::shared_ptr<CvDataObjDetection> objset);

  //
  // Misc
  //

  std::string DebugString() const;

  /*! Convert this data into a TOML value, toml.hpp define this interface, so
   * this function name not a pascal case. */
  toml::value into_toml() const;

  minieye_jsonhpp::json ToJsonValue() const;

 private:
  class Private;

  CvDataType type_;

  // Union of simple data.
  union Union {
    int classification;
    float confidence;
    Point point;
    Point32F point32f;
    BoxSize box_size;
    BoxSize32F box_size_32f;
  } union_;

  // Pointer of complex data.
  std::shared_ptr<CvDataComplex> complex_;
};

// Create a wrapper struct to type `INNER` as a subtype of CvData named
// `CvData{NAME}`.
#define OBJ_CNN_CVDATA_SUBTYPE(NAME, INNER)                                        \
  class CvData##NAME : public CvDataComplex {                                      \
   public:                                                                         \
    using Inner = INNER;                                                           \
    explicit CvData##NAME(Inner inner_) : inner(std::move(inner_)) {} /* NOLINT */ \
                                                                                   \
    minieye_jsonhpp::json ToJsonValue() const;                                     \
                                                                                   \
    virtual ~CvData##NAME() {}                                                     \
                                                                                   \
    INNER inner;                                                                   \
  };

OBJ_CNN_CVDATA_SUBTYPE(Rectangle, Rectangle);
OBJ_CNN_CVDATA_SUBTYPE(Rectangle32F, Rectangle32F);
OBJ_CNN_CVDATA_SUBTYPE(Ellipse32F, Ellipse32F);
OBJ_CNN_CVDATA_SUBTYPE(VehicleAttribute, VehicleAttribute);
OBJ_CNN_CVDATA_SUBTYPE(PedAttribute, PedAttribute);
OBJ_CNN_CVDATA_SUBTYPE(BufferView, std::shared_ptr<BufferView>);
OBJ_CNN_CVDATA_SUBTYPE(Image, Image);
OBJ_CNN_CVDATA_SUBTYPE(List, std::shared_ptr<std::vector<CvData>>);
OBJ_CNN_CVDATA_SUBTYPE(ObjDetection, std::vector<ObjectDetectionTypedSet>);
OBJ_CNN_CVDATA_SUBTYPE(NdArray, std::shared_ptr<NdArray>);

}  // namespace obj_cnn

#endif  // INCLUDE_OBJ_CNN_CV_H_

#ifndef R_MINIEYE_SOFTWARE_GROUP_DDS_CPP_API_H
#define R_MINIEYE_SOFTWARE_GROUP_DDS_CPP_API_H

#include <memory>
#include <string>
#include <thread>

#include "dds_capi.h"

namespace minieye {
namespace DDS {

struct ContentHint {};

/*
@brief: Init context.
*/
class Context {
 public:
  /*
  @brief: context constructor.
  @param config_file[in]: the path of config file.
  @param enable_thread[in]: when you want to use notify mode, you should
      set enable_thread = true, it will start a thread to call callback
      function.
  */
  explicit Context(const std::string& config_file, bool enable_thread = false);

  /*
  @brief: context constructor.
  @param config_content[in]: The content of config json.
  @param content_hint[in]: It's just a placeholder.
  @param enable_thread[in]: when you want to use notify mode, you should
      set enable_thread = true, it will start a thread to call callback
      function.
  */
  explicit Context(const std::string& config_content, ContentHint content_hint,
                   bool enable_thread = false);

  /*
  @brief: context destructor.
  */
  virtual ~Context();

 public:
  DDSContext* m_ctx;
  bool m_enable_thread;
  std::thread m_thread;
};

/*
@brief: Writer, write data to dds.
*/
class Writer {
 public:
  /*
  @brief: writer constructor.
  @param: ctx[in]: context.
  @param: topic[in]: which topic will be write to.
  */
  explicit Writer(Context* ctx, const std::string& topic);

  /*
  @brief: writer destructor.
  */
  virtual ~Writer();

  /*
  @brief: write data to dds.
  @param buf[in]: Buffer write to specified topic.
  @param buf_length[in]: Length of buf.
  @return On success: 0
          On error: error code is returned.
  */
  int Write(const void* buf, uint32_t buf_length);

  /*
  @brief: Get pointer to array[index].
  @param index[in]: array[index].
  @param id[out]: (optional) unique id.
  @param size[out]: (optinal) node current data size.
  @return pointer to array[index].
  */
  void* GetPtr(size_t index, size_t* id = nullptr, size_t* size = nullptr);

  /*
  @brief: Get pointer to array[latest_index].
    array[latest_index] means oldest data, it will be overwrite soon.
  @Notice: Reader.GetLatestPtr() point to array[latest_index - 1].
  @param id[out]: (optional) unique id.
  @param index[out]: (optional) index = latest_index.
  @param size[out]: (optinal) node current data size.
  @return pointer to array[latest_index].
  */
  void* GetLatestPtr(size_t* id = nullptr, size_t* index = nullptr,
                     size_t* size = nullptr);

  /*
  @brief: When you write data with raw pointer, you have to call
    this function to modify inner information and to notify peer.
  @param datalen[in]: Length of data you write to raw pointer.
  @return On success: 0
        On error: error code is returned.
  */
  int Commit(size_t datalen);

 protected:
  DDSContext* m_ctx;
  std::string m_topic;
};

/*
@brief: Reader, read data from dds.
*/
class Reader {
 public:
  /*
  @brief: reader constructor.
  @param ctx[in]: context.
  @param topic[in]: which topic will read from.
  @param callback[in]: default is null, you can call Read() or ReadLatest() to
      polling data. If you specify callback, it will use notify mode, and you
      should set enable_thread=true in Context constructor.
  @param user_data[in]: it will passed to callback function.
  */
  explicit Reader(Context* ctx, const std::string& topic,
                  DDS_OnDataCallBack callback = nullptr,
                  void* user_data = nullptr);

  /*
  @brief: reader destructor.
  */
  virtual ~Reader();

  /*
  @brief: Read data from topic, array[index].
  @param index[in]: index of array [0 ~ node_num).
  @param buf[out]: Buffer used to store received data.
  @param len[in|out]: Input is length of buf, Output is data length.
  @param id[out]: (optional) unique id.
  @return On success: 0
          On error: error code is returned.
  */
  int Read(size_t index, void* buf, size_t* len, size_t* id = nullptr);

  /*
  @brief: Read data from topic, array[latest_index - 1].
  @param buf[out]: Buffer used to store received data.
  @param len[in|out]: Input is length of buf, Output is data length.
  @param id[out]: (optional) unique id.
  @param index[out]: (optional) index = latest_index - 1.
  @return On success: 0
          On error: error code is returned.
  */
  int ReadLatest(void* buf, size_t* len, size_t* id = nullptr,
                 size_t* index = nullptr);

  /*
  @brief: Get current valid id range, [start, end).
  @param start[out]: start id (inclusive).
  @param end[out]: end id (exclusive).
  @return On success: 0
          On error: error code is returned.
  */
  int GetIDRange(size_t* start, size_t* end);

  /*
  @brief: Read data by unique id.
  @param id[in]: unique id.
  @param buf[out]: Buffer used to store received data.
  @param len[in|out]: Input is length of buf, Output is data length.
  @param index[out]: array[index].
  @return On success: 0
          On error: error code is returned.
  */
  int ReadByID(size_t id, void* buf, size_t* len, size_t* index = nullptr);

  /*
  @brief: Read data by unique id with timeout.
    If `id` is too small, it will return error immediately.
    If `id` is too big, it will try to wait for data until timeout.
    while (check timeout_millis) {
      polling...
      sleep(sleep_millis)
    }
  @param id[in]: unique id.
  @param buf[out]: Buffer used to store received data.
  @param len[in|out]: Input is length of buf, Output is data length.
  @param timeout_millis[in]: timeout in milliseconds. (default: 1000
  milliseconds)
  @param sleep_millis[in]: sleep in milliseconds. (default: 100 milliseconds)
  @param index[out]: array[index].
  @return On success: 0
          On error: error code is returned.
  */
  int ReadByIDTimeout(size_t id, void* buf, size_t* len,
                      size_t* index = nullptr, uint64_t timeout_millis = 1000,
                      uint64_t sleep_millis = 100);

  /*
  @brief: Read latest node data by polling with timeout.
    If latest_node.id is euqal to check_id,
    it will polling until they are not equal, or timeout.
  @param check_id[in|out]:
    input is check id, used to compare to latest node id.
    output is latest node id, output can be used for next read_polling().
  @param buf[out]: Buffer used to store received data.
  @param len[in|out]: Input is length of buf, Output is data length.
  @param index[out]: array[index].
  @param timeout_millis[in]: timeout in milliseconds.
  @param sleep_millis[in]: sleep in milliseconds.
  @return On success: 0
          On error: error code is returned.
  */
  int ReadPolling(size_t* check_id, void* buf, size_t* len,
                  size_t* index = nullptr, uint64_t timeout_millis = 1000,
                  uint64_t sleep_millis = 100);

  /*
  @brief: Read latest data until received one nanomsg notify.
    If this function is first time called for topic,
    this function will create a new nanomsg client for this topic.
  @param buf[out]: Buffer used to store received data.
  @param len[in|out]: Input is length of buf, Output is data length.
  @param id[out]: unique id.
  @param index[out]: array[index].
  @param timeout_millis[in]: timeout in milliseconds.
    -1 means infinite timeout.
  @return On success: 0
          On error: error code is returned.
  */
  int ReadNotify(void* buf, size_t* len, size_t* id = nullptr,
                 size_t* index = nullptr, int32_t timeout_millis = -1);

  /*
  @brief: Get pointer to array[index].
  @param index[in]: array[index].
  @param id[out]: (optional) unique id.
  @param size[out]: (optinal) node current data size.
  @return pointer to array[index].
  */
  void* GetPtr(size_t index, size_t* id = nullptr, size_t* size = nullptr);

  /*
  @brief: Get pointer to array[latest_index - 1].
    array[latest_index - 1] means latest data.
  @Notice: Writer.GetLatestPtr() point to array[latest_index].
  @param id[out]: (optional) unique id.
  @param index[out]: (optional) index = latest_index - 1.
  @param size[out]: (optinal) node current data size.
  @return pointer to array[latest_index - 1].
  */
  void* GetLatestPtr(size_t* id = nullptr, size_t* index = nullptr,
                     size_t* size = nullptr);

  /*
  @brief: Get pointer to array[latest_index - 1], until received one nanomsg notify.
    array[latest_index - 1] means latest data.
  @Notice: Writer.GetLatestPtr() point to array[latest_index].
  @param id[out]: (optional) unique id.
  @param index[out]: (optional) index = latest_index - 1.
  @param size[out]: (optinal) node current data size.
  @param timeout_millis[in]: timeout in milliseconds.
    -1 means infinite timeout.
  @return pointer to array[latest_index - 1].
  */
  void* GetLatestPtrNotify(size_t* id = nullptr, size_t* index = nullptr,
                     size_t* size = nullptr, int32_t timeout_millis = -1);

  /*
  @brief: Get topic's array capacity.
  @param cap[out]: capacity.
  @return On success: 0
          On error: error code is returned.
  */
  int Capacity(size_t* cap);

 protected:
  DDSContext* m_ctx;
  std::string m_topic;
};

}  // namespace DDS
}  // namespace minieye

#endif

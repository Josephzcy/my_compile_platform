#ifndef R_MINIEYE_SOFTWARE_GROUP_DDS_CAPI_H
#define R_MINIEYE_SOFTWARE_GROUP_DDS_CAPI_H

#include <inttypes.h>
#include <stdio.h>

#ifdef __cplusplus
extern "C" {
#endif

/*
@brief: DDS_OnDataCallBack, it will be callled when new data push.
@param topic[in]: topic is the name for shared memory.
@param index[in]: index of array [0 ~ node_num).
@param ptr[in]: pointer to array[index].
@param size[in]: size of array[index] in bytes.
@param user[in]: user data.
*/
typedef void (*DDS_OnDataCallBack)(const char *topic, size_t index, void *ptr, size_t size, void *user);

typedef struct _DDSContext DDSContext;

/*
@brief: Create one DDSContext opaque struct type.
@param config[in]: a json config string.
  example:
  {
    "topics": [
      {
        "topic": "adas",
        "dds_mode": "shm",
        "url": "ipc:///tmp/demo_dds.ipc",
        "buff_num": 10,
        "elem_max_size": 1000
      },
      {
        "topic": "dms",
        "dds_mode": "shm",
        "url": "tcp://127.0.0.1:8080",
        "buff_num": 100,
        "elem_max_size": 10000
      }
    ],
    "log_level": 5
  }

  @param topic: topic name.
  @param dds_mode: "shm" or "redis".
  @param url: (optional) nanomsg url.
    If no url field, it will auto generated from topic.
  @param buff_num: node numbers in fixed-size array.
  @param elem_max_size: every node's max size (bytes).
  @param log_level: (optional)
        1: Error, 2: Warn, 3: Info, 4: Debug, 5: Trace
@return DDSContext.
*/
DDSContext *dds_init(const char *config);

/*
@brief: The same as dds_init, but read config from config_file.
*/
DDSContext *dds_init_with_file(const char *config_file);

/*
@brief: Release ctx control object. This function will not
  delete shared memory from operating system.
*/
void dds_release(DDSContext *ctx);

/*
@brief: Add one writer to DDSContext.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@return On success: 0.
        On error: error code is returned.
*/
int dds_add_writer(DDSContext *ctx, const char *topic);

/*
@brief: Add one consumer to DDSContext.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@param OnDataCallBack[in]: callback.
@param user[in]: user data will be passed to onData.
@return On success: 0.
        On error: error code is returned.
*/
int dds_add_reader(DDSContext *ctx, const char *topic, DDS_OnDataCallBack func, void *user);

/*
@brief: Write data to topic, array[nput].
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@param buf[in]: Buffer write to array[index].
@param len[in]: Length of buf.
@return On success: 0
        On error: error code is returned.
*/
int dds_write(DDSContext *ctx, const char *topic, const void *buf, size_t len);

/*
@brief: Read data from topic, array[index].
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@param index[in]: index of array [0 ~ node_num).
@param buf[out]: Buffer used to store received data.
@param len[in|out]: Input is length of buf, Output is data length.
@param id[out]: unique id.
@return On success: 0
        On error: error code is returned.
*/
int dds_read(DDSContext *ctx, const char *topic, size_t index, void *buf, size_t *len, size_t *id);

/*
@brief: Read data from topic, array[latest_index - 1].
  It's a ring, `latest_index` point to oldest data, `latest_index - 1` point to
latest data.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@param buf[out]: Buffer used to store received data.
@param len[in|out]: Input is length of buf, Output is data length.
@param id[out]: unique id.
@param index[out]: index = latest_index - 1.
@return On success: 0
        On error: error code is returned.
*/
int dds_read_latest(DDSContext *ctx, const char *topic, void *buf, size_t *len, size_t *id, size_t *index);

/*
@brief: Get current valid id range, [start, end).
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@param start[out]: start id (inclusive).
@param end[out]: end id (exclusive).
@return On success: 0
        On error: error code is returned.
*/
int dds_get_id_range(DDSContext *ctx, const char *topic, size_t *start, size_t *end);

/*
@brief: Read data by unique id.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@param id[in]: unique id.
@param buf[out]: Buffer used to store received data.
@param len[in|out]: Input is length of buf, Output is data length.
@param index[out]: array[index].
@return On success: 0
        On error: error code is returned.
*/
int dds_read_by_id(DDSContext *ctx, const char *topic, size_t id, void *buf, size_t *len, size_t *index);

/*
@brief: Read data by unique id with timeout.
  If `id` is too small, it will return error immediately.
  If `id` is too big, it will try to wait for data until timeout.
  while (check timeout_millis) {
    polling...
    sleep(sleep_millis)
  }
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@param id[in]: unique id.
@param buf[out]: Buffer used to store received data.
@param len[in|out]: Input is length of buf, Output is data length.
@param index[out]: array[index].
@param timeout_millis[in]: timeout in milliseconds.
@param sleep_millis[in]: sleep in milliseconds.
@return On success: 0
        On error: error code is returned.
*/
int dds_read_by_id_timeout(DDSContext *ctx, const char *topic, size_t id, void *buf, size_t *len, size_t *index,
                           uint64_t timeout_millis, uint64_t sleep_millis);

/*
@brief: Read latest node data by polling with timeout.
  If latest_node.id is euqal to check_id,
  it will polling until they are not equal, or timeout.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
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
int dds_read_polling(DDSContext *ctx, const char *topic, size_t *check_id, void *buf, size_t *len, size_t *index,
                     uint64_t timeout_millis, uint64_t sleep_millis);

/*
@brief: Read latest data until received one nanomsg notify.
  If this function is first time called for topic,
  this function will create a new nanomsg client for this topic.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@param buf[out]: Buffer used to store received data.
@param len[in|out]: Input is length of buf, Output is data length.
@param id[out]: unique id.
@param index[out]: array[index].
@param timeout_millis[in]: timeout in milliseconds.
  -1 means infinite timeout.
@return On success: 0
        On error: error code is returned.
*/
int dds_read_notify(DDSContext *ctx, const char *topic, void *buf, size_t *len, size_t *id, size_t *index,
                    int32_t timeout_millis);

/*
@brief: Get pointer to array[latest_index - 1], until received one nanomsg
  notify. `latest_index - 1` point to latest data,
  usually this function is for read.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@param id[out]: unique id.
@param index[out]: index = latest_index - 1.
@param size[out]: node current data size.
@param timeout_millis[in]: timeout in milliseconds.
  -1 means infinite timeout.
@return pointer to array[latest_index - 1].
*/
void *dds_ptr_latest_pre_notify(DDSContext *ctx, const char *topic, size_t *id, size_t *index, size_t *size,
                                int32_t timeout_millis);

/*
@brief: Get pointer to array[latest_index - 1].
  `latest_index - 1` point to latest data, usually this function is for read.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@param id[out]: unique id.
@param index[out]: index = latest_index - 1.
@param size[out]: node current data size.
@return pointer to array[latest_index - 1].
*/
void *dds_ptr_latest_pre(DDSContext *ctx, const char *topic, size_t *id, size_t *index, size_t *size);

/*
@brief: Get pointer to array[index].
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@param index[in]: array[index].
@param id[out]: unique id.
@param size[out]: node current data size.
@return pointer to array[index].
*/
void *dds_ptr(DDSContext *ctx, const char *topic, size_t index, size_t *id, size_t *size);

/*
@brief: Get pointer to array[latest_index].
  `latest_index` point to oldest data, usually this function is for write.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@param id[out]: unique id.
@param index[out]: index = latest_index.
@param size[out]: node current data size.
@return pointer to array[latest_index].
*/
void *dds_ptr_latest(DDSContext *ctx, const char *topic, size_t *id, size_t *index, size_t *size);

/*
@brief: When you write data with raw pointer, you have to call
  this function to modify inner information and to notify peer.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@param datalen[in]: Length of data you write to raw pointer.
@return On success: 0
        On error: error code is returned.
*/
int dds_commit(DDSContext *ctx, const char *topic, size_t datalen);

/*
@brief: register onData call back.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@param OnDataCallBack[in]: callback.
@param user[in]: user data will be passed to onData.
*/
void dds_register_read_callback(DDSContext *ctx, const char *topic, DDS_OnDataCallBack func, void *user);

/*
@brief: Enter event loop, it will call onData callback.
@param ctx[in]: ctx.
*/
void dds_enter_event_loop(DDSContext *ctx);

/*
@brief: Exit event loop.
@param ctx[in]: ctx.
*/
void dds_loopexit(DDSContext *ctx);

/*
@brief: Disable event loop.
*/
void dds_disable_event_loop(DDSContext *ctx);

/*
@brief: Enable event loop.
*/
void dds_enable_event_loop(DDSContext *ctx);

/*
@brief: Get topic's array capacity.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@param cap[out]: capacity.
@return On success: 0
        On error: error code is returned.
*/
int dds_cap(DDSContext *ctx, const char *topic, size_t *cap);

/*
@brief: Get topic's array node_size.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@param node_size[out]: node_size.
@return On success: 0
        On error: error code is returned.
*/
int dds_node_size(DDSContext *ctx, const char *topic, size_t *node_size);

/*
@brief: Enable topic write.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@return On success: 0
        On error: error code is returned.
*/
int dds_enable_write(DDSContext *ctx, const char *topic);

/*
@brief: Disable topic write. After call this function, `dds_write` will do nothing, just return success.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@return On success: 0
        On error: error code is returned.
*/
int dds_disable_write(DDSContext *ctx, const char *topic);

/*
@brief: Enable topic's callback function.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@return On success: 0
        On error: error code is returned.
*/
int dds_enable_callback(DDSContext *ctx, const char *topic);

/*
@brief: Disable topic's callback function.
@param ctx[in]: ctx.
@param topic[in]: topic is the name for shared memory.
@return On success: 0
        On error: error code is returned.
*/
int dds_disable_callback(DDSContext *ctx, const char *topic);

#ifdef __cplusplus
}
#endif

#endif

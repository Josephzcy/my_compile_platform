#ifndef     __DATA_PARSER__
#define     __DATA_PARSER__


typedef int32_t (*data_callback)(const char *data, uint32_t size);

void * json_parser_init(void);
int32_t json_parser_read(void *handle, int fd, data_callback cb);
void json_parser_deinit(void *handle);
#endif //__DATA_PARSER__

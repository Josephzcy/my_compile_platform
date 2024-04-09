#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <unistd.h>
#include <fcntl.h>
#include <pthread.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <sys/wait.h>

#include "data_parser.h"

#define FTLIB_DEBUG(fmt, args...) do {printf("[FTLIB_D#%05d] " fmt, __LINE__, ## args);} while(0)
#define FTLIB_ERROR(fmt, args...) do {fprintf(stderr, "[FTLIB_E#%05d] " fmt, __LINE__, ## args);} while(0)

struct data_parser
{
    uint32_t    magic;

    char        *buffer;
    uint32_t    capacity;
    uint32_t    got;
};

#define PARSER_DEF_BUF_SIZE (64 * 1024)
#define PARSER_MAGIC_LINE   0x6C696E65
#define PARSER_MAGIC_JSON   0x6A61736E
void *json_parser_init(void)
{
    struct data_parser *parser = (struct data_parser*) malloc(sizeof(struct data_parser));
    if (NULL == parser) {
        FTLIB_ERROR("malloc parser failed\n");
        goto INIT_PARSER_FAILED;
    }
    memset(parser, 0, sizeof(*parser));

    parser->magic = PARSER_MAGIC_JSON;
    parser->buffer = (char *) malloc(PARSER_DEF_BUF_SIZE);
    if (NULL == parser->buffer) {
        FTLIB_ERROR("malloc parser buffer failed\n");
        goto INIT_PARSER_FAILED;
    }
    parser->capacity = PARSER_DEF_BUF_SIZE;
    parser->got = 0;

    return parser;

INIT_PARSER_FAILED:
    if (parser && parser->buffer) {
        free(parser->buffer);
    }
    if (parser) {
        free(parser);
    }
    return NULL;
}

void json_parser_deinit(void *handle)
{
    struct data_parser *parser = (struct data_parser *) handle;
    if (!parser) {
        return;
    }
    if (parser->buffer) {
        free(parser->buffer);
    }
    memset(parser, 0, sizeof(*parser));
    free(parser);
}

int32_t json_parser_read(void *handle, int fd, data_callback cb)
{
    int32_t ret = 0;
    uint32_t size = 0;
    uint32_t size_be = 0;
    #define HEADER_SIZE ((uint32_t)sizeof(size_be))
    #define JSON_DATA_SIZE  (parser->got - HEADER_SIZE)
    uint32_t to_read = 0;
    struct data_parser *parser = (struct data_parser *) handle;

    FTLIB_DEBUG("json_parser_read %p %d %p\n", handle, fd, cb);
    if (PARSER_MAGIC_JSON != parser->magic) {
        FTLIB_ERROR("%p is not a valid json parser handle magic 0x%x\n",
                parser, parser->magic);
        return -1;
    }
    for (uint32_t i = 0; i < 2; i++) {
        if (parser->got >= HEADER_SIZE) {
            size_be = *((uint32_t*) parser->buffer);
            size = ntohl(size_be);
            /*
             * +1 since we need a tailing \0
             */
            if (size + HEADER_SIZE + 1 >= parser->capacity) {
                FTLIB_ERROR("%d exceeded parser buffer capacity %d\n",
                        size, parser->capacity);
                return -1;
            }
            to_read = size - (parser->got - HEADER_SIZE);
        } else {
            to_read = HEADER_SIZE - parser->got;
        }
        FTLIB_DEBUG("read 0x%p %06d + %06d\n",
                parser->buffer, parser->got, to_read);
        ret = read(fd, parser->buffer + parser->got, to_read);
        if (ret < 0) {
            FTLIB_ERROR("read failed return %d %s\n", ret, strerror(errno));
            return ret;
        } else if (0 == ret) {
            FTLIB_ERROR("read return EOF\n");
            return -1;
        }
        parser->got += ret;

        if (size && parser->got >= (size + HEADER_SIZE)) {
            break;
        }
    }

    if (0 == size || JSON_DATA_SIZE < size) {
        FTLIB_ERROR("need more data size %d got %d\n", size, parser->got);
        return 0;
    } else if (size && JSON_DATA_SIZE == size) {
        * (parser->buffer + parser->got) = '\0';
        cb(parser->buffer + sizeof(size_be), size);
        parser->got = 0;
    } else {
        FTLIB_ERROR("Invalid size %d got %d\n", size, parser->got);
        parser->got = 0;
        return -1;
    }

    return 0;
}


#pragma once

#include <memory>
#include <functional>

class nn_req {
   public:
    nn_req(const char *url, size_t rbuf_len);
    int send(const void *buf, size_t len, int flags,
             std::function<int(const void *, int)> rcallback);

   private:
    class nn_req_impl;
    std::shared_ptr<nn_req_impl> pimp;
};

class nn_rep {
   public:
    nn_rep(
        const char *url,
        std::function<int(const void *, int, void *, int, int *)> repcallback,
        size_t rbuf_len, size_t sbuf_len);
#if 1
    nn_rep(
        const char *url,
        std::function<int(const void *, int)> repcallback,
        size_t buf_len);
#endif

   private:
    class nn_rep_impl;
    std::shared_ptr<nn_rep_impl> pimp;
};

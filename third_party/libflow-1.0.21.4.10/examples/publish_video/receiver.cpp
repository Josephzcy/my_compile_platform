#include <errno.h>
#include <stdlib.h>
#include <mongoose.h>
#include <map>
#include <sstream>
#include <iostream>
#include <msgpack.hpp>
#include "client1.h"

static int s_done = 0;
static int s_is_connected = 0;

static void ev_handler(struct mg_connection *nc, int ev, void *ev_data) {
  switch (ev) {
    case MG_EV_CONNECT: {
      int status = *(int*)ev_data;
      if (status != 0) {
        fprintf(stderr, "Connection error: %s\n", strerror(status));
      }

      break;
    }
    case MG_EV_WEBSOCKET_HANDSHAKE_DONE: {
      s_is_connected = 1;

      std::map<std::string, std::string> msg;
      msg["source"] = "test_client";
      msg["topic"] = "subscribe";
      msg["data"] = "*";
      msgpack::sbuffer sb;
      msgpack::pack(sb, msg);

      mg_send_websocket_frame(nc, WEBSOCKET_OP_BINARY, sb.data(), sb.size());
      break;
    }
    case MG_EV_WEBSOCKET_FRAME: {
      struct websocket_message *wm = (struct websocket_message *) ev_data;
      int opcode = wm->flags & 0xf;
      bool is_binary = opcode == WEBSOCKET_OP_BINARY;
      if (is_binary) {
        decode_server_message((const char*)wm->data, wm->size);
      } else {
        fprintf(stderr, "Error: invalid websocket opcode: %d\n", opcode);
      }

      break;
    }
    case MG_EV_CLOSE: {
      if (s_is_connected) {
        s_is_connected = false;
        fprintf(stderr, "Disconnected\n");
      }

      s_done = 1;
      break;
    }
  }
}

void RecvThreadFunc(void) {
  // std::string uri = "ws://localhost:24012";
  std::string uri = "ws://127.0.0.1:24012";

  struct mg_mgr mgr;
  mg_mgr_init(&mgr, NULL);

  struct mg_connection *nc;
  nc = mg_connect_ws(&mgr, ev_handler, uri.c_str(), NULL, NULL);
  if (nc == NULL) {
    fprintf(stderr, "Error: mg_connect_ws: invalid address\n");
    return;
  }

  while (s_done == 0) {
    mg_mgr_poll(&mgr, 1000);
  }

  mg_mgr_free(&mgr);

  fprintf(stderr, "the th1 thread is done\n");
}

MESSAGE(STATUS "CMAKE_SYSTEM_NAME: ${CMAKE_SYSTEM_NAME}")

ADD_DEFINITIONS(
  -DMG_ENABLE_SSL=0
  -DMG_ENABLE_IPV6=0
  -DMG_ENABLE_MQTT=0
  -DMG_ENABLE_MQTT_BROKER=0
  -DMG_ENABLE_DNS_SERVER=0
  -DMG_ENABLE_COAP=0
  -DMG_ENABLE_HTTP=1
  -DMG_ENABLE_HTTP_CGI=0
  -DMG_ENABLE_HTTP_SSI=0
  -DMG_ENABLE_HTTP_SSI_EXEC=0
  -DMG_ENABLE_HTTP_WEBDAV=0
  -DMG_ENABLE_HTTP_WEBSOCKET=1
  -DMG_ENABLE_DIRECTORY_LISTING=0
  -DMG_ENABLE_FILESYSTEM=0
  -DMG_ENABLE_BROADCAST=1
  -DMG_ENABLE_GETADDRINFO=0
  -DMG_ENABLE_THREADS=1
)

ADD_LIBRARY(flow STATIC
  flow_inner.cpp
  context.cpp
  event_loop.cpp
  outmsg.cpp
  sock_pair.cpp
  sender.cpp
  sender_impl.cpp
  client_conn.cpp
  web_server.cpp
  http_conn.cpp
  http_request.cpp
  http_response.cpp
  client.cpp
  utils.cpp
  String.cpp
  platform.cpp
  ../vendor/mongoose/mongoose.c
)

IF(${CMAKE_SYSTEM_NAME} MATCHES "Linux")
 TARGET_LINK_LIBRARIES(flow pthread)
ENDIF()

const WebSocket = require('ws');
const msgpack = require('@msgpack/msgpack');

const ws = new WebSocket('ws://localhost:24012/');

const packMessage = function (source, topic, data) {
  const obj = {source, topic, data};
  return msgpack.encode(obj);
};

ws.on('open', function open() {
  const msg = packMessage('test-client', 'subscribe', 'test.*');
  ws.send(msg);
});

ws.on('message', function incoming(data) {
  if (data instanceof Buffer) {
    const obj = msgpack.decode(data);
    console.log('obj:', obj);
  } else {
    console.log('text:', data);
  }
});
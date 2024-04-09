const WebSocket = require('ws');
const msgpack = require('@msgpack/msgpack');

const ws = new WebSocket('ws://127.0.0.1:24012/');

const packMessage = function (source, topic, data) {
  const obj = {source, topic, data};
  return msgpack.encode(obj);
};

ws.on('open', function () {
  const msg = packMessage('test-client', 'subscribe', '*');
  ws.send(msg);
});

ws.on('message', function (data) {
  // console.log(data);
  if (data instanceof Buffer) {
    console.log(`Received a message of ${data.byteLength} bytes`);
    console.log(data.toString());
  } else {
    console.log('text:', data);
  }
});

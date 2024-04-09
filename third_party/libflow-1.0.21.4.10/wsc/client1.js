const WebSocket = require('ws');
const msgpack = require('@msgpack/msgpack');

const ws = new WebSocket('ws://127.0.0.1:24012/');

const packMessage = function (source, topic, data) {
  const obj = {source, topic, data};
  return msgpack.encode(obj);
};

ws.on('open', function () {
  const msg = packMessage('test-client', 'subscribe',
    'test.foo, *.bar');
  ws.send(msg);
});

ws.on('message', function (data) {
  // console.log(data);
  if (data instanceof Buffer) {
    const obj = msgpack.decode(data);
    console.log('obj:', obj);

    const msg = packMessage('test-client', 'post', 'this is a post');
    ws.send(msg);
  } else {
    console.log('text:', data);
  }
});

const WebSocket = require('ws');
const msgpack = require('msgpack');

const ws = new WebSocket('ws://localhost:24012/');

const packMessage = function (source, topic, data) {
  const obj = {source, topic, data};
  return msgpack.pack(obj);
};

const subscribe = function (ws, topic) {
  const name = 'client3';
  const msg = packMessage(name, 'subscribe', topic);
  ws.send(msg);
};

ws.on('open', function open() {
  subscribe(ws, 'output.can.0x700');
  subscribe(ws, 'output.can.0x760');
});

ws.on('message', function incoming(data) {
  if (data instanceof Buffer) {
    const obj = msgpack.unpack(data);
    // console.log('obj:', obj);
    const codes = [];
    for (let i = 0; i < obj.data.length; i += 1) {
      const code = obj.data.charCodeAt(i);
      codes.push(code);
    }

    console.log(obj.topic, codes);
  } else {
    console.log('text:', data);
  }
});

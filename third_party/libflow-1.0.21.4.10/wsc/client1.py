#!/usr/bin/env python3

"""
1. Run test1
2. Run this program

Dependencies:
  sudo pip3 install websockets
  sudo pip3 install msgpack-python
"""

import asyncio
import msgpack
import websockets

async def hello(uri):
    async with websockets.connect(uri) as websocket:
        msg = {
            'source': 'test-client',
            'topic': 'subscribe',
            'data': '*.bar',
        }
        data = msgpack.packb(msg)
        await websocket.send(data)

        while True:
            # receive from server
            try:
                data = await websocket.recv()
                msg = msgpack.unpackb(data, use_list=False)
                print(msg)
            except websockets.exceptions.ConnectionClosed as err:
                print('Connection was closed')
                break

            # send a message to server
            data = msgpack.packb({
                'source': 'test-client',
                'topic': 'client_topic',
                'data': 'hello, dude!',
            })

            await websocket.send(data)

asyncio.get_event_loop().run_until_complete(
    hello('ws://localhost:24012'))

package cc.minieye.software;

import java.net.URI;
import java.nio.ByteBuffer;

import org.java_websocket.client.WebSocketClient;
import org.java_websocket.handshake.ServerHandshake;

public class FlowConnection extends WebSocketClient {
	private String name = "JavaClient";
	private String topics = "*";
	private FlowMessageHandler handler;

	public FlowConnection(URI uri) {
		super(uri);
		this.handler = new SimpleMessageHandler();
	}

	public FlowConnection(URI uri, String name, String topics) {
		super(uri);
		this.name = name;
		this.topics = topics;
		this.handler = new SimpleMessageHandler();
	}

	@Override
	public void onOpen(ServerHandshake handshakedata) {
		byte[] data = this.topics.getBytes();
		FlowMessage message = new FlowMessage(this.name, "subscribe", data);
		send(message.toMsgpack());
	}

	/**
	 * Callback for WebSocket text message.
	 */
	@Override
	public void onMessage(String message) {
		System.err.println("unexpected text message: " + message);
	}

	/**
	 * Callback for WebSocket binary message.
	 */
	@Override
	public void onMessage(ByteBuffer message) {
		FlowMessage msg = FlowMessage.fromMsgpack(message.array());
		this.handler.recv(msg);
	}

	@Override
	public void onClose(int code, String reason, boolean remote) {
		// The codes are documented in class org.java_websocket.framing.CloseFrame
		String who = remote ? "remote peer" : "us";
		System.err.printf("Connection closed by %s, code: %d, reason: %s\n", who, code, reason);
	}

	@Override
	public void onError(Exception ex) {
		ex.printStackTrace();
	}

	/**
	 * Override this method to handle the message.
	 * 
	 * @param msg: flow message.
	 */
//	public void recv(FlowMessage msg) {
//		System.out.println(msg);
//		System.out.printf("Data: %s\n", new String(msg.getData()));
//	}

	public FlowMessageHandler getHandler() {
		return handler;
	}

	public void setHandler(FlowMessageHandler handler) {
		this.handler = handler;
	}
}

package cc.minieye.software;

import org.java_websocket.exceptions.WebsocketNotConnectedException;

import java.net.URI;
import java.net.URISyntaxException;
import java.util.concurrent.atomic.AtomicBoolean;

public class FlowClient extends Thread {
	/**
	 * Server URL.
	 */
	private String url = "ws://127.0.0.1:24012/";

	/**
	 * Client name.
	 */
	private String name = "JavaClient";

	/**
	 * The topics to subscribe.
	 */
	private String topics = "*";

	/**
	 * Reconnection interval in milliseconds.
	 */
	private int reconnectTime = 5000;

	/**
	 * Flow message handler. Set this property to use your own handler.
	 */
	private FlowMessageHandler handler;

	private URI serverURI;
	private FlowConnection conn;
	private AtomicBoolean running = new AtomicBoolean(false);

	public FlowClient() {
		try {
			this.serverURI = new URI(this.url);
		} catch (URISyntaxException e) {
			e.printStackTrace();
		}
	}

	public FlowClient(String url, String name, String topics) throws URISyntaxException {
		this.url = url;
		this.name = name;
		this.topics = topics;
		this.serverURI = new URI(url);
	}

	public String getUrl() {
		return url;
	}

	public void setUrl(String url) {
		this.url = url;
	}

	public String getClientName() {
		return name;
	}

	public void setClientName(String name) {
		this.name = name;
	}

	public String getTopics() {
		return topics;
	}

	public void setTopics(String topics) {
		this.topics = topics;
	}

	public int getReconnectTime() {
		return reconnectTime;
	}

	public void setReconnectTime(int reconnectTime) {
		this.reconnectTime = reconnectTime;
	}

	public FlowMessageHandler getHandler() {
		return handler;
	}

	public void setHandler(FlowMessageHandler handler) {
		this.handler = handler;
	}

	/**
	 * Send binary data serialized as msgpack
	 */
	public void send(byte [] data) throws WebsocketNotConnectedException {
		conn.send(data);
	}

	/**
	 * Close the WebSocket connection, and don't reconnect.
	 */
	public void close() {
		if (this.running.compareAndSet(true, false)) {
			this.conn.close();
		}
	}

	@Override
	public void run() {
		this.conn = new FlowConnection(this.serverURI, this.name, this.topics);
		this.conn.setHandler(this.handler);
		this.conn.setConnectionLostTimeout(0);
		
		this.conn.connect();
		this.running.set(true);

		while (true) {
			try {
				Thread.sleep(this.reconnectTime);
			} catch (InterruptedException e) {
				e.printStackTrace();
			}

			if (this.running.get() == false) {
				break;
			}

			if (this.conn.isClosed()) {
				System.out.println("reconnecting...");
				this.conn.reconnect();
			}
		}
	}
}


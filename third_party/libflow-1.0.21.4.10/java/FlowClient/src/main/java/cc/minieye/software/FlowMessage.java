package cc.minieye.software;

import java.io.IOException;

import org.msgpack.core.MessageBufferPacker;
import org.msgpack.core.MessagePack;
import org.msgpack.core.MessageUnpacker;
import org.msgpack.value.Value;

public class FlowMessage {
	private String source;
	private String topic;
	private byte[] data;
	private long time;

	public FlowMessage(String source, String topic, byte[] data) {
		this.source = source;
		this.topic = topic;
		this.data = data;
		this.time = System.currentTimeMillis();
	}

	public FlowMessage(String source, String topic, byte[] data, long time) {
		this.source = source;
		this.topic = topic;
		this.data = data;
		this.time = time;
	}

	public String getSource() {
		return source;
	}

	public void setSource(String source) {
		this.source = source;
	}

	public String getTopic() {
		return topic;
	}

	public void setTopic(String topic) {
		this.topic = topic;
	}

	public byte[] getData() {
		return data;
	}

	public void setData(byte[] data) {
		this.data = data;
	}

	public long getTime() {
		return time;
	}

	public void setTime(long time) {
		this.time = time;
	}

	/**
	 * Encode the message into MessagePack data.
	 * @return
	 */
	public byte[] toMsgpack() {
		try {
			MessageBufferPacker packer = MessagePack.newDefaultBufferPacker();
			packer.packMapHeader(4);

			packer.packString("source");
			packer.packString(this.source);

			packer.packString("topic");
			packer.packString(this.topic);

			packer.packString("data");
			packer.packBinaryHeader(this.data.length);
			packer.addPayload(data);

			packer.packString("time");
			packer.packLong(time);

			packer.close(); // Never forget to close (or flush) the buffer
			return packer.toByteArray();
		} catch (IOException e) {
			System.err.println(e);
			return null;
		}
	}

	/**
	 * Decode the message from MessagePack data.
	 * @param data
	 * @return
	 */
	public static FlowMessage fromMsgpack(byte[] data) {
		FlowMessage res = new FlowMessage("", "", new byte[0], 0);  // trivial result

		try {
			MessageUnpacker unpacker = MessagePack.newDefaultUnpacker(data);
			int numFields = unpacker.unpackMapHeader();

			for (int i = 0; i < numFields; i += 1) {
				String key = unpacker.unpackString();

				if (key.equals("source")) {
					String source = unpacker.unpackString();
					res.setSource(source);
				} else if (key.equals("topic")) {
					String topic = unpacker.unpackString();
					res.setTopic(topic);
				} else if (key.equals("data")) {
					Value v = unpacker.unpackValue();
					switch (v.getValueType()) {
					case BINARY:
						res.data = v.asBinaryValue().asByteArray();
						break;
					case STRING:
						res.data = v.asStringValue().asByteArray();
						break;
					default:
						break;
					}
				} else if (key.equals("time")) {
					long time = unpacker.unpackLong();
					res.setTime(time);
				} else {
					// ignore this field
					unpacker.unpackValue();
				}
			}

			unpacker.close();
		} catch (IOException e) {
			System.err.println(e);
		}

		return res;
	}

	public String toString() {
		return String.format("FlowMessage(%s, %s, byte[%d], %d)", this.source, this.topic, this.data.length, this.time);
	}
}

package cc.minieye.software;

public interface FlowMessageHandler {
	/**
	 * Override this method to handle the message.
	 * 
	 * @param msg: flow message.
	 */
	public void recv(FlowMessage msg);
}

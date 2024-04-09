package cc.minieye.software;

public class SimpleMessageHandler implements FlowMessageHandler {

	public void recv(FlowMessage msg) {
		System.out.println(msg);
		System.out.printf("Data: %s\n", new String(msg.getData()));
	}

}

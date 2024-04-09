package cc.minieye.software;

public class App {
	public static void main(String[] args) {		
		FlowClient c = new FlowClient();
		c.setHandler(new SimpleMessageHandler());
		c.start();

		System.out.println("Thread " + Thread.currentThread().getName() + " is over.");
	}
}

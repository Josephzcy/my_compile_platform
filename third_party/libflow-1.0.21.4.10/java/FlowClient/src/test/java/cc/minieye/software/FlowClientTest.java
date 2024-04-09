package cc.minieye.software;

import org.java_websocket.exceptions.WebsocketNotConnectedException;
import java.net.URISyntaxException;

import cc.minieye.software.FlowClient;
import cc.minieye.software.FlowMessage;
import cc.minieye.software.FlowMessageHandler;

class ByteArrayDumper {
    private static String dumpByte(byte b) {
        return String.format("%02x", b);
    }

    public static String dumps(byte [] buf) {
        int i = 0;
        StringBuffer sb = new StringBuffer();
        for (i = 0; i < buf.length; ++i) {
            sb.append(dumpByte(buf[i]));
            sb.append(" ");
        }
        return sb.toString();
    }
}

public class FlowClientTest {

    private final String TAG = "flow_client_test";
    private final String SOURCE = "JavaClient";

    private String mIpAddr;
    private String mPort;

    private FlowClient mFlowClient;
    private FlowMessageHandler mFlowMessageHandler = new FlowMessageHandler() {
        @Override
        public void recv(FlowMessage msg) {
            System.out.println("topic : " + msg.getTopic());
            System.out.println("source : " + msg.getSource());
            System.out.println("data : " + ByteArrayDumper.dumps(msg.getData()));
        }
    };

    private FlowClient createFlowClient(String wsUri) {
        FlowClient flowClient;
        try {
            flowClient = new FlowClient(wsUri, SOURCE, "*");
        } catch (URISyntaxException e) {
            return null;
        }
        flowClient.setHandler(mFlowMessageHandler);
        flowClient.start();
        return flowClient;
    }

    /**
     * send byte buffer to libflow server
     *
     * @param buffer byte array of data
     * @throws WebsocketNotConnectedException when sending is failed
     */
    public void sendBuffer(byte [] buffer) throws WebsocketNotConnectedException {
        FlowMessage msg = new FlowMessage(SOURCE, "test_topic", buffer);
        mFlowClient.send(msg.toMsgpack());
    }

    public FlowClientTest(String wsUri) {
        mFlowClient = createFlowClient(wsUri);
    }

    public void run() {
        while (true) {
            try {
                String str = "{\"info\" : \"just_for_test\"}";
                sendBuffer(str.getBytes());
                Thread.sleep(100);
            } catch (WebsocketNotConnectedException e) {
                System.err.println("send failed");
            } catch (InterruptedException e) {
                System.err.println("interrupted");
            }
        }
    }

    public static void main(String [] args) {
        FlowClientTest flowClientTest = new FlowClientTest("ws://127.0.0.1:24011/");
        try {
            Thread.sleep(5000);
        } catch (InterruptedException e) {
        }
        flowClientTest.run();
    }
}

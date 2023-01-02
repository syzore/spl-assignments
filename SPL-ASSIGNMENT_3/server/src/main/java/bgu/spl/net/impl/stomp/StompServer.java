package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.echo.FrameMessageEncoderDecoder;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        Server.threadPerClient(
                7777, // port
                () -> new StompProtocol<String>(), // protocol factory
                FrameMessageEncoderDecoder::new // message encoder decoder factory
        ).serve();
    }
}

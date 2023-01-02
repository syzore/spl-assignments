package bgu.spl.net.impl.stomp;

import bgu.spl.net.impl.echo.EchoProtocol;
import bgu.spl.net.impl.echo.LineMessageEncoderDecoder;
import bgu.spl.net.srv.Server;

public class StompServer {

    public static void main(String[] args) {
        Server.threadPerClient(
                7777, // port
                () -> new EchoProtocol(), // protocol factory
                LineMessageEncoderDecoder::new // message encoder decoder factory
        ).serve();
    }
}

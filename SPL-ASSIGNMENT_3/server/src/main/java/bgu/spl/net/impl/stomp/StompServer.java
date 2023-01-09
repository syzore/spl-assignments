package bgu.spl.net.impl.stomp;

import java.io.IOException;
import java.net.ServerSocket;
import java.net.Socket;
import java.util.function.Supplier;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.BlockingConnectionHandler;
import bgu.spl.net.srv.Connection;
import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;
import bgu.spl.net.srv.Server;

public class StompServer<T> implements Server<T> {

    private final int port;
    private final Supplier<StompMessagingProtocol<T>> protocolFactory;
    private final Supplier<MessageEncoderDecoder<T>> encdecFactory;
    private ServerSocket sock;

    private int currentConnectionId = 0;

    public StompServer(
            int port,
            Supplier<StompMessagingProtocol<T>> protocolFactory,
            Supplier<MessageEncoderDecoder<T>> encdecFactory) {

        this.port = port;
        this.protocolFactory = protocolFactory;
        this.encdecFactory = encdecFactory;
        this.sock = null;
    }

    @Override
    public void serve() {
        Connections<T> connections = new ConnectionsImpl<>();

        try (ServerSocket serverSock = new ServerSocket(port)) {
            System.out.println("Server started");

            this.sock = serverSock; // just to be able to close

            while (!Thread.currentThread().isInterrupted()) {
                Socket clientSock = serverSock.accept();

                Connection<T> connection = new Connection();
                connection.setConnectionId(++currentConnectionId);
                connections.addConnection(connection);

                BlockingConnectionHandler<T> handler = new BlockingConnectionHandler<>(
                        clientSock,
                        encdecFactory.get(),
                        protocolFactory.get(),
                        connections,
                        currentConnectionId);

                connection.setHandler(handler);

                execute(handler);
            }

        } catch (IOException ex) {
        }

        System.out.println("server closed!!!");
    }

    @Override
    public void close() throws IOException {
        System.out.println("socket closing");
        if (sock != null)
            sock.close();
    }

    protected void execute(BlockingConnectionHandler<T> handler) {
        new Thread(handler).start();
    }

    public static <T> Server<T> threadPerClient(
            int port,
            Supplier<StompMessagingProtocol<T>> protocolFactory,
            Supplier<MessageEncoderDecoder<T>> encoderDecoderFactory) {

        return new StompServer<T>(port, protocolFactory, encoderDecoderFactory) {
            @Override
            protected void execute(BlockingConnectionHandler<T> handler) {
                new Thread(handler).start();
            }
        };
    }

    public static void main(String[] args) {
        threadPerClient(
                7777, // port
                () -> new StompProtocol<String>(), // protocol factory
                FrameMessageEncoderDecoder::new // message encoder decoder factory
        ).serve();
    }
}

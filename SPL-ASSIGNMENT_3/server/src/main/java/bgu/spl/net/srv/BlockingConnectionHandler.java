package bgu.spl.net.srv;

import java.io.BufferedInputStream;
import java.io.BufferedOutputStream;
import java.io.IOException;
import java.net.Socket;

import bgu.spl.net.api.MessageEncoderDecoder;
import bgu.spl.net.api.StompMessagingProtocol;

public class BlockingConnectionHandler<T> implements Runnable, ConnectionHandler<T> {

    private final StompMessagingProtocol<T> protocol;
    private final MessageEncoderDecoder<T> encdec;
    private final Socket sock;
    private BufferedInputStream in;
    private BufferedOutputStream out;
    private volatile boolean connected = true;
    private Connections<T> connections;
    private int connectionId;

    public BlockingConnectionHandler(Socket sock, MessageEncoderDecoder<T> reader, StompMessagingProtocol<T> protocol,
            Connections<T> connections, int connectionId) {
        this.sock = sock;
        this.encdec = reader;
        this.protocol = protocol;
        this.connections = connections;
        this.connectionId = connectionId;
    }

    @Override
    public void run() {
        System.out.println("entered handler run");
        try (Socket sock = this.sock) { // just for automatic closing
            int read = -1;

            synchronized (this) {
                in = new BufferedInputStream(sock.getInputStream());
                out = new BufferedOutputStream(sock.getOutputStream());

                protocol.start(connectionId, connections);

                while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
                    T nextMessage = encdec.decodeNextByte((byte) read);
                    if (nextMessage != null) {
                        T response = protocol.process(nextMessage);
                        System.out.println("response = \n" + response);
                        if (response != null) {
                            out.write(encdec.encode(response));
                            out.flush();
                        }
                        System.out.println("response sent to client.");
                    }
                }
            }

            System.out.println("should terminate = " + protocol.shouldTerminate());
            System.out.println("connected = " + connected);
            System.out.println("read = " + read);

        } catch (IOException ex) {
            System.out.println("error when assigning socket " + ex);
            ex.printStackTrace();
        }

    }

    @Override
    public void close() throws IOException {
        System.out.println("Socket close called from BlockingConnectionHandler");
        connected = false;
        sock.close();
    }

    @Override
    public void send(T msg) {
        try (Socket sock = this.sock) {
            out = new BufferedOutputStream(sock.getOutputStream());

            if (!protocol.shouldTerminate() && connected && msg != null) {
                System.out.println("sending message = \n" + msg);
                out.write(encdec.encode(msg));
                out.flush();
            }
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
}

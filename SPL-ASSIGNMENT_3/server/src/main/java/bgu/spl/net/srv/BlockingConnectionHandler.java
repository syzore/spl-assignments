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

    public BlockingConnectionHandler(Socket sock, MessageEncoderDecoder<T> reader, StompMessagingProtocol<T> protocol) {
        this.sock = sock;
        this.encdec = reader;
        this.protocol = protocol;
    }

    @Override
    public void run() {
        System.out.println("entered handler run");
        try (Socket sock = this.sock) { // just for automatic closing
            int read;

            synchronized (this) {
                in = new BufferedInputStream(sock.getInputStream());
                out = new BufferedOutputStream(sock.getOutputStream());

                protocol.start(0, null);

                while (!protocol.shouldTerminate() && connected && (read = in.read()) >= 0) {
                    T nextMessage = encdec.decodeNextByte((byte) read);
                    if (nextMessage != null) {
                        T response = protocol.process(nextMessage);
                        System.out.println("response = " + response);
                        if (response != null) {
                            out.write(encdec.encode(response));
                            out.flush();
                        }
                    }
                }
            }

        } catch (IOException ex) {
            System.out.println("error when assigning socket " + ex);
            ex.printStackTrace();
        }

    }

    @Override
    public void close() throws IOException {
        connected = false;
        sock.close();
    }

    @Override
    public void send(T msg) {
        try (Socket sock = this.sock) {
            synchronized (this) {
                out = new BufferedOutputStream(sock.getOutputStream());

                while (!protocol.shouldTerminate() && connected) {
                    if (msg != null) {
                        T response = protocol.process(msg);
                        System.out.println("response = " + response);
                        if (response != null) {
                            out.write(encdec.encode(response));
                            out.flush();
                        }
                    }
                }
            }
        } catch (IOException e) {
            // TODO Auto-generated catch block
            e.printStackTrace();
        }
    }
}

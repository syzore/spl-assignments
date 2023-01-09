package bgu.spl.net.srv;

import bgu.spl.net.impl.stomp.User;

public interface Connections<T> {
    boolean subscribe(String username, int id, String destination);

    boolean send(int connectionId, T msg);

    void send(String channel, T msg);

    void disconnect(int connectionId);

    void connect(User user, int connectionId);

    Connection<T> getConnectionById(int connectionId);

    void addConnection(Connection<T> connection);

    void register(User user);

    boolean isRegistered(User user);

    boolean isConnected(int connectionId, User user);

    boolean checkPasscode(User user);
}

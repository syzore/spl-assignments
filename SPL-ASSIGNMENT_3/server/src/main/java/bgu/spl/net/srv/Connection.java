package bgu.spl.net.srv;

import bgu.spl.net.impl.stomp.User;

public class Connection<T> {
    private User user;
    private int connectionId;
    private ConnectionHandler<T> handler;
    private boolean isConnected = false;

    public Connection(User user, int connectionId) {
        this.user = user;
        this.connectionId = connectionId;
    }

    public Connection() {
        this.user = null;
        this.connectionId = -1;
    }

    public int getConnectionId() {
        return connectionId;
    }

    public User getUser() {
        return user;
    }

    public void setConnectionId(int connectionId) {
        this.connectionId = connectionId;
    }

    public void setUser(User user) {
        this.user = user;
    }

    public void setHandler(ConnectionHandler<T> handler) {
        this.handler = handler;
    }

    public ConnectionHandler<T> getHandler() {
        return handler;
    }

    public boolean connect() {
        if (isConnected)
            return false;
        else
            isConnected = true;
        return true;
    }
}

package bgu.spl.net.srv;

import java.util.Map;

import bgu.spl.net.impl.stomp.User;

public class ConnectionsImpl<T> implements Connections<T> {

    private Map<String, String> subscriptionMap;
    private Map<String, Connection<T>> connectionsIdMap;

    // returns true if subscription succeeded, otherwise returns false
    public boolean subscribe(String username, int id, String destination) {
        String key = username + "_" + id;
        String abc = subscriptionMap.get(key);
        if (abc == null) {
            subscriptionMap.put(key, destination);
            return true;
        } else {
            return false;
        }
    }

    @Override
    public boolean send(int connectionId, T msg) {
        Connection<T> connection = connectionsIdMap.get(connectionId);
        if (connection == null) connection.getHandler().send(msg);
        return false;
    }

    @Override
    public void send(String channel, T msg) {
        // TODO Auto-generated method stub
    }

    @Override
    public void disconnect(int connectionId) {
        // TODO Auto-generated method stub
    }

    @Override
    public void connect(User user, int connectionId) {
        // check if connectionId exists in map
        // if exists check if its the same user
        // if same user return error already connected
        // if not same user return error another user is already connected
        // if doesnt exists add and return true
    }
}

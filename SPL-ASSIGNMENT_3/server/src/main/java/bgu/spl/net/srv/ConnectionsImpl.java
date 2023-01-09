package bgu.spl.net.srv;

import java.util.Map;
import java.util.Vector;
import java.util.WeakHashMap;

import bgu.spl.net.impl.stomp.User;

public class ConnectionsImpl<T> implements Connections<T> {

    private Vector<String> topics;
    private Map<String, String> subscriptionMap;
    private Map<Integer, Connection<T>> connectionsIdMap;
    private Map<String, String> usersDatabase;

    public ConnectionsImpl() {
        this.topics = new Vector<>();
        this.subscriptionMap = new WeakHashMap<String, String>();
        this.connectionsIdMap = new WeakHashMap<Integer, Connection<T>>();
        this.usersDatabase = new WeakHashMap<String, String>();
    }

    // returns true if subscription succeeded, otherwise returns false
    public boolean subscribe(String username, int id, String destination) {
        String key = username + "_" + id;
        String abc = subscriptionMap.get(key);
        if (!topics.contains(destination))
            topics.add(destination);

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
        if (connection == null)
            connection.getHandler().send(msg);
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

    @Override
    public Connection<T> getConnectionById(int connectionId) {
        Connection<T> connection = connectionsIdMap.get(connectionsIdMap);
        return connection;
    }

    @Override
    public void addConnection(Connection<T> connection) {
        connectionsIdMap.put(connection.getConnectionId(), connection);
        
    }


    @Override
    public void register(User user) {
        // TODO Auto-generated method stub
    }

    @Override
    public boolean isRegistered(User user) {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean isConnected(User user) {
        // TODO Auto-generated method stub
        return false;
    }

    @Override
    public boolean checkPasscode(User user) {
        return user.getPasscode().equals(usersDatabase.get(user.getLogin()));
    }
}

package bgu.spl.net.srv;

import java.util.Map;
import java.util.Map.Entry;
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

    public void unsubscribe(String username, int id, String destination) {
        String key = username + "_" + id;
        String abc = subscriptionMap.get(key);
        if (abc == null)
            System.out.println("user does not subscribe this game");
        else{
            //topics.remove(destination);
            subscriptionMap.remove(key, destination);
            if (!subscriptionMap.containsValue(destination))
                topics.remove(destination);
        }
    }

    @Override
    public boolean send(int connectionId, T msg) {
        Connection<T> connection = getConnectionById(connectionId);
        if (connection != null)
            connection.getHandler().send(msg);
        return false;
    }

    @Override
    public void send(String channel, T msg) {
        // get vector of all connectionIds that are subscribed to this channel..
        // for each connectionId send(connectionId, msg)
    }

    @Override
    public void disconnect(int connectionId) {
        if (connectionsIdMap.containsKey(connectionId))
            connectionsIdMap.remove(connectionId);
    }

    @Override
    public ConnectionResult connect(User user, int connectionId) {
        Connection<T> connection = getConnectionById(connectionId);
        if (connection.getUser() != null) {
            if (connection.getUser() == user) {
                return ConnectionResult.ALREADY_CONNECTED;
            } else {
                return ConnectionResult.ANOTHER_USER_CONNECTED;
            }
        } else {
            connection.setUser(user);
            user.connect();

            System.out.println("testing user connection ---");
            Connection<T> connectionTest = getConnectionById(connectionId);
            System.out.println("is user not null, as expected? " + connectionTest.getUser() != null);
        }

        return ConnectionResult.CONNECTED;
    }

    @Override
    public Connection<T> getConnectionById(int connectionId) {
        return connectionsIdMap.get(new Integer(connectionId));
    }

    @Override
    public void addConnection(Connection<T> connection) {
        System.out.println("adding connection " + connection + " with id " + connection.getConnectionId());
        connectionsIdMap.put(new Integer(connection.getConnectionId()), connection);
    }

    @Override
    public void register(User user) {
        usersDatabase.put(user.getLogin(), user.getPasscode());
    }

    @Override
    public boolean isRegistered(User user) {
        for (Entry<String, String> entry : usersDatabase.entrySet()) {
            if (entry.getKey().equals(user.getLogin()))
                return true;
        }
        return false;
    }

    @Override
    public boolean isConnected(int connectionId, User user) {
        Connection<T> connection = connectionsIdMap.get(connectionId);
        return user.getLogin().equals(connection.getUser().getLogin());
    }

    @Override
    public boolean checkPasscode(User user) {
        return user.getPasscode().equals(usersDatabase.get(user.getLogin()));
    }
}

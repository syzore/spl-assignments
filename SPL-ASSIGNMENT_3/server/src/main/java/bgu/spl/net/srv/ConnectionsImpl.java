package bgu.spl.net.srv;

import java.util.Map;
import java.util.Map.Entry;
import java.util.Vector;
import java.util.WeakHashMap;

import bgu.spl.net.impl.stomp.User;

public class ConnectionsImpl<T> implements Connections<T> {

    private Vector<String> topics;

    // <username_subscriptionId, topic>
    private Map<String, String> subscriptionMap;
    private Map<Integer, Connection<T>> connectionsIdMap;
    private Map<Integer, String> usernameConnectionIdMap;
    private Map<String, String> usersDatabase;

    public ConnectionsImpl() {
        this.topics = new Vector<>();
        this.subscriptionMap = new WeakHashMap<String, String>();
        this.connectionsIdMap = new WeakHashMap<Integer, Connection<T>>();
        this.usernameConnectionIdMap = new WeakHashMap<Integer, String>();
        this.usersDatabase = new WeakHashMap<String, String>();
    }

    // returns true if subscription succeeded, otherwise returns false
    public void subscribe(String username, int id, String destination) {
        String key = username + "_" + id;
        subscriptionMap.putIfAbsent(key, destination);
        if (!topics.contains(destination))
            topics.add(destination);
    }

    public void unsubscribe(String username, int id, String destination) {
        String key = username + "_" + id;
        String abc = subscriptionMap.get(key);
        if (abc == null)
            System.out.println("The user is not subscribed to " + destination);
        else {
            subscriptionMap.remove(key, destination);
            if (!subscriptionMap.containsValue(destination))
                topics.remove(destination);
        }
    }

    @Override
    public int getSubscriptionId(String username, String destination) {
        for (Entry<String, String> set : subscriptionMap.entrySet()) {
            String key = set.getKey();
            String name = key.substring(0, key.indexOf('_'));
            if (set.getValue().equals(destination) && name.equals(username)) {
                return Integer.parseInt(key.substring(key.indexOf("_") + 1, key.length() - 1), 10);
            }
        }
        return -1;
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
        Vector<String> usernameVector = new Vector<String>();
        for (Entry<String, String> set : subscriptionMap.entrySet()) {
            if (set.getValue().equals(channel)) {
                String name = set.getKey().substring(0, set.getKey().indexOf('_'));
                usernameVector.add(name);
            }
        }
        for (Entry<Integer, String> set : usernameConnectionIdMap.entrySet()) {
            if (usernameVector.contains(set.getValue())) {
                send(set.getKey(), msg);
            }
        }
    }

    @Override
    public void disconnect(int connectionId) {
        if (connectionsIdMap.containsKey(connectionId)) {
            connectionsIdMap.remove(connectionId);
            usernameConnectionIdMap.remove(connectionId);
        }
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
        }
        usernameConnectionIdMap.put(connectionId, user.getLogin());
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

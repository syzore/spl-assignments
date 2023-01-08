package bgu.spl.net.srv;

import java.util.HashMap;
import java.util.Map;

public class ConnectionsImpl <T> implements Connections <T> {
    
    private Map<Integer, String> connectionsIdMap;

    public ConnectionsImpl() {
        connectionsIdMap = new HashMap<Integer, String>();
    }
    // returns true if subscription succeeded, otherwise returns false
    public boolean subscribe(int id, String destination){
        String abc = connectionsIdMap.get(id);
        if (abc == null){
            connectionsIdMap.put(id, destination);
            return true;
        } else{
            return false;
        }
    }

    @Override
    public boolean send(int connectionId, T msg) {
        // TODO Auto-generated method stub
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
}

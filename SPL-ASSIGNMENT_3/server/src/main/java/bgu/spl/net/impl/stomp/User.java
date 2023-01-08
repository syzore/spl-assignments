package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Connections;
import bgu.spl.net.srv.ConnectionsImpl;

public class User {

    private String login;
    private String passcode;
    private boolean isConnected;
    private Connections<String> connections;

    public User (String login, String passcode){
        this.login = login;
        this.passcode = passcode;
        isConnected = false;
        connections = new ConnectionsImpl<String>();
    }

    public void connect(){
        isConnected = true;
    }

    public void disconnect(){
        isConnected = false;
    }

    public String getLogin() {
        return login;
    }

    public String getPasscode() {
        return passcode;
    }

    public boolean getIsConnected(){
        return isConnected;
    }

}

package bgu.spl.net.impl.stomp;

import bgu.spl.net.srv.Connections;

public class User {

    private String login;
    private String passcode;
    private boolean isConnected;

    public User(String login, String passcode) {
        this.login = login;
        this.passcode = passcode;
        isConnected = false;
    }

    public void connect() {
        isConnected = true;
    }

    public void disconnect() {
        isConnected = false;
    }

    public String getLogin() {
        return login;
    }

    public String getPasscode() {
        return passcode;
    }

    public boolean isConnected() {
        return isConnected;
    }

}

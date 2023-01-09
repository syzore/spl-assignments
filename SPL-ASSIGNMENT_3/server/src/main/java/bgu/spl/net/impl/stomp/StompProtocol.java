package bgu.spl.net.impl.stomp;

import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;
import java.util.Vector;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connection;
import bgu.spl.net.srv.Connections;

public class StompProtocol<T> implements StompMessagingProtocol<T> {

  private boolean shouldTerminate = false;
  private Vector<String> asdasd;
  private Connections<T> connections;
  private int connectionId;

  @Override
  public void start(int connectionId, Connections<T> connections) {
    this.connections = connections;
    this.connectionId = connectionId;
  }

  @Override
  public T process(T message) {
    System.out.println("protocol process message = " + message);
    String msg = message.toString();
    Scanner scanner = new Scanner(msg);
    String command = scanner.nextLine();
    String nextLine = "";
    Map<String, String> key_Value_Map = new HashMap<String, String>();
    while (scanner.hasNext()) {
      nextLine = scanner.nextLine();
      if (!nextLine.contains(":")) {
        break;
      }
      System.out.println("next line = " + nextLine);
      int index = nextLine.indexOf(':');
      String key = nextLine.substring(0, index);
      String value = nextLine.substring(index + 1, nextLine.length());
      key_Value_Map.put(key, value);
    }
    String body = "";
    while (scanner.hasNextLine()) {
      body = body + scanner.nextLine() + "\n";
    }

    System.out.println("body = " + body);
    scanner.close();
    T result;

    switch (command) {
      case "CONNECT":
        result = handleConnect(key_Value_Map);
        System.out.println(result);
        return result;
      case "SEND":
        result = handleSend(key_Value_Map, body);
        System.out.println(result);
        return result;
      case "SUBSCRIBE":
        result = handleSubscribe(key_Value_Map);
        System.out.println(result);
        return result;
      case "UNSUBSCRIBE":
        result = handleUnsubscribe(key_Value_Map, body);
        System.out.println(result);
        return result;
      case "DISCONNECT":
        result = handleDisconnect(key_Value_Map, body);
        System.out.println(result);
        return result;
      default:
        return (T) handleError("command not found", "");
    }
  }

  private T handleDisconnect(Map<String, String> key_Value_Map, String body) {
    return (T) "handleDisconnect";
  }

  private T handleUnsubscribe(Map<String, String> key_Value_Map, String body) {
    int connectionId = 1231;
    connections.disconnect(connectionId);
    return (T) "handleUnSubscribe";
  }

  private T handleSubscribe(Map<String, String> key_Value_Map) {
    String destination = key_Value_Map.get(StompConstants.DESTINATION_KEY);
    if (destination == null) return (T) handleError("no topic was mentioned when subscribing", destination)
    
    boolean success = connections.subscribe("" ,connectionId, destination);
    if (success) {
      return (T) "handleSubscribe";
    } else {
      return (T) handleError("cant subscribe to " + , null);
    }
  }

  private T handleSend(Map<String, String> key_Value_Map, String body) {
    connections.send(body, null);
    return (T) "SEND";
  }

  private T handleConnect(Map<String, String> key_Value_Map) {
    System.out.println("handle connect");
    String acceptVersion = key_Value_Map.get("accept-version");
    String host = key_Value_Map.get("host");
    String login = key_Value_Map.get("login");
    String passcode = key_Value_Map.get("passcode");
    if (!acceptVersion.equals(StompConstants.ACCEPT_VERSION_VALUE)) {
      return (T) handleError("only version supported is 1.2", "add something");
    }
    if (!host.equals(StompConstants.HOST_VALUE)) {
      return (T) handleError("not the host we support here", "add something");
    }

    User user = new User(login, passcode);

    connections.connect(user, connectionId);
    Connection<T> connection = connections.getConnectionById(connectionId);
    connection.setUser(user);

    String frame = "";

    // check if users exists
    boolean isRegistered = connections.isRegistered(user);
    if (!isRegistered) {
      connections.register(user);
      Map<String, String> someMap = new HashMap<String, String>();
      someMap.put(StompConstants.VERSION_KEY, StompConstants.VERSION_VALUE);
      frame = buildFrame(StompConstants.CONNECTED, someMap, "");
    }
    // if yes, check if password in map
    else if (user.getPasscode().equals(existingUser.getPasscode())) {
      if (user.isConnected()) {
        return (T) handleError("user is already connected", "");
      } else {
        Map<String, String> args = new HashMap<String, String>();
        args.put(StompConstants.VERSION_KEY, StompConstants.VERSION_VALUE);
        frame = buildFrame(StompConstants.CONNECTED, args, "");
        // add to connections
      }
    } else {
      return (T) handleError("incorrect passcode", "add something");
    }

    user.connect();

    return (T) frame;
    // check what to do if passcode is incorrect

  }

  private String handleError(String message, String body) {
    Map<String, String> errorMap = new HashMap<String, String>();
    // errorMap.put(StompConstants.RECEIPT_ID_KEY, ""+receiptId);
    errorMap.put(StompConstants.MESSAGE_KEY, message);
    String frame = buildFrame("ERROR", errorMap, body);
    return frame;
  }

  public String buildFrame(
      String command,
      Map<String, String> arguments,
      String body) {
    String output = "";
    output += command;
    for (String key : arguments.keySet()) {
      output += "\n";
      output = output + key + ":" + arguments.get(key);
    }
    output = output + "\n\n" + body + '\u0000';
    return output;
  }

  @Override
  public boolean shouldTerminate() {
    // TODO Auto-generated method stub
    return false;
  }
}

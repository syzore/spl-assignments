package bgu.spl.net.impl.stomp;

import bgu.spl.net.api.MessagingProtocol;
import bgu.spl.net.srv.Connections;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Scanner;

public class StompProtocol<T> implements MessagingProtocol<T> {

  private boolean shouldTerminate = false;
  private Map<Integer, Connections<T>> connectionsMap;
  private List<User> usersList = new ArrayList<>();

  // @Override
  // public void start(int connectionId, Connections<T> connections) {
  // // TODO Auto-generated method stub

  // }

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
        result = handleUnSubscribe(key_Value_Map, body);
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

  private T handleUnSubscribe(Map<String, String> key_Value_Map, String body) {
    return (T) "handleUnSubscribe";
  }

  private T handleSubscribe(Map<String, String> key_Value_Map) {
    // String output = buildFrame(StompConstants.SUBSCRIBE, key_Value_Map, "");
    return (T) "handleSubscribe";
  }

  private T handleSend(Map<String, String> key_Value_Map, String body) {
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
    String frame = "";
    // check if users exists
    User existingUser = usersList
      .stream()
      .filter(u -> login.equals(u.getLogin()))
      .findFirst()
      .orElse(null);
    if (existingUser == null) {
      usersList.add(user);
      user.connect();
      Map<String, String> someMap = new HashMap<String, String>();
      someMap.put(StompConstants.VERSION_KEY, StompConstants.VERSION_VALUE);
      frame = buildFrame(StompConstants.CONNECTED, someMap, "");
    }
    // if yes, check if password in map
    else if (user.getPasscode().equals(existingUser.getPasscode())) {
      if (user.getIsConnected()) {
        return (T) handleError("user is already connected", "");
      } else {
        user.connect();
        Map<String, String> someMap = new HashMap<String, String>();
        someMap.put(StompConstants.VERSION_KEY, StompConstants.VERSION_VALUE);
        frame = buildFrame(StompConstants.CONNECTED, someMap, "");
        // add to connections
      }
    } else {
      return (T) handleError("incorrect passcode", "add something");
    }
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
    String body
  ) {
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

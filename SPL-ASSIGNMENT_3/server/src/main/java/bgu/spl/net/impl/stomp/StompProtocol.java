package bgu.spl.net.impl.stomp;

import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connection;
import bgu.spl.net.srv.ConnectionResult;
import bgu.spl.net.srv.Connections;

public class StompProtocol<T> implements StompMessagingProtocol<T> {

  private boolean shouldTerminate = false;
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
        result = handleConnect(key_Value_Map, msg);
        System.out.println(result);
        return result;
      case "SEND":
        result = handleSend(key_Value_Map, body, msg);
        System.out.println(result);
        return result;
      case "SUBSCRIBE":
        result = handleSubscribe(key_Value_Map, msg);
        System.out.println(result);
        return result;
      case "UNSUBSCRIBE":
        result = handleUnsubscribe(key_Value_Map, body, msg);
        System.out.println(result);
        return result;
      case "DISCONNECT":
        result = handleDisconnect(key_Value_Map, body, msg);
        System.out.println(result);
        return result;
      default:
        return handleError("command not found.", msg, key_Value_Map.get(StompConstants.RECEIPT_ID_KEY),
            "The frame should contain a command.");
    }
  }

  private T handleDisconnect(Map<String, String> key_Value_Map, String body, String originalMessage) {
    String receiptId = key_Value_Map.get(StompConstants.RECEIPT_ID_KEY);
    connections.disconnect(connectionId);
    Map<String, String> args = new HashMap<String, String>();
    args.put(StompConstants.RECEIPT_ID_KEY, receiptId);
    shouldTerminate = true;
    return buildFrame(StompConstants.RECEIPT, args, StompConstants.EMPTY_BODY);
  }

  private T handleUnsubscribe(Map<String, String> key_Value_Map, String body, String originalMessage) {
    String receiptId = key_Value_Map.get(StompConstants.RECEIPT_ID_KEY);
    String destination = key_Value_Map.get(StompConstants.DESTINATION_KEY);
    User user = connections.getConnectionById(connectionId).getUser();
    connections.subscribe(user.getLogin(), connectionId, destination);
    Map<String, String> args = new HashMap<String, String>();
    args.put(StompConstants.RECEIPT_ID_KEY, receiptId);
    return buildFrame(StompConstants.RECEIPT, args, StompConstants.EMPTY_BODY);
  }

  private T handleSubscribe(Map<String, String> key_Value_Map, String originalMessage) {
    String receiptId = key_Value_Map.get(StompConstants.RECEIPT_ID_KEY);
    String destination = key_Value_Map.get(StompConstants.DESTINATION_KEY);
    if (destination == null)
      return handleError("No topic was mentioned when subscribing.", originalMessage, receiptId,
          "When subscribing, you must include which topic you would like to subscribe to.");
    User user = connections.getConnectionById(connectionId).getUser();
    boolean success = connections.subscribe(user.getLogin(), connectionId, destination);
    if (success) {
      return (T) "handleSubscribe\0";
    } else {
      return handleError("Was not able to subscribe to " + destination, originalMessage, receiptId, "");
    }
  }

  private T handleSend(Map<String, String> key_Value_Map, String body, String originalMessage) {
    String receiptId = key_Value_Map.get(StompConstants.RECEIPT_ID_KEY);
    connections.send(body, null);
    return (T) "SEND";
  }

  private T handleConnect(Map<String, String> key_Value_Map, String originalMessage) {
    System.out.println("handle connect");

    String acceptVersion = key_Value_Map.get("accept-version");
    String host = key_Value_Map.get("host");
    String login = key_Value_Map.get("login");
    String passcode = key_Value_Map.get("passcode");
    String receiptId = key_Value_Map.get(StompConstants.RECEIPT_ID_KEY);

    if (!acceptVersion.equals(StompConstants.ACCEPT_VERSION_VALUE)) {
      return handleError("only version supported is 1.2", originalMessage, receiptId, "");
    }
    if (!host.equals(StompConstants.HOST_VALUE)) {
      return handleError("not the host we support here", originalMessage, receiptId, "...");
    }

    User user = new User(login, passcode);

    // handle registration and shit..
    boolean isRegistered = connections.isRegistered(user);
    if (!isRegistered) {
      connections.register(user);
    } else if (!connections.checkPasscode(user)) {
      return handleError("Incorrect passcode", originalMessage, receiptId, "The entered password was not corrent.");
    }

    // if he got here it means the login / registration was successful..

    // handle connection and shit..
    ConnectionResult result = connections.connect(user, connectionId);
    if (result == ConnectionResult.ALREADY_CONNECTED) {
      return handleError("User is already connected", originalMessage, receiptId,
          "This user is already connected through this client.");
    } else if (result == ConnectionResult.ANOTHER_USER_CONNECTED) {
      return handleError("Another user is already connected", originalMessage, receiptId,
          "Another is already connected through this client.");
    }

    Map<String, String> args = new HashMap<String, String>();
    args.put(StompConstants.VERSION_KEY, StompConstants.VERSION_VALUE);

    return buildFrame(StompConstants.CONNECTED, args, "");
  }

  private T handleError(String message, String originalMessage, String receiptId, String explanation) {
    shouldTerminate = true;
    Map<String, String> errorMap = new HashMap<String, String>();
    errorMap.put(StompConstants.MESSAGE_KEY, message);
    if (receiptId != null && !receiptId.isEmpty()) {
      errorMap.put(StompConstants.RECEIPT_ID_KEY, receiptId);
    }
    String body = "The message: \n----- \n";
    body += originalMessage;
    body += "----- \n";
    if (!explanation.isEmpty()) {
      body += explanation;
    }
    return buildFrame("ERROR", errorMap, body);
  }

  public T buildFrame(
      String command,
      Map<String, String> arguments,
      String body) {
    String frame = command;
    for (String key : arguments.keySet()) {
      frame += "\n";
      frame = frame + key + ":" + arguments.get(key);
    }
    frame = frame + "\n\n" + body + "\n";

    return (T) frame;
  }

  @Override
  public boolean shouldTerminate() {
    return shouldTerminate;
  }
}

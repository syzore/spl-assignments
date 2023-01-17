package bgu.spl.net.impl.stomp;

import java.util.HashMap;
import java.util.Map;
import java.util.Scanner;

import bgu.spl.net.api.StompMessagingProtocol;
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
    if (message.equals(""))
      return null;
    String msg = message.toString();
    Scanner scanner = new Scanner(msg);
    String command = scanner.nextLine();
    String nextLine = "";
    Map<String, String> key_Value_Map = new HashMap<String, String>();
    while (scanner.hasNextLine()) {
      nextLine = scanner.nextLine();
      if (!nextLine.contains(":")) {
        break;
      }
      int index = nextLine.indexOf(':');
      String key = nextLine.substring(0, index);
      String value = nextLine.substring(index + 1, nextLine.length());
      key_Value_Map.put(key, value);
    }
    String body = "";
    while (scanner.hasNextLine()) {
      body = body + scanner.nextLine() + "\n";
    }

    scanner.close();

    switch (command) {
      case "CONNECT":
        return handleConnect(key_Value_Map, msg);
      case "SEND":
        return handleSend(key_Value_Map, body, msg);
      case "SUBSCRIBE":
        return handleSubscribe(key_Value_Map, msg);
      case "UNSUBSCRIBE":
        return handleUnsubscribe(key_Value_Map, body, msg);
      case "DISCONNECT":
        return handleDisconnect(key_Value_Map, body, msg);
      default:
        return handleError("command not found.", msg, key_Value_Map.get(StompConstants.RECEIPT_ID_KEY),
            "The frame should contain a command.");
    }
  }

  private T handleDisconnect(Map<String, String> key_Value_Map, String body, String originalMessage) {
    String receipt = key_Value_Map.get(StompConstants.RECEIPT_KEY);
    connections.disconnect(connectionId);
    Map<String, String> args = new HashMap<String, String>();
    args.put(StompConstants.RECEIPT_ID_KEY, receipt);
    shouldTerminate();
    return buildFrame(StompConstants.RECEIPT, args, StompConstants.EMPTY_BODY);
  }

  private T handleUnsubscribe(Map<String, String> key_Value_Map, String body, String originalMessage) {
    String receipt = key_Value_Map.get(StompConstants.RECEIPT_KEY);
    String destination = key_Value_Map.get(StompConstants.DESTINATION_KEY);
    User user = connections.getConnectionById(connectionId).getUser();
    connections.subscribe(user.getLogin(), connectionId, destination);
    Map<String, String> args = new HashMap<String, String>();
    args.put(StompConstants.RECEIPT_ID_KEY, receipt);
    return buildFrame(StompConstants.RECEIPT, args, StompConstants.EMPTY_BODY);
  }

  private T handleSubscribe(Map<String, String> key_Value_Map, String originalMessage) {
    String receipt = key_Value_Map.get(StompConstants.RECEIPT_KEY);
    String destination = key_Value_Map.get(StompConstants.DESTINATION_KEY);
    if (destination == null)
      return handleError("No topic was mentioned when subscribing.", originalMessage, receipt,
          "When subscribing, you must include which topic you would like to subscribe to.");
    User user = connections.getConnectionById(connectionId).getUser();
    connections.subscribe(user.getLogin(), connectionId, destination);

    Map<String, String> args = new HashMap<String, String>();
    args.put(StompConstants.RECEIPT_ID_KEY, receipt);
    return buildFrame(StompConstants.RECEIPT, args, destination);
  }

  private T handleSend(Map<String, String> key_Value_Map, String body, String originalMessage) {
    String receipt = key_Value_Map.get(StompConstants.RECEIPT_KEY);
    String destination = key_Value_Map.get(StompConstants.DESTINATION_KEY);
    String user = body.substring(body.indexOf(":") + 2, body.indexOf("\n"));
    int subscriptionId = connections.getSubscriptionId(user, destination);
    int messageId = StompServer.getNextMessageId();

    Map<String, String> args = new HashMap<String, String>();
    args.put(StompConstants.SUBSCRIPTION_KEY, "" + subscriptionId);
    args.put(StompConstants.MESSAGE_ID_KEY, "" + messageId);
    args.put(StompConstants.DESTINATION_KEY, destination);

    T frame = buildFrame(StompConstants.MESSAGE, args, body);
    connections.send(destination, frame);
    return null;
  }

  private T handleConnect(Map<String, String> key_Value_Map, String originalMessage) {
    String acceptVersion = key_Value_Map.get("accept-version");
    String host = key_Value_Map.get("host");
    String login = key_Value_Map.get("login");
    String passcode = key_Value_Map.get("passcode");
    String receipt = key_Value_Map.get(StompConstants.RECEIPT_KEY);

    if (!acceptVersion.equals(StompConstants.ACCEPT_VERSION_VALUE)) {
      return handleError("Only version supported is 1.2", originalMessage, receipt,
          "The only supported version is 1.2, change your version and try again.");
    }
    if (!host.equals(StompConstants.HOST_VALUE)) {
      return handleError("Not the host we support here", originalMessage, receipt,
          "Change to a supported host and try again.");
    }

    User user = new User(login, passcode);

    // handle registration and shit..
    boolean isRegistered = connections.isRegistered(user);
    if (!isRegistered) {
      connections.register(user);
    } else if (!connections.checkPasscode(user)) {
      return handleError("Incorrect passcode", originalMessage, receipt, "The entered password was not corrent.");
    }

    // if he got here it means the login / registration was successful..

    // handle connection and shit..
    ConnectionResult result = connections.connect(user, connectionId);
    if (result == ConnectionResult.ALREADY_CONNECTED) {
      return handleError("User is already connected", originalMessage, receipt,
          "This user is already connected through this client.");
    } else if (result == ConnectionResult.ANOTHER_USER_CONNECTED) {
      return handleError("Another user is already connected", originalMessage, receipt,
          "Another is already connected through this client.");
    }

    Map<String, String> args = new HashMap<String, String>();
    args.put(StompConstants.VERSION_KEY, StompConstants.VERSION_VALUE);

    return buildFrame(StompConstants.CONNECTED, args, "");
  }

  private T handleError(String message, String originalMessage, String receiptId, String explanation) {
    shouldTerminate();
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

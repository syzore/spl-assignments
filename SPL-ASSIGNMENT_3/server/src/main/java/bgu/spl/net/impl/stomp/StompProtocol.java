package bgu.spl.net.impl.stomp;

import java.util.HashMap;
import java.util.List;
import java.util.Map;
import java.util.Scanner;

import bgu.spl.net.api.StompMessagingProtocol;
import bgu.spl.net.srv.Connections;

public class StompProtocol<T> implements StompMessagingProtocol<T> {

    private boolean shouldTerminate = false;
    private Map<Integer, Connections<T>> connectionsMap;
    private List<User> usersList;

    @Override
    public void start(int connectionId, Connections<T> connections) {
        // TODO Auto-generated method stub

    }

    @Override
    public void process(T message) {
        // TODO Auto-generated method stub
        String msg = message.toString();
        Scanner scanner = new Scanner(msg);
        String command = scanner.nextLine();
        String nextLine = "";
        Map<String, String> key_Value_Map = new HashMap<String, String>();
        while (true) {
            if (!nextLine.contains(":")) {
                break;
            }
            nextLine = scanner.nextLine();
            int index = nextLine.indexOf(':');
            String key = nextLine.substring(0, index);
            String value = nextLine.substring(index + 1, nextLine.length());
            key_Value_Map.put(key, value);

        }
        String body = "";
        while (scanner.hasNextLine()) {
            body = body + scanner.nextLine();
        }
        scanner.close();
        String result;

        switch (command) {
            case "CONNECT":
                result = handleConnect(key_Value_Map);
                System.out.println(result);
                break;
            case "SEND":
                result = handleSend(key_Value_Map, body);
                System.out.println(result);
                break;
            case "SUBSCRIBE":
                result = handleSubscribe(key_Value_Map);
                System.out.println(result);
                break;
            case "UNSUBSCRIBE":
                result = handleUnSubscribe(key_Value_Map, body);
                System.out.println(result);
                break;
            case "DISCONNECT":
                result = handleDisconnect(key_Value_Map, body);
                System.out.println(result);
                break;
            default:
                break;
        }

    }

    private String handleDisconnect(Map<String, String> key_Value_Map, String body) {
        return "handleDisconnect";
    }

    private String handleUnSubscribe(Map<String, String> key_Value_Map, String body) {
        return "handleUnSubscribe";
    }

    private String handleSubscribe(Map<String, String> key_Value_Map) {
        // String output = buildFrame(StompConstants.SUBSCRIBE, key_Value_Map, "");
        return "handleSubscribe";

    }

    private String handleSend(Map<String, String> key_Value_Map, String body) {
        return "SEND";
    }

    private String handleConnect(Map<String, String> key_Value_Map) {
        String acceptVersion = key_Value_Map.get("accept-version");
        String host = key_Value_Map.get("host");
        String login = key_Value_Map.get("login");
        String passcode = key_Value_Map.get("passcode");
        if (!acceptVersion.equals(StompConstants.ACCEPT_VERSION_VALUE)) {
            return handleError("only version supported is 1.2", "add something");
        }
        if (!host.equals(StompConstants.HOST_VALUE)) {
            return handleError("not the host we support here", "add something");
        }
        User user = new User(login, passcode);
        String frame = "";
        // check if users exists
        User existingUser = usersList.stream().filter(u -> login.equals(u.getLogin())).findFirst().orElse(null);
        if (existingUser == null) {
            usersList.add(user);
            user.connect();
            Map<String, String> someMap = new HashMap<String, String>();
            someMap.put(StompConstants.VERSION_KEY, StompConstants.VERSION_VALUE);
            frame = buildFrame(StompConstants.CONNECTED, someMap, "add something");
        }
        // if yes, check if password in map
        else if (user.getPasscode().equals(existingUser.getPasscode())) {
            if (user.getIsConnected()) {
                return handleError("incorrect passcode", "add something");
            } else {
                user.connect();
                Map<String, String> someMap = new HashMap<String, String>();
                someMap.put(StompConstants.VERSION_KEY, StompConstants.VERSION_VALUE);
                frame = buildFrame(StompConstants.CONNECTED, someMap, "add something");
                // add to connections
            }
        }
        return frame;

        // check what to do if passcode is incorrect

    }

    private String handleError(String message, String body) {
        Map<String, String> errorMap = new HashMap<String, String>();
        // errorMap.put(StompConstants.RECEIPT_ID_KEY, ""+receiptId);
        errorMap.put(StompConstants.MESSAGE_KEY, message);
        String frame = buildFrame("ERROR", errorMap, body);
        return frame;
    }

    public String buildFrame(String command, Map<String, String> arguments, String body) {
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

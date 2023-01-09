package bgu.spl.net.impl.stomp;

import java.util.ArrayList;
import java.util.List;
import java.util.Map;

import bgu.spl.net.srv.Connections;

public class DataBase {
    private static DataBase instance;

    private DataBase() {
    }

    public static DataBase getDataBase() {
        if (instance == null)
            instance = new DataBase();

        return instance;
    }

    private Map<Integer, Connections<String>> connectionsMap;
    private List<User> usersList = new ArrayList<>();

    
}

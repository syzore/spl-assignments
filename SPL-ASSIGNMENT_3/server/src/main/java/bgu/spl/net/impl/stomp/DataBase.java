package bgu.spl.net.impl.stomp;

public class DataBase {
    private static DataBase instance;

    private DataBase() {
    }

    public static DataBase getDataBase() {
        if (instance == null) instance = new DataBase();

        return instance;
    }
    
}

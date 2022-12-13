package bguspl.set.ex;

public class Pair {
    private int playerId;
    private int[] set;

    public Pair(int playerId, int[] set){
        this.playerId = playerId;
        this.set = set;
    }

    public int getId(){
        return playerId;
    }

    public int[] getSet(){
        return set;
    }
}

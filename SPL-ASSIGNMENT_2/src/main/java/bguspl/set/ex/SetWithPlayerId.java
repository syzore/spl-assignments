package bguspl.set.ex;

public class SetWithPlayerId {
    private int playerId;
    private int[] set;

    public SetWithPlayerId(int playerId, int[] set) {
        this.playerId = playerId;
        this.set = set;
    }

    public int getId() {
        return playerId;
    }

    public int[] getSet() {
        return set;
    }
}

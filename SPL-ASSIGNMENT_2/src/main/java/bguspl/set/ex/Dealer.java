package bguspl.set.ex;

import java.util.Collections;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

import bguspl.set.Env;
import javafx.util.Pair;

/**
 * This class manages the dealer's threads and data
 */
public class Dealer implements Runnable {

    /**
     * The game environment object.
     */
    private final Env env;

    /**
     * Game entities.
     */
    private final Table table;
    private final Player[] players;

    private Thread[] playerThreads;

    /**
     * The list of card ids that are left in the dealer's deck.
     */
    private final List<Integer> deck;

    /**
     * True iff game should be terminated due to an external event.
     */
    private volatile boolean terminate;

    /**
     * A queue of incoming sets from players
     * the key of the Pair is the player id, and the value is the set
     */
    private BlockingQueue<Pair> setsQueue;

    // private static final int[] allCards;

    /**
     * The time when the dealer needs to reshuffle the deck due to turn timeout.
     */
    private long reshuffleTime = Long.MAX_VALUE;

    public Dealer(Env env, Table table, Player[] players) {
        this.env = env;
        this.table = table;
        this.players = players;
        this.reshuffleTime = env.config.turnTimeoutMillis;
        setsQueue = new ArrayBlockingQueue<>(players.length);
        deck = IntStream.range(0, env.config.deckSize).boxed().collect(Collectors.toList());
    }

    /**
     * The dealer thread starts here (main loop for the dealer thread).
     */
    @Override
    public void run() {
        System.out.printf("Info: Thread %s starting.%n", Thread.currentThread().getName());
        // init players threads
        playerThreads = new Thread[players.length];
        for (Player player : players) {
            Thread playerThread = new Thread(player, "player" + player.id);
            playerThread.start();
        }

        placeAllCardsOnTable();
        while (!shouldFinish()) {
            timerLoop();
            updateTimerDisplay(false);
            removeAllCardsFromTable();
        }
        announceWinners();
        System.out.printf("Info: Thread %s terminated.%n", Thread.currentThread().getName());
    }

    /**
     * The inner loop of the dealer thread that runs as long as the countdown did
     * not time out.
     */
    private void timerLoop() {
        while (!terminate && System.currentTimeMillis() < reshuffleTime) {
            sleepUntilWokenOrTimeout();
            if (!setsQueue.isEmpty()) {
                Pair<Integer, int[]> set = setsQueue.poll();
                handleSet(set);
            }
            updateTimerDisplay(false);
        }
    }

    public void onSetFound(Player player, int[] set) {
        if (set.length != 3) {
        } // throw bad set exception.

        Pair<Integer, int[]> pair = new Pair<>(player.id, set);
        setsQueue.add(pair);
    }

    private void handleSet(Pair<Integer, int[]> pair) {
        Player player = players[pair.getKey()];
        int[] set = pair.getValue();

    }

    /**
     * Called when the game should be terminated due to an external event.
     */
    public void terminate() {
        terminate = true;
    }

    /**
     * Check if the game should be terminated or the game end conditions are met.
     *
     * @return true iff the game should be finished.
     */
    private boolean shouldFinish() {
        return terminate || env.util.findSets(deck, 1).size() == 0;
    }

    /**
     * Sleep for a fixed amount of time or until the thread is awakened for some
     * purpose.
     */
    private void sleepUntilWokenOrTimeout() {
        try {
            Thread.sleep(15);
        } catch (InterruptedException e) {
            // interrupted, worken up
            e.printStackTrace();
        }
    }

    /**
     * Reset and/or update the countdown and the countdown display.
     */
    private void updateTimerDisplay(boolean reset) {
        long countdown = reshuffleTime - System.currentTimeMillis();
        env.ui.setCountdown(countdown, countdown < env.config.turnTimeoutWarningMillis);
        // TODO implement
    }

    private void shuffleDeck() {
        Collections.shuffle(deck);
    }

    /**
     * Checks if any cards should be removed from the table and returns them to the
     * deck.
     */
    private void removeCardsFromTable(int[] slots) {
        for (int slot : slots) {
            env.ui.removeCard(slot);
            if (throwCards) {
                int index = deck.indexOf(slot);
                deck.remove(index);
            }
            for (Player player : players) {
                player.removeTokenIfPlaced(slot);
            }
        }
    }

    /**
     * Returns all the cards from the table to the deck.
     */
    private void removeAllCardsFromTable() {
        int[] slots = new int[env.config.tableSize];
        for (int i = 0; i < env.config.tableSize; i++) {
            slots[i] = i;
        }
        removeCardsFromTable(slots, false);

    }

    /**
     * Check if any cards can be removed from the deck and placed on the table.
     */
    private void placeCardsOnTable(int[] slots) {
        // TODO deck isempty, is there a set in the deck
        if (deck.isEmpty() || env.util.findSets(deck, 1).isEmpty()) {
            terminate();
        }

        // TODO implement

        for (int i = 0; i < slots.length; i++) {
            int slot = slots[i];
            int card = deck.remove(i);
            table.placeCard(card, slot);
        }
        // wake players
        notifyAll();
        ;
    }

    private void placeAllCardsOnTable() {
        for (int i = 0; i < env.config.tableSize; i++){
            int card = deck.get(i);
            table.placeCard(card, i);
        }
    }

    /**
     * Check who is/are the winner/s and displays them.
     */
    private void announceWinners() {
        // TODO implement
    }
}

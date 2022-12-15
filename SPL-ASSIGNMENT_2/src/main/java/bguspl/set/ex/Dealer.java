package bguspl.set.ex;

import bguspl.set.Env;
import java.util.Collections;
import java.util.List;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.stream.Collectors;
import java.util.stream.IntStream;

/**
 * This class manages the dealer's threads and data
 */
public class Dealer implements Runnable, TableListener {

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

  /**
   * time since
   */
  private long lastShuffleTime = Long.MIN_VALUE;

  public Dealer(Env env, Table table, Player[] players) {
    this.env = env;
    this.table = table;
    this.players = players;
    this.reshuffleTime = env.config.turnTimeoutMillis;
    setsQueue = new ArrayBlockingQueue<>(players.length);
    deck =
      IntStream
        .range(0, env.config.deckSize)
        .boxed()
        .collect(Collectors.toList());
    registerToTableChanges();
  }

  /**
   * The dealer thread starts here (main loop for the dealer thread).
   */
  @Override
  public void run() {
    System.out.printf(
      "Info: Thread %s starting.%n",
      Thread.currentThread().getName()
    );
    // init players threads
    playerThreads = new Thread[players.length];
    for (Player player : players) {
      Thread playerThread = new Thread(player, "player" + player.id);
      playerThread.start();
    }

    while (!shouldFinish()) {
      placeAllCardsOnTable();
      timerLoop();
      updateTimerDisplay(true);
      removeAllCardsFromTable();
    }
    announceWinners();
    System.out.printf(
      "Info: Thread %s terminated.%n",
      Thread.currentThread().getName()
    );
  }

  /**
   * The inner loop of the dealer thread that runs as long as the countdown did
   * not time out.
   */
  private void timerLoop() {
    while (
      !terminate && System.currentTimeMillis() - lastShuffleTime < reshuffleTime
    ) {
      sleepUntilWokenOrTimeout();
      if (!setsQueue.isEmpty()) {
        System.out.println("dealer q isnt empty");
        Pair set = setsQueue.poll();
        handleSet(set);
      }
      updateTimerDisplay(false);
    }
  }

  public void onSetFound(Player player, int[] set) {
    if (set.length != 3) {} // throw bad set exception.

    Pair pair = new Pair(player.id, set);
    setsQueue.add(pair);
  }

  private void handleSet(Pair pair) {
    Player player = players[pair.getId()];
    synchronized (player) {
      int[] set = pair.getSet();
      int[] cards = new int[3];
      for (int i = 0; i < cards.length; i++) {
        cards[i] = table.slotToCard[set[i]];
      }
      boolean isLegalSet = env.util.testSet(cards);
      if (isLegalSet) {
        player.point();
        player.penalize(env.config.pointFreezeMillis);
        removeCardsFromTable(set, true);
        placeCardsOnTable(set);
      } else {
        table.removePlayerTokens(player.id);
        player.penalize(env.config.penaltyFreezeMillis);
        player.setAcceptInput(true);
        player.notify();
      }
    }
  }

  /**
   * Called when the game should be terminated due to an external event.
   */
  public void terminate() {
    setsQueue.clear();
    for (Thread playerThread : playerThreads) {
      playerThread.interrupt();
    }
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

  private void registerToTableChanges() {
    table.register(this);
  }

  /**
   * Reset and/or update the countdown and the countdown display.
   */
  private void updateTimerDisplay(boolean reset) {
    if (reset) {
      reshuffleTime = env.config.turnTimeoutMillis;
      lastShuffleTime = System.currentTimeMillis();
    }
    long countdown =
      lastShuffleTime + reshuffleTime - System.currentTimeMillis();
    env.ui.setCountdown(
      countdown,
      countdown < env.config.turnTimeoutWarningMillis
    );
    // TODO implement
  }

  private void shuffleDeck() {
    Collections.shuffle(deck);
  }

  /**
   * Checks if any cards should be removed from the table and returns them to the
   * deck.
   */
  private void removeCardsFromTable(int[] slots, boolean throwCards) {
    setAllPlayersFreezeState(true);
    for (int slot : slots) {
      table.removeCard(slot);
      if (throwCards) {
        int index = deck.indexOf(table.slotToCard[slot]);
        deck.remove(index);
      }
      for (Player player : players) {
        table.removeToken(player.id, slot);
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
      int card = deck.get(i);
      table.placeCard(card, slot);
    }
    // wake players
    setAllPlayersFreezeState(false);
  }

  private void placeAllCardsOnTable() {
    setAllPlayersFreezeState(true);

    if (env.util.findSets(deck, 1).size() <= 0) terminate();

    shuffleDeck();
    for (int i = 0; i < env.config.tableSize; i++) {
      int card = deck.get(i);
      table.placeCard(card, i);
    }
    lastShuffleTime = System.currentTimeMillis();

    setAllPlayersFreezeState(false);
  }

  /**
   * Check who is/are the winner/s and displays them.
   */
  private void announceWinners() {
    // TODO implement
  }

  @Override
  public void onSetAvailable(Pair pair) {
    System.out.println("on available set");
    setsQueue.add(pair);
  }

  private void setAllPlayersFreezeState(boolean freeze) {
    for (Player player : players) {
      player.setAcceptInput(!freeze);
    }
  }
}

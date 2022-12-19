package bguspl.set.ex;

import bguspl.set.Env;
import java.util.ArrayList;
import java.util.Arrays;
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

  private Thread dealerThread;

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
  private BlockingQueue<SetWithPlayerId> setsQueue;

  // private static final int[] allCards;

  /**
   * The time when the dealer needs to reshuffle the deck due to turn timeout.
   */
  private long reshuffleTime = Long.MAX_VALUE;

  /**
   * time since
   */
  private long lastShuffleTime = Long.MIN_VALUE;

  /*
   * index of next card to pull from deck
   *
   */
  private int deckIndex = 0;

  /*
   * Dictates how long the dealer thread should sleep each timerLoop iteration.
   */
  private long sleepTime = 1000;

  private boolean timerWarning = false;

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
    dealerThread = Thread.currentThread();
    System.out.printf(
      "Info: Thread %s starting.%n",
      Thread.currentThread().getName()
    );

    initPlayersThreads();

    while (!shouldFinish()) {
      placeAllCardsOnTable();
      updateTimerDisplay(true);
      timerLoop();
      removeAllCardsFromTable();
    }
    announceWinners();

    System.out.printf(
      "Info: Thread %s terminated.%n",
      Thread.currentThread().getName()
    );
  }

  private void initPlayersThreads() {
    playerThreads = new Thread[players.length];
    for (int i = 0; i < playerThreads.length; i++) {
      Player player = players[i];
      Thread playerThread = new Thread(player, "player" + player.id);
      playerThreads[i] = playerThread;
      playerThread.start();
    }
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
      updateTimerDisplay(false);
      if (!setsQueue.isEmpty()) {
        SetWithPlayerId set = setsQueue.poll();
        handleSet(set);
      }
    }
  }

  public void onSetFound(Player player, int[] set) {
    if (set.length != 3) {} // throw bad set exception.

    SetWithPlayerId pair = new SetWithPlayerId(player.id, set);
    setsQueue.add(pair);
  }

  private void handleSet(SetWithPlayerId pair) {
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
        updateTimerDisplay(true);
        placeCardsOnTable(set);
      } else {
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

    System.out.println("player threads = " + Arrays.toString(playerThreads));

    for (int i = players.length - 1; i >= 0; i--) {
      Player player = players[i];
      Thread playerThread = playerThreads[i];
      player.terminate();
      try {
        playerThread.join();
      } catch (InterruptedException e) {
        e.printStackTrace();
      }
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
      Thread.sleep(sleepTime);
    } catch (InterruptedException ignored) {
      // interrupted, worken up
    }
  }

  private void registerToTableChanges() {
    table.register(this);
  }

  /**
   * Reset and/or update the countdown and the countdown display.
   */
  private void updateTimerDisplay(boolean reset) {
    long countdown;

    if (reset) {
      reshuffleTime = env.config.turnTimeoutMillis;
      lastShuffleTime = System.currentTimeMillis();
      countdown = reshuffleTime;
    } else {
      countdown = lastShuffleTime + reshuffleTime - System.currentTimeMillis();
    }

    timerWarning = countdown < env.config.turnTimeoutWarningMillis;

    sleepTime = timerWarning ? 20 : 1000;

    env.ui.setCountdown(countdown, timerWarning);
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
      if (slot == -1) continue;
      if (throwCards) {
        int index = deck.indexOf(table.slotToCard[slot]);
        deck.remove(index);
        deckIndex--;
      }
      for (Player player : players) {
        table.removeToken(player.id, slot);
      }
      table.removeCard(slot);
    }
  }

  /**
   * Returns all the cards from the table to the deck.
   */
  private void removeAllCardsFromTable() {
    int[] slots = new int[env.config.tableSize];
    for (int i = 0; i < env.config.tableSize; i++) {
      slots[i] = i;
      if (table.slotToCard[i] == -1) slots[i] = -1;
    }
    removeCardsFromTable(slots, false);

    deckIndex = 0;
  }

  /**
   * Check if any cards can be removed from the deck and placed on the table.
   */
  private void placeCardsOnTable(int[] slots) {
    if (deck.isEmpty() || env.util.findSets(deck, 1).isEmpty()) {
      terminate();
    }

    List<Integer> slotsToShuffle = new ArrayList<>();
    for (int i = 0; i < slots.length; i++) {
      slotsToShuffle.add(slots[i]);
    }
    Collections.shuffle(slotsToShuffle);

    for (int i = 0; i < slotsToShuffle.size() && deckIndex < deck.size(); i++) {
      int slot = slotsToShuffle.get(i);
      int card = deck.get(deckIndex);
      System.out.println();
      deckIndex++;
      table.placeCard(card, slot);
    }

    // wake the players
    setAllPlayersFreezeState(false);
  }

  private void placeAllCardsOnTable() {
    setAllPlayersFreezeState(true);

    if (env.util.findSets(deck, 1).size() <= 0) {
      terminate();
      return;
    }

    shuffleDeck();

    int[] allTableSlots = new int[env.config.tableSize];

    for (int i = 0; i < env.config.tableSize; i++) {
      allTableSlots[i] = i;
    }

    placeCardsOnTable(allTableSlots);
  }

  /**
   * Check who is/are the winner/s and displays them.
   */
  private void announceWinners() {
    int highestScore = 0;

    List<Integer> list = new ArrayList<>();

    for (Player player : players) {
      int score = player.getScore();
      if (score > highestScore) {
        highestScore = score;
        list = new ArrayList<>();
        list.add(player.id);
      } else if (score == highestScore) {
        list.add(player.id);
      }
    }

    int[] winners = new int[list.size()];

    for (int i = 0; i < winners.length; i++) {
      winners[i] = list.get(i);
    }

    env.ui.announceWinner(winners);
  }

  @Override
  public void onSetAvailable(SetWithPlayerId pair) {
    try {
      setsQueue.add(pair);
    } catch (Exception e) {
      // queue is full
    }

    dealerThread.interrupt();
  }

  private void setAllPlayersFreezeState(boolean freeze) {
    for (Player player : players) {
      player.setAcceptInput(!freeze);
    }
  }
}

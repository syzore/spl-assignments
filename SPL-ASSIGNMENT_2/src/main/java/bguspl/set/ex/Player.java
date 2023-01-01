package bguspl.set.ex;

import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.BlockingQueue;
import java.util.concurrent.ThreadLocalRandom;

import bguspl.set.Env;

/**
 * This class manages the players' threads and data
 *
 * @inv id >= 0
 * @inv score >= 0
 */
public class Player implements Runnable {

  /**
   * The game environment object.
   */
  private final Env env;

  /**
   * The game dealer object.
   */
  private final Dealer dealer;

  /**
   * Game entities.
   */
  private final Table table;

  /**
   * The id of the player (starting from 0).
   */
  public final int id;

  /**
   * The thread representing the current player.
   */
  private Thread playerThread;

  /**
   * The thread of the AI (computer) player (an additional thread used to generate
   * key presses).
   */
  private Thread aiThread;

  /**
   * True iff the player is human (not a computer player).
   */
  private final boolean human;

  /**
   * True iff game should be terminated due to an external event.
   */
  private volatile boolean terminate;

  /**
   * The current score of the player.
   */
  private int score;

  /*
   * Tells the player if it can accept inputs or not from the InputManager
   */
  private boolean acceptInput;

  /**
   * A list that keeps the slots the player chose at the current try to make a
   * set.
   */
  private BlockingQueue<Integer> keyPressQueue;

  /*
   * A boolean telling the player thread whether or not he has a penalty or not
   */
  private boolean penalty;

  /*
   * Sets the length of the player's penalty in milliseconds
   */
  private long penaltyTime = 0;

  /**
   * The class constructor.
   *
   * @param env    - the environment object.
   * @param dealer - the dealer object.
   * @param table  - the table object.
   * @param id     - the id of the player.
   * @param human  - true iff the player is a human player (i.e. input is provided
   *               manually, via the keyboard).
   */
  public Player(Env env, Dealer dealer, Table table, int id, boolean human) {
    this.env = env;
    this.dealer = dealer;
    this.table = table;
    this.id = id;
    this.human = human;
    this.keyPressQueue = new ArrayBlockingQueue<>(env.config.featureSize);
    acceptInput = true;
  }

  /**
   * The main player thread of each player starts here (main loop for the player
   * thread).
   */
  @Override
  public void run() {
    playerThread = Thread.currentThread();
    System.out.printf(
        "Info: Thread %s starting.%n",
        Thread.currentThread().getName());
    if (!human)
      createArtificialIntelligence();

    while (!terminate) {
      synchronized (this) {
        if (penalty) {
          handlePenalty(penaltyTime);
        } else {
          if (keyPressQueue.isEmpty()) {
            try {
              wait();
            } catch (InterruptedException e) {
              e.printStackTrace();
            }
          } else {
            int slot = keyPressQueue.poll();
            table.handleToken(this, slot);
            if (keyPressQueue.isEmpty()) {
              try {
                wait();
              } catch (InterruptedException e) {
                e.printStackTrace();
              }
            }
          }
        }
      }
    }

    System.out.printf("outside %s while loop ", id);

    if (!human)
      try {
        synchronized (aiThread) {
          aiThread.join();
        }
      } catch (InterruptedException ignored) {
      }
    System.out.printf(
        "Info: Thread %s terminated.%n",
        Thread.currentThread().getName());
  }

  /**
   * Creates an additional thread for an AI (computer) player. The main loop of
   * this thread repeatedly generates
   * key presses. If the queue of key presses is full, the thread waits until it
   * is not full.
   */
  private void createArtificialIntelligence() {
    // note: this is a very very smart AI (!)
    aiThread = new Thread(
        () -> {
          System.out.printf(
              "Info: Thread %s starting.%n",
              Thread.currentThread().getName());

          while (!terminate) {
            int slot = ThreadLocalRandom
                .current()
                .nextInt(0, env.config.tableSize);
            if (acceptInput) {
              synchronized (this) {
                try {
                  keyPressQueue.add(slot);
                } catch (Exception e) {
                  // queue is full
                }
                this.notify();
              }
              try {
                Thread.sleep(15);
              } catch (InterruptedException e) {
                // TODO Auto-generated catch block
                e.printStackTrace();
              }
            } else {
              synchronized (aiThread) {
                try {
                  aiThread.wait();
                } catch (InterruptedException e) {
                  // TODO Auto-generated catch block
                  e.printStackTrace();
                }
              }
            }
          }
          System.out.printf(
              "Info: Thread %s terminated.%n",
              Thread.currentThread().getName());
        },
        "computer-" + id);
    aiThread.start();
  }

  /**
   * Called when the game should be terminated due to an external event.
   */
  public void terminate() {
    keyPressQueue.clear();
    synchronized (this) {
      terminate = true;
      this.notifyAll();
      playerThread.interrupt();
    }
  }

  /**
   * This method is called when a key is pressed.
   *
   * @param slot - the slot corresponding to the key pressed.
   */
  public void keyPressed(int slot) {
    if (!acceptInput || !human)
      return;

    synchronized (this) {
      keyPressQueue.add(slot);
      this.notify();
    }
  }

  /**
   * Award a point to a player and perform other related actions.
   *
   * @post - the player's score is increased by 1.
   * @post - the player's score is updated in the ui.
   */
  public void point() {
    int ignored = table.countCards(); // this part is just for demonstration in the unit tests
    env.ui.setScore(id, ++score);
  }

  /**
   * Handles a player penalty and updates the penalty freeze clock.
   */
  private void handlePenalty(long millis) {
    penalty = false;
    setAcceptInput(false);
    long startingTime = System.currentTimeMillis();
    long remainingTime = millis;
    env.ui.setFreeze(id, remainingTime);
    while (remainingTime > 0) {
      try {
        Thread.sleep(1000);
      } catch (InterruptedException ignored) {
      }

      remainingTime = (long) ((Math.ceil(
          (millis + startingTime - System.currentTimeMillis()) / 1000.0)) *
          1000.0);

      env.ui.setFreeze(id, remainingTime);
    }
    env.ui.setFreeze(id, 0);
    setAcceptInput(true);
  }

  public void penalize(long millis) {
    penaltyTime = millis;
    synchronized (this) {
      penalty = true;
      this.notify();
    }
  }

  public int getScore() {
    return score;
  }

  public void setAcceptInput(boolean b) {
    acceptInput = b;

    if (!human) {
      if (b) {
        synchronized (aiThread) {
          aiThread.notify();
        }
      }
    }
  }

  /**
   * only for testing
   */
  public boolean getAcceptInput() {
    return acceptInput;
  }
}

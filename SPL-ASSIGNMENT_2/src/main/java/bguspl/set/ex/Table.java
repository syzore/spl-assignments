package bguspl.set.ex;

import bguspl.set.Env;
import java.util.ArrayList;
import java.util.Arrays;
import java.util.List;
import java.util.Objects;
import java.util.stream.Collectors;

/**
 * This class contains the data that is visible to the player.
 *
 * @inv slotToCard[x] == y iff cardToSlot[y] == x
 */
public class Table {

  /**
   * The game environment object.
   */
  private final Env env;

  /**
   * Mapping between a slot and the card placed in it (null if none).
   */
  protected final Integer[] slotToCard; // card per slot (if any)

  /**
   * Mapping between a card and the slot it is in (null if none).
   */
  protected final Integer[] cardToSlot; // slot per card (if any)

  private TableListener listener;

  private List<Integer>[] currentTokens;

  /**
   * Constructor for testing.
   *
   * @param env        - the game environment objects.
   * @param slotToCard - mapping between a slot and the card placed in it (null if
   *                   none).
   * @param cardToSlot - mapping between a card and the slot it is in (null if
   *                   none).
   */
  public Table(Env env, Integer[] slotToCard, Integer[] cardToSlot) {
    this.env = env;
    this.slotToCard = slotToCard;
    this.cardToSlot = cardToSlot;
    currentTokens = new ArrayList[env.config.players];
    for (int i = 0; i < currentTokens.length; i++) {
      currentTokens[i] = new ArrayList<Integer>();
    }
  }

  /**
   * Constructor for actual usage.
   *
   * @param env - the game environment objects.
   */
  public Table(Env env) {
    this(
      env,
      new Integer[env.config.tableSize],
      new Integer[env.config.deckSize]
    );
  }

  /**
   * This method prints all possible legal sets of cards that are currently on the
   * table.
   */
  public void hints() {
    List<Integer> deck = Arrays
      .stream(slotToCard)
      .filter(Objects::nonNull)
      .collect(Collectors.toList());
    env.util
      .findSets(deck, Integer.MAX_VALUE)
      .forEach(set -> {
        StringBuilder sb = new StringBuilder().append("Hint: Set found: ");
        List<Integer> slots = Arrays
          .stream(set)
          .mapToObj(card -> cardToSlot[card])
          .sorted()
          .collect(Collectors.toList());
        int[][] features = env.util.cardsToFeatures(set);
        System.out.println(
          sb
            .append("slots: ")
            .append(slots)
            .append(" features: ")
            .append(Arrays.deepToString(features))
        );
      });
  }

  /**
   * Count the number of cards currently on the table.
   *
   * @return - the number of cards on the table.
   */
  public int countCards() {
    int cards = 0;
    for (Integer card : slotToCard) if (card != null) ++cards;
    return cards;
  }

  /*
   * Anyone who calls this method can listen and be notified when a set is formed.
   */
  public void register(TableListener listener) {
    this.listener = listener;
  }

  /**
   * Places a card on the table in a grid slot.
   *
   * @param card - the card id to place in the slot.
   * @param slot - the slot in which the card should be placed.
   *
   * @post - the card placed is on the table, in the assigned slot.
   */
  public void placeCard(int card, int slot) {
    try {
      Thread.sleep(env.config.tableDelayMillis);
    } catch (InterruptedException ignored) {}

    cardToSlot[card] = slot;
    slotToCard[slot] = card;

    env.ui.placeCard(card, slot);
  }

  /**
   * Removes a card from a grid slot on the table.
   *
   * @param slot - the slot from which to remove the card.
   */
  public synchronized void removeCard(int slot) {
    try {
      Thread.sleep(env.config.tableDelayMillis);
    } catch (InterruptedException ignored) {}

    slotToCard[slot] = -1;

    env.ui.removeCard(slot);
  }

  /**
   * Places a player token on a grid slot.
   *
   * @param player - the player the token belongs to.
   * @param slot   - the slot on which to place the token.
   */
  public void placeToken(Player player, int slot) {
    int playerId = player.id;

    if (currentTokens[playerId].size() == 3) {
      // it means he already has 3 token
      return;
    }

    if (slotToCard[slot] == null || slotToCard[slot] == -1) {
      return;
    }

    env.ui.placeToken(playerId, slot);
    currentTokens[playerId].add(slot);
    if (currentTokens[playerId].size() == 3) {
      player.setAcceptInput(false);
      int[] set = new int[3];
      for (int i = 0; i < set.length; i++) {
        set[i] = currentTokens[playerId].get(i);
      }
      SetWithPlayerId pair = new SetWithPlayerId(playerId, set);
      listener.onSetAvailable(pair);
    }
  }

  /**
   * Removes a token of a player from a grid slot.
   *
   * @param player - the player the token belongs to.
   * @param slot   - the slot from which to remove the token.
   * @return - true iff a token was successfully removed.
   */
  public boolean removeToken(int player, int slot) {
    int index = currentTokens[player].indexOf(slot);
    if (index != -1) {
      currentTokens[player].remove(index);
      env.ui.removeToken(player, slot);
      return true;
    } else {
      return false;
    }
  }

  public void handleToken(Player player, int slot) {
    synchronized (player) {
      // exists
      if (currentTokens[player.id].contains(slot)) {
        removeToken(player.id, slot);
        player.notify();
      } else {
        placeToken(player, slot);
      }
      // else
    }
  }

  public void removePlayerTokens(int id) {
    for (int i = 0; i < currentTokens[id].size();) {
      Integer token = currentTokens[id].get(i);
      removeToken(id, token);
    }
  }
}

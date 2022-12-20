package bguspl.set.ex;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertFalse;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import bguspl.set.Config;
import bguspl.set.Env;
import bguspl.set.UserInterface;
import bguspl.set.UserInterfaceImpl;
import bguspl.set.Util;

import java.util.List;
import java.util.concurrent.BlockingQueue;
import java.util.logging.Logger;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

@ExtendWith(MockitoExtension.class)
class PlayerTest {

  Player player;

  @Mock
  Util util;

  @Mock
  private UserInterface ui;

  @Mock
  private Table table;

  @Mock
  private Dealer dealer;

  @Mock
  private Logger logger;

  void assertInvariants() {
    assertTrue(player.id >= 0);
    assertTrue(player.getScore() >= 0);
  }

  @BeforeEach
  void setUp() {
    // purposely do not find the configuration files (use defaults here).
    Config config = new Config(logger, (String) null);
    Env env = new Env(logger, new Config(logger, (String) null), ui, util);
    player = new Player(env, dealer, table, 0, true);
    assertInvariants();
  }

  @AfterEach
  void tearDown() {
    assertInvariants();
  }

  @Test
  void point() {
    // force table.countCards to return 3
    when(table.countCards()).thenReturn(3); // this part is just for demonstration

    // calculate the expected score for later
    int expectedScore = player.getScore() + 1;

    // call the method we are testing
    player.point();

    // check that the score was increased correctly
    assertEquals(expectedScore, player.getScore());

    // check that ui.setScore was called with the player's id and the correct score
    verify(ui).setScore(eq(player.id), eq(expectedScore));
  }

  @Test
  void setAcceptInput() {
    // sets player's acceptInput field to true value
    player.setAcceptInput(true);

    // checks if the field was successfully changed
    assertTrue(player.getAcceptInput());

    // sets
    player.setAcceptInput(false);
    assertFalse(player.getAcceptInput());
  }

  @Test
  void keyPressed() {

    new Thread(player).start();
    
    player.keyPressed(0);
   
    try {
      Thread.sleep(1000);
    } catch (InterruptedException e) {
      // TODO Auto-generated catch block
      e.printStackTrace();
    }

    verify(table).handleToken(eq(player), eq(0));
  }

}

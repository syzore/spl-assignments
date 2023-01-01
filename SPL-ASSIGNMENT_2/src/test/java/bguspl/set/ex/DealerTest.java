package bguspl.set.ex;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.verify;

import java.util.logging.Logger;

import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

import bguspl.set.Config;
import bguspl.set.Env;
import bguspl.set.UserInterface;
import bguspl.set.Util;
import bguspl.set.UtilImpl;

@ExtendWith(MockitoExtension.class)
class DealerTest {

  Dealer dealer;

  Env env;

  @Mock
  Util util;

  @Mock
  private UserInterface ui;

  private Table table;

  private Player player;

  @Mock
  private Logger logger;

  void assertInvariants() {
    assertTrue(1 >= 0);
  }

  @BeforeEach
  void setUp() {
    // purposely do not find the configuration files (use defaults here).
    Config config = new Config(logger, (String) null);
    util = new UtilImpl(config);
    env = new Env(logger, config, ui, util);
    table = new Table(env);
    player = new Player(env, dealer, table, 0, true);
    dealer = new Dealer(env, table, new Player[] { player });

    assertInvariants();
  }

  @AfterEach
  void tearDown() {
    assertInvariants();
  }

  @Test
  void placeAllCardsOnTable() {
    int initialCount = table.countCards();

    assertEquals(0, initialCount);

    dealer.placeAllCardsOnTableTest();

    int finalCount = table.countCards();

    // check that the number of cards on the table equals to the table size.
    assertEquals(env.config.tableSize, finalCount);
  }

  @Test
  void resetTimer() {
    dealer.updateTimerDisplayTest(true);

    boolean warn = env.config.turnTimeoutMillis > env.config.turnTimeoutWarningMillis;

    // verify that the ui was updated to the wanted time and that the warning correlates.
    verify(ui).setCountdown(eq(env.config.turnTimeoutMillis), eq(warn));
  }
}

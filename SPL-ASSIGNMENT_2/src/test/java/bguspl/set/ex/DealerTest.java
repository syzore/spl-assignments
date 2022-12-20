package bguspl.set.ex;

import static org.junit.jupiter.api.Assertions.assertEquals;
import static org.junit.jupiter.api.Assertions.assertTrue;
import static org.mockito.ArgumentMatchers.eq;
import static org.mockito.Mockito.verify;
import static org.mockito.Mockito.when;

import bguspl.set.Config;
import bguspl.set.Env;
import bguspl.set.UserInterface;
import bguspl.set.Util;
import bguspl.set.UtilImpl;
import java.util.logging.Logger;
import org.junit.jupiter.api.AfterEach;
import org.junit.jupiter.api.BeforeEach;
import org.junit.jupiter.api.Test;
import org.junit.jupiter.api.extension.ExtendWith;
import org.mockito.Mock;
import org.mockito.junit.jupiter.MockitoExtension;

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

    // check that the score was increased correctly
    assertEquals(env.config.tableSize, finalCount);
  }

  @Test
  void resetTimer() {
    dealer.updateTimerDisplayTest(true);

    boolean warn =
      env.config.turnTimeoutMillis > env.config.turnTimeoutWarningMillis;

    verify(ui).setCountdown(eq(env.config.turnTimeoutMillis), eq(warn));
  }
}

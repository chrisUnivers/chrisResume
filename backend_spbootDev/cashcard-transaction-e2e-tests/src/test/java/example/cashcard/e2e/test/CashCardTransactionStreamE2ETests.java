package example.cashcard.e2e.test;

import static org.assertj.core.api.Assertions.assertThat;

import java.io.IOException;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.List;

import org.awaitility.Awaitility;
import org.junit.jupiter.api.Test;
import org.springframework.boot.autoconfigure.EnableAutoConfiguration;
import org.springframework.boot.test.context.SpringBootTest;
import org.springframework.context.annotation.Import;
import org.springframework.kafka.test.context.EmbeddedKafka;

import example.cashcard.enricher.CashCardTransactionEnricher;
import example.cashcard.sink.CashCardTransactionSink;
import example.cashcard.stream.CashCardTransactionStream;

// Enable Embedded Kafka
@EmbeddedKafka
// Configure auto-configuration of embedded Kafka for the individual application configurations
@SpringBootTest(classes = CashCardTransactionStreamE2ETests.StreamTestConfig.class, properties = {
  "spring.cloud.function.definition=approvalRequest;enrichTransaction;cashCardTransactionFileSink",
  "spring.cloud.stream.bindings.approvalRequest-out-0.destination=approval-requests",
  "spring.cloud.stream.bindings.enrichTransaction-in-0.destination=approval-requests",
  "spring.cloud.stream.bindings.enrichTransaction-out-0.destination=enriched-transactions",
  "spring.cloud.stream.bindings.cashCardTransactionFileSink-in-0.destination=enriched-transactions"
  })
  public class CashCardTransactionStreamE2ETests {

  @Test
  void cashCardTransactionStreamEndToEnd() throws IOException {
    Path path = Paths.get(CashCardTransactionSink.CSV_FILE_PATH);;

    // Remove the old sink-file if needed.
    if (Files.exists(path)) {
        Files.delete(path);
    }

    // Wait for the sink file to appear and fetch the first line
    Awaitility.await().until(() -> Files.exists(path));
    List<String> lines = Files.readAllLines(path);
    String csvLine = lines.get(0);

    // Test for information we know about the enriched transactions
    assertThat(csvLine).contains("sarah1");
    assertThat(csvLine).contains("123 Main street");
    assertThat(csvLine).contains("APPROVED");

  }

   // Configure auto-configuration of embedded Kafka for the individual components
   @EnableAutoConfiguration
   @Import({CashCardTransactionStream.class, CashCardTransactionEnricher.class, CashCardTransactionSink.class})
   public static class StreamTestConfig {

   }
}
package ch16Examples;

import java.util.concurrent.CompletableFuture;
import java.util.concurrent.Future;

public class exOneSixFour {
    
    public Future<Double> getPriceAsync(String product) {
        CompletableFuture<Double> futurePrice = new CompletableFuture<>();
        new Thread( () -> {
            double price = calculatePrice(product);
            futurePrice.complete(price);
        }).start();
        return futurePrice;
    }

    public Future<Double> getPriceAsyncImproved(String product) {
        // Throws exception notifying clients of exceptions
        CompletableFuture<Double> futurePrice = new CompletableFuture<>();
        new Thread( () -> {
            try {
                double price = calculatePrice(product);
                futurePrice.complete(price);
            } catch (Exception ex) {
                futurePrice.completeExceptionally(ex);
            }
        }).start();
        return futurePrice;
    }

    public Future<Double> getPriceAsyncSimple(String product) {
        return CompletableFuture.supplyAsync(() -> calculatePrice(product));
    }

    public static void main() {
        Shop shop = new Shop("BestShop");
        long start = System.nanoTime();
        Future<Double> futurePrice = shop.getPriceAsync("my favorite product");
        long invocationTime = ((System.nanoTime() - start) / 1_000_000);
        System.out.println("Invocation returned after " + invocationTime + " msecs");

        // do some more tasks, like querying other shops
        doSomethingElse();
        // while the price of the product is being calculated
        try {
            // the .get operation of the Future<>: futurePrice will return if futurePrice has returned of will block untill the return/ value is available.
            double price = futurePrice.get();
            System.out.printf("Price is %.2f%n", price);
        } catch (Exception e) {
            throw new RuntimeException(e);
        }
        long retrievalTime = ((System.nanoTime() - start) / 1_000_000);
        System.out.println("Price returned after " + retrievalTime + " msecs");
    }
}


/** NOTES
 * creating an async method and consuming synchronous methods in an asynchronous methods are two different things. Illustrated in Section 16.3
 */
package ch16Examples;

import java.util.List;
import java.util.Random;
import java.util.concurrent.CompletableFuture;
import java.util.concurrent.Executor;
import java.util.concurrent.Executors;
import java.util.stream.Collectors;
import java.util.stream.Stream;

/*Using CompletablFutures to pipeline multiple asynchronous operations in a declarative style similar to what we learned by insg the Streams API */
public class sectOneSixThree {
    
    public class Shop {

        private final String name;
        private final Random random;

        public Shop(String name) {
            this.name = name;
            random = new Random(name.charAt(0) * name.charAt(1)*name.charAt(2));
        }

        public String getName() {
            return this.name;
        }

        public String getPrice(String product) {
            double price = calculatePrice(product);
            // Discount.Code code = Discount.Code.values()[random.nextInt(Discount.Code.values().length)];
            // return name + ":" + price + ":" + code;
            return name + ":" + price + ":" + "code";
        }

        public double calculatePrice(String product) {
            // delay();
            // return format(random.nextDouble() * product.charAt(0) + product.charAt(1));
            return random.nextDouble() * product.charAt(0) + product.charAt(1);

        }

        
    }

    public class Clients {

        // The shop class implements synchronous methods
        List<Shop> shops = List.of(new Shop("BestPrice"),
                   new Shop("LetsSaveBig"),
                   new Shop("MyFavoriteShop"),
                   new Shop("BuyItAll"));

        private final Executor executor = Executors.newFixedThreadPool(Math.min(shops.size(), 100), (Runnable r) -> {
            Thread t = new Thread(r);
            t.setDaemon(true);
            return t;
        });
        
        // Don't do this
        public List<String> findPrices(String product) {
            return shops.stream().map(shop -> String.format("%s price is %.2f", shop.getName(), shop.getPrice(product))).collect(Collectors.toList());
        }

        // improved
        public List<String> findPricesVTwo(String product) {
            return shops.parallelStream().map(shop -> String.format("%s price is %.2f", shop.getName(), shop.getPrice(product))).collect(Collectors.toList());
        }

        // improved v3
        public List<String> findPricesVThree(String product) {
            List<CompletableFuture<String>> priceFutures = shops.stream().map(shop -> CompletableFuture.supplyAsync( () -> shop.getName() + " price is " + shop.getPrice(product))).collect(Collectors.toList());
            
            /**The join used above is to avoid using a try/catch block as before. */

            return priceFutures.stream().map(CompletableFuture::join).collect(Collectors.toList());
        }

        // improved v4
        public List<String> findPricesVFour(String product) {
            List<CompletableFuture<String>> priceFutures = findPricesStream(product).collect(Collectors.<CompletableFuture<String>>toList());

            // shops.stream().map(shop -> CompletableFuture.supplyAsync(() -> shop.getName() + " price is " + shop.getPrice(product) , executor));
        }


        public Stream<CompletableFuture<String>> findPricesStream(String product) {
            return shops.stream()
                .map(shop -> CompletableFuture.supplyAsync(() -> shop.getPrice(product), executor))
                .map(future -> future.thenApply(Quote::parse))
                .map(future -> future.thenCompose(quote -> CompletableFuture.supplyAsync(() -> Discount.applyDiscount(quote), executor)));
                
        }

    }

    public static void main(String[] args) {
        sectOneSixThree root = new sectOneSixThree();
        Clients clients = root.new Clients();
        long start = System.nanoTime();
        System.out.println(clients.findPrices("myPhone27S"));
        long duration = (System.nanoTime() - start) / 1_000_000;
        System.out.println("Done in " + duration + " msces");
        /**
         * N_threads = N^(#CPUs) * U^(#CPUs) * (1 + W/C)
         * U^(#CPU) is the target CPU use(between 0 and 1)
         * W/C is the ratio of wait time to compute time
         */
    }
}

package modernJavaInActionExs.ch015Examples;

import java.util.concurrent.Executors;
import java.util.concurrent.ScheduledExecutorService;
import java.util.concurrent.TimeUnit;

/**
 * Section 15.2.3 Sleeping (and other blocking operations) considered harmful
 * 
 * Using the private class is better since once the thread that called work1() is done this thread is released and can be used for something else.
 */

public class blockingNnonBlocking {

    public void wk1()
    {
        // do something
    }

    public void wk2()
    {
        // do something else
    }
    
    public void blockingEx()
    {
        wk1();
        try {
            Thread.sleep(10000);
        } catch (InterruptedException ie) {
            System.err.println("This thread was interrupted");
        }
        wk2();
    }

    private class ScheduledExecutorServiceExample {

        public static void main(String[] args) {
            ScheduledExecutorService schExService = Executors.newScheduledThreadPool(1);

            work1();
            schExService.schedule(ScheduledExecutorServiceExample::work2, 10, TimeUnit.SECONDS);

            schExService.shutdown();
        }

        private static void work1()
        {
            // do something
        }

        private static void work2()
        {
            // do somehting else
        }
    }

}

import java.util.ArrayList;
import java.util.List;
import java.util.concurrent.Flow.Publisher;
import java.util.concurrent.Flow.Subscriber;

public class SubPubExample {
    

    // Section 15.5.1 spreadshee cell3 = cell1 +cell2
    int flowId;
    public SubPubExample(int flowId)
    {
        this.flowId = flowId;
    }

    private class SimpleCell implements Publisher<Integer>, Subscriber<Integer> {
        private int value = 0;
        private String name;
        private List<Subscriber> subscribers = new ArrayList<>();
    
        public SimpleCell(String name) {
            this.name = name;
        }

        @Override
        public void subscribe(Subscriber<? super Integer> subscriber) {
            subscribers.add(subscriber);
        }

        private void notifyAllSubscribers() {
            subscribers.forEach(subscriber -> subscriber.onNext(this.value));
        }

        @Override
        public void onNext(Integer newValue)
        {
            this.value = newValue;
            System.out.println(this.name + ":" + this.value);
            notifyAllSubscribers();
        }
    }

    private class ArithmeticCell extends SimpleCell {
        
        private int left;
        private int right;

        public ArithmeticCell(String name) {
            super(name);
        }

        public void setLeft(int left) {
            this.left = left;
            onNext(left + this.right);
        }

        public void setRight(int right) {
            this.right = right;
            onNext(right + this.left);
        }
    }
    

    public static void main(String[] args) {
        SubPubExample flow = new SubPubExample(0);
        SimpleCell c3 = flow.new SimpleCell("C3");
        SimpleCell c2 = flow.new SimpleCell("C2");
        SimpleCell c1 = flow.new SimpleCell("C1");

        c1.subscribe(c3);

        c1.onNext(10); // Update value of C1 to 10
        c2.onNext(20); // update value of C2 to 20
    }
}

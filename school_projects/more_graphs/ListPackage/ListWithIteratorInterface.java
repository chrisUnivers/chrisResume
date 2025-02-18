package ListPackage;
import java.util.Iterator;
public interface ListWithIteratorInterface<T> extends ListInterface<T>, 
Iterable<T>
{
    public Iterator<T> getIterator();
} // end ListWithIteratorInterface

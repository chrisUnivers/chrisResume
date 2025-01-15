package ListPackage;

import java.util.Iterator;
import java.util.NoSuchElementException;
public class LinkedListWithIterator<T> implements ListWithIteratorInterface<T>
{
    private Node firstNode;
    private Node lastNode;
    private int numberOfEntries;

    public LinkedListWithIterator()
    {
        initializeDataFields();
    }
    
    public void clear(){
        initializeDataFields();
    }

    private void initializeDataFields()
    {
        firstNode = null;
        lastNode = null;
        numberOfEntries = 0;
    } // end initializeDataFields

    public void add(T newEntry)
    {
        Node newNode = new Node(newEntry);

        if (isEmpty())
            firstNode = newNode;
        else
            lastNode.setNextNode(newNode);

        lastNode = newNode;
        numberOfEntries++;
    }  // end add

    public void add(int givenPosition, T newEntry)
    {
        if ((givenPosition >= 1) && (givenPosition <= numberOfEntries + 1))
        {
            Node newNode = new Node(newEntry);
            if (isEmpty())
            {
                firstNode = newNode;
                lastNode = newNode;
            }
            else if (givenPosition == 1)
            {
                newNode.setNextNode(firstNode);
                firstNode = newNode;
            }
            else if (givenPosition == numberOfEntries + 1)
            {
                lastNode.setNextNode(newNode);
                lastNode = newNode;
            }
            else
            {
                Node nodeBefore = getNodeAt(givenPosition - 1);
                Node nodeAfter = nodeBefore.getNextNode();
                newNode.setNextNode(nodeAfter);
                nodeBefore.setNextNode(newNode);
            } // end if
            numberOfEntries++;
        }
        else
            throw new IndexOutOfBoundsException(
                        "Illegal position given to add operation.");
    } // end add

    public T remove(int givenPosition)
    {
        T result = null;                           // Return value
        if ((givenPosition >= 1) && (givenPosition <= numberOfEntries))
        {
        // Assertion: !isEmpty()
            if (givenPosition == 1)                 // Case 1: Remove first entry
            {
                result = firstNode.getData();        // Save entry to be removed
                firstNode = firstNode.getNextNode();
                if (numberOfEntries == 1)
                    lastNode = null;                  // Solitary entry was removed
            }
            else                                    // Case 2: Not first entry
            {
                Node nodeBefore = getNodeAt(givenPosition - 1);
                Node nodeToRemove = nodeBefore.getNextNode();
                Node nodeAfter = nodeToRemove.getNextNode();
                nodeBefore.setNextNode(nodeAfter);
                result = nodeToRemove.getData();
                if (givenPosition == numberOfEntries)
                    lastNode = nodeBefore;            // Last node was removed
            } // end if
            numberOfEntries--;
        }
        else
            throw new IndexOutOfBoundsException(
                        "Illegal position given to remove operation.");

        return result;                             // Return removed entry
    } // end remove

    public boolean isEmpty()
    {
        boolean result;
        if (numberOfEntries == 0)
        {
            assert firstNode == null : "numberOfEntries is 0 but firstNode is not null";
            assert lastNode == null : "numberOfEntries is 0 but lastNode is not null";
            result = true;
        }else
        {
            assert firstNode != null : "numberOfEntries is not 0 but firstNode is null";
            assert lastNode == null : "numberOfEntries is  not 0 but lastNode is null";
            result = false;
        }
        // return (numberOfEntries == 0) && (firstNode == null);
        return result;
    } // end isEmpty

    public int getLength()
    {
        return numberOfEntries;
    }

    public T[] toArray()
    {
        // The cast is safe because the new array contains null entries
        @SuppressWarnings("unchecked")
        T[] result = (T[])new Object[numberOfEntries];
        
        int index = 0;
        Node currentNode = firstNode;
        while ((index < numberOfEntries) && (currentNode != null))
        {
            result[index] = currentNode.getData();
            currentNode = currentNode.getNextNode();
            index++;
        } // end while
        
        return result;
    } // end toArray

    public T replace(int givenPosition, T newEntry)
    {
        if ((givenPosition >= 1) && (givenPosition <= numberOfEntries))
        {
            // Assertion: !isEmpty()
            Node desiredNode = getNodeAt(givenPosition);
            T originalEntry = desiredNode.getData();
            desiredNode.setData(newEntry);
            return originalEntry;
        }
        else
            throw new IndexOutOfBoundsException(
                        "Illegal position given to replace operation.");
    } // end replace

    public T getEntry(int givenPosition)
    {
        if ((givenPosition >= 1) && (givenPosition <= numberOfEntries))
        {
            assert !isEmpty();
            return getNodeAt(givenPosition).getData();
        }
        else
            throw new IndexOutOfBoundsException(
                        "Illegal position given to getEntry operation.");
    } // end getEntry

    public boolean contains(T anEntry)
    {
        boolean found = false;
        Node currentNode = firstNode;
        
        while (!found && (currentNode != null))
        {
            if (anEntry.equals(currentNode.getData()))
                found = true;
            else
                currentNode = currentNode.getNextNode();
        } // end while
        
        return found;
    } // end contains

    private Node getNodeAt(int givenPosition)
    {
        // to be worked on! chp 12 operation on a chain of linked lists(pg 340 at the end of page)
        //NOTE: The node firstNode contains a reference to the first node in the chain.
        assert firstNode != null : "The current list is empty";
        if (givenPosition >= 1 && (givenPosition <= numberOfEntries))
        {
            Node currentNode = null;
            if (givenPosition == numberOfEntries)
            {
                currentNode = lastNode;
            }
            else
            {
                currentNode = firstNode;
                for (int counter = 1; counter < givenPosition; counter++)
                {
                    currentNode = currentNode.getNextNode();
                }
            }
            assert currentNode != null;
            return currentNode;
        }else
        {
            throw new IndexOutOfBoundsException("Illegal position given to getNodeAt operation");
        }
    } // end getNodeAt

    public Iterator<T> iterator()
    {
        return new IteratorForLinkedList();
    } // end iterator

    public Iterator<T> getIterator()
    {
        return iterator();
    } // end getIterator

    private class IteratorForLinkedList implements Iterator<T>
    {
        private Node nextNode;
        // pg 383
        private IteratorForLinkedList()
        {
            // nextNode = LinkedListWithIterator.this.firstNode
            nextNode = firstNode;
        } // end default constructor

        public T next()
        {
            T result;
            if (hasNext())
            {
                result = nextNode.getData();
                nextNode = nextNode.getNextNode(); // Advance iterator
            }
            else
                throw new NoSuchElementException("Illegal call to next(); " +
                                                "iterator is after end of list.");
            return result; // Return next entry in iteration
        } // end next

        public boolean hasNext()
        {
            return nextNode != null;
        } // end hasNext

        public void remove()
        {
            throw new UnsupportedOperationException("remove() is not supported" + " by this iterator");
        } // end remove


    }
    
    private class Node 
    {
        // < see Listing 3-4 in chapter 3. >
        private T data; // Entry in bag
        private Node next;

        private Node(T dataPortion)
        {
            this(dataPortion, null);
        }
        private Node(T dataPortion, Node nextNode)
        {
            data = dataPortion;
            next = nextNode;
        }

        private T getData()
        {
            return data;
        }

        private void setData(T newData)
        {
            data = newData;
        }

        private Node getNextNode()
        {
            return next;
        }

        private void setNextNode(Node nextNode)
        {
            next = nextNode;
        }
    } // end Node
} // end LinkedListWithIterator(Segment 13.8 and 13.9 chapter 13.)
/*
 * The list part is implemented using LListWithTail from chapter 12.
 * This is all so that I can implement the class Basic and DirectedGraph from chapter 30.
 */

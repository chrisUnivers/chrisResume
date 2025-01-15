package test;

import static org.junit.Assert.assertEquals;
import static org.junit.Assert.assertTrue;

import java.util.Iterator;

import org.junit.Test;

import GraphPackage.VertexInterface;

/**Tests for Graph Algorithms in the graph package  
 * WILL DEAL WITH UNIT TESTS LATER. SEE:
 * https://stackoverflow.com/questions/52373469/how-to-launch-junit-5-platform-from-the-command-line-without-maven-gradle/52373592#52373592
 * https://stackoverflow.com/questions/2235276/how-to-run-junit-test-cases-from-the-command-line
 * or search junit console.
*/


public class SimpleUnitTests {
    
    @Test
    public void AllNeighborsAreAddedToEdgeList(VertexInterface<String> vertexM, int iterartion) {
        Iterator<VertexInterface<String>> mNeighbors;

        mNeighbors = vertexM.getNeighborIterator();
        do {
            assertTrue(mNeighbors.hasNext());
            mNeighbors.next();
            iterartion--;
        }while(iterartion > 0);
        

    }

    @Test
    public void edgeAddedToEdgeList(boolean wasAdded) {

        assertTrue(wasAdded);
    }
    
}

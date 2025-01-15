package GraphPackage;
import java.util.Iterator;
import java.util.Queue;
import java.util.ArrayDeque;
import java.util.Collection;
import java.util.HashMap;
import java.util.Map;
import java.util.PriorityQueue;

import test.AdditionalMethods;
public class DirectedGraph<T> implements GraphInterface<T>
{    
    private Map<T, VertexInterface<T>> vertices;
    private int edgeCount;
    
    public DirectedGraph()
    {
        vertices = new HashMap<>();
        edgeCount = 0;
    } // end default constructor

    public boolean addVertex(T vertexLabel)
    {
        VertexInterface<T> addOutCome = vertices.put(vertexLabel, 
                                        new Vertex<>(vertexLabel));
        return addOutCome == null; // was addition to dictionary successful
    } // end addVertex

    public boolean addEdge(T begin, T end, double edgeWeight)
    {
        boolean result = false;
        VertexInterface<T> beginVertex = vertices.get(begin);
        VertexInterface<T> endVertex = vertices.get(end);
        if (beginVertex != null && (endVertex != null))
        {
            result = beginVertex.connect(endVertex, edgeWeight);
            
        }
        if (result)
        {
            edgeCount++;
        }
        else // TODO: not part of original code
        {
            AdditionalMethods<T> writeGraph = 
            new AdditionalMethods<>();
            writeGraph.writeGraph(vertices);
        }
        return result;
    } // end addEdge

    public boolean addEdge(T begin, T end)
    {
        return addEdge(begin, end, 0);
    } // end addEdge

    public boolean hasEdge(T begin, T end)
    {
        boolean found = false;
        VertexInterface<T> beginVertex = vertices.get(begin);
        VertexInterface<T> endVertex = vertices.get(end);
        if ((beginVertex != null) && (endVertex != null))
        {
            Iterator<VertexInterface<T>> neighbors = beginVertex.getNeighborIterator();
            while (!found && neighbors.hasNext())
            {
                VertexInterface<T> nextNeighbor = neighbors.next();
                if (endVertex.equals((nextNeighbor)))
                {
                    found = true;
                } // end if
            } // end while
        } // end if
        return found;
    } // end hasEdge

    public boolean isEmpty()
    {
        return vertices.isEmpty();
    } // end isEmpty

    public void clear()
    {
        vertices.clear();
        edgeCount = 0;
    } // end clear

    public int getNumberOfVertices()
    {
        return vertices.size();
    } // end getNumberOfVertices

    public int getNumberOfEdges()
    {
        return edgeCount;
    } // end getNumberOfEdges

    protected void resetVertices()
    {
        Collection<VertexInterface<T>> vertexCollection = vertices.values();
        Iterator<VertexInterface<T>> vertexIterator = vertexCollection.iterator();
        while (vertexIterator.hasNext())
        {
            VertexInterface<T> nextVertex = vertexIterator.next();
            nextVertex.unvisit();
            nextVertex.setCost(0);
            nextVertex.setPredecessor(null);
        } // end while

    } // end resetVertices

    public Queue<T> getBreadthFirstTraversal(T origin)
    {
            resetVertices();
            Queue<T> traversalOrder = new ArrayDeque<>();
            Queue<VertexInterface<T>> vertexQueue = new ArrayDeque<>();
            VertexInterface<T> originVertex = vertices.get(origin);
            VertexInterface<T> frontVertex;
            VertexInterface<T> nextNeighbor;
            Iterator<VertexInterface<T>> neighbors;
            originVertex.visit();
            traversalOrder.add(origin);
            vertexQueue.add(originVertex);

            while (!vertexQueue.isEmpty())
            {
                frontVertex = vertexQueue.poll();
                neighbors = frontVertex.getNeighborIterator();
                
                while (neighbors.hasNext())
                {
                    nextNeighbor = neighbors.next();
                    if (!nextNeighbor.isVisited())
                    {
                        nextNeighbor.visit();
                        traversalOrder.add(nextNeighbor.getLabel());
                        vertexQueue.add(nextNeighbor);
                    }
                    
                }
                
            }

            return traversalOrder;
    } // getBreadthFirstTraversal

    public Queue<T> getDepthFirstTraversal(T origin)
    {
            resetVertices();
            Queue<T> traversalOrder = new ArrayDeque<>();
            ArrayDeque<VertexInterface<T>> vertexQueue = new ArrayDeque<>();
            VertexInterface<T> originVertex = vertices.get(origin);
            VertexInterface<T> frontVertex;
            VertexInterface<T> nextNeighbor;
            originVertex.visit();
            traversalOrder.add(originVertex.getLabel());
            vertexQueue.add(originVertex);

            while (!vertexQueue.isEmpty())
            {
                frontVertex = vertexQueue.peekLast();
                nextNeighbor = frontVertex.getUnvisitedNeighbor();
                if (nextNeighbor != null)
                {
                    if (!nextNeighbor.isVisited())
                    {
                        nextNeighbor.visit();
                        vertexQueue.add(nextNeighbor);
                        traversalOrder.add(nextNeighbor.getLabel());
                    }
                }else
                {
                    vertexQueue.removeLast();
                }  
            }

            return traversalOrder;
    } // end getDepthFirstTraversal.(pg863) Left as exercise
    public ArrayDeque<T> getTopologicalOrder()
    {
            ArrayDeque<T> topologicalOrder = new ArrayDeque<>();

            return topologicalOrder;
    }// end getTopologicalOrder.(pg863) I think left as exercise

    public int getShortestPath(T begin, T end, ArrayDeque<T> path)
    {
            resetVertices();
            boolean done = false;
            Queue<VertexInterface<T>> vertexQueue = new ArrayDeque<>();
            VertexInterface<T> beginVertex = vertices.get(begin);
            VertexInterface<T> endVertex = vertices.get(end);
            VertexInterface<T> frontVertex;
            VertexInterface<T> nextNeighbor;
            Iterator<VertexInterface<T>> neighbors;
            beginVertex.visit();
            vertexQueue.add(beginVertex);

            while (!done && !vertexQueue.isEmpty())
            {
                frontVertex = vertexQueue.poll();
                // traversalOrder.add(frontVertex.getLabel());
                neighbors = frontVertex.getNeighborIterator();
                
                while (!done && neighbors.hasNext())
                {
                    nextNeighbor = vertices.get(neighbors.next()
                        .getLabel());
                    if (!nextNeighbor.isVisited())
                    {
                        nextNeighbor.visit();
                        nextNeighbor.setCost(frontVertex.getCost() + 1.0);
                        nextNeighbor.setPredecessor(frontVertex);
                        vertexQueue.add(nextNeighbor);
                    }
                    if (nextNeighbor.equals(endVertex))
                        done = true;  
                }
            }
            int pathLength = (int) endVertex.getCost();
            path.add(endVertex.getLabel());
            VertexInterface<T> vertex = endVertex;
            while (vertex.hasPredecessor())
            {
                vertex = vertex.getPredecessor();
                path.add(vertex.getLabel());
            }
            return pathLength; // pathLength = shortestPath
    }// end getShortestPath

    
    public double getCheapestPath(T begin, T end, ArrayDeque<T> path)
    {
            Queue<EntryPQ> vertexPQ = new PriorityQueue<>();
            VertexInterface<T> beginVertex = vertices.get(begin);
            VertexInterface<T> endVertex = vertices.get(end);
            EntryPQ beginEntry = new EntryPQ(beginVertex, 0.0, null);
            double pathLength = 0.0;
            boolean done = false;
            vertexPQ.add(beginEntry);

            while (!done && !vertexPQ.isEmpty())
            {
                EntryPQ frontEntry = vertexPQ.poll();
                VertexInterface<T> frontVertex = frontEntry.vertex;
                if (!frontVertex.isVisited())
                {
                    frontVertex.visit();
                    frontVertex.setCost(frontEntry.cost);
                    frontVertex.setPredecessor(frontEntry.prevVertex);
                    if (frontVertex.equals(endVertex))
                    {
                        done = true;
                    }
                    else
                    {
                        /**TODO: See note 1 in README */
                        Iterator<VertexInterface<T>> neighbors = frontVertex
                            .getNeighborIterator();
                        Iterator<Double> weightIterator = frontVertex.getWeightIterator();
                        while (neighbors.hasNext())
                        { /**TODO: See note 2 in README. 
                                nextNeighbor = neighbors.next() causes problems */
                            VertexInterface<T> nextNeighbor 
                                = vertices.get(neighbors
                                .next().getLabel());
                            if (!nextNeighbor.isVisited())
                            {
                                double nextNeighborWeight = weightIterator.next() 
                                    + frontEntry.cost;
                                EntryPQ nextEntry = new EntryPQ(nextNeighbor, 
                                    nextNeighborWeight, frontVertex);
                                vertexPQ.add(nextEntry);
                            }
                        }
                    }
                }

            }
            pathLength = endVertex.getCost();
            VertexInterface<T> curVertex = endVertex;
            path.add(curVertex.getLabel());
            while(curVertex.hasPredecessor())
            {
                curVertex = curVertex.getPredecessor();
                path.add(curVertex.getLabel());
            }
            return pathLength;
    }// end getcheapestPath.(pg863) Left as exercise

    private class EntryPQ implements Comparable<EntryPQ>
    {
            private VertexInterface<T> vertex;
            private double cost;
            private VertexInterface<T> prevVertex;
            public EntryPQ(VertexInterface<T> vertex, 
                double cost, VertexInterface<T> prevVertex)
            {
                this.vertex = vertex;
                this.prevVertex = prevVertex;
                this.cost = cost;
            }

            public double getCost()
            {
                return cost;
            }
            public int compareTo(EntryPQ other)
            {
                return (int) (this.cost - other.cost);
            }
    }
}

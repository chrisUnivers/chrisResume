package test;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.Iterator;
import java.util.Map;

import GraphPackage.VertexInterface;

public class AdditionalMethods<T> {
    
    public static final String FILE_TO_WRITE_TO = "newGraph.txt";
    public void allNeighbors(VertexInterface<T> vertex)
    {
        Iterator<VertexInterface<T>> mIterator = vertex.getNeighborIterator();
        System.out.println(vertex + ":");
        if (isDone()) {
            while(mIterator.hasNext())
        {
            System.out.println("Neighbor: " + mIterator.next().toString());
        }
        }
        // System.out.println("\n");
    }

    public void writeGraph(Map<T, VertexInterface<T>> vertices)
    {
        if (isDone())
        {
            System.out.println("Ready to write graph");
            try {
                BufferedWriter writer = new BufferedWriter(
                new FileWriter(FILE_TO_WRITE_TO));
                String line = "";
                for (T label : vertices.keySet())
                {
                    // System.out.println("THE LABEL IS: " + label);
                    Iterator<VertexInterface<T>> neighbors = vertices.
                    get(label).getNeighborIterator();
                    Iterator<Double> edgeWeights = vertices.
                    get(label).getWeightIterator(); 
                    
                    line += label.toString() + ":"; 
                    if (neighbors.hasNext())
                    {
                        while (neighbors.hasNext())
                        {
                            VertexInterface<T> nextNeighbor = neighbors.next();
                            double nextWeight = edgeWeights.next();
                            line += " " + nextNeighbor.toString() + " "
                            + nextWeight;
                            writer.write(line);
                            line = "";
                        }
                    }
                    line += "\n"; 
                    writer.write(line);
                }
                writer.close();
            } catch (IOException e) {
                e.printStackTrace();
            }
        }
    }
    public boolean isDone()
    {
        boolean result = false;
        try{
            BufferedReader reader = new BufferedReader(new FileReader("output.txt"));
            if (reader.readLine().equals("DONE")) {
                result = true;
            }
            reader.close();
        } catch (IOException e){
            // e.printStackTrace();
        }
        return result;
    }

}

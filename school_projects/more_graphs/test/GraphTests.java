package test;

import java.io.BufferedReader;
import java.io.BufferedWriter;
import java.io.FileReader;
import java.io.FileWriter;
import java.io.IOException;
import java.util.ArrayDeque;
import java.util.Scanner;

import GraphPackage.DirectedGraph;
import GraphPackage.GraphInterface;
import ListPackage.LinkedListWithIterator;
import static test.NamedConstants.*;

/**
 * @author Chris
 */
public class GraphTests<T> {
    
    public static void main(String[] args)
    {
        GraphInterface<String> init_graph = new DirectedGraph<>();
        ArrayDeque<String> path = new ArrayDeque<>();
        Double path_length = 0.0;
        int algo = 0;

        algo = algorithmToRun();

        if (algo == 1)
        {
            init_graph = getInput();
            path_length = (double)init_graph.getShortestPath("A", "H", path);
            ALGORITHM_STRING = "Shortest Path";
            writeToFile(path_length, path, CHEAPEST_LENGTH_STRING, GRAPH_FILE
            , ALGORITHM_STRING);
        }else
        { // the exceptions in algorithmToRun should catch invalid inputs
            init_graph = getInput();
            path_length = init_graph.getCheapestPath("A", "H", path);
            ALGORITHM_STRING = "Cheapest Path";
            writeToFile(path_length, path, CHEAPEST_LENGTH_STRING, GRAPH_FILE
            , ALGORITHM_STRING);
        }

    }

    public static int algorithmToRun()
    {
        Scanner scanner = new Scanner(System.in);
        String input;
        boolean done = false;
        int valid_input = 0;

        do
        {
            try
            {
                System.out.print("Enter 1 to run the Shortest path"
                    + " algorithm, 2 to run"
                    + " Cheapest path algorithm: ");
                input = scanner.nextLine();
                System.out.println("");
                valid_input = Integer.parseInt(input);
                if (valid_input == 1 || (valid_input == 2))
                {
                    done = true;
                }
                else
                {
                    throw new InvalidInputException();
                }
            }
            catch (InvalidInputException iie)
            {
                System.out.println(iie);
            }
            catch (NumberFormatException nfe)
            {
                System.out.println("Hey, that's not a number");
            }
        } while(!done);
        return valid_input;
    }

    public static GraphInterface<String> getInput()
    {
        Scanner scanner = new Scanner(System.in);
        boolean done = false;
        GraphInterface<String> graph = new DirectedGraph<>();
        LinkedListWithIterator<String> labels = 
        new LinkedListWithIterator<>();
        
        do 
        {
            String response;
            System.out.println("Enter the text file to be read." 
                + " For exampe: file.txt");
            INPUT_FILE_STRING = scanner.nextLine();
            System.out.println("The file< " 
                + INPUT_FILE_STRING + " > will be used." 
                + " Enter ok to begin, enter no to use another"
                + " file:)");
            response = scanner.nextLine();
            if (response.equals("ok"))
                done = true;
        }while(!done);

        scanner.close();
        graph = readFileInput(INPUT_FILE_STRING);

        return graph;
    }

    public static GraphInterface<String> readFileInput(String inputFile)
    {
        boolean added = true;
        String[] noEntry = {};
        GraphInterface<String> graph = new DirectedGraph<>();
        LinkedListWithIterator<String> labels = 
            new LinkedListWithIterator<>();
        boolean fileOk = false;
        try {
            BufferedReader reader = new BufferedReader(
                new FileReader(INPUT_FILE_STRING));
            BufferedWriter writer = new BufferedWriter(
                new FileWriter("output.txt"));
            String line;
            String[] entry;

            try {
                while (((line = reader.readLine()) != null) && added)
                {
                    entry = line.split(" ");
                    added = addToGraph(entry, graph, labels);
                }
            } catch (IOException i) {
                System.out.println(INVALID_FILE_FORMAT_STRING);
            }
            fileOk = true;
            if (fileOk)
                writer.write("DONE");
            writer.close();
            reader.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
        assert(fileOk == true);
        // Causes addToGraph to write output files.
        added = addToGraph(noEntry, graph, labels);
        return graph;
    }
    public static boolean addToGraph(String[] anEdge, GraphInterface<String> graph, LinkedListWithIterator<String> labels)
    {
        String startVertex;
        String endVertex;
        double weight;
        boolean result = false;
        try{
            boolean k;
            if (!(anEdge.length == 0))
            {
                startVertex = anEdge[0];
                endVertex = anEdge[1];
            }else { // to trigger the graph to write to the output file
                startVertex = "";
                endVertex = "";
            }
            if (!startVertex.equals(""))
            {
                if (!labels.contains(startVertex))
                {
                    labels.add(startVertex);
                    k = graph.addVertex(startVertex);
                }
                if (!labels.contains(endVertex))
                {
                    k = graph.addVertex(endVertex);
                }
                weight = Double.parseDouble(anEdge[2]);
            }else {
                weight = 0.0;
            }

            boolean edgeAdded = graph.addEdge(startVertex, endVertex, weight);
            result = true;
        }
        catch(NumberFormatException nfe) { // node format exception
            System.out.println(NOT_FLOAT_A_FORMAT_STRING);
        }
        return result;
    }

    public static boolean addVertices(GraphInterface<String> graph, String beginLabel, String endLabel)
    {
        boolean result = false;
        if (beginLabel.equals(null))
        result = graph.addVertex(beginLabel);
        result = graph.addVertex(endLabel);

        return result;
    }

    public static void writeToFile(double pathLength,
        ArrayDeque<String> path, String title, String fileName,
        String algorithm)
    {
        int pathSize = path.size();
        System.out.println("Check the file: " + fileName 
            + " to see the output of the " + algorithm + " Algorithm");
        try {
            BufferedWriter writer = new BufferedWriter(
                new FileWriter(fileName));
            String line = CHEAPEST_LENGTH_STRING + pathLength + "\n";
            writer.write(line);
            line = "The path is:\n";
            writer.write(line);
            line = "";
            while (!path.isEmpty())
            {
                line += path.pollLast();
                pathSize--;
                if (!(pathSize == 0))
                    line += ARROW_STRING;
            }
            writer.write(line);
            writer.close();
        } catch (IOException e) {
            e.printStackTrace();
        }
    }
}
/**TODO: in no particular order.
 * 1) use a command line parser like the one shown here:
 *   How do I parse command line arguments in java(stackoverflow) or 
 *   read through more than one answer.
 *   Although argument parsing is recommend by some 
 *   when a program is not user interactive.
 * 
 * 2) Add FileNotFoundExceptions when trying to read files
 * ...
 */


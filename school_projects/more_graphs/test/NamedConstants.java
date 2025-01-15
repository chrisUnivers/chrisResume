package test;

public class NamedConstants {
    public static  String INPUT_FILE_STRING = "";
    public static final String OUTPUT_FILE = "output.txt";
    public static final String GRAPH_FILE = "graphAlgorithm.txt";
    public static final String CHEAPEST_LENGTH_STRING = 
        "The weight of the cheapest path from the <start vertex> to"
        +" the <end vertex> is: ";
    public static String ALGORITHM_STRING;
    public static final String ARROW_STRING = "->";
    public static final String NOT_FLOAT_A_FORMAT_STRING = 
    "The edge weight needs to be a number.";
    public static final String INVALID_FILE_FORMAT_STRING = 
        "Make sure the each line of the input type" 
        + " has the format: start_vertex end_vertex"
        + " double_edge_weight";
}

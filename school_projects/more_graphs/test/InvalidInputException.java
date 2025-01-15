package test;

public class InvalidInputException extends RuntimeException{
    
    public InvalidInputException()
    {
        super("Hey, that's not a valid input!");
    }

    public InvalidInputException(String message)
    {
        super(message);
    }
}

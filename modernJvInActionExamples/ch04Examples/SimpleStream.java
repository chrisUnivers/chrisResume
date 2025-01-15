package modernJavaInActionExs.ch04Examples;

public class SimpleStream {
    

    public static void main(String[] args) {
        List<String> threeHighCaloricDishNames = menu.stream().filter(dish -> dish.getCalories() > 300).map(Dish::getName).limit(3).collect(toList());
        System.out.println(threeHighCaloricDishNames);
    }
    /**
     * 1) menu.stream(): Gets a stream from menu(the list of dishes)
     * 2) Creates a pipeline of operations: 1st filter high-calorie dishes
     * 3) Gets the names of the dishes
     * 4) Selects only the first three
     * 5) Stores the results in another List
     * 6) Gives results [pork, beef, chicken]
     */
}

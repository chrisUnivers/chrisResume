package modernJavaInActionExs.ch04Examples;

import java.util.Arrays;
import java.util.List;

public class Exstablishment {

    int exst_id = 0;

    public Exstablishment(int exst_id) {
        this.exst_id = exst_id;
    }

    public class Dish {
        private final String name;
        private final boolean vegetarian;
        private final int calories;
        private final Type type;
        
        public Dish(String name, boolean vegetarian, int calories, Type type) {
            this.name = name;
            this.vegetarian = vegetarian;
            this.calories = calories;
            this.type = type;
        }
        /** 
         * getName(), isVegetarian(), getCalories(), getType() */

         @Override
         public String toString() {
            return name;
         }

         public enum Type {MEAT, FISH, OTHER}
    }

    public static void main(String[] args) {
        Exstablishment exst = new Exstablishment(1);
        List<Dish> menu = Arrays.asList
        (exst.new Dish("pork", false, 800, Dish.Type.MEAT),
        exst.new Dish("beef", false, 700, Dish.Type.MEAT));
    }

    
}




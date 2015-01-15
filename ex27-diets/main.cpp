#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact integral type
#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpz ET;
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif

using namespace std;

// program and solution types
typedef CGAL::Quadratic_program<int> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

const int var_x = 0; const int X = var_x;
const int var_y = 1; const int Y = var_y;
const int var_z = 2; const int Z = var_z;

int floor_to_double(const CGAL::Quotient<ET>& x) {
  double a = floor(CGAL::to_double(x));
  while (a > x) a -= 1;
  while (a + 1 <= x) a += 1;
  return a;
}


int ceil_to_double(const CGAL::Quotient<ET>& x) {
  double a = ceil(CGAL::to_double(x));
  while (a < x) a += 1;
  while (a - 1 >= x) a -= 1;
  return a;
}

int main() {

  // some basic setup stuff
  cin.sync_with_stdio(false);
  cout.sync_with_stdio(false);
  cout << fixed << setprecision(0);

  while(true) {
    int nutrient_count, food_count;
    cin >> nutrient_count >> food_count;

    // kill switch for application
    if(nutrient_count == 0 && food_count == 0) {
      break;
    }

    // create linear programming application
    Program lp(CGAL::SMALLER, true, 0, false, 0);

    // read in nutrients
    vector<int> nutrient_min;
    vector<int> nutrient_max;
    nutrient_min.reserve(nutrient_count);
    nutrient_max.reserve(nutrient_count);
    for(int i = 0; i < nutrient_count; i++) {
      int min_amount, max_amount;
      cin >> nutrient_min[i] >> nutrient_max[i];
    }

    // read in foods and amount of nutrients
    vector<int> prices;
    vector<vector<int> > nutrient_amount(food_count, vector<int>(nutrient_count));
    prices.reserve(food_count);
    for(int i = 0; i < food_count; i++) {
      cin >> prices[i];

      for(int j = 0; j < nutrient_count; j++) {
        cin >> nutrient_amount[i][j];
      }
    }

    // vars: first food, then nutrients

    // set objective
    for(int i = 0; i < food_count; i++) {
      // we want to pay the minimum
      // results in the sum of food_var * food_price
      lp.set_c(i, prices[i]);
    }

    // set inequalities
    int eq_counter = 0;
    for(int nutrient_index = 0; nutrient_index < nutrient_count; nutrient_index++) {
      for(int food_index = 0; food_index < food_count; food_index++) {
        lp.set_a(food_index, eq_counter, nutrient_amount[food_index][nutrient_index]); // A := sum over: food * amount of nutrition
      }

      lp.set_b(eq_counter, nutrient_max[nutrient_index]); // A <= maximum needed
      eq_counter++;
    }

    for(int nutrient_index = 0; nutrient_index < nutrient_count; nutrient_index++) {
      for(int food_index = 0; food_index < food_count; food_index++) {
        lp.set_a(food_index, eq_counter, nutrient_amount[food_index][nutrient_index]); // B := sum over: food * amount of nutrition
      }
      
      // B >= minimum needed
      lp.set_b(eq_counter, nutrient_min[nutrient_index]);
      lp.set_r(eq_counter, CGAL::LARGER);
      eq_counter++;
    }

    // find solution and print out
    Solution s = CGAL::solve_nonnegative_quadratic_program(lp, ET());
    
    if(s.is_optimal()) {
      cout << floor_to_double(s.objective_value()) << endl;
    } else {
      cout << "No such diet." << endl;
    }
  }
}

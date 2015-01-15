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
    int inequality_count;
    cin >> inequality_count;

    // kill switch for application
    if(inequality_count == 0) {
      break;
    }

    int dimension;
    cin >> dimension;

    // create linear programming application
    Program lp(CGAL::SMALLER, false, 0, false, 0);

    // set our target of maximixing the radius, i.e. we take the negative of the radius and minimize the whole thing
    int var_radius = dimension; // everything below `dimension` is a variable for the dimensions (?)
    lp.set_c(var_radius, -1); // maximize by minimizing the negative

    // add constraints from input
    for(int constraint_index = 0; constraint_index < inequality_count; constraint_index++) {
      int distance = 0;

      for(int dim_var_index = 0; dim_var_index < dimension; dim_var_index++) {
        int a_i;
        cin >> a_i;

        lp.set_a(dim_var_index, constraint_index, a_i);
        lp.set_a(dim_var_index, constraint_index + inequality_count, a_i);

        distance += a_i * a_i;
      }

      lp.set_a(var_radius, constraint_index, sqrt(distance));

      int b_i;
      cin >> b_i;
      lp.set_b(constraint_index, b_i);
      lp.set_b(constraint_index + inequality_count, b_i);
    }

    // find solution and print out
    Solution s = CGAL::solve_linear_program(lp, ET());
    assert(s.solves_linear_program(lp));
    if(s.is_optimal()) {
      cout << -1 * ceil_to_double(s.objective_value()) << endl;
    } else if (s.is_unbounded()) {
      cout << "inf" << endl;
    } else {
      cout << "none" << endl;
    }
  }
}

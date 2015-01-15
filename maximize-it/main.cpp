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
    int p, a, b;
    cin >> p;

    // kill switch for application
    if(p == 0) {
      break;
    }

    cin >> a >> b;

    if(p == 1) {
      //                          v- set finite lower bound (x, y >= 0), defaults to 0
      //                                    v- no upper bound
      Program lp(CGAL::SMALLER, true, 0, false, 0);
      
      // x + y <= 4
      lp.set_a(var_x, 0, 1);
      lp.set_a(var_y, 0, 1);
      lp.set_b(0, 4);
      
      // 4x + 2y <= ab
      lp.set_a(var_x, 1, 4);
      lp.set_a(var_y, 1, 2);
      lp.set_b(1, a * b);
      
      // -x + y <= 1
      lp.set_a(var_x, 2, -1);
      lp.set_a(var_y, 2, 1);
      lp.set_b(2, 1);
      
      // maximize: by -ax^2 so we have to minimize -by + ax^2
      lp.set_c(var_y, -b);
      lp.set_d(var_x, var_x, 2*a);
      
      // solve it
      Solution s = CGAL::solve_nonnegative_quadratic_program(lp, ET());
      assert(s.solves_quadratic_program(lp));
      
      if(s.is_unbounded()) {
        cout << "unbounded" << endl;
      } else if(s.is_infeasible()) {
        cout << "no" << endl;
      } else {
        cout << floor_to_double(-s.objective_value()) << endl;
      }
    } else {
      // here we have an upper bound for the variables, but no lower bound
      Program lp(CGAL::LARGER, false, 0, false, 0);
      
      // bounds by benji
      // z >= 0
      lp.set_u(X, true, 0);
      lp.set_u(Y, true, 0);
      lp.set_l(Z, true, 0);
      
      // x + y >= -4
      lp.set_a(var_x, 0, 1);
      lp.set_a(var_y, 0, 1);
      lp.set_b(0, -4);
      
      // 4x + 2y + z^2 => -ab => we substitute z^2 by just z (don't forget to do the same for the minimized formula dingsi)
      // i.e. we get 4x + 2y + z >= -ab
      lp.set_a(var_x, 1, 4);
      lp.set_a(var_y, 1, 2);
      lp.set_a(var_z, 1, 1);
      lp.set_b(1, -(a*b));
      
      // -x + y >= -11
      lp.set_a(var_x, 2, -1);
      lp.set_a(var_y, 2, 1);
      lp.set_b(2, -1);
      
      // minimize ax^2 + by + z^4 => after our substitution: ax^2 + by + z^2
      lp.set_d(var_x, var_x, 2*a);
      lp.set_d(var_z, var_z, 2 * 1);
      lp.set_c(var_y, b);
      
      // solve it
      Solution s = CGAL::solve_quadratic_program(lp, ET());
      assert(s.solves_quadratic_program(lp));
      
      if(s.is_unbounded()) {
        cout << "unbounded" << endl;
      } else if(s.is_infeasible()) {
        cout << "no" << endl;
      } else {
        cout << ceil_to_double(s.objective_value()) << endl;
      }
    }
  }
}

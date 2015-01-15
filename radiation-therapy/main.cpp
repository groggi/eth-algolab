#include <iostream>
#include <cassert>
#include <tuple>
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
typedef CGAL::Quadratic_program<ET> Program;
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

bool can_solve(int dimension, vector<tuple<int, int, int> >& healthy, vector<tuple<int, int, int> >& cancer) {
  // use EQUAL, as we set it manually if we wish => or <=
  Program qp(CGAL::EQUAL, false, 0, false, 0);

  // add healthy cell inequations
  for(int cell = 0; cell < healthy.size(); cell++) {
    int var_index = 0;
    for(int x_dim = 0; x_dim <= dimension; x_dim++) {
      for(int y_dim = 0; y_dim <= dimension; y_dim++) {
        for(int z_dim = 0; z_dim <= dimension; z_dim++) {
          if(x_dim + y_dim + z_dim <= dimension) {
            double value = pow(get<0>(healthy.at(cell)), x_dim) *
              pow(get<1>(healthy.at(cell)), y_dim) *
              pow(get<2>(healthy.at(cell)), z_dim);

            qp.set_a(var_index, cell, value);
            //cout << "value H: " << value << endl;

            var_index++;
          } else {
            break;
          }
        }
      }
    }

    qp.set_r(cell, CGAL::LARGER);
    qp.set_b(cell, 1);
  }

  // add cancer cell inequations
  for(int cell = 0; cell < cancer.size(); cell++) {
    int var_index = 0;
    for(int x_dim = 0; x_dim <= dimension; x_dim++) {
      for(int y_dim = 0; y_dim <= dimension; y_dim++) {
        for(int z_dim = 0; z_dim <= dimension; z_dim++) {
          if(x_dim + y_dim + z_dim <= dimension) {
            double value = pow(get<0>(cancer.at(cell)), x_dim) *
              pow(get<1>(cancer.at(cell)), y_dim) *
              pow(get<2>(cancer.at(cell)), z_dim);

            qp.set_a(var_index, cell + healthy.size(), value);

            //cout << "value C: " << value << endl;

            var_index++;
          } else {
            break;
          }
        }
      }
    }

    qp.set_r(cell + healthy.size(), CGAL::SMALLER);
    qp.set_b(cell + healthy.size(), -1);
  }

  // solve
  CGAL::Quadratic_program_options options;
  options.set_pricing_strategy(CGAL::QP_BLAND);     // Bland's rule to avoid cycling...
  Solution s = CGAL::solve_linear_program(qp, ET(), options);

  //cout << s << endl;

  return !s.is_infeasible();
}

int main() {

  // some basic setup stuff
  cin.sync_with_stdio(false);
  cout.sync_with_stdio(false);
  cout << fixed << setprecision(0);

  int test_count;
  cin >> test_count;
  for(int test = 0; test < test_count; test++) {
    //cout << endl << endl;
    int healthy_count, cancer_count;
    cin >> healthy_count >> cancer_count;

    // read in healthy cell location
    vector<tuple<int, int, int> > healthy(healthy_count);
    for(int healthy_index = 0; healthy_index < healthy_count; healthy_index++) {
      int x, y, z;
      cin >> x >> y >> z;

      healthy.at(healthy_index) = make_tuple(x, y, z);
    }

    // read in cancer cell location
    vector<tuple<int, int, int> > cancer(cancer_count);
    for(int cancer_index = 0; cancer_index < cancer_count; cancer_index++) {
      int x, y, z;
      cin >> x >> y >> z;

      cancer.at(cancer_index) = make_tuple(x, y, z);
    }

    // search upper lower
    int exp_value = 1;
    bool exp_found = false;
    do {
      exp_value = exp_value * 2;
      exp_found = true;
      //cout << "inc: for: " << exp_value << " is: " << can_solve(exp_value, healthy, cancer) << endl;
    } while(exp_value <= 30 && !can_solve(exp_value, healthy, cancer));

    // do binary search for the right dimension
    int min_d = 0;
    int max_d = 30;
    if(exp_found) {
      min_d = exp_value / 2 - 1;
      max_d = exp_value > 30 ? 30 : exp_value;
    }

    //cout << "min set to " << min_d << " and max to " << max_d << endl;

    bool last_worked = false;
    while(min_d < max_d) {
      int mid_d = (min_d + max_d) / 2;
      //cout << "binary search from: " << min_d << " to " << max_d << ", checking: " << mid_d << endl;
      if(can_solve(mid_d, healthy, cancer)) {
        // [min_d, mid_d] contains the solution
        max_d = mid_d;
        last_worked = true;
        //cout << "\tOK" << endl;
      } else {
        // [mid_d + 1, max_d] contains the solution
        min_d = mid_d + 1;
        last_worked = false;
        //cout << "\tBAD!" << endl;
      }
    }

    if(min_d == 30 && !last_worked) {
      cout << "Impossible!" << endl;
    } else {
      cout << min_d << endl;
    }
  }
}

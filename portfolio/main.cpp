#include <iostream>
#include <cassert>
#include <CGAL/basic.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

// choose exact integral type
#ifdef CGAL_USE_GMP
#include <CGAL/Gmpzf.h>
typedef CGAL::Gmpzf ET;
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
    int asset_count, people_count;
    cin >> asset_count >> people_count;

    // kill switch for application
    if(asset_count == 0 && people_count == 0) {
      break;
    }

    // containers for asset properties
    vector<int> asset_cost(asset_count);
    vector<int> asset_expected_return(asset_count);

    // read in asset properties
    for(int asset_index = 0; asset_index < asset_count; asset_index++) {
      cin >> asset_cost.at(asset_index) >> asset_expected_return.at(asset_index);
    }

    // read and store covariance
    vector<vector<int> > covariance(asset_count, vector<int>(asset_count));
    for(int assetA = 0; assetA < asset_count; assetA++) {
      for(int assetB = 0; assetB < asset_count; assetB++) {
        cin >> covariance.at(assetA).at(assetB);
      }
    }

    // go trough each investor and calculate for him/her the result
    for(int person_index = 0; person_index < people_count; person_index++) {
      int max_cost, min_return, max_variance;
      cin >> max_cost >> min_return >> max_variance;

      // by default, we have a nonnegative QP with Ax >= b
      Program qp (CGAL::LARGER, true, 0, false, 0);

      // equation counter
      int eq_counter = 0;

      // add inequastion for expected return
      for(int asset_index = 0; asset_index < asset_count; asset_index++) {
        // sum of each asset amount times its expected return ...
        qp.set_a(asset_index, eq_counter, asset_expected_return[asset_index]);
      }
      // ... >= investor's expected minimal return
      qp.set_b(eq_counter, min_return);
      eq_counter++;

      // add inequastion for max cost for the investor
      for(int asset_index = 0; asset_index < asset_count; asset_index++) {
        // sum of each asset's cost times how many of them we buy ...
        qp.set_a(asset_index, eq_counter, asset_cost[asset_index]);
      }
      // ... <= investor's maximum cost
      qp.set_b(eq_counter, max_cost);
      qp.set_r(eq_counter, CGAL::SMALLER);
      eq_counter++;

      // objective function
      for(int assetA = 0; assetA < asset_count; assetA++) {
        for(int assetB = 0; assetB < asset_count; assetB++) {
          qp.set_d(assetA, assetB, 2 * covariance[assetA][assetB]);
        }
      }

      // calculate
      Solution s = CGAL::solve_nonnegative_quadratic_program(qp, ET());
      assert(s.solves_quadratic_program(qp));
      
      // Output
      if (s.is_optimal() && s.objective_value() <= max_variance) {
        cout << "Yes." << endl;
      } else {
        cout << "No." << endl;
      }
    }
  }
}

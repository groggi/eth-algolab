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

bool feasible(int asset_count, int max_cost, int max_covar, int expected_return, vector<int>& asset_costs, vector<int>& asset_returns, vector<vector<int> >& asset_covar) {
  // use quadratic programming to check if given parameters result in a feasable result, i.e. given parameters result in a possible solution

  // create QP program: lower bound is 0 for amount of assets bough and generally we're interested in <= inequastions
  Program qp(CGAL::SMALLER, true, 0, false, 0);

  // restrict total cost
  for(int asset_index = 0; asset_index < asset_count; asset_index++) {
    qp.set_a(asset_index, 0, asset_costs.at(asset_index)); // sum of all asset's cost that were bought must be ...
  }

  qp.set_b(0, max_cost); // ... <= maximum cost investor is ready to pay

  // restrict how much expected return we expect, be careful as our general operation is `<=`, we have to switch it here, as we wan't at least some amount of
  // expected return
  for(int asset_index = 0; asset_index < asset_count; asset_index++) {
    qp.set_a(asset_index, 1, asset_returns[asset_index]); // sum of all asset's expected return that were bought must be ...
  }
  qp.set_r(1, CGAL::LARGER); // ... >= ...
  qp.set_b(1, expected_return); // the expected return

  // set the objective function:
  // total portfolio variance must be not too large
  for(int assetA_index = 0; assetA_index < asset_count; assetA_index++) {
    for(int assetB_index = 0; assetB_index <= assetA_index; assetB_index++) { // careful: `<= assetA_index`!
      qp.set_d(assetA_index, assetB_index, 2 * asset_covar.at(assetA_index).at(assetB_index)); // careful: don't forget the 2 * because of how the matrix works...
    }
  }

  // find solution
  Solution sol = CGAL::solve_nonnegative_quadratic_program(qp, ET());
  return sol.is_optimal() && sol.objective_value() <= max_covar;
}

int main() {

  // some basic setup stuff
  cin.sync_with_stdio(false);
  cout.sync_with_stdio(false);
  cout << fixed << setprecision(0);

  while(true) {
    int asset_count, friend_count;
    cin >> asset_count >> friend_count;

    // kill switch
    if(asset_count == 0 && friend_count == 0) {
      break;
    }

    // collect asset costs and risk factors
    vector<int> asset_costs(asset_count);
    vector<int> asset_returns(asset_count);
    for(int asset_index = 0; asset_index < asset_count; asset_index++) {
      cin >> asset_costs.at(asset_index) >> asset_returns.at(asset_index);
    }

    // collect covariance between two assets
    vector<vector<int > > asset_covar(asset_count, vector<int>(asset_count));
    for(int assetA_index = 0; assetA_index < asset_count; assetA_index++) {
      for(int assetB_index = 0; assetB_index < asset_count; assetB_index++) {
        cin >> asset_covar.at(assetA_index).at(assetB_index);
      }
    }

    // go trough investors and calculate maximum expected outcome for given conditions
    for(int investor_index = 0; investor_index < friend_count; investor_index++) {
      int max_cost, max_covar;
      cin >> max_cost >> max_covar;

      // search for upper bound for return, so we don't have to search through every possible combination
      int r = 1;
      while(feasible(asset_count, max_cost, max_covar, r, asset_costs, asset_returns, asset_covar)) {
        r *= 2;
      }

      // now we know in which range ([r/2, r]) to search for the maximum
      // use binary search to find it
      int ok = r / 2;
      int low = r / 2;
      int high = r;
      while(low <= high) {
        r = (high - low) / 2 + low;
        if(feasible(asset_count, max_cost, max_covar, r, asset_costs, asset_returns, asset_covar)) {
          ok = r;
          low = r + 1;
        } else {
          high = r - 1;
        }
      }

      cout << ok << endl;
    }
  }
}

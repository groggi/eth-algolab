#include <iostream>
#include <cassert>
#include <vector>
#include <limits>
#include <algorithm> 
#include <queue>
#include <set>
#include <utility>
#include <cmath>
#include <CGAL/basic.h>
#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/QP_models.h>
#include <CGAL/QP_functions.h>

#ifdef CGAL_USE_GMP
#include <CGAL/Gmpz.h>
typedef CGAL::Gmpzf ET;
#else
#include <CGAL/MP_Float.h>
typedef CGAL::MP_Float ET;
#endif

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;

typedef CGAL::Quadratic_program<ET> Program;
typedef CGAL::Quadratic_program_solution<ET> Solution;

using namespace std;

using namespace CGAL;

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
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);

	//cout << fixed << setprecision(0);

	int test_count;
	cin >> test_count;

	for (int test = 0; test < test_count; test++) {
		int lamp_count, stamp_count, wall_count;
		cin >> lamp_count >> stamp_count >> wall_count;

		// read in lamps
		vector<Point> lamps;
		lamps.reserve(lamp_count);

		for(int lamp_index = 0; lamp_index < lamp_count; lamp_index++) {
			int lamp_x, lamp_y;
			cin >> lamp_x >> lamp_y;

			lamps.push_back(Point(lamp_x, lamp_y));
		}

		// collect lamps and the maximum light they are allowed to get
		vector<Point> stamps;
		stamps.reserve(stamp_count);

		vector<int> stamp_max_light;
		stamp_max_light.reserve(stamp_count);

		for(int stamp_index = 0; stamp_index < stamp_count; stamp_index++) {
			int stamp_x, stamp_y, max_light;
			cin >> stamp_x >> stamp_y >> max_light;

			stamps.push_back(Point(stamp_x, stamp_y));
			stamp_max_light.push_back(max_light);
		}

		// read in walls
		vector<Segment> walls;
		walls.reserve(wall_count);
		for(int wall_index = 0; wall_index < wall_count; wall_index++) {
			int wall_start_x, wall_start_y, wall_end_x, wall_end_y;
			cin >> wall_start_x >> wall_start_y >> wall_end_x >> wall_end_y;

			walls.push_back(Segment(Point(wall_start_x, wall_start_y), Point(wall_end_x, wall_end_y)));
		}

		// Create quadratic programming instance
		Program qp(CGAL::SMALLER, true, 1, true, 4096);
		for(int stamp_index = 0; stamp_index < stamp_count; stamp_index++) {
			for(int lamp_index = 0; lamp_index < lamp_count; lamp_index++) {
				Segment light_to_stamp(lamps.at(lamp_index), stamps.at(stamp_index));

				// check if a wall blocks the light beam from lamp to stamp
				bool light_blocked = false;
				for(int wall_index = 0; wall_index < wall_count; wall_index++) {
					if(do_intersect(light_to_stamp, walls.at(wall_index))) {
						light_blocked = true;
						break;
					}
				}

				if(!light_blocked) {
					double d = CGAL::to_double(1 / light_to_stamp.squared_length());
					qp.set_a(lamp_index, stamp_index, d);
					qp.set_a(lamp_index, stamp_count + stamp_index, d);
				}
			}

			qp.set_b(stamp_index, stamp_max_light.at(stamp_index));
			qp.set_b(stamp_count + stamp_index, 1);
			qp.set_r(stamp_count + stamp_index, CGAL::LARGER);
		}

		// get solution
		Solution sol = solve_linear_program(qp, ET());
		if(sol.is_infeasible()) {
			cout << "no" << endl;
		} else {
			cout << "yes" << endl;
		}
	}

	return 0;
}
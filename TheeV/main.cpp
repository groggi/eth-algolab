#include <iostream>
#include <cmath>
#include <unordered_set>

#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/number_utils.h>

// typedefs
typedef CGAL::Exact_predicates_exact_constructions_kernel K; 
typedef CGAL::Min_circle_2_traits_2<K>  Traits;
typedef CGAL::Min_circle_2<Traits>      Min_circle;
typedef K::Point_2 Point;

using namespace std;

// global vars, needed by multiple functions
Point capital;
vector<Point> cities;
vector<K::FT> distances;

// from slides, fun!
double floor_to_double(const K::FT& x)
{
	double a = std::floor(CGAL::to_double(x));
	while (a > x) a -= 1;
	while (a+1 <= x) a += 1;
	return a;
}

double ceil_to_double(const K::FT& x)
{
	double a = std::ceil(CGAL::to_double(x));
	while (a < x) a += 1;
	while (a-1 >= x) a -= 1;
	return a;
}

bool city_order(Point cityA, Point cityB) {
	return squared_distance(cityA, capital) > squared_distance(cityB, capital);
}

int main() {
	// some basic setup stuff
	cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);
	cout << fixed << setprecision(0);

	int test_count;
	cin >> test_count;

	for(int t = 0; t < test_count; t++) {
		int city_count;
		cin >> city_count;

		// check for early termination
		if(city_count <= 2) {
			int ignore;

			// read in not needed information
			cin >> ignore >> ignore;
			if(city_count == 2) {
				cin >> ignore >> ignore;
			}

			// put in each of the cities one antenna with radius 0, done.
			cout << 0 << endl;
			continue;
		}

		// read in capital city
		double capital_x, capital_y;
		cin >> capital_x >> capital_y;
		capital = Point(capital_x, capital_y);

		// create vector containing all non-capital cities
		cities = vector<Point>(city_count - 1); // capital not part of it

		// read in city coordinates
		for(int city_index = 1; city_index < city_count; city_index++) { // capital already read, so start with 1, i.e. second city
			double city_x, city_y;
			cin >> city_x >> city_y;

			cities.at(city_index - 1) = Point(city_x, city_y);
		}

		// sort cities by descending distance from the capital
		sort(cities.begin(), cities.end(), city_order);

		// create vector with all distances for each city from the capital
		distances = vector<K::FT>(city_count - 1);
		for(int i = 0; i < city_count - 1; i++) {
			distances.at(i) = squared_distance(cities.at(i), capital);
		}

		// initiate radius temporary information
		K::FT radius1 = distances[1]; // currently the first antenna has maximal radius to contain in the furthest city
		K::FT old_radius1 = radius1;

		K::FT radius2 = 0.0; // second antenno has no radius yet
		K::FT old_radius2 = 0.0;

		// create min circle for all cities
		Min_circle min_circ_antenna2(cities.begin(), ++cities.begin(), true); // antenna 2 contains only the city farthest away from first anntena as a start
		Traits::Circle circ2;
		int cur_index = 1;

		// make first antenna's radius smaller and second antenna's radius larger to find an optimum
		while(radius2 < radius1) { // iterate as long as the second antenna is still smaller as the first one
			// move current radius to old radius
			old_radius1 = radius1;
			old_radius2 = radius2;

			// add next outmost city to antenna 2's reach
			min_circ_antenna2.insert(cities[cur_index]);

			// get the circle of the min circ to get information about the radius
			circ2 = min_circ_antenna2.circle();

			// update radius
			radius1 = distances[cur_index + 1]; // first antenna only has to reach one city less as the one after that is now covered by the second antenna
			radius2 = circ2.squared_radius();

			// update index, maybe we add another city to second antenna's reach
			cur_index++;
		}

		// we updates all four radius variables, now extract the optimum
		K::FT result = min( // we're interested in the minimum radius needed
			max(radius1, radius2), // we must take the maximum of both, as both antenna have to have the same radius!
			max(old_radius1, old_radius2) // same reason as before
			);
		cout << ceil_to_double(result) << endl;
	}
}

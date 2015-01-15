#include <iostream>
#include <cmath>

#include <CGAL/Exact_predicates_exact_constructions_kernel_with_sqrt.h>
#include <CGAL/Min_circle_2.h>
#include <CGAL/Min_circle_2_traits_2.h>
#include <CGAL/number_utils.h>

// typedefs
typedef CGAL::Exact_predicates_exact_constructions_kernel_with_sqrt K; // IMPORTANT!!!!! OTHERWISE NO SQRT!!!!!
typedef CGAL::Min_circle_2_traits_2<K>  Traits;
typedef CGAL::Min_circle_2<Traits>      Min_circle;
typedef K::Point_2 Point;

using namespace std;

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

int main() {
	// some basic setup stuff
	cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);
	cout << fixed << setprecision(0);

	while(true) {
	    int people_count;
	    cin >> people_count;

	    // kill switch for application
	    if(people_count == 0) {
	      break;
	    }

	    // collect all positions of people living at
	    vector<Point> peoples(people_count);
	    for(int i = 0; i < people_count; i++) {
	    	double person_x, person_y;
	    	cin >> person_x >> person_y;

	    	peoples.at(i) = Point(person_x, person_y);
	    }

	    // create the circle covering all people with minimal surface
	    Min_circle min_circle(peoples.begin(), peoples.end(), true);

	    // get radius
	    cout << ceil_to_double(sqrt(min_circle.circle().squared_radius())) << endl;
	}
}

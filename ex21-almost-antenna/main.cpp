#include <iostream>
#include <cmath>
#include <unordered_set>

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

	    // now we remove the support points, calculate the radius for the resulting min circles and choose the smallest one as the solution
	    K::FT min_rad = sqrt(min_circle.circle().squared_radius());
	    for(auto iter = min_circle.support_points_begin(); iter != min_circle.support_points_end(); iter++) {
	    	for(int i = 0; i < people_count; i++) {
	    		if(peoples.at(i) == *iter) {
	    			int add = i > 0 ? -1 : 1;
	    			peoples.at(i) = peoples.at(i + add);

	    			Min_circle almost_circ(peoples.begin(), peoples.end(), true);
	    			K::FT almost_rad = sqrt(almost_circ.circle().squared_radius());
	    			if(almost_rad < min_rad) {
	    				min_rad = almost_rad;
	    			}

	    			peoples.at(i) = *iter;

	    			break;
	    		}
	    	}
	    }

	    // get radius
	    cout << ceil_to_double(min_rad) << endl;
	}
}

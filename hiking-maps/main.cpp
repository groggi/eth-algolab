//
// NOT DONE
//

#include <iostream>
#include <algorithm>
#include <vector>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef K::Point_2 Point;

using namespace std;

int main() {
	// some basic setup stuff
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);
	cout << fixed << setprecision(0);

	int test_count;
	cin >> test_count;

	for(int test = 0; test < test_count; test++) {
		int leg_count, maps_count;
		cin >> leg_count >> maps_count;

		// read in the path
		vector<Point> path_points(leg_count);
		for(int leg_index = 0; leg_index < leg_count; leg_index++) {
			int x, y;
			cin >> x >> y;

			path_points.at(leg_index) = Point(x, y);
		}

		// read in the triangles representing maps
		vector<vector<Point> > maps(maps_count, vector<Point>(6));
		for(int map_index = 0; map_index < maps_count; map_index++) {
			for(int i = 0; i < 5; i++) {
				int x, y;
				cin >> x >> y;

				maps.at(map_index).at(i) = Point(x, y);
			}
		}


	}

	return 0;
}
#include <iostream>
#include <cassert>
#include <vector>
#include <limits>
#include <algorithm> 
#include <queue>
#include <set>
#include <utility>
#include <cmath>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/graph_traits.hpp>
#include <boost/graph/bipartite.hpp>
#include <boost/graph/connected_components.hpp>

#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/basic.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Point_set_2.h>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property> Graph;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::out_edge_iterator Edge_iterator;

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Circle_2 Circle;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Point_set_2<K, Tds> PSet;
typedef CGAL::Point_set_2<K, Tds>::Vertex_handle Vertex_handle;

int main() {
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);
	cout << fixed << setprecision(0);

	int test_count;
	cin >> test_count;

	for(int test = 0; test < test_count; test++) {
		int station_count, clue_count;
		K::FT range;
		cin >> station_count >> clue_count >> range;

		range = pow(range, 2); // work with squared distances

		// read in station location
		// pair.first: the point where the station is located
		// pair.second: the index of the station
		vector<pair<Point, int> > stations(station_count);
		for(int station_index = 0; station_index < station_count; station_index++) {
			int x, y;
			cin >> x >> y;

			stations.at(station_index) = make_pair(Point(x, y), station_index);
		}

		// read in clue location
		vector<pair<Point, Point> > clues(clue_count);
		for(int clue_index = 0; clue_index < clue_count; clue_index++) {
			// read in from where to where we would like to go
			int start_x, start_y, target_x, target_y;
			cin >> start_x >> start_y >> target_x >> target_y;

			clues.at(clue_index) = make_pair(Point(start_x, start_y), Point(target_x, target_y));
		}

		// create a graph that can be used to check for graph properties
		Graph graph(station_count);

		// calculate the point set so we can find the nearest one for a given point and all stations
		// inside a circle of given radius
		PSet station_triang;
		station_triang.insert(stations.begin(), stations.end());

		// go trough stations, search for stations that are inside the radius and process
		// them.
		for(int stationA_index = 0; stationA_index < station_count; stationA_index++) {
			Point stationA = stations.at(stationA_index).first;
			Circle stationA_area = Circle(stationA, range);

			// collect all stations inside the circle
			vector<Vertex_handle> stations_in_range;
			station_triang.range_search(stationA_area, back_inserter(stations_in_range));

			// go trough found stations and make sure that each pair is at least `range` away from each other
			// as long as it's not our stationA one of the pair's stations
			for(int i = 0; i < stations_in_range.size(); i++) {
				for(int j = i + 1; j < stations_in_range.size(); j++) {
					int some_station_1 = stations_in_range.at(i)->info();
					int some_station_2 = stations_in_range.at(j)->info();

					if(some_station_1 != stationA_index && some_station_2 != stationA_index) {
						if(CGAL::squared_distance(stations_in_range.at(i)->point(), stations_in_range.at(j)->point()) <= range) {
							// another pair of stations in reachable station, conflict!
							//cout << "blub" << endl;
							goto not_bipartit;
						}
					}
				}
			}

			// add edges
			for(int i = 0; i < stations_in_range.size(); i++) {
				int to = stations_in_range.at(i)->info();
				if(to != stationA_index) {
					add_edge(stationA_index, to, graph);
				}
			}
		}

		// check if graph is not bipartit, if so, we can stop
		if(!is_bipartite(graph)) {
			//cout << "no blub" << endl;
			not_bipartit:
			for(int clue_index = 0; clue_index < clue_count; clue_index++) {
				cout << "n";
			}
			cout << endl;
		} else {
			// now we have to check which of the clues can be received
			// get connected components information, so we can find out if start and end of a clue is in the same
			// component and therefore reachable
			vector<int> in_component(station_count);
			connected_components(graph, &in_component[0]);

			for(int clue_index = 0; clue_index < clue_count; clue_index++) {
				// get where the clue starts and should end
				Point start = clues.at(clue_index).first;
				Point target = clues.at(clue_index).second;

				// check if start and target of the clue are near each other and communicate directly
				if(range >= CGAL::squared_distance(start, target)) {
					cout << "y";
					continue;
				}

				// get nearest vertex to the start and check if we can reach it
				Vertex_handle near_start_station = station_triang.nearest_vertex(start);
				if(range < CGAL::squared_distance(start, near_start_station->point())) {
					cout << "n"; // next station from start too far away
					continue;
				}

				// now the same for the end station/clue location
				Vertex_handle near_end_station = station_triang.nearest_vertex(target);
				if(range < CGAL::squared_distance(target, near_end_station->point())) {
					cout << "n";
					continue;
				}

				// last part is to check if both are in the same component
				if(in_component.at(near_start_station->info()) == in_component.at(near_end_station->info())) {
					cout << "y";
				} else {
					cout << "n";
				}
			}

			cout << endl;
		}
	}

	return 0;
}
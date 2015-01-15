#include <iostream>
#include <vector>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <algorithm>

#include <boost/tuple/tuple.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS> Traits;

typedef adjacency_list<vecS, vecS, directedS, no_property,
                            property<edge_capacity_t, long,
                            property<edge_residual_capacity_t, long,
                            property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;

typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef graph_traits<Graph>::edge_descriptor Edge;

int main() {
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);

	int test_count;
	cin >> test_count;
	for(int test = 0; test < test_count; test++) {
		int location_count, path_count;
		cin >> location_count >> path_count;

		// create graph
		Graph graph(location_count + 2); // don't forget source and sink
		EdgeCapacityMap capacity = get(edge_capacity, graph);
		ReverseEdgeMap rev_edge = get(edge_reverse, graph);
		ResidualCapacityMap res_capacity = get(edge_residual_capacity, graph);
		
		// define source and sink vertex
		int source = location_count;
		int sink = location_count + 1;

		// holds locations, first element of pair is the amount of soldier it has and
		// the seond the amount it needs
		vector<pair<int, int> > locations(location_count);
		for(int location_index = 0; location_index < location_count; location_index++) {
			int has_soldiers, needs_soliders;
			cin >> has_soldiers >> needs_soliders;

			locations.at(location_index) = make_pair(has_soldiers, needs_soliders);
		}

		// read in paths for soldiers
		for(int path_index = 0; path_index < path_count; path_index++) {
			int from, to, min_soldiers, max_soldiers;

			cin >> from >> to >> min_soldiers >> max_soldiers;

			// modify how many soldiers a location needs
			// it needs more soldiers as some of them must move on to the next city
			locations.at(from).second += min_soldiers;
			// needs less soldiers, as it will for sure get some from the current path!
			locations.at(to).second -= min_soldiers;

			// add edge
			Edge edge, r_edge;
			tie(edge, tuples::ignore) = add_edge(from, to, graph);
			tie(r_edge, tuples::ignore) = add_edge(to, from, graph);
			// we already basically moved the minimum amount of soldiers above by modifieng the locations
			// we therefore are interested in the rest that might move over the path and can compare the resulting
			// flow with the expected sum of soldiers
			capacity[edge] = max_soldiers - min_soldiers;
			capacity[r_edge] = 0;
			rev_edge[edge] = r_edge;
			rev_edge[r_edge] = edge;
		}

		// add edges from the source to the city, with its "have soldiers" weights
		for(int location_index = 0; location_index < location_count; location_index++) {
			Edge edge, r_edge;
			tie(edge, tuples::ignore) = add_edge(source, location_index, graph);
			tie(r_edge, tuples::ignore) = add_edge(location_index, source, graph);
			capacity[edge] = locations.at(location_index).first;
			capacity[r_edge] = 0;
			rev_edge[edge] = r_edge;
			rev_edge[r_edge] = edge;
		}

		// add edges from the city to the sink with the city's "needs soldiers" weights
		// also keep track how much we need in total
		int need_total = 0;
		for(int location_index = 0; location_index < location_count; location_index++) {
			int needs = locations.at(location_index).second;
			if(needs <= 0) {
				continue;
			}

			Edge edge, r_edge;
			tie(edge, tuples::ignore) = add_edge(location_index, sink, graph);
			tie(r_edge, tuples::ignore) = add_edge(sink, location_index, graph);
			capacity[edge] = needs;
			capacity[r_edge] = 0;
			rev_edge[edge] = r_edge;
			rev_edge[r_edge] = edge;

			need_total += needs;
		}

		// do max flow
		int max = push_relabel_max_flow(graph, source, sink);

		// check if it corresponds to the expected amount (at least)
		if(max >= need_total) {
			cout << "yes" << endl;
		} else {
			cout << "no" << endl;
		}

	}

	return 0;
}
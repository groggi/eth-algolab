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

void addEdge(int from, int to, int weight, Graph& graph, EdgeCapacityMap& capacity, ReverseEdgeMap& rev_edge) {
	Edge edge, r_edge;
	tie(edge, tuples::ignore) = add_edge(from, to, graph);
	tie(r_edge, tuples::ignore) = add_edge(to, from, graph);
	capacity[edge] = weight;
	capacity[r_edge] = 0;
	rev_edge[edge] = r_edge;
	rev_edge[r_edge] = edge;
}

int main() {
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);

	int test_count;
	cin >> test_count;
	for(int test = 0; test < test_count; test++) {
		int width, height, knight_count;
		cin >> width >> height >> knight_count;

		// create graph
		Graph graph(2 * width * height + 2 + knight_count); // don't forget source and sink
		EdgeCapacityMap capacity = get(edge_capacity, graph);
		ReverseEdgeMap rev_edge = get(edge_reverse, graph);
		ResidualCapacityMap res_capacity = get(edge_residual_capacity, graph);

		/* Index
		 * [0 .. knight_count - 1]: knights
		 * [knight_count .. knight_count + width * height - 1]: intersections of set 1 with edges to set 2
		 * [knight_count + width * height .. knight_count + 2 * width * height - 1]: intersections of set 2 with edges to set 1
		 */

		// source / sink
		int source = 2 * width * height + knight_count;
		int sink = source + 1;

		// read in knight position
		for(int knight_index = 0; knight_index < knight_count; knight_index++) {
			int x, y;
			cin >> x >> y;

			// source -> sink
			addEdge(source, knight_index, 1, graph, capacity, rev_edge);
			//cout << "source -> " << knight_index << endl;

			// knight -> starting position in first intersection set
			addEdge(knight_index, knight_count + x + y * width, 1, graph, capacity, rev_edge);
			//cout << knight_index << " -> " << knight_count + x + y * width << endl;
		}

		// add missing edges
		for(int edge_x = 0; edge_x < width; edge_x++) {
			for(int edge_y = 0; edge_y < height; edge_y++) {
				int intersection_offset = edge_x + edge_y * width;

				// edge from first intersection set to second one
				addEdge(knight_count + intersection_offset, knight_count + width * height + intersection_offset, 1, graph, capacity, rev_edge);
				//cout << "first set to second set: " <<  intersection_offset << " -> " << intersection_offset << endl;

				// edge from second intersection set to all in set one that can be reached by a path of length 1 in the cave
				if(edge_x - 1 >= 0) {
					addEdge(knight_count + width * height + intersection_offset, knight_count + edge_x - 1 + edge_y * width, 1, graph, capacity, rev_edge);
					//cout << "A second set to first set: " << intersection_offset << " -> " << edge_x - 1 + edge_y * width << endl;
				}

				if(edge_x + 1 < width) {
					addEdge(knight_count + width * height + intersection_offset, knight_count + edge_x + 1 + edge_y * width, 1, graph, capacity, rev_edge);
					//cout << "B second set to first set: " << intersection_offset << " -> " << edge_x + 1 + edge_y * width << endl;
				}

				if(edge_y - 1 >= 0) {
					addEdge(knight_count + width * height + intersection_offset, knight_count + edge_x + (edge_y - 1) * width, 1, graph, capacity, rev_edge);
					//cout << "C second set to first set: " << intersection_offset << " -> " << edge_x + (edge_y - 1) * width << endl;
				}

				if(edge_y + 1 < height) {
					addEdge(knight_count + width * height + intersection_offset, knight_count + edge_x + (edge_y + 1) * width, 1, graph, capacity, rev_edge);
					//cout << "D second set to first set: " << intersection_offset << " -> " << edge_x + (edge_y + 1) * width << endl;
				}

				// add sink edge to second intersection set
				if(edge_x == 0 || edge_x + 1 == width || edge_y == 0 || edge_y + 1 == height) {
					addEdge(knight_count + width * height + intersection_offset, sink, 1, graph, capacity, rev_edge);
					//cout << knight_count + intersection_offset << " -> sink" << endl;
				}
			}
		}

		// do max flow
		int max = push_relabel_max_flow(graph, source, sink);

		cout << max << endl;

	}

	return 0;
}
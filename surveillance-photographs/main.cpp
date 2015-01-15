#include <iostream>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/tuple/tuple.hpp>

using namespace std;
using namespace boost;

typedef	adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
	property<edge_capacity_t, long,
		property<edge_residual_capacity_t, long,
			property<edge_reverse_t, Traits::edge_descriptor> > > >	Graph;
typedef	property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef	property_map<Graph, edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;
typedef	graph_traits<Graph>::vertex_descriptor Vertex;
typedef	graph_traits<Graph>::edge_descriptor Edge;



// Custom add_edge, also creates reverse edges with corresponding capacities.
void addEdge(int u, int v, long c, EdgeCapacityMap &capacity, ReverseEdgeMap &rev_edge, Graph &G) {
	Edge e, reverseE;
	tie(e, tuples::ignore) = add_edge(u, v, G);
	tie(reverseE, tuples::ignore) = add_edge(v, u, G);
	capacity[e] = c;
	capacity[reverseE] = 0;
	rev_edge[e] = reverseE;
	rev_edge[reverseE] = e;
}


int main() {
	cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int test = 0; test < test_cases; test++) {
        int intersection_count, street_count, station_count, photograph_count;
        cin >> intersection_count >> street_count >> station_count >> photograph_count;

        // create graph and additional stuff
        // we need a graph that can hold a sink, a source ant twice the citie's map
        Graph graph(2 * intersection_count + 2);
        EdgeCapacityMap capacity = get(edge_capacity, graph);
        ReverseEdgeMap  rev_edge = get(edge_reverse, graph);
        // not used: ResidualCapacityMap res_capacity = get(edge_residual_capacity, graph);
        int source = 2 * intersection_count;
        int sink = 2 * intersection_count + 1;


        // read in where police stations are located and connect them to source/sink
        for(int station_index = 0; station_index < station_count; station_index++) {
            int station_loc;
            cin >> station_loc;

            // source to police station with weight 1 as each station has only one officer
            addEdge(source, station_loc, 1, capacity, rev_edge, graph);

            // police station to sink with weight 1 as each station can only hold one photograph,
            // ATTENTION here police station is in the second set!
            addEdge(intersection_count + station_loc, sink, 1, capacity, rev_edge, graph);
        }

        // read where photographs are stored and connect this location from our first set (where policemen reach the location)
        // to our second set (where policement can only use a street once to get beck to a station)
        for(int photo_index = 0; photo_index < photograph_count; photo_index++) {
            int photo_loc;
            cin >> photo_loc;

            addEdge(photo_loc, intersection_count + photo_loc, 1, capacity, rev_edge, graph);
        }

        // read where the streets are
        for(int street_index = 0; street_index < street_count; street_index++) {
            int from, to;
            cin >> from >> to;

            // first add street with unbound traffic to the first set, as all policemen are free to move multiple times
            // through the same street without a photograph
            addEdge(from, to, numeric_limits<int>::max(), capacity, rev_edge, graph);

            // now we add the same street, but it can be used only once as now it is used by policemen
            // with photographs
            addEdge(from + intersection_count, to + intersection_count, 1, capacity, rev_edge, graph);
        }

    	long flowValue = push_relabel_max_flow(graph, source, sink);
    	
    	cout <<flowValue<< endl;
    }

	return 0;
}
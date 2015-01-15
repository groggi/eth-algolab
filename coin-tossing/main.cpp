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
typedef	property_map<Graph, edge_capacity_t>::type		EdgeCapacityMap;
typedef	property_map<Graph, edge_residual_capacity_t>::type	ResidualCapacityMap;
typedef	property_map<Graph, edge_reverse_t>::type		ReverseEdgeMap;
typedef	graph_traits<Graph>::vertex_descriptor			Vertex;
typedef	graph_traits<Graph>::edge_descriptor			Edge;



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

    for(int i = 0; i < test_cases; i++) {
    	int players, rounds;
    	cin >> players >> rounds;

    	// create graph and additional stuff
    	Graph graph(players + rounds + 2);
    	EdgeCapacityMap	capacity = get(edge_capacity, graph);
		ReverseEdgeMap	rev_edge = get(edge_reverse, graph);
		ResidualCapacityMap	res_capacity = get(edge_residual_capacity, graph);

		// source ans target vertex
		int source = players + rounds;
		int target = players + rounds + 1;

    	for(int roundIndex = 0; roundIndex < rounds; roundIndex++) {
    		int playerA, playerB, outcome;
    		cin >> playerA >> playerB >> outcome;

    		int roundVertex = players + roundIndex;
    		// source -> round vertex
    		addEdge(source, roundVertex, 1, capacity, rev_edge, graph);

    		// round vertex -> player vertex representing possible wins
    		if(outcome == 0) {
    			// both might have won
    			addEdge(roundVertex, playerA, 1, capacity, rev_edge, graph);
    			addEdge(roundVertex, playerB, 1, capacity, rev_edge, graph);
    		} else if(outcome == 1) {
    			// player A won
    			addEdge(roundVertex, playerA, 1, capacity, rev_edge, graph);
    		} else if(outcome == 2) {
    			// player B won
    			addEdge(roundVertex, playerB, 1, capacity, rev_edge, graph);
    		}
    	}

    	int totalPoints = 0;
    	for(int playerIndex = 0; playerIndex < players; playerIndex++) {
    		int points;
    		cin >> points;

    		totalPoints += points;

    		// player vertex -> target
    		addEdge(playerIndex, target, points, capacity, rev_edge, graph);
    	}

    	long flowValue = push_relabel_max_flow(graph, source, target);
    	
    	if(flowValue == totalPoints && totalPoints == rounds) {
    		cout << "yes" << endl;
    	} else {
    		cout << "no" << endl;
    	}
    }

	return 0;
}
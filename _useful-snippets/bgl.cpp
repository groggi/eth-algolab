#include <climits>
#include <iostream>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/tuple/tuple.hpp> // tuples::ignore

// BGL algo specific includes ...
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/graph/strong_components.hpp>
// .. end

using namespace std;
using namespace boost;

// Directed graph with int weights on edges.
typedef adjacency_list<vecS, vecS, directedS, no_property, property<edge_weight_t, int> > Graph;

// ... or one with multiple properties for some fancy algorithm
typedef adjacency_list<vecS, vecS, directedS, property<vertex_name_t, string,
	property<vertex_distance_t, int> > > Graph;

	// and don't forget:
	typedef property_map<Graph, vertex_name_t>::type NameMap;

// Edge type (edge descriptor in BGL speak).
typedef graph_traits<Graph>::edge_descriptor Edge;

// Edge iterator.
typedef graph_traits<Graph>::edge_iterator EdgeIterator;

// Out Edge iterator (directed graph)
typedef graph_traits<Graph>::out_edge_iterator OutEdgeIterator;

// Map edge -> weight.
typedef property_map<Graph, edge_weight_t>::type WeightMap;

// ...

void main() {
	// create graph with `n` vertices
	Graph graph(n);

	// accessing a property map
	NameMap name_map = get(vertex_name, graph_instance);

	// iterate over all outgoing (directed graph) edges
	OutEdgeIterator out_edge_iterator, out_edge_end;
	for(tie(out_edge_iterator, out_edge_end) = out_edges(some_vertices, graph);
		out_edge_iterator != out_edge_end;
		++out_edge_iterator) {
		// get the other end's vertex
		int other_vertex = target(*out_edge_iterator, graph);
	}
}
#include <iostream>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/breadth_first_search.hpp>
#include <boost/graph/depth_first_search.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property> Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

class BFSVisitorClass: public default_bfs_visitor {
public:
	BFSVisitorClass() {
		cout << "here do some initialisation before passing to breadth_first_search()" << endl;
	}

	void initialize_vertex(Vertex v, const Graph& graph) {
		cout << "initialize_vertex() for vertex: " << v << endl;
	}

	void discover_vertex(Vertex v, const Graph& graph) {
		cout << "discover_vertex() for vertex: " << v << endl;
	}

	// this is called right before all edges are taken and tree_edge() calls done
	void examine_vertex(Vertex v, const Graph& graph) {
		cout << "examine_vertex() for vertex: " << v << endl;
	}

	void examine_edge(Edge e, const Graph& graph) {
		cout << "examine_edge() for edge: " << e << endl;
	}

	// this one is interesting if you want to know the visited/followed edges
	// this edge forms the search tree
	void tree_edge(Edge e, const Graph& graph) {
		cout << "tree_edge() for edge: " << e << endl;
	}

	void non_tree_edge(Edge e, const Graph& graph) {
		cout << "non_tree_edge() for edge: " << e << endl;
	}

	void gray_target(Edge e, const Graph& graph) {
		cout << "gray_target() for edge: " << e << endl;
	}

	void black_target(Edge e, const Graph& graph) {
		cout << "black_target() for edge: " << e << endl;
	}

	void finish_vertex(Vertex v, const Graph& graph) {
		cout << "finish_vertex() for vertex: " << v << endl;
	}
};

class DFSVisitorClass: public default_dfs_visitor {
public:
	DFSVisitorClass() {
		cout << "here do some initialisation before passing to depth_first_search()" << endl;
	}

	void initialize_vertex(Vertex v, const Graph& graph) {
		cout << "initialize_vertex() for vertex: " << v << endl;
	}

	void start_vertex(Vertex v, const Graph& graph) {
		cout << "start_vertex() for vertex: " << v << endl;
	}

	void discover_vertex(Vertex v, const Graph& graph) {
		cout << "discover_vertex() for vertex: " << v << endl;
	}

	void examine_edge(Edge e, const Graph& graph) {
		cout << "examine_edge() for edge: " << e << endl;
	}

	void tree_edge(Edge e, const Graph& graph) {
		cout << "tree_edge() for edge: " << e << endl;
	}

	void back_edge(Edge e, const Graph& graph) {
		cout << "back_edge() for edge: " << e << endl;
	}

	void forward_or_cross_edge(Edge e, const Graph& graph) {
		cout << "forward_or_cross_edge() for edge: " << e << endl;
	}

	void finish_vertex(Vertex v, const Graph& graph) {
		cout << "finish_vertex() for vertex: " << v << endl;
	}
};

int main() {
	/*
                0
               / \
              /   \
             1     2
            /\     |
           /  \    |
          3   4    5
              |
              6
    */

	Graph graph(8);
	add_edge(0, 1, graph);
	add_edge(1, 3, graph);
	add_edge(1, 4, graph);
	add_edge(2, 5, graph);
	add_edge(4, 6, graph);

	cout << "Depth First Search:" << endl;
	depth_first_search(graph, visitor(DFSVisitorClass()));
	cout << endl << endl;

	cout << "Breath First Search" << endl;
	breadth_first_search(graph, 0, visitor(BFSVisitorClass()));
	//                          ^- source to start from

	return 0;
}
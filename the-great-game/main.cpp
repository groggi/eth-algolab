#include <iostream>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>
#include <boost/tuple/tuple.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list < listS, vecS, directedS, no_property, property < edge_weight_t, int > > Graph;
//typedef graph_traits < graph_t >::vertex_descriptor vertex_descriptor;
//typedef graph_traits < graph_t >::edge_descriptor edge_descriptor;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;


int main() {
	cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int i = 0; i < test_cases; i++) {
        // read in basic information
        int position_count, transition_count;
        cin >> position_count >> transition_count;

        int start_red, start_black;
        cin >> start_red >> start_black;
        start_red -= 1; start_black -= 1; // let it start with 0, not 1

        // create graph
        // source and sink are defined by player start and the target location, no additional nodes therefore needed
        Graph graph(position_count);
        property_map<Graph, edge_weight_t>::type weightMap = get(edge_weight, graph);

        for(int transition_index = 0; transition_index < transition_count; transition_index++) {
            int from, to;
            cin >> from >> to;
            from -= 1; to -= 1; // let it start at 0

            // create edge representing the transition
            Edge edge;
            tie(edge, tuples::ignore) = add_edge(from, to, graph);
            weightMap[edge] = 1;
        }

        // calculate shortest paths
        vector<Vertex> predecessorsR(num_vertices(graph));
        vector<int> distancesR(num_vertices(graph));
        dijkstra_shortest_paths(graph, start_red, &predecessorsR[0], &distancesR[0], get(edge_weight, graph), get(vertex_index, graph), less<int>(), plus<int>(), (numeric_limits<int>::max)(), 0, default_dijkstra_visitor());
        int red_len = distancesR[position_count - 1];

        vector<Vertex> predecessorsB(num_vertices(graph));
        vector<int> distancesB(num_vertices(graph));
        dijkstra_shortest_paths(graph, start_black, &predecessorsB[0], &distancesB[0], get(edge_weight, graph), get(vertex_index, graph), less<int>(), plus<int>(), (numeric_limits<int>::max)(), 0, default_dijkstra_visitor());
        int black_len = distancesB[position_count - 1];

        if(red_len >= black_len) {
            cout << "1" << endl;
        } else {
            cout << "0" << endl;
        }

//        cout << "red: " << red_len << ", black: " << black_len << endl;

    }

	return 0;
}
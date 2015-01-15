//
//
// WRONG WRONG WRONG :-(
//
//

#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cmath>
#include <climits>
#include <algorithm>
#include <climits>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/kruskal_min_spanning_tree.hpp>
#include <boost/graph/dijkstra_shortest_paths.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_weight_t, int> > Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

int main(void)
{
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int i = 0; i < test_cases; i++) {
        int vertices, edges;
        cin >> vertices;
        cin >> edges;

        Graph graph(vertices);
        property_map<Graph, edge_weight_t>::type weightMap = get(edge_weight, graph);

        for(int j = 0; j < edges; j++) {
            int from, to, weight;
            cin >> from;
            cin >> to;
            cin >> weight;

            // add edge
            bool success;
            Edge edge;
            tie(edge, success) = add_edge(from, to, graph);
            // set weight
            weightMap[edge] = weight;
        }

        // find minimum spanning tree
        vector<Edge> spanning_tree;
        kruskal_minimum_spanning_tree(graph, back_inserter(spanning_tree));

        int sum_spanning_tree_weights = 0;
        for(vector<Edge>::iterator spanning_tree_edge = spanning_tree.begin(); spanning_tree_edge != spanning_tree.end(); ++spanning_tree_edge) {
            //cout << source(*spanning_tree_edge, graph) << " <--> " << target(*spanning_tree_edge, graph) << " with weight: " << weightMap[*spanning_tree_edge] << endl; 
            sum_spanning_tree_weights += weightMap[*spanning_tree_edge];
        }

        // find longest path
        vector<Vertex> predecessors(num_vertices(graph));
        vector<int> distances(num_vertices(graph));
        dijkstra_shortest_paths(graph, 0, &predecessors[0], &distances[0], get(edge_weight, graph), get(vertex_index, graph), less<int>(), plus<int>(), (numeric_limits<int>::max)(), 0, default_dijkstra_visitor());

        int sum_farthest_away = *max_element(distances.begin(), distances.end());

        cout << sum_spanning_tree_weights << " " << sum_farthest_away << endl;
    }

    return 0;
}
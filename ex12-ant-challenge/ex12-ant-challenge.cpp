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

typedef adjacency_list<vecS, vecS, undirectedS, no_property, property<edge_index_t, int> > Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

int main(void)
{
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int i = 0; i < test_cases; i++) {
        int vertices, edges, species, start, end;
        cin >> vertices;
        cin >> edges;
        cin >> species;
        cin >> start;
        cin >> end;

        Graph graph(vertices);
        property_map<Graph, edge_index_t>::type indices = get(edge_index, graph);

        vector<vector<int > > weights(species, vector<int>(edges, -1));

        for(int j = 0; j < edges; j++) {
            int from, to;
            cin >> from;
            cin >> to;

            // add edge
            bool success;
            Edge edge;
            tie(edge, success) = add_edge(from, to, graph);
            indices[edge] = j;

            for(int k = 0; k < species; k++) {
                int species_weight;
                cin >> species_weight;

                // set weight
                weights[k][j] = species_weight;
            }
        }

        // no reason to know where the hives are...
        int ignore;
        for(int k = 0; k < species; k++) {
            cin >> ignore;
        }

        // find minimum spanning for each species
        vector<int> spanning_tree_weights(edges, numeric_limits<int>::max());
        for(int k = 0; k < species; k++) {
            vector<Edge> spanning_tree;
            kruskal_minimum_spanning_tree(graph, back_inserter(spanning_tree), weight_map(make_iterator_property_map(weights[k].begin(), indices)));

            for(vector<Edge>::iterator spanning_tree_edge = spanning_tree.begin(); spanning_tree_edge != spanning_tree.end(); ++spanning_tree_edge) {
                if(weights[k][indices[*spanning_tree_edge]] < spanning_tree_weights[indices[*spanning_tree_edge]]) {
                    spanning_tree_weights[indices[*spanning_tree_edge]] = weights[k][indices[*spanning_tree_edge]];
                }
            }
        }

        // now we have a minimal spanning tree we can use to find the shortest path from start to end
        vector<Vertex> predecessors(num_vertices(graph));
        vector<int> distances(num_vertices(graph));
        dijkstra_shortest_paths(graph, start, predecessor_map(&predecessors[0]).distance_map(&distances[0]).weight_map(make_iterator_property_map(&spanning_tree_weights[0], indices)));

        // ok, now we can read out the path length
        cout << distances[end] << endl;
    }

    return 0;
}
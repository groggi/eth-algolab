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
#include <boost/graph/biconnected_components.hpp>

#define ISLAND 1
#define BRIDGE 2

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property> Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

int main(void)
{
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int i = 0; i < test_cases; i++) {
        // read test case
        int islands, bridges;
        cin >> islands;
        cin >> bridges;

        // create graph for test case
        Graph graph(islands + bridges);

        // keep track which vertex is what
        vector<int> vertex_type(islands + bridges, -1);

        // keep track of bridges trough island pairs
        vector<pair<int, int> > bridge_to_island(bridges, make_pair(-1, -1));

        // set vertex properties
        for(int bridge_vertex = 0; bridge_vertex < bridges; bridge_vertex++) {
            assert(vertex_type.at(bridge_vertex) == -1);
            vertex_type.at(bridge_vertex) = BRIDGE;
        }

        for(int island_vertex = bridges + 1; island_vertex < bridges+islands; island_vertex++) {
            assert(vertex_type.at(island_vertex) == -1);
            vertex_type.at(island_vertex) = ISLAND;
        }

        // read in bridges
        for(int k = 0; k < bridges; k++) {
            int from_island, to_island;
            cin >> from_island;
            cin >> to_island;

            // each bridge is also a vertices and connects both islands (also vertices) through edges
            // calculate vertices indexes first
            int from_island_index = bridges + from_island - 1; // input starts at 1, we start at 0
            int to_island_index = bridges + to_island - 1; // input starts at 1, we start at 0
            int bridge_index = k;

            // add edges:
            //         bridge
            //         /    \
            // from island  to island
            bool success;
            Edge edge;
            tie(edge, success) = add_edge(from_island_index, bridge_index, graph);
            tie(edge, success) = add_edge(to_island_index, bridge_index, graph);

            // keep track of islands the bridge connects
            bridge_to_island.at(bridge_index) = make_pair(from_island, to_island);
        }

        // find the important bridges
        vector<Vertex> vertices;
        articulation_points(graph, back_inserter(vertices));

        set<pair<int, int>> out_bridges;
        for(vector<Vertex>::iterator v2 = vertices.begin(); v2 != vertices.end(); ++v2) {
            //cout << "type: " << vertex_type[*v2] << " edge nr: " << *v2 << endl;
            // only if articulation point is a bridge, we're interested because it's an important bridge!
            if(vertex_type[*v2] == BRIDGE) {
                pair<int, int> islands_connected_by_important_bridge = bridge_to_island.at(*v2);
                out_bridges.insert(make_pair(min(islands_connected_by_important_bridge.first, islands_connected_by_important_bridge.second), max(islands_connected_by_important_bridge.first, islands_connected_by_important_bridge.second)));
            }
        }

        /*// print out the edges, sorted order
        set<pair<int, int>> out_bridges;
        for(vector<Vertex>::iterator v2 = vertices.begin(); v2 != vertices.end(); ++v2) {
            //cout << "city: " << *v2 << endl;
            //sort(island_to_bridge[*v2].begin(), island_to_bridge[*v2].end());
            for(vector<pair<int, int> >::iterator bridge_islands = island_to_bridge[*v2].begin(); bridge_islands != island_to_bridge[*v2].end(); ++bridge_islands) {
                //cout << (*bridge_islands).first << " " << (*bridge_islands).second << endl;
                out_bridges.insert(make_pair(min((*bridge_islands).first, (*bridge_islands).second), max((*bridge_islands).first, (*bridge_islands).second)));
            }
        }*/

        cout << out_bridges.size() << endl;
        for(pair<int, int> bout : out_bridges) {
            cout << bout.first << " " << bout.second << endl;
        }
    }

    return 0;
}
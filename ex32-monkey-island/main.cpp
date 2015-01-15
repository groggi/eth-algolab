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
#include <boost/graph/strong_components.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, directedS, no_property, no_property> Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;
typedef graph_traits<Graph>::edge_iterator EdgeIterator;

int main(void)
{
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int i = 0; i < test_cases; i++) {
        int location_count, road_count;
        cin >> location_count >> road_count;

        // create graph
        Graph graph(location_count);

        // read in directed roads and create edges for them
        for(int j = 0; j < road_count; j++) {
            // read vertices information
            int from, to;
            cin >> from >> to;

            // locations start at 1, inside our graph at 0
            from -= 1;
            to -= 1;

            // create edge
            add_edge(from, to, graph);
        }

        // read in costs for building police station at a vertex
        vector<int> costs(location_count);
        for(int j= 0; j < location_count; j++) {
            cin >> costs.at(j);
            //cout << "-> " << costs.at(j) << endl;;
        }

        // retrieve strong components (maximum set of vertices for which we have a path from any pair in this set).
        vector<int> scc(location_count); // each vertex is labeled with an int representing the set the vertex belongs to (to which strong component set)
        int scc_count = strong_components(graph, &scc[0]);

        // records which strong component set has an incoming edge from an other strong component set
        vector<bool> incoming(scc_count, false);

        // iterate over all graph edges
        EdgeIterator edge_iter, edge_end;
        for(tie(edge_iter, edge_end) = edges(graph); edge_iter != edge_end; ++edge_iter) {
            // get vertices connected by the edge
            int from_vertex = source(*edge_iter, graph);
            int to_vertex = target(*edge_iter, graph);

            // check that both vertices are not part of the same strong component
            if(scc[from_vertex] != scc[to_vertex]) {
                // ok, so both vertices don't have edges in both directions.
                // furthermore we have a *directed* edge from the strong component set of from_vertex to the one of to_vertex, record that we have an incoming
                // edge.
                incoming[scc[to_vertex]] = true;
            }
        }

        // so, now we know which strong component set has incoming edges from other sets. Therefore we don't have to build a police station inside a strong component
        // set that has an incoming edge! Reason: we can build one in the strong component set that has an edge to the other set and with that we reach all vertices in
        // both strong component sets.

        // keeps track of minimum costs needed
        vector<int> min_costs(scc_count, numeric_limits<int>::max());

        // iterate over all locations
        for(int location_index = 0; location_index < location_count; location_index++) {
            if(!incoming[scc[location_index]]) {
                // the strong component set the location belongs to has *no* incoming edge.

                // assign the minimum cost for the strong component set the current location belongs to is defined
                // by the smaller number from the set {already known minimum, cost to build in the current location a police station}
                //cout << "min_costs = " << min(min_costs[scc[location_index]], costs[location_index]) << " ( " << min_costs[scc[location_index]] << " or " << costs[location_index] << " )" << endl;
                min_costs[scc[location_index]] = min(min_costs[scc[location_index]], costs[location_index]);
            }
        }

        // sum up the costs for all police stations, jumping over strong component sets with incoming edges!
        int cost_sum = 0;
        for(int scc_index = 0; scc_index < scc_count; scc_index++) {
            if(!incoming[scc_index]) {
                //cout << "==> " << cost_sum << " + " << min_costs[scc_index] << " = ";
                cost_sum += min_costs[scc_index];
                //cout << cost_sum << endl;
            }
        }

        // out with result
        cout << cost_sum << endl;
    }

    return 0;
}
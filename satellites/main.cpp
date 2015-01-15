#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cmath>
#include <climits>
#include <algorithm>
#include <climits>
#include <string>

#include <boost/config.hpp>
#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/max_cardinality_matching.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list<vecS, vecS, undirectedS> Graph;
typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;


// code given
typedef graph_traits<Graph>::out_edge_iterator  OutEdgeIt;

void DFS(int u, Graph &G, vector<bool> &visited, vector<Vertex> &mate) {
    OutEdgeIt ebeg, eend;
    visited[u] = true;
    for (tie(ebeg, eend) = out_edges(u, G); ebeg != eend; ++ebeg) {
        const int v = target(*ebeg, G);
        //  v not vis.  && left to right with Non-Matching edges
        //             right to left with Matching edges
        if (!visited[v] && (((v == mate[u]) != (u < v)))) {
            DFS(v, G, visited, mate);
        }
    }
}
// END code given

int main(void)
{
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int i = 0; i < test_cases; i++) {
        // read test case
        int ground_count, satellite_count, link_count;
        cin >> ground_count >> satellite_count >> link_count;

        // create bipartit graph
        Graph graph(ground_count + satellite_count);

        // read links and create edges for them
        for(int li = 0; li < link_count; li++) {
            int ground_index, satellite_index;
            cin >> ground_index >> satellite_index;

            bool success;
            Edge edge;
            tie(edge, success) = add_edge(ground_index, satellite_index + ground_count, graph);
        }

        // find maximum matching
        vector<Vertex> mate(ground_count + satellite_count);
        checked_edmonds_maximum_cardinality_matching(graph, &mate[0]);

        // keep track of visited vertices
        vector<bool> visited(ground_count + satellite_count);

// code given
        // starting points for VC
        vector<int> startpoints;
        //cout << "Matching:" << endl;
        for (int i = 0; i < ground_count + satellite_count; ++i) {
            // output the matching
            if (mate[i] == graph_traits<Graph>::null_vertex() && i < ground_count) {
                startpoints.push_back(i);
            }
        }

        // run depth first visit
        for (int i = 0; i < startpoints.size(); ++i) {
            DFS(startpoints[i], graph, visited, mate);
        }
// END code given

        // collect unmarked
        vector<int> out_ground_ids;
        vector<int> out_satellite_ids;
        for(int index = 0; index < ground_count + satellite_count; index++) {
            if(index < ground_count && !visited[index]) {
                //cout << "ground ID: " << index << endl;
                out_ground_ids.push_back(index);
            } else if(index >= ground_count && visited[index]) {
                //cout << "satellite ID: " << index - ground_count << endl;
                out_satellite_ids.push_back(index - ground_count);
            }
        }

        // NOTE: abstand am ende kein Problem! So kompliziertes spaces einfügen nicht noetig :)
        cout << out_ground_ids.size() << " " << out_satellite_ids.size() << endl;
        bool first = true;
        for(int og : out_ground_ids) {
            if(!first) {
                 cout << " ";
            } else {
                first = false;
            }

            cout << og;
        }

        if(!first) {
            cout << " ";
        }

        first = true;
        for(int os : out_satellite_ids) {
            if(!first) {
                 cout << " ";
            } else {
                first = false;
            }

            cout << os;
        }

        cout << endl;
    }

    return 0;
}
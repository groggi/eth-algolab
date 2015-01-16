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
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;

typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, int,
    property<edge_residual_capacity_t, int,
    property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;

typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;

// Custom add_edge, also creates reverse edges with corresponding capacities.
void addEdge(int u, int v, int c, EdgeCapacityMap &capacity, ReverseEdgeMap &rev_edge, Graph &G) {
    Edge e, reverseE;
    tie(e, tuples::ignore) = add_edge(u, v, G);
    tie(reverseE, tuples::ignore) = add_edge(v, u, G);
    capacity[e] = c;
    capacity[reverseE] = 0;
    rev_edge[e] = reverseE;
    rev_edge[reverseE] = e;
}

int main(void)
{
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int test = 0; test < test_cases; test++) {
        int zone_count, job_count;
        cin >> zone_count >> job_count;

        // create graph for flow analysis
        Graph graph(zone_count + job_count + 2);

        // helper variables to find
        int zone_offset = 0;
        int job_offset = zone_count;
        int source = zone_count + job_count;
        int sink = source + 1;

        // get graph's properties
        EdgeCapacityMap capacity = get(edge_capacity, graph);
        ReverseEdgeMap rev_edge = get(edge_reverse, graph);

        // read in the cost for a zone ticket
        for(int zone_index = 0; zone_index < zone_count; zone_index++) {
            int cost;
            cin >> cost;

            addEdge(zone_offset + zone_index, sink, cost, capacity, rev_edge, graph);
        }

        // read in job rewards
        int total_reward = 0;
        for(int job_index = 0; job_index < job_count; job_index++) {
            int reward;
            cin >> reward;

            total_reward += reward;

            addEdge(source, job_offset + job_index, reward, capacity, rev_edge, graph);
        }

        // read in needed zones
        for(int job_index = 0; job_index < job_count; job_index++) {
            int zones_needed_count;
            cin >> zones_needed_count;
            for(int col = 0; col < zones_needed_count; col++) {
                int zone;
                cin >> zone;
                zone--;

                addEdge(job_offset + job_index, zone_offset + zone, numeric_limits<int>::max(), capacity, rev_edge, graph);
            }
        }

        // calculate flow which represents the amount of money we have to spent
        int we_pay = push_relabel_max_flow(graph, source, sink);

        cout << total_reward - we_pay << endl;
        
    }

    return 0;
}
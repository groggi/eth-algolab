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
#include <boost/graph/push_relabel_max_flow.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;

typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, unsigned int,
    property<edge_residual_capacity_t, unsigned int,
    property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;

typedef graph_traits<Graph>::edge_descriptor Edge;
typedef graph_traits<Graph>::vertex_descriptor Vertex;

typedef property_map<Graph, edge_capacity_t>::type EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type ReverseEdgeMap;

int main(void)
{
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int test_index = 0; test_index < test_cases; test_index++) {
        int game_width, brick_count;
        cin >> game_width >> brick_count;

        // define index of source and sink vertex
        int source_index = game_width + 1;
        int sink_index = game_width;

        // create graph, don't forget we need a source and sink too
        Graph graph(game_width * 2 + 1);

        // get graph's properties
        EdgeCapacityMap capacity = get(edge_capacity, graph);
        ReverseEdgeMap rev_edge = get(edge_reverse, graph);
        // not used: ResidualCapacityMap res_capacity = get(edge_residual_capacity, graph);


        // create edges for splits, as only one can exist at any location. So add edges with weight 1
        for(int game_location = 0; game_location < game_width; game_location++) {
            bool new_edge;
            Edge edge, reverse_edge;
            tie(edge, new_edge) = add_edge(game_location, game_width + game_location + 1, graph);
            tie(reverse_edge, new_edge) = add_edge(game_width + game_location + 1, game_location, graph);

            capacity[edge] = 1;
            capacity[reverse_edge] = 0;
            rev_edge[edge] = reverse_edge;
            rev_edge[reverse_edge] = edge;
        }

        // read in widths of blocks
        for(int brick_index = 0; brick_index < brick_count; brick_index++) {
            int start, end;
            cin >> start >> end;

            // make sure we get expected order
            if(start > end) {
                int tmp = end;
                end = start;
                start = tmp;
            }

            // check if we stay inside the game field
            if(end > game_width) {
                continue;
            }

            // add edge for brick
            bool new_edge;
            Edge edge, reverse_edge;
            tie(edge, new_edge) = add_edge(start + game_width + 1, end, graph);
            tie(reverse_edge, new_edge) = add_edge(end, start + game_width + 1, graph);

            capacity[edge] = 1;
            capacity[reverse_edge] = 0;
            rev_edge[edge] = reverse_edge;
            rev_edge[reverse_edge] = edge;
        }

        long max_flow = push_relabel_max_flow(graph, source_index, sink_index);
        cout << max_flow << endl;
    }

    return 0;
}
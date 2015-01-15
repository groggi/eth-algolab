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
    property<edge_capacity_t, int,
    property<edge_residual_capacity_t, int,
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

    int test_count;
    cin >> test_count;

    for(int test_index = 0; test_index < test_count; test_index++) {
        int figure_count, limb_count;
        cin >> figure_count >> limb_count;

        // create graph
        Graph graph(figure_count);

        // get graph's properties
        EdgeCapacityMap capacity = get(edge_capacity, graph);
        ReverseEdgeMap reverse_edge_map = get(edge_reverse, graph);
        ResidualCapacityMap res_capacity = get(edge_residual_capacity, graph);

        // read in limbs
        for(int limb_index = 0; limb_index < limb_count; limb_index++) {
            int from, to, cost;
            cin >> from >> to >> cost;

            Edge edge;
            Edge rev_edge;
            tie(edge, tuples::ignore) = add_edge(from, to, graph);
            tie(rev_edge, tuples::ignore) = add_edge(to, from, graph);
            capacity[edge] = cost;
            capacity[rev_edge] = 0;
            reverse_edge_map[edge] = rev_edge;
            reverse_edge_map[rev_edge] = edge;
        }

        // find bist source and sink
        int best_source = -1;
        int best_sink = -1;
        int best_value = numeric_limits<int>::max();

        // attention: start at 1, otherwise assertion because sink == source
        for(int figure_index = 1; figure_index < figure_count; figure_index++) {
            // search for best sink
            int max_flow = push_relabel_max_flow(graph, 0, figure_index);
            if(max_flow < best_value) {
                best_value = max_flow;
                best_source = 0;
                best_sink = figure_index;
            }

            // search for best source
            max_flow = push_relabel_max_flow(graph, figure_index, 0);
            if(max_flow < best_value) {
                best_value = max_flow;
                best_source = figure_index;
                best_sink = 0;
            }
        }

        // rerun for found best sink and source
        push_relabel_max_flow(graph, best_source, best_sink);

        std::queue<int> Q;
        Q.push(best_source);

        vector<bool> visited(figure_count, false);
        visited.at(best_source) = true;

        while(!Q.empty()) {
            const int figure = Q.front();
            Q.pop();
            graph_traits<Graph>::out_edge_iterator out_iter, out_end;
            for(tie(out_iter, out_end) = out_edges(figure, graph); out_iter != out_end; ++out_iter) {
                const int edge_end_v = target(*out_iter, graph);
                
                if(res_capacity[*out_iter] == 0 || visited[edge_end_v]) {
                    continue;
                }

                visited[edge_end_v] = true;
                Q.push(edge_end_v);
            }
        }

        cout << best_value << endl << count(visited.begin(), visited.end(), true);
        for(int i = 0; i < figure_count; i++) {
            if(visited[i]) {
                cout << " " << i;
            }
        }
        cout << endl;
    }

    return 0;
}
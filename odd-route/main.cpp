#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>

using namespace std;

int main() {
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int test = 0; test < test_cases; test++) {
        // get graph properties
        int vertex_count, edge_count;
        cin >> vertex_count >> edge_count;

        // get start and target vertices
        int start_vertex, target_vertex;
        cin >> start_vertex >> target_vertex;

        // read in vertices
        vector<Vertex> vertices(vertex_count);
        for(int edge_index = 0; edge_index < edge_count; edge_index++) {
            int from, to, weight;
            cin >> from >> to >> weight;

            vertices.at(from).edges.push_back(make_pair(to, weight));
        }

        priority_queue<NextMove, vector<NextMove>, MoveCompare> to_visit;
        to_visit.push(NextMove(start_vertex, 0, 0));

        int shortest_weight_sum = numeric_limits<int>::max();

        while(!to_visit.empty()) {
            NextMove cur_move = to_visit.top();
            to_visit.pop();

            if(cur_move.is_odd_weight()) {
                if(cur_move.is_odd_length()) {
                    // odd weight, odd length
                    if(vertices.at(cur_move.current_vertex).visited_odd_edges_odd_weight) {
                        // next edge was already visited with odd weight and odd length, do not revisit
                        continue;
                    } else {
                        if(cur_move.current_vertex == target_vertex) {
                            // found the end with searched configuration, update shortest weight
                            shortest_weight_sum = cur_move.weight_sum;
                            break;
                        }

                        // visited it now with the configuration
                        vertices[cur_move.current_vertex].visited_odd_edges_odd_weight = true;
                    }
                } else {
                    // odd weight, even length
                    if(vertices.at(cur_move.current_vertex).visited_even_edges_odd_weight) {
                        // already visited with that configuration
                        continue;
                    } else {
                        vertices.at(cur_move.current_vertex).visited_even_edges_odd_weight = true;
                    }
                }
            } else {
                if(cur_move.is_odd_length()) {
                    // even weight, odd length
                    if(vertices.at(cur_move.current_vertex).visited_odd_edges_even_weight) {
                        // already visited with that configuration
                        continue;
                    } else {
                        vertices.at(cur_move.current_vertex).visited_odd_edges_even_weight = true;
                    }
                } else {
                    // even weight, even length
                    if(vertices.at(cur_move.current_vertex).visited_even_edges_even_weight) {
                        // already visited with that configuration
                        continue;
                    } else {
                        vertices.at(cur_move.current_vertex).visited_even_edges_even_weight = true;
                    }
                }
            }

            // ok, if we reach this point, we found an edge that we didn't visit in the current configuration,
            // have to visit it with current configuration
            vector<pair<int, int> >& edges = vertices.at(cur_move.current_vertex).edges;
            for(int next_edge = 0; next_edge < edges.size(); next_edge++) {
                to_visit.push(NextMove(edges.at(next_edge).first, // use the next vertex that can be reached by current vertex
                    cur_move.weight_sum + edges.at(next_edge).second, // add weight of the edge we would follow
                    cur_move.edges_length + 1)); // we use an edge more, wow!
            }
        }

        if(shortest_weight_sum == numeric_limits<int>::max()) {
            cout << "no" << endl;
        } else {
            cout << shortest_weight_sum - 1 << endl;
        }
    }

    return 0;
}
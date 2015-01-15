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
        // clean up
        best_distance.clear();

        // get graph properties
        int vertex_count, edge_count;
        cin >> vertex_count >> edge_count;

        // get start and target vertices
        int start_vertex, target_vertex;
        cin >> start_vertex >> target_vertex;
        start_vertex -= 1; // input starts at 1
        target_vertex -= 1; // input starts at 1

        // read in edges
        vector<vector<int > > edges(vertex_count, vector<int>());
        for(int edge_index = 0; edge_index < edge_count; edge_index++) {
            int from, to;
            cin >> from >> to;
            from -= 1; // input starts at 1
            to -= 1; // input starts at 1

            // create edge
            edges.at(from).push_back(to);
        }

        // keeps track of the second best solution
        int second_best = -1;

        // queue for vertices to visit
        // pair.first: lenth so far
        // pair.second: next vertex
        priority_queue<pair<int, int>, vector<pair<int, int> >, PairCompare> next_moves;
        next_moves.push(make_pair(0, start_vertex));

        // keep track how many times a vertex was reached
        vector<int> visited_counters(vertex_count, 0);
        visited_counters.at(start_vertex) = 1;

        // "The Algorithm" TM
        while(!next_moves.empty()) {
            pair<int, int> cur_move = next_moves.top();
            next_moves.pop();

            // extract information from current vertex we sit on
            int vertex = cur_move.second;
            int length = cur_move.first;

            // iterate over all neighbors
            for(int neighbor_index = 0; neighbor_index < edges.at(vertex).size(); neighbor_index++) {
                int neighbor_vertex = edges.at(vertex).at(neighbor_index);

                if(visited_counters.at(neighbor_vertex) < 2) {
                    // ^- only visit neighbor if it wasn't visited already twice
                    // v- update visited counter
                    visited_counters.at(neighbor_vertex)++;

                    if(neighbor_vertex == target_vertex && visited_counters.at(neighbor_vertex) == 2) {
                        // ok, so we found our target vertex and it was already visited twice (i.e. we visit it right now for the second time)
                        // we can abort early :-D
                        second_best = length + 1;
                        goto _solution; // haha
                    } else {
                        // visit neighbor
                        next_moves.push(make_pair(length + 1, neighbor_vertex));
                    }
                }
            }
        }

    _solution:
        if(second_best == -1) {
            cout << "no" << endl;
        } else {
            cout << second_best << endl;
        }

    }

	return 0;
}
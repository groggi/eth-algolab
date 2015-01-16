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

int main(void)
{
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int test = 0; test < test_cases; test++) {
        int width;
        cin >> width;

        // 2D vector indicating if the field is not a hole
        vector<bool> can_occupy(width * width);
        int hole_count = 0;
        for(int row = 0; row < width; row++) {
            for(int column = 0; column < width; column++) {
                int indicator;
                cin >> indicator;
                
                if(indicator == 1) {
                    can_occupy.at(row + column * width) = true;
                } else {
                    hole_count++;
                    can_occupy.at(row + column * width) = false;
                }
            }
        }

        // create graph for game field
        Graph graph(width * width);

        // create edges
        for(int row = 0; row < width; row++) {
            for(int column = 0; column < width; column++) {
                int field_index = row + column * width;

                if(!can_occupy.at(field_index)) {
                    continue;
                }
                
                // i = row, j = column
                // [i − 1, j − 2]
                if(row - 1 >= 0 && column - 2 >= 0 && can_occupy.at((row - 1) + (column - 2) * width)) {
                    add_edge(field_index, (row - 1) + (column - 2) * width, graph);
                }

                // [i − 1, j + 2]
                if(row - 1 >= 0 && column + 2 < width && can_occupy.at((row - 1) + (column + 2) * width)) {
                    add_edge(field_index, (row - 1) + (column + 2) * width, graph);
                }

                // [i + 1, j − 2]
                if(row + 1 < width && column - 2 >= 0 && can_occupy.at((row + 1) + (column - 2) * width)) {
                    add_edge(field_index, (row + 1) + (column - 2) * width, graph);
                }

                // [i + 1, j + 2]
                if(row + 1 < width && column + 2 < width && can_occupy.at((row + 1) + (column + 2) * width)) {
                    add_edge(field_index, (row + 1) + (column + 2) * width, graph);
                }

                // [i − 2, j − 1]
                if(row - 2 >= 0 && column - 1 >= 0 && can_occupy.at((row - 2) + (column - 1) * width)) {
                    add_edge(field_index, (row - 2) + (column - 1) * width, graph);
                }

                // [i − 2, j + 1]
                if(row - 2 >= 0 && column + 1 < width && can_occupy.at((row - 2) + (column + 1) * width)) {
                    add_edge(field_index, (row - 2) + (column + 1) * width, graph);
                }

                // [i + 2, j − 1]
                if(row + 2 < width && column - 1 >= 0 && can_occupy.at((row + 2) + (column - 1) * width)) {
                    add_edge(field_index, (row + 2) + (column - 1) * width, graph);
                }

                // [i + 2, j + 1]
                if(row + 2 < width && column + 1 < width && can_occupy.at((row + 2) + (column + 1) * width)) {
                    add_edge(field_index, (row + 2) + (column + 1) * width, graph);
                }
            }
        }

        // max cardinality
        vector<Vertex> mate(width * width);
        checked_edmonds_maximum_cardinality_matching(graph, &mate[0]);
        int matches = matching_size(graph, &mate[0]);

        //cout << "-> " << matches << endl;

        cout << ((width * width) - hole_count) - matches << endl;

    }

    return 0;
}
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
#include <boost/graph/boyer_myrvold_planar_test.hpp>

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
        int employee_count, friendship_count;
        cin >> employee_count;
        cin >> friendship_count;

        // create graph for test case
        Graph graph(employee_count + 1);

        // add personal door edge
        for(int k = 0; k < employee_count; k++) {
            bool success;
            Edge edge;
            tie(edge, success) = add_edge(k, employee_count + 1, graph);
        }
        
        // add friendship connections
        for(int k = 0; k < friendship_count; k++) {
            int friend_A, friend_B;
            cin >> friend_A;
            cin >> friend_B;

            bool success;
            Edge edge;
            tie(edge, success) = add_edge(friend_A, friend_B, graph);
        }

        if(boyer_myrvold_planarity_test(graph)) {
            cout << "yes" << endl;
        } else {
            cout << "no" << endl;
        }
    }

    return 0;
}
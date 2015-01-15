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
        int meta_blob_count;
        cin >> meta_blob_count;

        // create graph
        Graph graph(meta_blob_count);

        // read in election results
        vector<vector<int> > election_results(meta_blob_count, vector<int>(meta_blob_count - 1, -1));
        for(int voting_meta_blob = 0; voting_meta_blob < meta_blob_count; voting_meta_blob++) {
            for(int vote_place_index = 0; vote_place_index < meta_blob_count - 1; vote_place_index++) {
                // read in the meta-blob number that is at `vote_place_index` place in the election of meta-blob `voting_meta_blob`
                cin >> election_results.at(voting_meta_blob).at(vote_place_index);
            }
        }

        int t = 0; // needed as result later
        for(; t < meta_blob_count - 1; t++) {
            for(int voting_meta_blob = 0; voting_meta_blob < meta_blob_count; voting_meta_blob++) {
                // add edge for current place under check `t`
                Edge edge;
                tie(edge, tuples::ignore) = add_edge(voting_meta_blob, election_results.at(voting_meta_blob).at(t), graph);
            }

            // check if it's possible
            if(!boyer_myrvold_planarity_test(graph)) {
                break;
            }
        }

        cout << t << endl;
    }

    return 0;
}
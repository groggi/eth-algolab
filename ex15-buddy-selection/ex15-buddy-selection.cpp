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

    for(int i = 0; i < test_cases; i++) {
        // read test case
        int student_count, characteristic_count, min_solution;
        cin >> student_count >> characteristic_count >> min_solution;

        // create graph for test case
        Graph graph(student_count);

        // create a set of characteristics for each student from input
        map<int, set<string> > student_to_characs;
        for(int student_i = 0; student_i < student_count; student_i++) {
            set<string> charac_set;

            for(int j = 0; j < characteristic_count; j++) {
                string charac_value;
                cin >> charac_value;

                //cout << "\tbla: " << charac_value << endl;

                charac_set.insert(charac_value);
            }

            student_to_characs[student_i] = charac_set;
        }

        for(int k = 0; k < student_count; k++) {
            for(int p = k + 1; p < student_count; p++) {
                vector<string> intersec;

                set_intersection(student_to_characs.at(k).begin(), student_to_characs.at(k).end(),
                                 student_to_characs.at(p).begin(), student_to_characs.at(p).end(),
                                 back_inserter(intersec));

                //cout << "size A: " << student_to_characs.at(k).size() << endl;
                //cout << "size B: " << student_to_characs.at(p).size() << endl;
                //cout << "intersec size: " << intersec.size() << endl;
                if(intersec.size() > min_solution) {
                    //cout << "added" << endl;
                    // more in common than minimum given, add edge between students
                    bool success;
                    Edge edge;
                    tie(edge, success) = add_edge(k, p, graph);
                }
            }
        }

        // find matching
        vector<Vertex> mate(student_count);
        checked_edmonds_maximum_cardinality_matching(graph, &mate[0]);

        //cout << "size: " << matching_size(graph, &mate[0]) << endl;
        //cout << "half: " << (int)(student_count / 2) << endl;

        if(matching_size(graph, &mate[0]) == (int)(student_count / 2)) {
            cout << "not optimal" << endl;
        } else {
            cout << "optimal" << endl;
        }
    }

    return 0;
}
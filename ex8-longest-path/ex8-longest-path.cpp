#include <iostream>
#include <vector>
#include <stack>

using namespace std;

int main(void) {
    int testcases;
    cin >> testcases;

    for(int t = 0; t < testcases; t++) {
        int vertices;
        cin >> vertices;

        vector<vector<int>> graph(vertices, vector<int>());
        for(int i = 0; i < vertices - 1; i++) {
            int from, to;
            cin >> from >> to;

            graph.at(from).push_back(to);
            graph.at(to).push_back(from);
        }

        int longest = 0;
        int longest_v = -1;

        int i = 0; // random start
        stack<int> tovisit;
        vector<bool> visited(vertices, false);
        int longest_from_i = 0;

        tovisit.push(i);
        int prev = -1;
        while(!tovisit.empty()) {
            int next = tovisit.top();
            tovisit.pop();

            if(next == -1) {
                if(longest < longest_from_i) {
                    longest = longest_from_i;
                    longest_v = prev;
                    //cout << "longest_v: " << longest_v << endl;
                }

                longest_from_i--;
                //cout << " ... " << endl;
                continue;
            }

            if(!visited.at(next)) {
                prev = next;

                longest_from_i++;
                //cout << "visiting: " << next << ", depth: " << longest_from_i << endl;
                visited.at(next) = true;
                tovisit.push(-1);
                for(auto add_v : graph.at(next)) {
                    tovisit.push(add_v);
                }
            }
        }

        // --------------

        stack<int> tovisit2;
        vector<bool> visited2(vertices, false);
        int longest_from_i2 = 0;

        tovisit2.push(longest_v);

        while(!tovisit2.empty()) {
            int next = tovisit2.top();
            tovisit2.pop();

            if(next == -1) {
                longest = max(longest, longest_from_i2);
                longest_from_i2--;
                //cout << " ... " << endl;
                continue;
            }

            if(!visited2.at(next)) {
                longest_from_i2++;
                //cout << "visiting: " << next << ", depth: " << longest_from_i2 << endl;
                visited2.at(next) = true;
                tovisit2.push(-1);
                for(auto add_v : graph.at(next)) {
                    tovisit2.push(add_v);
                }
            }
        }

        cout << longest << endl;

        /*int longest = 0;
        for(int i = 0; i < vertices; i++) {
            if(graph.at(i).size() != 1) {
                continue;
            }

            stack<int> tovisit;
            vector<bool> visited(vertices, false);
            int longest_from_i = 0;

            tovisit.push(i);

            while(!tovisit.empty()) {
                int next = tovisit.top();
                tovisit.pop();

                if(next == -1) {
                    longest = max(longest, longest_from_i);
                    longest_from_i--;
                    //cout << " ... " << endl;
                    continue;
                }

                if(!visited.at(next)) {
                    longest_from_i++;
                    //cout << "visiting: " << next << ", depth: " << longest_from_i << endl;
                    visited.at(next) = true;
                    tovisit.push(-1);
                    for(auto add_v : graph.at(next)) {
                        tovisit.push(add_v);
                    }
                }
            }
        }

        cout << longest << endl;*/
    }
}
#include <iostream>
#include <vector>
#include <limits>
#include <algorithm> 
#include <queue>
#include <set>
#include <utility>
#include <map>
#include <string>

using namespace std;

int get_min_moves(int start, vector<vector<int> >&trans, vector<int>& min_fields, vector<int>& max_fields, int field_count);

int get_max_moves(int start, vector<vector<int> >&trans, vector<int>& min_fields, vector<int>& max_fields, int field_count) {
    // if we reached the end, nothing can be doen
    if(start == field_count - 1) {
        return 0;
    }

    // only calculate if value unknown
    if(max_fields.at(start) == -1) {
        // keeps track of the maximum amount of steps needed to reach the end from the given starting point
        int cur_max_found = -1;

        // iterate over all edges one can follow at the `start` position
        for(vector<int>::iterator iter = trans.at(start).begin();
            iter != trans.at(start).end();
            ++iter) {
            // now we search for the largest minimum amount of steps needed to reach the end
            int possible_max = get_min_moves(*iter, trans, min_fields, max_fields, field_count);
            if(possible_max > cur_max_found) {
                cur_max_found = possible_max;
            }
        }

        // update value
        max_fields.at(start) = cur_max_found + 1; // we still have to take the edge we followed
    }

    // return solution
    return max_fields.at(start);
}

int get_min_moves(int start, vector<vector<int> >&trans, vector<int>& min_fields, vector<int>& max_fields, int field_count) {
    // if we reached the end, no more moves needed
    if(start == field_count - 1) {
        return 0;
    }

    // only calculate if we don't know the solution yet
    if(min_fields.at(start) == -1) {
        // search for the minimum amount of moves to win from the current starting point
        int cur_min_found = numeric_limits<int>::max();

        // iterate over all possible next moves, i.e. the edges leaving the starting position
        for(vector<int>::iterator iter = trans.at(start).begin();
            iter != trans.at(start).end();
            ++iter) {
            // `iter` refers now to the edge we can follow, i.e. the next position we reach

            // now we have to assume that our opponent will be in our way and make our life complicated.
            // search for the maximum of moves from the next point we can reach to the finish line.
            int possible_min = get_max_moves(*iter, trans, min_fields, max_fields, field_count);
            if(possible_min < cur_min_found) {
                cur_min_found = possible_min;
            }
        }

        min_fields.at(start) = cur_min_found + 1; // we still have to use the edge, so add one
    }

    // return solution
    return min_fields.at(start);
}


int main() {
	cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int i = 0; i < test_cases; i++) {
        // read in basic information
        int position_count, transition_count;
        cin >> position_count >> transition_count;

        int start_red, start_black;
        cin >> start_red >> start_black;
        start_red -= 1; start_black -= 1; // let it start with 0, not 1

        // read in the possible transitions
        vector<vector<int> > trans(position_count, vector<int>());
        for(int transition_index = 0; transition_index < transition_count; transition_index++) {
            int from, to;
            cin >> from >> to;
            from -= 1; to -= 1; // let it start at 0

            // create transition, it's directed!
            trans.at(from).push_back(to);
        }

        vector<int> min_fields(position_count, -1);
        vector<int> max_fields(position_count, -1);

        int red_min = get_min_moves(start_red, trans, min_fields, max_fields, position_count);
        int black_min = get_min_moves(start_black, trans, min_fields, max_fields, position_count);

        // now we calculate the minimum amount of games each of the players does
        int min_sherlock = -1;
        int min_moriarty = -1;

        // check if the steps needed to win with the red meeple is even ...
        if(red_min % 2 == 0) {
            // ... ok it is even. Now we have to find out how many game steps were needed to
            // move the red meeple to the target position, as every second move sherlock moves the
            // black and not the red meeple
            min_sherlock = ((red_min - 2) / 2) * 4 + 4;
        } else {
            min_sherlock = ((red_min - 1) / 2) * 4 + 1;
        }

        if(black_min % 2 == 0) {
            min_moriarty = ((black_min - 2) / 2) * 4 + 3;
        } else {
            min_moriarty = ((black_min - 1) / 2) * 4 + 2;
        }

        if(min_sherlock < min_moriarty) {
            cout << 0 << endl;
        } else {
            cout << 1 << endl;
        }

    }

	return 0;
}
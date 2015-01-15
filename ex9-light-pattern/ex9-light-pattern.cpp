#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cmath>
#include <climits>
#include <algorithm>

using namespace std;

void debug_state(vector<bool> state) {
    for(int i = 0; i < state.size(); i++) {
        if(state.at(i)) {
            cout << "[X]";
        } else {
            cout << "[ ]";
        }
    }

    cout << endl << endl;
}

int main(void)
{
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int i = 0; i < test_cases; i++) {
        int bulb_count, bulbs_in_lightpattern, pattern;
        cin >> bulb_count >> bulbs_in_lightpattern >> pattern;

        // read initial state
        vector<bool> state(bulb_count);
        for(int j = 0; j < bulb_count; j++) {
            int v;
            cin >> v;
            state.at(j) = v == 1;
        }

        //debug_state(state);
        //cout << "TO" << endl;

        // calc target state
        vector<bool> target_state(bulbs_in_lightpattern);
        for(int j = bulbs_in_lightpattern - 1; j >= 0; j--) {
            target_state.at(j) = pattern & 0x1;
            pattern = pattern >> 1;
        }

        //debug_state(target_state);

        int group_count = (int)bulb_count / bulbs_in_lightpattern;
        int swapping_all = 1;
        int single_switching = 0;
        for(int group = group_count; group > 0; group--) {
            int from = group * bulbs_in_lightpattern - bulbs_in_lightpattern;
            //cout << "working on group: " << group << " from: " << from << " to: " << from + bulbs_in_lightpattern - 1 << endl;
            int local_swapping_all = 0;
            int local_single_switching = 0;
            for(int pos = 0; pos < bulbs_in_lightpattern; pos++) {
                if(state.at(from + pos) != target_state.at(pos)) {
                    local_single_switching++;
                } else {
                    local_swapping_all++;
                }
            }
            //cout << "\t current all: " << local_swapping_all << ", current single: " << local_single_switching << endl;

            int tmp_swapped = min(
                // already swapped till at least previously visited block
                min(
                    swapping_all + local_swapping_all, // how much do we need to changes in case we are already swapped
                    swapping_all + 2 + local_single_switching // we swap again, but only till current block and do normal changes
                    ),
                // not swapped yet
                min(
                    single_switching + 1 + local_swapping_all, // we swap till current block and do swapping changes
                    single_switching + 1 + local_single_switching // we swap till the next block and do normal changes in current block 
                    )
                );

            int tmp_single_change = min(
                // not swapped yet
                min(
                    single_switching + local_single_switching, // we just change each bulb by itself
                    single_switching + 2 + local_swapping_all // swap, change swapped, swap back
                    ),
                // swapped already
                min(
                    swapping_all + 1 + local_single_switching, // swap back, do normal changes 
                    swapping_all + 1 + local_swapping_all
                    )
            );

            swapping_all = tmp_swapped;
            single_switching = tmp_single_change;

            //cout << "\tall: " << swapping_all << ", single: " << single_switching << endl;
        }

        cout << min(swapping_all, single_switching) << endl;
    }

    return 0;
}
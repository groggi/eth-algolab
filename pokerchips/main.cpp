#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cmath>
#include <climits>
#include <algorithm>

#define EMPTY_CELL -1

using namespace std;

int round(int stack_count, vector<int>& stack_sizes, vector<vector<int> >& chips, vector<vector<vector<vector<vector<int > > > > >& table, vector<int>& conf) {
	//cout << "at: " << conf.at(0) << " " << conf.at(1) << " " << conf.at(2) << " " << conf.at(3) << " " << conf.at(4) << endl;
	// only calculate unknown cell values
	if(table[conf.at(0)][conf.at(1)][conf.at(2)][conf.at(3)][conf.at(4)] == EMPTY_CELL) {
		int max_value = EMPTY_CELL;

		// go over possible sets of poker chip stacks
		for(int set_index = 1; set_index < 32; set_index++) {
			int color = EMPTY_CELL;
			bool valid = true;
			int removed = 0;
			vector<int> new_conf(5);

			// go over all stacks and decide if it is part of the set, if so do calculation
			for(int stack_index = 0; stack_index < 5; stack_index++) {
				if((set_index >> stack_index) & 1) { // stack is in the current set
					if(conf.at(stack_index) == 0) {
						// can't remove a chip from current stack
						valid = false;
						break;
					}

					if(color == EMPTY_CELL) {
						// set current color
						color = chips.at(stack_index).at(conf.at(stack_index) - 1);
					} else if(color != chips.at(stack_index).at(conf.at(stack_index) - 1)) {
						// color top of current stack not the same as we selected
						valid = false;
						break;
					}

					removed++;
					new_conf.at(stack_index) = conf.at(stack_index) - 1;
				} else {
					new_conf.at(stack_index) = conf.at(stack_index);
				}
			}

			if(valid) {
				int points = 0;
				if(removed > 1) {
					points = pow(2, removed - 2);
				}

				int val = points + round(stack_count, stack_sizes, chips, table, new_conf);
				max_value = max(max_value, val);
			}
		}

		table[conf.at(0)][conf.at(1)][conf.at(2)][conf.at(3)][conf.at(4)] = max_value;
	}

	return table[conf.at(0)][conf.at(1)][conf.at(2)][conf.at(3)][conf.at(4)];
}

int main() {
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);

	int test_count;
	cin >> test_count;

	for(int test_index = 0; test_index < test_count; test_index++) {
		int stack_count;
		cin >> stack_count;

		// read in size of each stack
		vector<int> stack_sizes(5, 0);
		vector<int> conf(5, 0);
		for(int stack_index = 0; stack_index < stack_count; stack_index++) {
			int chip_amount;
			cin >> chip_amount;

			stack_sizes.at(stack_index) = chip_amount;
			conf.at(stack_index) = chip_amount;
		}

		// contains the chips of each stack at the start
		vector<vector<int > > chips;
		chips.reserve(stack_count);

		// read in chips in stack
		for(int stack_index = 0; stack_index < stack_count; stack_index++) {
			chips.push_back(vector<int>(stack_sizes[stack_index], -1));
			for(int chip_index = 0; chip_index < stack_sizes[stack_index]; chip_index++) {
				cin >> chips.at(stack_index).at(chip_index);
			}
		}

		// create DP table, initial value is EMPTY_CELL
		vector<vector<vector<vector<vector<int > > > > > table(stack_sizes[0] + 1,
			vector<vector<vector<vector<int > > > >(stack_sizes[1] + 1,
				vector<vector<vector<int > > >(stack_sizes[2] + 1,
					vector<vector<int > >(stack_sizes[3] + 1,
						vector<int>(stack_sizes[4] + 1, EMPTY_CELL)))));

		// initialise table
		table[0][0][0][0][0] = 0;

		// play the game
		cout << round(stack_count, stack_sizes, chips, table, conf) << endl;
	}
}
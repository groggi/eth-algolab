#include <iostream>
#include <vector>
#include <algorithm>
#include <limits>
#include <utility>
#include <set>
#include <limits>

using namespace std;

#define MAX_BAR_RANGE 200

int main() {
	cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

	int test_count;
	cin >> test_count;

	for(int test = 0; test < test_count; test++) {
		int parasols_count;
		cin >> parasols_count;

		// read in parasol locations
		vector<int> parasols_locs(parasols_count);
		for(int parasol_index = 0; parasol_index < parasols_count; parasol_index++) {
			cin >> parasols_locs.at(parasol_index);
		}

		// first sort parasols, so we can start from the one furthest away in negative direction
		sort(parasols_locs.begin(), parasols_locs.end());

		// keep track of the maximum amount of parasols
		int max_parasols = 0;
		int cur_parasol_count = 0;

		// keep track of `parasols_locs` indexes resulting in a ranges in which we can build a bar
		vector<pair<int, int> > bar_ranges;

		// keeps track of current start/stop index
		int start_index = 0;
		int stop_index = 0;

		bar_ranges.push_back(make_pair(start_index, stop_index));

		// iterate through parasols and collect bar ranges
		for(int parasol_index = 0; parasol_index < parasols_count; parasol_index++) {
			// if the current parasol is too far away from the previous one (defined by `start_index`), remove
			// parasols as long as there are more between the original `start_index` and current one until we're
			// back in the range of a bar
			for(; parasols_locs.at(parasol_index) - parasols_locs.at(start_index) > MAX_BAR_RANGE; start_index++) {
				cur_parasol_count--;
			}

			stop_index = parasol_index; // new "last" parasol in range of the bar
			cur_parasol_count++; // as we added the current parasol, increase the amount

			if(max_parasols < cur_parasol_count) {
				// we found a range with more parasols as before!
				max_parasols = cur_parasol_count;

				// as we found a better range, remove the previously found ones
				bar_ranges.clear();
				// ... and add the better one
				bar_ranges.push_back(make_pair(start_index, stop_index));
			} else if(max_parasols == cur_parasol_count) {
				// found another location for the bar with equal amount of reached parasols
				// we add it to our list of possible locations
				bar_ranges.push_back(make_pair(start_index, stop_index));
			}
		}

		// so, now we have a list of parisol ranges where we can build a bar in the middle and reach maximum
		// amount of customers. Next we have to find the one bar with the minimum distance.

		set<int> bar_locs;
		int min_distance_found = numeric_limits<int>::max();

		for(int range_index = 0; range_index < bar_ranges.size(); range_index++) {
			// stupid name, but basically `added_range / 2` is the location of the bar :-)
			int added_range = parasols_locs[bar_ranges.at(range_index).first] +
				parasols_locs[bar_ranges.at(range_index).second];

			int diff = (parasols_locs[bar_ranges.at(range_index).second] -
				parasols_locs[bar_ranges.at(range_index).first] + 1) / 2; // +1 because of rounding

			if(diff > min_distance_found) {
				// we found something with a better minimal distance, so ignore this bar!
				continue;
			} else if(diff < min_distance_found) {
				// oh, we found a better minimum, replace everything
				min_distance_found = diff;
				bar_locs.clear();
			}

			// now add the current bar to our bar location collection, as we know it is the best one
			// or as good as the best we not till now
			if(added_range % 2 == 0) {
				// even distance
				bar_locs.insert(added_range / 2);
			} else {
				bar_locs.insert((added_range - 1) / 2);
				bar_locs.insert((added_range - 1) / 2 + 1);
			}
		}

		// output basic information
		cout << max_parasols << " " << min_distance_found << endl;

		// output bar location in order: it is, we use a set!

		set<int>::iterator loc_iter = bar_locs.begin();
		cout << *loc_iter++;
		for(; loc_iter != bar_locs.end(); ++loc_iter) {
			cout << " " << *loc_iter;
		}

		cout << endl;
	}   
}
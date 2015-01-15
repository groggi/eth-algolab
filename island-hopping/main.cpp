#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <cassert>

using namespace std;

int main() {
	cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int test = 0; test < test_cases; test++) {
        int island_count, attack_radius, attacker_strength_start;
        cin >> island_count >> attack_radius >> attacker_strength_start;

        // read in the defense strength
        vector<int> defense(island_count + 1, 0);
        for(int island_index = 1; island_index < island_count; island_index++) {
            cin >> defense.at(island_index);
        }

        assert(defense.at(0) == 0);
        assert(defense.at(island_count) == 0);

        // keeps track of the path length
        vector<int> path_length(island_count + 1, numeric_limits<int>::max()); // first island is the attacker's island, this is just always -inf
        path_length.at(0) = 0;

        // our DP table containing the resulting fighter strength left at an island
        vector<int> table(island_count + 1, 0);
        table.at(0) = attacker_strength_start;

        // DP algo
        for(int cur_island = 0; cur_island < island_count; cur_island++) {
            if(table.at(cur_island) <= 0) {
                continue;
            }

            for(int next_island = cur_island + 1;
                next_island <= cur_island + attack_radius && next_island <= island_count;
                next_island++) {

                // calculate how many attackers are left if we send the people from island `cur_island` to
                // island `next_island`
                int attackers_left = table.at(cur_island) - defense.at(next_island);

                // if this is a greater number than the one already in our DP table, we found a better way
                if(attackers_left > table.at(next_island)) {
                    table.at(next_island) = attackers_left;
                    // don't forget to keep track of the path
                    path_length.at(next_island) = path_length.at(cur_island) + 1;
                } else if(attackers_left == table.at(next_island)) {
                    // maybe we found an other, better path with the same results
                    path_length.at(next_island) = min(path_length.at(cur_island) + 1, path_length.at(next_island));
                }
            }
        }

        if(table.at(island_count) <= 0) {
            cout << "safe" << endl;
        } else {
            cout << path_length.at(island_count) - 1 << " " << table.at(island_count) << endl;
        }
    }

	return 0;
}
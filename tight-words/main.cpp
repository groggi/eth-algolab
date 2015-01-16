#include <iostream>
#include <cassert>
#include <vector>
#include <limits>
#include <algorithm> 
#include <queue>
#include <set>
#include <utility>
#include <cmath>

#define MOD_NUM 100003

using namespace std;

int main() {
	cin.sync_with_stdio(false);
	cout.sync_with_stdio(false);

	int test_count;
	cin >> test_count;

	for(int test = 0; test < test_count; test++) {
		int max_word, max_length;
		cin >> max_word >> max_length;

		if(max_length > 0) {
			vector<vector<int> > table(max_word + 1, vector<int>(max_length + 1, 0));
			
			// init for length 1
			for(int word = 0; word <= max_word; word++) {
				table.at(word).at(1) = 1;
			}

			// go trough length of the word
			for(int length = 2; length <= max_length; length++) {
				// go trough each additional word
				for(int word = 0; word <= max_word; word++) {
					// add current word to a string ending with the previous word
					if(word - 1 >= 0) { // only if there is a "previous word" of course
						table.at(word).at(length) += table.at(word - 1).at(length - 1);
					}

					// add current word to a string ending with the same word
					table.at(word).at(length) += table.at(word).at(length - 1);

					// add current word to a string ending with the next word
					if(word + 1 <= max_word) {
						table.at(word).at(length) += table.at(word + 1).at(length - 1);
					}

					table.at(word).at(length) = table.at(word).at(length) % MOD_NUM;
				}
			}

			// now we have to count together all the words possible (it can end with any of the words)
			int solution = 0;
			for(int word = 0; word <= max_word; word++) {
				solution += table.at(word).at(max_length);
			}

			cout << solution % MOD_NUM << endl;
		} else {
			cout << "1" << endl;
		}
	}

	return 0;
}
#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <bitset>
#include <set>

using namespace std;

class PairCompare
{
public:
    bool operator() (pair<int, int>& lhs, pair<int, int>& rhs) {
        return lhs.first > rhs.first;
    }
};

int main() {
	cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int test = 0; test < test_cases; test++) {
    	int word_count;
        cin >> word_count;

        // read in how often each word occurs
        vector<int> occurs(word_count, 0);
        for(int word_index = 0; word_index < word_count; word_index++) {
            cin >> occurs.at(word_index);
        }

        // read in where it occurs
        priority_queue<pair<int, int>, vector<pair<int, int> >, PairCompare> locs;
        for(int word_index = 0; word_index < word_count; word_index++) {
            for(int i = 0; i < occurs.at(word_index); i++) {
                int l;
                cin >> l;
                locs.push(make_pair(l, word_index));
            }
        }

        // find first range with all words
        vector<int> first_set_loc(word_count, -1);
        set<pair<int, int> > words;
        int words_found = 0;
        int last_loc = numeric_limits<int>::min();
        while(!locs.empty() && words_found != word_count) {
            pair<int, int> cur = locs.top();
            locs.pop();

            if(first_set_loc.at(cur.second) == -1) {
                words_found++;
            } else {
                words.erase(make_pair(first_set_loc.at(cur.second), cur.second));
            }

            first_set_loc.at(cur.second) = cur.first;
            words.insert(cur);
            last_loc = cur.first;
        }

        //cout << "words_found: " << words_found << endl;
        //for(int x = 0; x < word_count; x++) {
            //cout << "\tword " << x << ", loc: " << first_set_loc.at(x) << endl;
            //locs.push(make_pair(first_set_loc.at(x), x));
        //}
        //cout << "min: " << words.begin()->first << ", max: " << last_loc << endl;
        
        int min_dist = last_loc - words.begin()->first;

        while(!locs.empty()) {
            pair<int, int> cur = locs.top();
            locs.pop();

            // modify location
            words.erase(make_pair(first_set_loc.at(cur.second), cur.second));
            words.insert(cur);
            first_set_loc.at(cur.second) = cur.first;

            // check range
            int d = cur.first - words.begin()->first;
            min_dist = min(min_dist, d);
        }

        cout << min_dist + 1 << endl;
    }

    return 0;
}
#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cmath>
#include <climits>
#include <algorithm>

using namespace std;

int at_least(vector<vector<int> >& table, vector<int>& values, int left, int right)
{
    // check first if we know the value already
    if(left == right) {
        // only one coin on the table, we can choose so it's ours
        return values[left];
    }

    if(table[left][right] > 0) {
        // already calculated value, reuse
        return table[left][right];
    }

    // calculate easy cases, fast paths
    int delta = right - left;
    if(delta == 1) {
        // case: left_coin, right_coin
        int best_of_both = max(values[left], values[right]);
        table[left][right] = best_of_both;
        return best_of_both;
    } else if(delta == 2) {
        // case: left_coin, middle_coin, right_coin
        // we have to take middle coin into account and resolve all three coins
        int left_coin = values[left];
        int middle_coin = values[left+1];
        int right_coin = values[right];

        int resulting_value = -2;
        if(left_coin > right_coin) {
            // we take the left coin, now we have to take into account that opposite
            // side will take the best of middle/right coin
            resulting_value = left_coin;
            if(middle_coin > right_coin) {
                resulting_value += right_coin;
            } else {
                resulting_value += middle_coin;
            }
        } else {
            // we take right coin, again take middle coin into account
            resulting_value = right_coin;
            if(middle_coin > left_coin) {
                resulting_value += left_coin;
            } else {
                resulting_value += middle_coin;
            }
        }

        table[left][right] = resulting_value;
        return resulting_value;
    }

    /*
     * now decide what happens in general
     */

    // assume we took the left coin
    int we_left_other_left = at_least(table, values, left+2, right); // opposite side takes also the next left one
    int we_left_other_right = at_least(table, values, left+1, right-1); // opposite side takes the right one

    // assume we took the right coin
    int we_right_other_left = we_left_other_right; //at_least(table, values, left+1, right-1);
    int we_right_other_right = at_least(table, values, left, right-2); // opposite side takes the right one too

    // find the minimum value we're guaranteed to get
    int we_get_at_least = max(
        values[left] + min(we_left_other_left, we_left_other_right),
        values[right] + min(we_right_other_left, we_right_other_right)
        );
    table[left][right] = we_get_at_least;
    return we_get_at_least;
}

int main(void)
{
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int i = 0; i < test_cases; i++) {
        int coin_amount;
        cin >> coin_amount;
        vector<int> coins;
        for(int j = 0; j < coin_amount; j++) {
            int value;
            cin >> value;
            coins.push_back(value);
        }

        vector<vector<int> > table(coin_amount + 1, vector<int>(coin_amount + 1, -1));
        cout << at_least(table, coins, 0, coin_amount - 1) << endl;
    }

    return 0;
}
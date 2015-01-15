#include <iostream>
#include <vector>
#include <cmath>
#include <climits>

using namespace std;

int main(void)
{
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int n;
    cin >> n;

    for(int i = 0; i < n; i++) {
        int c, m;
        cin >> c;
        cin >> m;

        vector<int> coins(c);
        for(int j = 0; j < c; j++) {
            cin >> coins.at(j);
        }

        vector<int> ret(m);
        int max_ret = 0;
        for(int j = 0; j < m; j++) {
            int ret_val;
            cin >> ret_val;

            ret.at(j) = ret_val;
            if(ret_val > max_ret) {
                max_ret = ret_val;
            }
        }

        vector<int> table(max_ret + 1);
        table.at(0) = 0;

        for(int v = 1; v <= max_ret; v++) {
            table.at(v) = INT_MAX;
            for(auto coin : coins) {
                if(v - coin >= 0 && coin <= v) {
                    if(table.at(v-coin) != INT_MAX) {
                        table.at(v) = min(table.at(v), table.at(v-coin) + 1);
                    }
                }
            }
        }

        for(auto r : ret) {
            if(table.at(r) == INT_MAX) {
                cout << "not possible" << endl;
            } else {
                cout << table.at(r) << endl;
            }
        }
    }

    return 0;
}
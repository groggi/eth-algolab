#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cmath>
#include <climits>
#include <algorithm>

using namespace std;

class Alien {
public:
    int from;
    int to;
    bool dominated;
    Alien(int from, int to) : from(from), to(to), dominated(false) {}
};

bool cmp(Alien left, Alien right) {
    if(left.from == right.from) {
        return left.to > right.to;
    } else {
        return left.from < right.from;
    }
}

int main(void)
{
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int i = 0; i < test_cases; i++) {
        int alien_count, human_count;
        cin >> alien_count >> human_count;

        vector<Alien> aliens;

        for(int j = 0; j < alien_count; j++) {
            int p, q;
            cin >> p >> q;

            aliens.push_back(Alien(p, q));
        }

        sort(aliens.begin(), aliens.end(), cmp);

        /*for(auto a : aliens) {
            cout << "alien: [" << a.from << ", " << a.to << "]" << endl;
        }
        cout << endl << endl;
        */

        int last_human_attacked = 0;
        int last_alien_end = 0;
        int count = 0;
        bool not_all_humans = false;
        bool id = false;
        Alien last = Alien(0, 0);

        for(int cur = 0; cur < alien_count; cur++) {
            Alien &cur_alien = aliens.at(cur);

            if(cur_alien.from > last_human_attacked + 1) {
                not_all_humans = true;
                break;
            } else {
                last_human_attacked = max(cur_alien.to, last_human_attacked);
            }

            if(!id && last.from == cur_alien.from && last.to == cur_alien.to && cur_alien.to != 0) {
                count --;
                id = true;
            } else  if(cur_alien.to > last_alien_end) {
                count++;
                last = cur_alien;
                last_alien_end = cur_alien.to;
                id = false;
            }
        }

        if(not_all_humans || last_human_attacked != human_count) {
            //cout << "!0" << " last = " << last_human_attacked << endl;
            cout << "0" << endl;
        } else {
            cout << count << endl;
        }
    }

    return 0;
}
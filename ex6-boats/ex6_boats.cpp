#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cmath>
#include <climits>
#include <algorithm>

using namespace std;

class Data {
public:
    int length, ring_pos, min_pos;

    Data(int length, int ring_pos) : length(length), ring_pos(ring_pos), min_pos(ring_pos) {}

    bool operator< (const Data& other) const {
        if(min_pos == other.min_pos) {
            return ring_pos > other.ring_pos;
        }
        return min_pos > other.min_pos;
    }
};

int main(void)
{
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int i = 0; i < test_cases; i++) {
        int N;
        cin >> N;

        priority_queue<Data> boats;
        for(int j = 0; j < N; j++) {
            int l, p;
            cin >> l;
            cin >> p;
            boats.push(Data(l, p));
        }

        int last_end = boats.top().ring_pos;
        boats.pop();
        int counter = 1;
        while(!boats.empty()) {
            auto boat = boats.top();
            boats.pop();
            
            if(boat.min_pos - boat.length >= last_end) {
                counter++;
                last_end = boat.min_pos;
            } else if(last_end <= boat.ring_pos) {
                // boat overlaps with a previous boat, we have to move it
                boat.min_pos = last_end + boat.length;
                if(boat.min_pos >= boat.ring_pos) {
                    boats.push(boat);
                }
            }
        }

        cout << counter << endl;
    }

    return 0;
}
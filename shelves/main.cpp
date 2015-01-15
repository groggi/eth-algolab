#include <iostream>
#include <vector>
#include <cmath>

using namespace std;

int main(void)
{
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int N;
    cin >> N;

    for(int i = 0; i < N; i++) {
        int length, large_shelve_length, small_shelve_length;
        cin >> length;
        cin >> small_shelve_length;
        cin >> large_shelve_length;

        int opt_small_shelves = 0;
        int opt_large_shelves = 0;
        int opt_wall_left = length;

        int sqrt_len = (int)sqrt(length);
        if(large_shelve_length > sqrt_len) {
            for(int large_shelves = min(sqrt_len, (int)(length / large_shelve_length)); large_shelves >= 0; large_shelves--) {
                int wall_left_for_small = (length - (large_shelves * large_shelve_length));
                int wall_left = wall_left_for_small % small_shelve_length;
                if(wall_left < opt_wall_left) {
                    opt_wall_left = wall_left;
                    opt_small_shelves = (int)(wall_left_for_small / small_shelve_length);
                    opt_large_shelves = large_shelves;
                }

                if(opt_wall_left == 0) {
                    break;
                }
            }
        } else {
            int max_small = min(sqrt_len, (int)(length / small_shelve_length));
            for(int small_shelves = 0; small_shelves <= max_small; small_shelves++) {
                int wall_left_for_large = (length - (small_shelves * small_shelve_length));
                int wall_left = wall_left_for_large % large_shelve_length;
                if(wall_left < opt_wall_left) {
                    opt_wall_left = wall_left;
                    opt_small_shelves = small_shelves;
                    opt_large_shelves = (int)(wall_left_for_large / large_shelve_length);
                }

                if(opt_wall_left == 0) {
                    break;
                }
            }
        }

        cout << opt_small_shelves << " " << opt_large_shelves << " " << opt_wall_left << endl;
    }

    return 0;
}
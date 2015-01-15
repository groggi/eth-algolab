#include <iostream>
#include <vector>
#include <queue>
#include <tuple>
#include <cmath>
#include <climits>

using namespace std;

int main(void)
{
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int i = 0; i < test_cases; i++) {
        int width_x, height_y;
        cin >> width_x;
        cin >> height_y;

        int start_x, start_y;
        cin >> start_x;
        cin >> start_y;

        int end_x, end_y;
        cin >> end_x;
        cin >> end_y;

        int obstacle_count;
        cin >> obstacle_count;
        vector<vector<bool>> obstacles(width_x, vector<bool>(height_y, false));
        for(int j = 0; j < obstacle_count; j++) {
            int x1, x2, y1, y2;
            cin >> x1;
            cin >> y1;
            cin >> x2;
            cin >> y2;

            for(int k = x1; k <= x2; k++) {
                for(int l = y1; l <= y2; l++) {
                    obstacles.at(k).at(l) = true;
                }
            }
        }

        // check
        /*cout << "width: " << width_x << " & height: " << height_y << endl;
        cout << "start: x=" << start_x << ", y=" << start_y << endl;
        cout << "end: x=" << end_x << ", y=" << end_y << endl;
        cout << "obstacles:" << endl;
        for(int y = 0; y < height_y; y++) {
            for(int x = 0; x < width_x; x++) {
                if(obstacles.at(x).at(y)) {
                    cout << "X";
                } else if(start_x == x && start_y == y) {
                    cout << "S";
                } else if(end_x == x && end_y == y) {
                    cout << "E";
                } else {
                    cout << "O";
                }
            }
            cout << endl;
        }
        cout << "-----------------------" << endl << endl;*/

        // algo start
        //           x , y  , v_x, v_y, depth
        queue<tuple<int, int, int, int, int>> next_tiles;
        next_tiles.push(make_tuple(start_x, start_y, 0, 0, 0));
        bool found = false;
        vector<vector<vector<vector<bool>>>> visited(width_x, vector<vector<vector<bool>>>(height_y, vector<vector<bool>>(7, vector<bool>(7, false))));
        while(!next_tiles.empty()) {
            auto tile = next_tiles.front();
            next_tiles.pop();
            int x = get<0>(tile);
            int y = get<1>(tile);
            int v_x = get<2>(tile);
            int v_y = get<3>(tile);
            int depth = get<4>(tile);

            if(x < 0 || x >= width_x ||
                y < 0 || y >= height_y ||
                v_x < -3 || v_x > 3 ||
                v_y < -3 || v_y > 3 ||
                obstacles.at(x).at(y) ||
                visited.at(x).at(y).at(v_x + 3).at(v_y + 3)) {
                //cout << "ignore: x=" << x << ", y=" << y << ", v_x=" << v_x << ", v_y=" << v_y << ", depth=" << depth << endl;
                continue;
            }

            /*for(int i_y = 0; i_y < height_y; i_y++) {
                for(int i_x = 0; i_x < width_x; i_x++) {
                    if(x == i_x && y == i_y) {
                        cout << "P";
                    } else if(obstacles.at(i_x).at(i_y)) {
                        cout << "X";
                    } else if(start_x == i_x && start_y == i_y) {
                        cout << "S";
                    } else if(end_x == i_x && end_y == i_y) {
                        cout << "E";
                    } else {
                        cout << "O";
                    }
                }
                cout << endl;
            }
            cout << endl << endl;*/

            //cout << "jump: x=" << x << ", y=" << y << ", v_x=" << v_x << ", v_y=" << v_y << ", depth=" << depth << endl;

            if(x == end_x && y == end_y) {
                cout << "Optimal solution takes " << depth << " hops." << endl;
                found = true;
                break;
            }

            visited.at(x).at(y).at(v_x + 3).at(v_y + 3) = true;

            next_tiles.push(make_tuple(x + v_x - 1, y + v_y - 1,    v_x - 1, v_y - 1,   depth + 1));
            next_tiles.push(make_tuple(x + v_x - 1, y + v_y,        v_x - 1, v_y,       depth + 1));
            next_tiles.push(make_tuple(x + v_x - 1, y + v_y + 1 ,   v_x - 1, v_y + 1,   depth + 1));

            next_tiles.push(make_tuple(x + v_x, y + v_y - 1,    v_x,    v_y - 1,    depth + 1));
            next_tiles.push(make_tuple(x + v_x, y + v_y,        v_x,    v_y,        depth + 1));
            next_tiles.push(make_tuple(x + v_x, y + v_y + 1,    v_x,    v_y + 1,    depth + 1));

            next_tiles.push(make_tuple(x + v_x + 1, y + v_y - 1,    v_x + 1, v_y - 1,   depth + 1));
            next_tiles.push(make_tuple(x + v_x + 1, y + v_y,        v_x + 1, v_y,       depth + 1));
            next_tiles.push(make_tuple(x + v_x + 1, y + v_y + 1,    v_x + 1, v_y + 1,   depth + 1));

        }

        if(!found) {
            cout << "No solution." << endl;
        }
    }

    return 0;
}
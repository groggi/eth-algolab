#include <iostream>
#include <vector>

using namespace std;

int main(void)
{
    int n;
    cin >> n; // ignore this

    while(cin >> n) {
        float sum = 0.0;
        for(int i=0; i < n; i++) {
            float v = 0.0;
            cin >> v;
            sum += v;
        }
        cout << sum << endl;
    }

    return 0;
}
#include <iostream>
#include <vector>

using namespace std;

int main(void)
{
    int n;
    cin >> n;

    vector<int> elems(n);
    for(int i = 0; i < n; i++) {
        cin >> elems.at(i);
    }

    int even = 0;
    for(int start = 0; start < n; start++) {
        for(int end = start; end < n; end++) {
            int sum = 0;
            for(int i = start; i <= end; i++) {
                sum += elems.at(i);
            }

            if(sum % 2 == 0) {
                even++;
            }
        }
    }

    cout << even << endl;

    return 0;
}
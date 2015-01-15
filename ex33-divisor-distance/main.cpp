#include <iostream>
#include <algorithm>
#include <vector>
#include <queue>
#include <bitset>

using namespace std;

#define MAX_PRIM 10000000
bitset<MAX_PRIM> is_prime;

void prime_sieve() {
	is_prime.set(); // sets all to true
	// 0, 1 not prime
	is_prime.reset(0);
	is_prime.reset(1);

	for(int prime = 2; prime * prime <= MAX_PRIM; prime++) {
		if(is_prime.test(prime)) {
			// is a prime number, so all multiples of it are not
			for(int multiple = prime + prime; multiple < MAX_PRIM; multiple += prime) {
				is_prime.reset(multiple); // set multiple to false, as it is not prime
			}
		}
	}
}

int get_next_prime(int cur) {
	for(int i = cur + 1; i * i < MAX_PRIM; i ++) {
		if(is_prime.test(i)) {
			return i;
		}
	}

	return 0;
}

int main() {
	cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    prime_sieve();

    for(int test = 0; test < test_cases; test++) {
    	int max_nr, vertices_pair_count;
    	cin >> max_nr >> vertices_pair_count;

    	for(int vertices_pair_index = 0; vertices_pair_index < vertices_pair_count; vertices_pair_index++) {
    		int from, to;
    		cin >> from >> to;

    		int length = 0;
    		int a_factor = 2;
    		int b_factor = 2;

    		// divise `from` and `to` with prime numbers as long as they do not match
    		while(from != to) {
    			// always lower the current "maximum" of `from` and `to`
    			if(from > to) {
    				if(is_prime.test(from)) {
    					from = 1;
    				} else {
    					// find a divisor for current `from`
    					while(from % a_factor != 0) {
    						a_factor = get_next_prime(a_factor);
    					}

    					// found a divisor for `from`, " " remove " " it
    					from = from / a_factor;
    				}
    			} else {
    				// following code works the same as for `from` and `a_factor`
    				if(is_prime.test(to)) {
    					to = 1;
    				} else {
    					while(to % b_factor != 0) {
    						b_factor = get_next_prime(b_factor);
    					}

    					to = to / b_factor;
    				}
    			}

    			length++;
    		}

    		cout << length << endl;
    	}
    }

    return 0;
}
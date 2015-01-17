// sorting
// ATTENTION: be careful, each container has its own order!
#include <vector>
#include <algorithm>

/* 
 * #1: Define custom compare function
 */
struct Edge {
	int from, to, weight;
};

bool compare(const Edge& lhs, const Edge& rhs) {
	return lhs.weight > rhs.weight;
}

// ...
vector<Edge> edges;
// ...
sort(edges.begin(), edges.end(), compare);


/* #2: Define the `<` operator for a struct/class
 * Generally, for some type T define `bool operator<(T other) const {}`
 */
struct Edge {
	int from, to, weight;

	bool operator<(Edge other) const {
		return weight > other.weight;
	}
};

// ...
vector<Edge> edges;
// ...
sort(edges.begin(), edges.end());

/*
 * #3: Define `operator()`
 */

struct Edge {
	int from, to, weight;
};

class Compares {
	int ref_weight;

public:
	Compares(const int& weight) : ref_weight(weight) {}

	bool operator()(const Edge& lhs, const Edge& rhs) const {
		return lhs.weight > rhs.weight && lhs.weight >= ref_weight;
	}
};

// ...
Compares comp(100);
// ...
vector<Edge> edges;
// ...
sort(edges.begin(), edges.end(), comp);
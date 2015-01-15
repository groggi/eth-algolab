#include <iostream>
#include <algorithm>
#include <vector>

#include <boost/graph/adjacency_list.hpp>
#include <boost/graph/push_relabel_max_flow.hpp>
#include <boost/graph/edmonds_karp_max_flow.hpp>
#include <boost/tuple/tuple.hpp>
#include <boost/graph/connected_components.hpp>

using namespace std;
using namespace boost;

typedef adjacency_list_traits<vecS, vecS, directedS> Traits;
typedef adjacency_list<vecS, vecS, directedS, no_property,
    property<edge_capacity_t, long,
        property<edge_residual_capacity_t, long,
            property<edge_reverse_t, Traits::edge_descriptor> > > > Graph;
typedef adjacency_list<vecS, vecS, undirectedS, no_property, no_property> SimpleGraph;
typedef property_map<Graph, edge_capacity_t>::type      EdgeCapacityMap;
typedef property_map<Graph, edge_residual_capacity_t>::type ResidualCapacityMap;
typedef property_map<Graph, edge_reverse_t>::type       ReverseEdgeMap;
typedef graph_traits<Graph>::vertex_descriptor          Vertex;
typedef graph_traits<Graph>::edge_descriptor            Edge;



// Custom add_edge, also creates reverse edges with corresponding capacities.
void addEdge(int u, int v, long c, EdgeCapacityMap &capacity, ReverseEdgeMap &rev_edge, Graph &G) {
    Edge e, reverseE;
    tie(e, tuples::ignore) = add_edge(u, v, G);
    tie(reverseE, tuples::ignore) = add_edge(v, u, G);
    capacity[e] = c;
    capacity[reverseE] = 0;
    rev_edge[e] = reverseE;
    rev_edge[reverseE] = e;
}

int main() {
	cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);

    int test_cases;
    cin >> test_cases;

    for(int test = 0; test < test_cases; test++) {
    	int room_count, corridor_count, sweeper_count;
        cin >> room_count >> corridor_count >> sweeper_count;

        int door_count = sweeper_count;

        // create graph for flow analysis
        Graph flow_graph(room_count + 2);
        EdgeCapacityMap capacity = get(edge_capacity, flow_graph);
        ReverseEdgeMap  rev_edge = get(edge_reverse, flow_graph);
        //ResidualCapacityMap res_capacity = get(edge_residual_capacity, flow_graph);

        // create graph with just the rooms, corridors
        SimpleGraph castle_graph(room_count);

        // define source and sink
        int source = room_count;
        int sink = room_count + 1;

        // keep track of vertices degrees
        vector<int> degrees(room_count, 0);
        vector<int> non_modified_degrees(room_count, 0);

        // read in location of sweepers
        vector<int> sweepers(room_count, 0);
        for(int sweeper_index = 0; sweeper_index < sweeper_count; sweeper_index++) {
            int location;
            cin >> location;

            // source -> sweeper location
            addEdge(source, location, 1, capacity, rev_edge, flow_graph);

            degrees.at(location)++;
            sweepers.at(location)++;
        }

        // read in location of doors
        for(int door_index = 0; door_index < door_count; door_index++) {
            int location;
            cin >> location;

            // door location -> sink
            addEdge(location, sink, 1, capacity, rev_edge, flow_graph);

            degrees.at(location)--;
        }

        // read in corridors
        for(int corridor_index = 0; corridor_index < corridor_count; corridor_index++) {
            int from, to;
            cin >> from >> to;

            addEdge(from, to, 1, capacity, rev_edge, flow_graph);
            addEdge(to, from, 1, capacity,rev_edge, flow_graph);

            add_edge(from, to, castle_graph);

            degrees.at(from)++;
            degrees.at(to)++;
            non_modified_degrees.at(from)++;
            non_modified_degrees.at(to)++;
        }

        // so now we have a basic graph, check if all sweepers can reach a door.
        // do this by doing a flow and check if flow result same as sweepers count
        int sweepers_exited = push_relabel_max_flow(flow_graph, source, sink);

        //cout << "sweepers exited: " << sweepers_exited << ", total sweepers: " << sweeper_count << endl;
        if(sweepers_exited != sweeper_count) {
            cout << "no" << endl;
            continue;
        }

        // check for euler circuit
        bool has_even_degree = true;
        for(int vertex = 0; vertex < room_count; vertex++) {
            if(degrees.at(vertex) < 0 || degrees.at(vertex) % 2 != 0) {
                has_even_degree = false;
                break;
            }
        }

        //cout << "even: " << has_even_degree << endl;
        if(!has_even_degree) {
            cout << "no" << endl;
            continue;
        }

        // make sure every edge, i.e. corridor, was visited at least once
        // first get the connected component information
        vector<int> component(room_count);
        connected_components(castle_graph, &component[0]);
        vector<int> cleaned_by_sweepers(room_count, 0);
        int max_component_nr = 0;
        for(int room_index = 0; room_index < room_count; room_index++) {
            // we're interested that every component is cleaned, so we use this as the index. Keep track of
            // how many components we have
            int component_nr = component.at(room_index);
            max_component_nr = max(max_component_nr, component_nr);

            // if the room has no corridors attached, we assume it is cleaned (only corridors need cleaning,
            // no corridor, no cleaning needed).
            if(non_modified_degrees.at(room_index) == 0) {
                cleaned_by_sweepers.at(component_nr) = 1;
            } else {
                // the degree of the room might be 0, then there is no sweeper added to the component. If never one
                // is added, component not cleaned.
                cleaned_by_sweepers.at(component_nr) += sweepers.at(room_index);
            }
        }

        // make sure every component is cleaned, i.e. contains some sweepers. Other requirements make sure
        // the component is cleaned if it contains sweepers
        bool components_cleaned = true;
        for(int i = 0; i <= max_component_nr; i++) {
            if(cleaned_by_sweepers.at(i) == 0) {
                components_cleaned = false;
                break;
            }
        }

        if(components_cleaned) {
            cout << "yes" << endl;
        } else {
            cout << "no" << endl;
        }
    }

    return 0;
}
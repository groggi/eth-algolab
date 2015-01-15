#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/squared_distance_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

using namespace std;

// from slides, fun!
double floor_to_double(const K::FT& x)
{
    double a = std::floor(CGAL::to_double(x));
    while (a > x) a -= 1;
    while (a+1 <= x) a += 1;
    return a;
}

double ceil_to_double(const K::FT& x)
{
    double a = std::ceil(CGAL::to_double(x));
    while (a < x) a += 1;
    while (a-1 >= x) a -= 1;
    return a;
}

bool DFS(Triangulation &triang, double min_dist_for_edge, Triangulation::Face_handle &start, map<Triangulation::Face_handle, int> &visitor_map, int cur_iter) {
    // each infected person's point is connected to three edges, otherwise we wouldn't get triangle
    for(int edge_i = 0; edge_i < 3; edge_i++) {
        // get the segment representing the edge
        Triangulation::Segment edge_segment = triang.segment(start, edge_i);

        // check if we are still far away from both endpoints of the segment while passing through
        double distance = CGAL::to_double(edge_segment.squared_length());
        if(distance >= min_dist_for_edge) {
            // get the neighboring face
            Triangulation::Face_handle neighbor_face_h = start->neighbor(edge_i);

            // check that we didn't visit it already
            if(visitor_map[neighbor_face_h] == cur_iter) {
                continue; // use other edge
            }

            // check if neighboring face is infinite, i.e. we found a way out
            if(triang.is_infinite(neighbor_face_h)) {
                return true;
            }

            // mark as visited by current iteration
            visitor_map[neighbor_face_h] = cur_iter;

            // recursion, DFS
            if(DFS(triang, min_dist_for_edge, neighbor_face_h, visitor_map, cur_iter)) {
                return true;
            }
        }
    }

    return false;
}

int main() {
    // some basic setup stuff
    cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);
    cout << fixed << setprecision(0);

    while(true) {
        int infected_count;
        cin >> infected_count;

        // kill switch for application
        if(infected_count == 0) {
            break;
        }

        // read in infected people's location
        vector<K::Point_2> infected_locs;
        infected_locs.reserve(infected_count);
        for(int i = 0; i < infected_count; i++) {
            double infected_x, infected_y;
            cin >> infected_x >> infected_y;

            infected_locs.push_back(K::Point_2(infected_x, infected_y));
        }

        // create triangulation with the infected people
        Triangulation triang;
        triang.insert(infected_locs.begin(), infected_locs.end());

        // prepare a map mapping a face handle to an integer, the integer represents the DFS step in which the face was
        // visited already (to not visit multiple time the same face)
        map<Triangulation::Face_handle, int> visitor_map;
        for (Triangulation::Face_iterator it = triang.finite_faces_begin(); it != triang.finite_faces_end(); it++) {
            visitor_map[it] = -1;
        }

        // go trough people trying to escape
        int escapee_count;
        cin >> escapee_count;
        for(int i = 0; i < escapee_count; i++) {
            // read location of escapee and the minimum distance expected
            double escapee_x, escapee_y, min_dist;
            cin >> escapee_x >> escapee_y >> min_dist;

            K::Point_2 escapee_loc(escapee_x, escapee_y);

            // get nearst vertex's point in the triangulation
            Triangulation::Point nearest_vertex_point = triang.nearest_vertex(escapee_loc)->point();

            // check that we not already violate the distance condition
            if(CGAL::to_double(CGAL::squared_distance(escapee_loc, nearest_vertex_point)) < min_dist) {
                cout << "n";
                continue; // jump over the rest, we already know the escapee is near an infected person
            }

            // find the face handle of our escapee
            Triangulation::Face_handle face_h = triang.locate(escapee_loc);

            // update for the face handle that we visited it in the current round
            visitor_map[face_h] = i;
            if(triang.is_infinite(face_h) || // face is already outside, escapee can escape, no DFS needed
                DFS(triang, min_dist * 4.0, face_h, visitor_map, i)) { // use DFS to find a way out, multiply by 4.0 (= 2.0^2 (not squared distances)) as we must be min_dist away from one ende of an edge and min_dist from the other, multiply by 2^2
                cout << "y";
            } else {
                cout << "n";
            }
        }

        cout << endl;
    }
}

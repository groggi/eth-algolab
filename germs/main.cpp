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

double hours(double distance) {
    double x = sqrt(distance) - 0.5; // first sqrt to get the distance ...
    double result;
    if (x <= 0.0) {
        result = 0;
    } else {
        result = ceil(sqrt(x)); // second sqrt because t^2 + 0.5 = x => t = sqrt(x - 0.5);
    }
    return result;
}

int main() {

    while(true) {
        int bacteria_count;
        cin >> bacteria_count;

        // kill switch for application
        if(bacteria_count == 0) {
            break;
        }

        // read in boundaries of the dish
        double left_border, right_border, bottom_border, top_border;
        cin >> left_border >> bottom_border >> right_border >> top_border;

        // collect bacteria's center information
        vector<K::Point_2> bacteria_centers;
        bacteria_centers.reserve(bacteria_count);
        for(int i = 0; i < bacteria_count; i++) {
            double bacteria_x, bacteria_y;
            cin >> bacteria_x >> bacteria_y;

            bacteria_centers.push_back(K::Point_2(bacteria_x, bacteria_y));
        }

        // create triangulation
        Triangulation triang;
        triang.insert(bacteria_centers.begin(), bacteria_centers.end());

        // keep track of the distances for each bacteria
        map<Triangulation::Point, double> distances;
        //distances.reserve(bacteria_count);

        // calculate initial distance: distance between the bacteria and the nearest dish boundary
        for(Triangulation::Finite_vertices_iterator vertex_iter = triang.finite_vertices_begin(); vertex_iter != triang.finite_vertices_end(); ++vertex_iter) {
            Triangulation::Point vertex = vertex_iter->point();
            distances[vertex] = min(
                min(vertex.x() - left_border, right_border - vertex.x()), // left/right minimum
                min(vertex.y() - bottom_border, top_border - vertex.y()) // top/bottom minimum
            );

            distances[vertex] *= distances[vertex]; // square distance as we work with squared ones
        }

        // compute distance to other two neighbours and update distance if it is smaller
        for(Triangulation::Finite_edges_iterator edge_iter = triang.finite_edges_begin(); edge_iter != triang.finite_edges_end(); ++edge_iter) {
            Triangulation::Vertex_handle vertex1 = edge_iter->first->vertex(triang.cw(edge_iter->second));
            Triangulation::Vertex_handle vertex2 = edge_iter->first->vertex(triang.ccw(edge_iter->second));

            Triangulation::Point vertex1_point = vertex1->point();
            Triangulation::Point vertex2_point = vertex2->point();

            // calculate distance of the points of both vertex and half them (divide by 4 as distance is squared and 4 = 2^2)
            double vertex_distance = CGAL::to_double(CGAL::squared_distance(vertex1_point, vertex2_point)) / 4;

            // update distances to minimum
            distances[vertex1_point] = min(distances[vertex1_point], vertex_distance);
            distances[vertex2_point] = min(distances[vertex2_point], vertex_distance);
        }

        // now we know the minimum distance for each bacteria to another one or the borders of the dish

        // extract distances into a vector and sort it
        vector<double> only_distances;
        only_distances.reserve(bacteria_count);
        for(map<Triangulation::Point, double>::iterator iter = distances.begin(); iter != distances.end(); ++iter) {
            only_distances.push_back(iter->second);
        }

        // sort distances
        sort(only_distances.begin(), only_distances.end());

        // print out information
        cout << hours(only_distances[0]) << " " << hours(only_distances[bacteria_count/2]) << " " << hours(only_distances[bacteria_count - 1]) << endl;
    }
}

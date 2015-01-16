#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Point_set_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_face_base_2<K> Fb;
typedef CGAL::Triangulation_data_structure_2<Vb, Fb> Tds;
typedef CGAL::Delaunay_triangulation_2<K, Tds>  Triangulation;

typedef Triangulation::Edge_iterator Edge_iterator;
typedef Triangulation::Face_handle Face_handle;
typedef Triangulation::Face_iterator Face_iterator;
typedef Triangulation::All_faces_iterator All_faces_iterator;
typedef Triangulation::Finite_vertices_iterator Finite_vertices_iterator;
typedef Triangulation::Finite_edges_iterator Finite_edges_iterator;
typedef Triangulation::Point Point;
typedef Triangulation::Segment Segment;
typedef Triangulation::Vertex_handle Vertex_handle;

using namespace std;

int main() {
  // some basic setup stuff
  cin.sync_with_stdio(false);
  cout.sync_with_stdio(false);
  cout << fixed << setprecision(0);

  int test_count;
  cin >> test_count;
  for(int test = 0; test < test_count; test++) {
    int airplane_count;
    cin >> airplane_count;

    // read minimum distance
    K::FT input_dist;
    cin >> input_dist;

    K::FT min_dist = input_dist * input_dist;

    // read in airplane location
    vector<pair<Point, int> > airplanes;
    airplanes.reserve(airplane_count);
    for(int airplane_index = 0; airplane_index < airplane_count; airplane_index++) {
      int x, y;
      cin >> x >> y;

      airplanes.push_back(make_pair(Point(x, y), airplane_index));
    }

    // keeps track of planes on collision course
    vector<bool> collisions(airplane_count, false);

    // triangulate planes
    Triangulation airplane_triang;
    airplane_triang.insert(airplanes.begin(), airplanes.end());

    // iterate trough all edges
    for(Finite_edges_iterator edge_iter = airplane_triang.finite_edges_begin();
      edge_iter != airplane_triang.finite_edges_end();
      ++edge_iter) {
      // get the airplanes
      Vertex_handle plane1_vertex = edge_iter->first->vertex((edge_iter->second + 1) % 3);
      Vertex_handle plane2_vertex = edge_iter->first->vertex((edge_iter->second + 2) % 3);

      Point plane1 = plane1_vertex->point();
      Point plane2 = plane2_vertex->point();
      //cout << "plane1: " << plane1 << ", plane2: " << plane2 << endl;

      int plane1_index = plane1_vertex->info();
      int plane2_index = plane2_vertex->info();
      //cout << "\tindex: plane1: " << plane1_index << ", plane2: " << plane2_index << endl;

      // check if distance is not violated
      K::FT plane_dist = CGAL::squared_distance(plane1, plane2);
      //cout << "\tdist: " << plane_dist << endl;

      if(plane_dist < min_dist) {
        collisions.at(plane1_index) = true;
        collisions.at(plane2_index) = true;
      }
    }

    // calculate how many airplanes have a plane not far enough away
    int planes_in_danger = 0;
    for(int plane_index = 0; plane_index < airplane_count; plane_index++) {
      if(collisions.at(plane_index)) {
        planes_in_danger++;
      }
    }

    cout << planes_in_danger << endl;
  }

  return 0;
}
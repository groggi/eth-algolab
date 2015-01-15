#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>

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

int main() {
  // some basic setup stuff
  cin.sync_with_stdio(false);
    cout.sync_with_stdio(false);
  cout << fixed << setprecision(0);

  while(true) {
      int graype_count;
      cin >> graype_count;

      // kill switch for application
      if(graype_count == 0) {
        break;
      }

      // collect graype locations
      vector<K::Point_2> graype_locs;
      graype_locs.reserve(graype_count);

      for(int i = 0; i < graype_count; i++) {
        double graype_x, graype_y;
        cin >> graype_x >> graype_y;

        graype_locs.push_back(K::Point_2(graype_x, graype_y));
      }

      // construct triangulation
      Triangulation triang;
      triang.insert(graype_locs.begin(), graype_locs.end());

      // go trough apes, and search for shortest edge
      K::FT min_time;
      bool first = true;
      for (Edge_iterator edge = triang.finite_edges_begin(); edge != triang.finite_edges_end(); ++edge) {
        K::FT edge_time = triang.segment(edge).squared_length();
        if(edge_time < min_time || first) {
            first = false;
            min_time = edge_time;
        }
      }

      // calculate time to run
      cout << ceil(sqrt(CGAL::to_double(min_time)) / 2.0 * 100.0) << endl;
    }
}
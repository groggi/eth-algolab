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
      int existing_count;
      cin >> existing_count;

      // kill switch for application
      if(existing_count == 0) {
        break;
      }

      // collect existing restaurants
      vector<K::Point_2> existing_locs;
      existing_locs.reserve(existing_count);

      for(int i=0; i < existing_count; i++) {
        double loc_x, loc_y;
        cin >> loc_x >> loc_y;

        existing_locs.push_back(K::Point_2(loc_x, loc_y));
      }

      // cosntruct triangulation
      Triangulation triang;
      triang.insert(existing_locs.begin(), existing_locs.end());

      // go through possible location
      int possible_count;
      cin >> possible_count;

      for(int i = 0; i < possible_count; i++) {
        int possible_x, possible_y;
        cin >> possible_x >> possible_y;
        
        K::Point_2 possible_point = K::Point_2(possible_x, possible_y);

        // find nearest vertex and by that the nearest point
        K::Point_2 nearest = triang.nearest_vertex(possible_point)->point();
        cout << CGAL::to_double(CGAL::squared_distance(nearest, possible_point)) << endl;
      }
    }
}
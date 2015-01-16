#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Point_set_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;
typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef Triangulation::Edge_iterator  Edge_iterator;

typedef K::Point_2 Point;
typedef K::Circle_2 Circle;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Point_set_2<K, Tds> PSet;
typedef CGAL::Point_set_2<K, Tds>::Vertex_handle PSVertex_handle;

typedef CGAL::Delaunay_triangulation_2<K>  Triangulation;
typedef CGAL::Delaunay_triangulation_2<K>::Vertex_handle  TriangVertex_handle;

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

  int test_count;
  cin >> test_count;
  for(int test = 0; test < test_count; test++) {
    int healthy_count, cancer_count;
    cin >> healthy_count >> cancer_count;

    // read in healthy and cancer cells
    vector<Point> cancer_cells;
    cancer_cells.reserve(cancer_count);

    vector<Point> healthy_cells;
    healthy_cells.reserve(healthy_count);

    for(int healthy_index = 0; healthy_index < healthy_count; healthy_index++) {
      double x, y;
      cin >> x >> y;

      healthy_cells.push_back(Point(x, y));
    }

    for(int cancer_index = 0; cancer_index < cancer_count; cancer_index++) {
      double x, y;
      cin >> x >> y;

      cancer_cells.push_back(Point(x, y));
    }

    // create a point set of cancer cells, this allows us to find all cells inside a circle quickly (I hope!)
    PSet cancer_triang(cancer_cells.begin(), cancer_cells.end());

    // create a triangulation of healthy cells to find quickly the nearest one to a cancer point
    Triangulation healthy_triang(healthy_cells.begin(), healthy_cells.end());

    // iterate over all cancer cells to search for the best radius
    unsigned long max_cells_killed = 0;
    for(int cancer_index = 0; cancer_index < cancer_count; cancer_index++) {
      // gett the point of the cancer cell
      Point cancer_cell = cancer_cells.at(cancer_index);

      // get the nearest healthy cell, defining by that the radius
      TriangVertex_handle next_healthy = healthy_triang.nearest_vertex(cancer_cell);

      // create the circle which will be radiated
      Circle radiation_region = Circle(cancer_cell, CGAL::squared_distance(cancer_cell, next_healthy->point()));

      // get the cancer cells that would be eliminated
      vector<PSVertex_handle> killed_cancer_cells;
      cancer_triang.range_search(radiation_region, back_inserter(killed_cancer_cells));

      max_cells_killed = max(max_cells_killed, killed_cancer_cells.size());
    }

    cout << max_cells_killed << endl;
  }
}
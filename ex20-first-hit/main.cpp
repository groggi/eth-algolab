#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
typedef K::Ray_2 Ray;

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

inline void laser_obsticale_segment(Segment &lo_segment, CGAL::Object intersec_obj) {
  if (const Point* op = CGAL::object_cast<Point>(&intersec_obj)) {
    lo_segment = Segment(lo_segment.source(), *op);
  } else if(const Segment* os = CGAL::object_cast<Segment>(&intersec_obj)) {
    // ray hits a segment, three possibilities
    if(CGAL::collinear_are_ordered_along_line(lo_segment.source(), (*os).source(), (*os).target())) {
      // order is: laser source -> start of segment -> end of segment
      lo_segment = Segment(lo_segment.source(), (*os).source());
    } else {
      lo_segment = Segment(lo_segment.source(), (*os).target());
    }
  } else { 
    throw runtime_error("Bad Wolf");
  }
}

int main() {
  // some basic setup stuff
  cin.sync_with_stdio(false);
  cout.sync_with_stdio(false);
  std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(0);
  //cout << fixed << setprecision(0);
  
  while(true) {
    int obsticale_count;
    cin >> obsticale_count;
    
    // kill switch for application
    if(obsticale_count == 0) {
      break;
    }
    
    // get the laser
    double ray_start_x, ray_start_y, ray_other_x, ray_other_y;
    cin >> ray_start_x >> ray_start_y >> ray_other_x >> ray_other_y;
    Ray laser_ray = Ray(Point(ray_start_x, ray_start_y), Point(ray_other_x, ray_other_y));

    vector<Segment> obsticle_segments(obsticale_count);

    // read in and create obsticle list
    for(int i = 0; i < obsticale_count; i++) {
      double obsticle_start_x, obsticle_start_y, obsticle_end_x, obsticle_end_y;
      cin >> obsticle_start_x >> obsticle_start_y >> obsticle_end_x >> obsticle_end_y;
      
      obsticle_segments[i] = Segment(Point(obsticle_start_x, obsticle_start_y), Point(obsticle_end_x, obsticle_end_y));
    }

    random_shuffle(obsticle_segments.begin(), obsticle_segments.end());

    // segment that starts at the source of the laser and ends, after our algo is done, at the point where the laser hits an
    // obsticle.

    // search for one intersaction between the laser ray and an obsticle, create a segment that starts at the laser source and ends
    // at the point where the laser hits the obsticle
    bool hit_found = false;
    Segment lo_segment(laser_ray.source(), laser_ray.source());
    int obst_index = 0; // used to jump over already checked segments in the second for-loop

    for(; obst_index < obsticale_count; ++obst_index) {
      if(do_intersect(obsticle_segments[obst_index], laser_ray)) {
        hit_found = true;
        laser_obsticale_segment(lo_segment, intersection(obsticle_segments[obst_index], laser_ray));
        break;
      }
    }

    // check if we hit something
    if(!hit_found) {
      cout << "no" << endl;
      continue;
    }

    for(; obst_index < obsticale_count; ++obst_index) {
      if(do_intersect(lo_segment, obsticle_segments[obst_index])) {
        laser_obsticale_segment(lo_segment, intersection(obsticle_segments[obst_index], laser_ray));
      }
    }

    cout << floor_to_double(lo_segment.target().x()) << " " << floor_to_double(lo_segment.target().y()) << endl;
  }
}
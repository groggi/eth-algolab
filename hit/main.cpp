#include <CGAL/Exact_predicates_exact_constructions_kernel.h>
#include <iostream>
#include <stdexcept>

typedef CGAL::Exact_predicates_exact_constructions_kernel K;
typedef K::Point_2 Point;
typedef K::Segment_2 Segment;
typedef K::Ray_2 Ray;

using namespace std;

int main()
{
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

    // read in obsticles and check if we hit them with the ray
    bool hit = false;
    for(int i = 0; i < obsticale_count; i++) {
      // read in and create obsticle
      double obsticle_start_x, obsticle_start_y, obsticle_end_x, obsticle_end_y;
      cin >> obsticle_start_x >> obsticle_start_y >> obsticle_end_x >> obsticle_end_y;
      
      // we know we hit something, read input but do nothing with it :)
      if(hit) {
        continue;
      }

      Segment obsticle = Segment(Point(obsticle_start_x, obsticle_start_y), Point(obsticle_end_x, obsticle_end_y));

      // check if we hit it
      if(CGAL::do_intersect(laser_ray, obsticle)) {
        hit = true;
      }
    }

    if(hit) {
      cout << "yes" << endl;
    } else {
      cout << "no" << endl;
    }
  }
}

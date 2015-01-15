#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/basic.h>
#include <CGAL/squared_distance_2.h>
#include <CGAL/Triangulation_vertex_base_with_info_2.h>
#include <CGAL/Point_set_2.h>

typedef CGAL::Exact_predicates_inexact_constructions_kernel K;

typedef K::Point_2 Point;
typedef K::Circle_2 Circle;

typedef CGAL::Triangulation_vertex_base_with_info_2<int, K> Vb;
typedef CGAL::Triangulation_data_structure_2<Vb> Tds;
typedef CGAL::Point_set_2<K, Tds> PSet;
typedef CGAL::Point_set_2<K, Tds>::Vertex_handle Vertex_handle;

using namespace std;

int main() {
  // some basic setup stuff
  cin.sync_with_stdio(false);
  cout.sync_with_stdio(false);
  cout << fixed << setprecision(0);

  int test_count;
  cin >> test_count;

  for(int test = 0; test < test_count; test++) {
    int participant_count, lamp_count;
    cin >> participant_count >> lamp_count;

    // read in participants
    vector<K::Point_2> participant_locations(participant_count);
    vector<int> participant_radi(participant_count, -1);
    for(int participant_index = 0; participant_index < participant_count; participant_index++) {
      double p_x, p_y, p_r;
      cin >> p_x >> p_y >> p_r;

      participant_locations.at(participant_index) = K::Point_2(p_x, p_y);
      participant_radi.at(participant_index) = p_r;
    }

    // read in lamp information
    int lamp_height;
    cin >> lamp_height;

    // make it a vector of a pair. The moment we feed it in to Point_set_2 we can access the second
    // member of the pair by accessing ->info() :-)
    vector<pair<K::Point_2, int> > lamp_locations(lamp_count);
    for(int lamp_index = 0; lamp_index < lamp_count; lamp_index++) {
      double l_x, l_y;
      cin >> l_x >> l_y;

      lamp_locations.at(lamp_index) = make_pair(K::Point_2(l_x, l_y), lamp_index);
    }

    // create a triangulation of the lamps using Point_set_2
    PSet lamps_triang(lamp_locations.begin(), lamp_locations.end());

    // contains the ID of the light that hit a participant first, or MAX if he survived till the end
    vector<int> min_light_hit(participant_count, numeric_limits<int>::max());

    // keeps track of the max light that hit someone
    int max_light_hit = -1;

    // now we iterate over each person and find out which lamp hits each person first, i.e. how
    // long they survive
    for(int participant_index = 0; participant_index < participant_count; participant_index++) {
      K::Point_2 participant_point = participant_locations.at(participant_index); // we need it more than once :-)
      // get nearest lamp
      Vertex_handle lamp_vertex = lamps_triang.nearest_neighbor(participant_point);

      /* Geometry lesson:
       * We know that the light is a cone with given height and it's a 90 degree one:
       *
       *                x <---- lamp point
       *               /|\
       *              / | \
       *             g  h  g
       *            /   |   \
       *           ----------
       *                ^- `h` and the bottom have a 90 degree, therefore the triangle on the left/right (h, g and half of the bottom)
       * have a 90 degree and two 45 degrees, i.e. we can find out what the length of the bottom is (twice the bottom part of the triangle)
       * which is in this case 2 * `h` (bottom is of 2 * `h` length).
       */

      // calculate the distance from our participant that still could be hit by light to "kill" the
      // participant.
      // do it ^2 (squared) because we compare it to a squared distance
      double max_dist_to_kill = pow(participant_radi.at(participant_index) + lamp_height, 2);

      // if the nearest lamp is already too far away, we don't have to search for other lamps,
      // as they will even farther away
      if(max_dist_to_kill <= squared_distance(lamp_vertex->point(), participant_point)) {
        continue;
      }

      // iterate over all lamps in the search for one that hits the participant first
      for(int check_lamp_index = 0; check_lamp_index < lamp_count; check_lamp_index++) {
        if(max_dist_to_kill > squared_distance(lamp_locations.at(check_lamp_index).first, participant_point)) {
          // found such a lamp, stop searching as we only interested in the minimum!
          min_light_hit.at(participant_index) = lamp_locations.at(check_lamp_index).second;
          max_light_hit = max(max_light_hit, lamp_locations.at(check_lamp_index).second);
          break;
        }
      }

      /* nice idea, but too slow...
      
      // create a circle in which every light source would kill the current participant
      K::Circle_2 circle_of_death = K::Circle_2(participant_point, max_dist_to_kill);

      // create a vector of vertex handles that represent our lights that are inside the circle_of_death
      vector<Vertex_handle> lights_hitting_participant;

      // so, micro optimizing stuff: don't do the rest of the algorithm if the lamp ID is higher than the
      // already found one
      if(min_light_hit.at(participant_index) < lamp_vertex->info()) {
        continue;
      }

      // collect the lights "of death" :-)
      lamps_triang.range_search(circle_of_death, back_inserter(lights_hitting_participant));

      // iterate over the bad lights and write down the smallest light hitting
      for(vector<Vertex_handle>::iterator iter = lights_hitting_participant.begin();
        iter != lights_hitting_participant.end();
        ++iter) {
        int lamp_id = (*iter)->info();

        if(circle_of_death.has_on_boundary((*iter)->point())) {
          // point is on boundary, this is not a hit according to the exercise
          continue;
        }

        min_light_hit.at(participant_index) = min(min_light_hit.at(participant_index), lamp_id);
        max_light_hit = max(max_light_hit, lamp_id);
      }
      */
    }

    // now we have to extract for each light who survived the longest
    vector<int> rank_list;

    // first we search for participants which were not hit by a light
    for(int participant_index = 0; participant_index < participant_count; participant_index++) {
      if(min_light_hit.at(participant_index) == numeric_limits<int>::max()) {
        rank_list.push_back(participant_index);
      }
    }

    // if all participants were hit, we have to search for the ones that were hit last
    if(rank_list.size() <= 0) {
      for(int participant_index = 0; participant_index < participant_count; participant_index++) {
        if(min_light_hit.at(participant_index) == max_light_hit) {
          rank_list.push_back(participant_index);
        }
      }
    }

    // print out rank in sorted order
    sort(rank_list.begin(), rank_list.end());
    for(int i = 0; i < rank_list.size(); i++) {
      cout << rank_list.at(i) << " ";
    }
    cout << endl;
  }
}
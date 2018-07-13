#include "pitch.h"
#include "resource.h"

#include <sstream>
#include <fstream>
#include <iostream>
#include <iomanip>

extern uint32_t mesh_data[72252];

Pitch::Pitch() {

  tri * tri_ptr = (tri*)mesh_data;
  triangles = std::vector < tri >(tri_ptr, tri_ptr + 8028);

  mesh = bvh < tri >(triangles); 

}

bool Pitch::in_contact_with(const sphere & s) {

  hits = mesh.intersect(s);
  return hits.size() > 0;

}

bool Pitch::in_contact_with(const obb & o) {

  hits = mesh.intersect(o);
  return hits.size() > 0;

}

ray Pitch::last_contact_info() {

  float count = 0;
  vec3 pos{0.0f, 0.0f, 0.0f};
  vec3 normal{0.0f, 0.0f, 0.0f};

  for (int i = 0; i < hits.size(); i++) {
    normal += triangles[hits[i]].unit_normal();
    pos += triangles[hits[i]].center();
    count++;
  }

  return ray{pos / count, normalize(normal)};

}

vec3 closest_point_on_side(const vec3 & v) {
  return vec3{v[0], v[1], 4096.0f * sgn(v[2])};
}

#include "resource.h"
#include "primitives.h"

#include <vector>
#include <string>
#include <fstream>
#include <sstream>

std::string prefix(ASSET("intersections/"));

std::vector < aabb > read_aabbs(std::string filename) {

  std::vector < aabb > aabbs(0);
  std::ifstream infile(prefix + filename);

  if (!infile.good()) {
    std::cout << "file not found" << std::endl;
    exit(1);
  }

  std::string line;
  while (std::getline(infile, line)) {       

    std::stringstream ss(line);

    aabb box;
    
    ss >> box.min_x;
    ss >> box.min_y;
    ss >> box.min_z;

    ss >> box.max_x;
    ss >> box.max_y;
    ss >> box.max_z;

    aabbs.push_back(box);

  }

  infile.close(); 
  return aabbs;

}

std::vector < sphere > read_spheres(std::string filename) {

  std::vector < sphere > spheres(0);
  std::ifstream infile(prefix + filename);

  if (!infile.good()) {
    std::cout << "file not found" << std::endl;
    exit(1);
  }

  std::string line;
  while (std::getline(infile, line)) {       

    std::stringstream ss(line);

    sphere s;
    
    ss >> s.center[0];
    ss >> s.center[1];
    ss >> s.center[2];

    ss >> s.radius;

    spheres.push_back(s);

  }

  infile.close(); 
  return spheres;

}

std::vector < tri > read_tris(std::string filename) {

  std::vector < tri > tris(0);
  std::ifstream infile(prefix + filename);

  if (!infile.good()) {
    std::cout << "file not found" << std::endl;
    exit(1);
  }

  std::string line;
  while (std::getline(infile, line)) {       

    std::stringstream ss(line);

    tri t;
    
    ss >> t.p[0][0];
    ss >> t.p[0][1];
    ss >> t.p[0][2];

    ss >> t.p[1][0];
    ss >> t.p[1][1];
    ss >> t.p[1][2];

    ss >> t.p[2][0];
    ss >> t.p[2][1];
    ss >> t.p[2][2];

    tris.push_back(t);

  }

  infile.close(); 
  return tris;

}

std::vector < obb > read_obbs(std::string filename) {

  std::vector < obb > obbs(0);
  std::ifstream infile(prefix + filename);

  if (!infile.good()) {
    std::cout << "file not found" << std::endl;
    exit(1);
  }

  std::string line;
  while (std::getline(infile, line)) {       

    std::stringstream ss(line);

    obb o;
    
    ss >> o.center[0];
    ss >> o.center[1];
    ss >> o.center[2];

    ss >> o.orientation(0, 0);
    ss >> o.orientation(0, 1);
    ss >> o.orientation(0, 2);

    ss >> o.orientation(1, 0);
    ss >> o.orientation(1, 1);
    ss >> o.orientation(1, 2);

    ss >> o.orientation(2, 0);
    ss >> o.orientation(2, 1);
    ss >> o.orientation(2, 2);

    ss >> o.half_width[0];
    ss >> o.half_width[1];
    ss >> o.half_width[2];

    obbs.push_back(o);

  }

  infile.close(); 
  return obbs;

}

std::vector < int > read_ints(std::string filename) {

  std::ifstream infile(prefix + filename);

  if (!infile.good()) {
    std::cout << "file not found" << std::endl;
    exit(1);
  }

  int n;
  infile >> n;

  std::vector < int > ints(n);
  for (int i = 0; i < n ; i++) {
    infile >> ints[i];
  }

  infile.close(); 
  return ints;

}

template < typename S, typename T >
void check_intersections(
    const std::vector < S > & s_primitives,
    const std::vector < T > & t_primitives,
    const std::vector < int > expected) {

  size_t ns = s_primitives.size();
  size_t nt = t_primitives.size();

  if (expected.size() != ns * nt) {
    std::cout << "incorrect number of expected results" << std::endl;
    exit(1);
  }

  for (int s = 0; s < ns; s++) {
    for (int t = 0; t < nt; t++) {

      int intersected = intersect(s_primitives[s], t_primitives[t]);

      if (intersected != expected[nt * s + t]) {
        std::cout << "incorrect result between " << s << " and " << t << std::endl;
      }

    }
  }

  std::cout << "PASSED" << std::endl;

}

int main() {

  std::vector < int > results;

  auto aabbs = read_aabbs("aabbs.txt");
  auto obbs = read_obbs("obbs.txt");
  auto tris = read_tris("tris.txt");
  auto spheres = read_spheres("spheres.txt");

  results = read_ints("aabb_aabb.txt");
  std::cout << "Checking aabb-aabb intersections... ";
  check_intersections(aabbs, aabbs, results);
  
  results = read_ints("aabb_obb.txt");
  std::cout << "Checking aabb-obb intersections... ";
  check_intersections(aabbs, obbs, results);

  results = read_ints("aabb_tri.txt");
  std::cout << "Checking aabb-tri intersections... ";
  check_intersections(aabbs, tris, results);

  results = read_ints("aabb_sphere.txt");
  std::cout << "Checking aabb-sphere intersections... ";
  check_intersections(aabbs, spheres, results);


  results = read_ints("obb_aabb.txt");
  std::cout << "Checking obb-aabb intersections... ";
  check_intersections(obbs, aabbs, results);
  
  results = read_ints("obb_obb.txt");
  std::cout << "Checking obb-obb intersections... ";
  check_intersections(obbs, obbs, results);

  results = read_ints("obb_tri.txt");
  std::cout << "Checking obb-tri intersections... ";
  check_intersections(obbs, tris, results);

  results = read_ints("obb_sphere.txt");
  std::cout << "Checking obb-sphere intersections... ";
  check_intersections(obbs, spheres, results);


  results = read_ints("tri_aabb.txt");
  std::cout << "Checking tri-aabb intersections... ";
  check_intersections(tris, aabbs, results);
  
  results = read_ints("tri_obb.txt");
  std::cout << "Checking tri-obb intersections... ";
  check_intersections(tris, obbs, results);

  results = read_ints("tri_tri.txt");
  std::cout << "Checking tri-tri intersections... ";
  check_intersections(tris, tris, results);

  results = read_ints("tri_sphere.txt");
  std::cout << "Checking tri-sphere intersections... ";
  check_intersections(tris, spheres, results);


  results = read_ints("sphere_aabb.txt");
  std::cout << "Checking sphere-aabb intersections... ";
  check_intersections(spheres, aabbs, results);
  
  results = read_ints("sphere_obb.txt");
  std::cout << "Checking sphere-obb intersections... ";
  check_intersections(spheres, obbs, results);

  results = read_ints("sphere_tri.txt");
  std::cout << "Checking sphere-tri intersections... ";
  check_intersections(spheres, tris, results);

  results = read_ints("sphere_sphere.txt");
  std::cout << "Checking sphere-sphere intersections... ";
  check_intersections(spheres, spheres, results);

  return 0;

}

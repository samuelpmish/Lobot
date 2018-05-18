#include "pitch.h"

#include <sstream>
#include <fstream>
#include <iostream>

Pitch::Pitch() {

  std::string filename("../doc/meshes/pitch.dat");
  std::ifstream infile(filename);

  if (!infile.good()) {

    std::cout << "file not found" << std::endl;
    exit(1);

  } else {

    tri t;
    std::string line;

    while (std::getline(infile, line)) {       
  
      std::stringstream ss(line); 
      
      ss >> t.p[0][0]; 
      ss >> t.p[0][1]; 
      ss >> t.p[0][2];
  
      ss >> t.p[1][0];
      ss >> t.p[1][1];
      ss >> t.p[1][2];
  
      ss >> t.p[2][0];
      ss >> t.p[2][1];
      ss >> t.p[2][2];
  
      triangles.push_back(t);
  
    }
  
    infile.close(); 

    std::cout << "successfully read " << triangles.size() << " triangles" << std::endl;

    mesh = bvh < tri >(triangles); 

  }

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

  int count = 0;
  vec3 pos{0.0f, 0.0f, 0.0f};
  vec3 normal{0.0f, 0.0f, 0.0f};

  for (int i = 0; i < hits.size(); i++) {
    normal += triangles[hits[i]].unit_normal();
    pos += triangles[hits[i]].center();
    count++;
  }

  return ray{pos / count, normalize(normal)};

}

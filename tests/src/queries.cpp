#include "bvh.h"
#include "timer.h"
#include "pitch.h"

#include <random>
#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

int main() {

  Pitch p;

  timer stopwatch;
  double bvh_time = 0.0f;
  double brute_time = 0.0f;

  std::default_random_engine gen;
  std::uniform_real_distribution< float > distr(-6000.0f, 6000.0f);
  
  std::cout << "Starting tests" << std::endl;

  size_t nqueries = 1000;
  for (int i = 0; i < nqueries; i++) {

    sphere s;

    s.radius = 91.25;
    s.center[0] = distr(gen);
    s.center[1] = distr(gen);
    s.center[2] = distr(gen);

    stopwatch.start();
    p.in_contact_with(s);
    stopwatch.stop();
    bvh_time += stopwatch.elapsed();

    std::vector < int > brute_hits;

    stopwatch.start();
    for (int j = 0; j < p.triangles.size(); j++) {
      if (intersect(p.triangles[j], s)){
        brute_hits.push_back(j);
      }
    }
    stopwatch.stop();
    brute_time += stopwatch.elapsed();

    std::sort(p.hits.begin(), p.hits.end());
    std::sort(brute_hits.begin(), brute_hits.end());

    if (p.hits.size() == brute_hits.size()) {
      for (int j = 0; j < p.hits.size(); j++) {
        if (p.hits[j] != brute_hits[j]) {
          std::cout << "trial " << i << " has different hits" << std::endl;
          break;
        }
      }
    } else {
      std::cout << "trial " << i << " has different number of hits" << std::endl;
      std::cout << "bvh hits: " << p.hits.size() << std::endl;
      std::cout << "brute hits: " << brute_hits.size() << std::endl;
    }

  }

  std::cout << "completed " << nqueries << " queries in ";
  std::cout << bvh_time << " & " << brute_time << " seconds";
  std::cout << std::endl;

}

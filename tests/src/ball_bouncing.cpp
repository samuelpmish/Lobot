#include "ball.h"
#include "timer.h"
#include "DynamicState.h"

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

void write_to_file(std::string filename,
    const std::vector < DynamicState > & states) {

  std::ofstream outfile(filename);

  for (int i = 0; i < states.size(); i++) {

    outfile << states[i].x[0] << ", ";
    outfile << states[i].x[1] << ", ";
    outfile << states[i].x[2] << ", ";
    outfile << states[i].v[0] << ", ";
    outfile << states[i].v[1] << ", ";
    outfile << states[i].v[2] << ", ";
    outfile << states[i].w[0] << ", ";
    outfile << states[i].w[1] << ", ";
    outfile << states[i].w[2] << std::endl;

  }

  outfile.close(); 

}

std::vector < DynamicState > read_from_file(std::string filename) {

  float t;
  DynamicState s;
  std::vector < DynamicState > states;

  std::ifstream infile(filename);

  if (!infile.good()) {

    std::cout << "file not found" << std::endl;
    exit(1);

  } else {

    std::string line;
    while (std::getline(infile, line)) {       
  
      line.erase(std::remove(line.begin(), line.end(), ','), line.end());
      std::stringstream ss(line);
      
      // discard time stamp
      ss >> t;

      // position
      ss >> s.x[0];
      ss >> s.x[1];
      ss >> s.x[2];
  
      // velocity
      ss >> s.v[0];
      ss >> s.v[1];
      ss >> s.v[2];

      // discard euler angle info
      ss >> t; 
      ss >> t;
      ss >> t;
  
      // angular velocity
      ss >> s.w[0];
      ss >> s.w[1];
      ss >> s.w[2];
  
      states.push_back(s);
  
    }
  
    infile.close(); 

  }

  return states;

}

vec3 compare(const std::vector < DynamicState > & obs,
             const std::vector < DynamicState > & pred) {

  vec3 norms{0.0f, 0.0f, 0.0f};
  vec3 error{0.0f, 0.0f, 0.0f};

  for (int i = 0; i < obs.size(); i++) {
    error[0] += norm(obs[i].x - pred[i].x);
    error[1] += norm(obs[i].v - pred[i].v);
    error[2] += norm(obs[i].w - pred[i].w);

    norms[0] += norm(obs[i].x);
    norms[1] += norm(obs[i].v);
    norms[2] += norm(obs[i].w);
  }

  return vec3{error[0] / norms[0], 
              error[1] / norms[1], 
              error[2] / norms[2]};

}

int main() {

  const float dt = 1.0 / 61.0;
  char buf[] = "000000.csv";
  int n_episodes = 344;
  std::string in_prefix("../doc/ball_trajectories/episode_");
  std::string out_prefix("../doc/ball_trajectories/prediction_");

  Ball b;
  timer stopwatch;
  double time = 0.0;
  uint32_t count = 0;
  vec3 total_error{0.0f, 0.0f, 0.0f};

  for (int i = 0; i < n_episodes; i++) {

    sprintf(buf, "%06d.csv", i);
    std::string suffix(buf);

    std::cout << in_prefix + suffix << std::endl;

    auto observations = read_from_file(in_prefix + suffix);

    b.x = observations[0].x;
    b.v = observations[0].v;
    b.w = observations[0].w;

    std::vector < DynamicState > predictions; 
    predictions.reserve(observations.size());
    stopwatch.start();
    predictions.push_back(DynamicState{b.x, b.v, b.w, eye<3>()});
    while (predictions.size() < observations.size()) {
      b.step(dt * 0.5f);
      b.step(dt * 0.5f);
      predictions.push_back(DynamicState{b.x, b.v, b.w, eye<3>()});
    }
    stopwatch.stop();

    write_to_file(out_prefix + suffix, predictions);

    vec3 error = compare(observations, predictions);
    //std::cout << error << std::endl;

    time += stopwatch.elapsed();
    count += 2 * observations.size();
    total_error += error;

  }

  std::cout << b.count << std::endl;

  std::cout << "evaluated " << count << " steps in " << time;
  std::cout << "s (" << time / double(count) << "s / step)" << std::endl;
  std::cout << "total average relative errors: " << std::endl;
  std::cout << total_error / n_episodes << std::endl;

}

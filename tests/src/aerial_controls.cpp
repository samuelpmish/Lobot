#include "car.h"
#include "timer.h"
#include "RL_structs.h"

#include "resource.h"

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

void write_to_file(std::string filename,
    const std::vector < Car::state > & states) {

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

std::vector < std::pair < Car::state, Input > > read_from_file(std::string filename) {

  Input in;
  Car::state s;
  std::vector < std::pair < Car::state, Input > > data;

  std::ifstream infile(filename);

  if (!infile.good()) {

    std::cout << "file not found" << std::endl;
    exit(1);

  } else {

    std::string line;
    while (std::getline(infile, line)) {       
  
      std::replace(line.begin(), line.end(), ',', ' ');
      std::stringstream ss(line);
      
      // position
      ss >> s.x[0];
      ss >> s.x[1];
      ss >> s.x[2];
  
      // velocity
      ss >> s.v[0];
      ss >> s.v[1];
      ss >> s.v[2];

      // discard euler angle info
      vec3 euler_angles;
      ss >> euler_angles[0];
      ss >> euler_angles[1];
      ss >> euler_angles[2];
      s.o = ea_rotation(euler_angles * 0.0000958738f);
  
      // angular velocity
      ss >> s.w[0];
      ss >> s.w[1];
      ss >> s.w[2];

      // other car state variables
      ss >> s.supersonic;
      ss >> s.jumped;
      ss >> s.double_jumped;
      ss >> s.on_ground;
      ss >> s.boost;
      

      // input variables
      ss >> in.throttle;
      ss >> in.steer;
      ss >> in.pitch;
      ss >> in.yaw;
      ss >> in.roll;
      ss >> in.jump;
      ss >> in.boost;
      ss >> in.slide;
  
      data.push_back(std::make_pair(s, in));
  
    }
  
    infile.close(); 

  }

  return data;

}

vec3 compare(const std::vector < std::pair < Car::state, Input > > & obs,
             const std::vector < Car::state > & pred) {

  vec3 norms{0.0f, 0.0f, 0.0f};
  vec3 error{0.0f, 0.0f, 0.0f};

  for (int i = 0; i < obs.size(); i++) {
    error[0] += norm(obs[i].first.x - pred[i].x);
    error[1] += norm(obs[i].first.v - pred[i].v);
    error[2] += norm(obs[i].first.w - pred[i].w);

    norms[0] += norm(obs[i].first.x);
    norms[1] += norm(obs[i].first.v);
    norms[2] += norm(obs[i].first.w);
  }

  return vec3{error[0] / norms[0], 
              error[1] / norms[1], 
              error[2] / norms[2]};

}

int main() {

  const float dt = 1.0f / 61.0f;
  char buf[] = "00.csv";
  int n_episodes = 15;
  std::string in_prefix(ASSET("car_control/aerial/episode_"));
  std::string out_prefix(ASSET("car_control/aerial/prediction_"));

  Car c;
  timer stopwatch;
  double time = 0.0;
  size_t count = 0;
  vec3 total_error{0.0f, 0.0f, 0.0f};

  for (int i = 0; i < n_episodes; i++) {

    sprintf(buf, "%02d.csv", i);
    std::string suffix(buf);

    std::cout << in_prefix + suffix << std::endl;

    auto data = read_from_file(in_prefix + suffix);

    c.s = data[0].first;

    std::vector < Car::state > predictions; 
    predictions.reserve(data.size());
    stopwatch.start();
    predictions.push_back(c.s);
    for (int i = 0; i < data.size() - 1; i++) {
      c.step(data[i].second, dt);
      predictions.push_back(c.s);
    }
    stopwatch.stop();

    write_to_file(out_prefix + suffix, predictions);

    vec3 error = compare(data, predictions);

    time += stopwatch.elapsed();
    count += data.size();
    total_error += error;

  }

  std::cout << "evaluated " << count << " steps in " << time;
  std::cout << "s (" << time / double(count) << "s / step)" << std::endl;
  std::cout << "total average relative errors: " << std::endl;
  std::cout << total_error / n_episodes << std::endl;

}

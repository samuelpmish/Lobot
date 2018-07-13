#include "car.h"
#include "timer.h"
#include "RL_structs.h"

#include "resource.h"

#include <string>
#include <sstream>
#include <fstream>
#include <algorithm>

void write_to_file(std::string filename,
    const std::vector < Car > & states) {

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

std::vector < std::pair < Car, Input > > read_from_file(std::string filename) {

  Input in;
  Car c;
  std::vector < std::pair < Car, Input > > data;

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
      ss >> c.x[0];
      ss >> c.x[1];
      ss >> c.x[2];
  
      // velocity
      ss >> c.v[0];
      ss >> c.v[1];
      ss >> c.v[2];

      // discard euler angle info
      vec3 euler_angles;
      ss >> euler_angles[0];
      ss >> euler_angles[1];
      ss >> euler_angles[2];
      c.o = euler_rotation(euler_angles * 0.0000958738f);
  
      // angular velocity
      ss >> c.w[0];
      ss >> c.w[1];
      ss >> c.w[2];

      // other car state variables
      ss >> c.supersonic;
      ss >> c.jumped;
      ss >> c.double_jumped;
      ss >> c.on_ground;
      ss >> c.boost;
      

      // input variables
      ss >> in.throttle;
      ss >> in.steer;
      ss >> in.pitch;
      ss >> in.yaw;
      ss >> in.roll;
      ss >> in.jump;
      ss >> in.boost;
      ss >> in.slide;
  
      data.push_back(std::make_pair(c, in));
  
    }
  
    infile.close(); 

  }

  return data;

}

vec3 compare(const std::vector < std::pair < Car, Input > > & obs,
             const std::vector < Car > & pred) {

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
  int n_episodes = 46;
  std::string in_prefix(ASSET("car_control/ground_driving_no_handbrake/episode_"));
  std::string out_prefix(ASSET("car_control/ground_driving_no_handbrake/prediction_"));

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

    c = data[0].first;

    std::vector < Car > predictions; 
    predictions.reserve(data.size());
    stopwatch.start();
    predictions.push_back(c);
    for (int i = 0; i < data.size() - 1; i++) {
      c.step(data[i].second, dt);
      predictions.push_back(c);
    }
    stopwatch.stop();

    write_to_file(out_prefix + suffix, predictions);

    vec3 error = compare(data, predictions);
    std::cout << error << std::endl;

    time += stopwatch.elapsed();
    count += data.size();
    total_error += error;

  }

  std::cout << "evaluated " << count << " steps in " << time;
  std::cout << "s (" << time / double(count) << "s / step)" << std::endl;
  std::cout << "total average relative errors: " << std::endl;
  std::cout << total_error / n_episodes << std::endl;

}

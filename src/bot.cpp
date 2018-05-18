#pragma once

#include <thread>
#include <pybind11/pybind11.h>

#include "RL_structs.h"

class Bot {

public:

  Bot(int id) {
    player_id = id;
  }

  Input get_response(LiveDataPacket game_state) {

    // Do something
    return  Input{
      1.0,   // Throttle
      0.0,   // Steer
      0.0,   // Pitch
      0.0,   // Yaw
      0.0,   // Roll
      false, // Jump
      false, // Boost
      false  // Handbrake
    };

  }

private:

  int player_id;

};

PYBIND11_MODULE(ibrahimobot, m) {
  pybind11::class_<Bot>(m, "Bot")
    .def(pybind11::init<int>())
    .def("get_response", &Bot::get_response);
}

#pragma once

#include "Ball.h"
#include "Pitch.h"
#include "DynamicState.h"
#include "RL_structs.h"

#define DELTA_T 0.016388

class Car {

  public:

    int boost;
    float dodge_timer;
    bool on_ground;
    DynamicState current;

    Pitch env;
    Ball ball;

    obb collider;
    vec3 pivot_offset;

    Car();

    void step(Input in, float dt = DELTA_T);

    //void integrate(Input in, float dt) {

    //  const double boost_acc = 1000.0;

    //  vec3 a = drag * current.v + vec3{0.0, 0.0, g}; 

    //  if (in.boost && boost > 0) {
    //    a += dot(current.o, vec3{1.0, 0.0, 0.0}) * boost_acc;
    //  }

    //  vec3 T{0.0, 0.0, 0.0};

    //  if (onGround) {

    //  } else {

    //  }

    //  DynamicState predicted{
    //    current.x + current.v * dt,
    //    current.v + a * dt,
    //    current.w,
    //    dot(rotation(current.w * dt), current.o)
    //  };

    //}

};

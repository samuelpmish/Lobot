#pragma once

#include "Ball.h"
#include "Pitch.h"
#include "DynamicState.h"
#include "RL_structs.h"

#define DELTA_T 0.016388

class Car {

  public:

    struct state {
  
      vec3 x;
      vec3 v;
      vec3 w;
      mat3 o;
  
      bool supersonic;
      bool jumped;
      bool double_jumped;
      bool on_ground;
      
      int boost;
  
    };

    state s;

    bool can_dodge;
    float dodge_timer;

    Pitch env;
    Ball ball;

    obb collider;
    vec3 pivot_offset;

    Car();

    void step(Input in, float dt = DELTA_T);

  private:

    void jump(const Input & in, float dt);
    void air_dodge(const Input & in, float dt);
    void aerial_control(const Input & in, float dt);

    float drive_force_left(const Input & in);
    float drive_force_forward(const Input & in);
    float drive_torque_up(const Input & in);
    void driving(const Input & in, float dt);

    void driving_handbrake(const Input & in, float dt);

    vec3 forward();
    vec3 left();
    vec3 up();

    void check_collision();

};

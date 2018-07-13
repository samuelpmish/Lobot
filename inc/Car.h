#pragma once

#include "Ball.h"
#include "Pitch.h"
#include "DynamicState.h"
#include "RL_structs.h"

#define DELTA_T 0.016388

class Car {

  public:

    vec3 x;
    vec3 v;
    vec3 w;
    mat3 o;
  
    bool supersonic;
    bool jumped;
    bool double_jumped;
    bool on_ground;
    
    int boost;

    bool can_dodge;
    float dodge_timer;

    sphere sphere_collider;
    obb obb_collider;
    vec3 pivot_offset;

    float time;
    float ETA; 

    vec3 target;

    Input last;

    Car();

    void step(Input in = Input(), float dt = DELTA_T);
    vec3 pitch_surface_normal();
    obb bounding_box();
    void extrapolate(float);

    vec3 forward();
    vec3 left();
    vec3 up();

  private:

    static Pitch env;

    void jump(const Input & in, float dt);
    void air_dodge(const Input & in, float dt);
    void aerial_control(const Input & in, float dt);

    float drive_force_left(const Input & in);
    float drive_force_forward(const Input & in);
    float drive_torque_up(const Input & in);
    void driving(const Input & in, float dt);

    void driving_handbrake(const Input & in, float dt);

    void check_collision();

};

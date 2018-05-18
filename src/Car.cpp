#include "Car.h"

#if 1

//enum d_state {
//  braking = 1,
//  accelerating = 1 << 1,
//  boosting = 1 << 2, 
//  speed_slow = 1 << 3,
//  speed_medium = 1 << 4,
//  speed_high = 1 << 5
//};

float w_target(float vf) {

  vec < 6 > breaks{0.0f, 500.0f, 1000.0f, 1500.0f, 1750.0f, 2500.0f};

  if (breaks[0] <= vf && vf < breaks[1]) {
		return  0.2719536733704970f + vf * (0.001658118981246462f - vf * 3.96113308697317e-7);
  } else if (breaks[1] <= vf && vf < breaks[2]) {
		return -1.5708421090289044f + vf * (0.004687995113863351f - vf * 1.52574186369491e-6);
  } else if (breaks[2] <= vf && vf < breaks[3]) {
		return -0.8142298712086391f + vf * (0.005629720388359599f - vf * 2.48983081905698e-6);
  } else if (breaks[3] <= vf && vf < breaks[4]) {
		return -0.0750149085518883f + vf * (0.005901886439895422f - vf * 3.50121183324955e-6);
  } else if (breaks[4] <= vf && vf < breaks[5]) {
		return  0.0361197319396348f + vf * (0.007166087839723428f - vf * 6.47415319487165e-6);
  } else {
    return 0.0f;
  }

}

void Car::step(Input in, float dt) {

  constexpr float driving_speed = 1450.0f;
  constexpr float braking_force = 3500.0f;
  constexpr float coasting_force = 525.0f;
  constexpr float throttle_threshold = 0.05f;
  constexpr float throttle_force = 1550.0f;
  constexpr float max_speed = 2275.0f;
  constexpr float min_speed = 10.0f;
  constexpr float boost_force = 1500.0f;
  constexpr float steering_torque = 25.75f;
  constexpr float braking_threshold = -10.0f;

  // the net force and torque on the car for this step
  vec3 F{0.0f, 0.0f, 0.0f};
  vec3 T{0.0f, 0.0f, 0.0f};

  vec3 forward = vec3{current.o(0, 0), current.o(1, 0), current.o(2, 0)};
  vec3 left    = vec3{current.o(0, 1), current.o(1, 1), current.o(2, 2)};
  vec3 up      = vec3{current.o(0, 2), current.o(1, 2), current.o(2, 1)};

  // driving
  if (on_ground) {

    // on the first frame where jump is pressed
    // reset the air dodge timer
    if (in.jump == 1) {
      
      on_ground = false;
      dodge_timer = 0.0f;

      F += vec3{0.0f, 0.0f, 1.0f};

    } else {

      // force and velocity in the "forward" direction of the car
      float Ff = 0.0f;
      float vf = dot(current.v, forward);

      // velocity in the "left" direction of the car
      float Fl = 0.0f;
      float vl = dot(current.v, left);

      // torque and angular velocity in the "up" direction of the car
      float Tu = 0.0f;
      float wu = dot(current.w, up);

      // braking
      if (sgn(vf) * sgn(fmaxf(in.throttle, in.boost)) < braking_threshold) {

        Ff = braking_force * sgn(vf); 

      // not braking
      } else {

        // boosting
        if (in.boost) {

          // while moving slowly
          if (vf < driving_speed) {

            Ff = max_speed - fabs(vf); 

          // while moving quickly
          } else {

            // supersonic
            if (vf > max_speed) {

              Ff = supersonic_turn_drag * wu;

            // approaching supersonic
            } else {

              Ff = boost_force;

            }

          }

        // not boosting
        } else {

          // coasting
          if ((fabs(in.throttle) < throttle_threshold) && 
              (fabs(vf) < min_speed)) {

            Ff = coasting_force * sgn(vf);

          // driving
          } else {

            // while moving slowly
            if (fabs(vf) < driving_speed) {



            // while moving quickly
            } else {


            }

          }

        }

      }


//      // lateral forces on the car
//      F += (1.081f * in.throttle + 1438.9f * in.steer - 15.356f * vl + 644.85 * wu) * 
//					(1.0f - exp(0.00115325 * fabs(vf))) * left;
//
//      // steering produces a "yaw" torque about the "up" axis
//      T += steering_torque * (in.steer - current.w / (w_target(fabs(vf)) + 0.001f)) * up;

        F = Ff * forward + Fl * left;
        T = Tu * up;

    }

  // aerial
  } else {



    dodge_timer += dt;

  }

}
#endif

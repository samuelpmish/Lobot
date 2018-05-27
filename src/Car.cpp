#include "Car.h"

const float g = -650.0f;
const float w_max = 5.5f;
const float boost_accel = 1000.0f;
const float dodge_limit = 0.5f; // TODO

float kappa(float v) {

  if (0.0f <= v && v < 500.0f) {
		return  0.006900f - 5.84e-6 * v;
  } else if (500.0f <= v && v < 1000.0f) {
		return  0.005610f - 3.26e-6 * v;
  } else if (1000.0f <= v && v < 1500.0f) {
		return  0.004300f - 1.95e-6 * v;
  } else if (1500.0f <= v && v < 1750.0f) {
		return  0.003025f - 1.10e-6 * v;
  } else if (1750.0f <= v && v < 2500.0f) {
		return  0.001800f - 0.40e-6 * v;
  } else {
		return 0.0f;
  }

}

vec3 Car::forward() {

  return vec3{s.o(0, 0), s.o(1, 0), s.o(2, 0)};

}

vec3 Car::left() {

  return vec3{s.o(0, 1), s.o(1, 1), s.o(2, 1)};

}

vec3 Car::up() {

  return vec3{s.o(0, 2), s.o(1, 2), s.o(2, 2)};

}

void Car::jump(const Input & in, float dt) {
  
  // TODO

}

void Car::air_dodge(const Input & in, float dt) {
  
  // TODO

}

void Car::aerial_control(const Input & in, float dt) {

  const vec3 rpy{in.roll, in.pitch, in.yaw};
  const vec3 T{-36.080f, -12.146f,  8.920f};
  const vec3 D{
    -4.472f,
    -2.798f * (1.0f - fabs(in.pitch)), 
    -1.886f * (1.0f - fabs(in.yaw))
  };

  s.v[2] += g * dt;

  if (in.boost && s.boost > 0) {
    s.v += forward() * boost_accel * dt;
    s.boost--;
  }

  s.x += s.v * dt;

  vec3 w_local = dot(s.w, s.o); 

  vec3 old_w = s.w;
  s.w += dot(s.o, T * rpy + D * w_local) * dt;
  s.w *= fminf(1.0f, w_max / norm(s.w));

  s.o = dot(aa_rotation(0.5f * (s.w + old_w)) * dt, s.o);

}

float Car::drive_force_forward(const Input & in) {

  constexpr float driving_speed = 1450.0f;
  constexpr float braking_force = -3500.0f;
  constexpr float coasting_force = -525.0f;
  constexpr float throttle_threshold = 0.05f;
  constexpr float throttle_force = 1550.0f;
  constexpr float max_speed = 2275.0f;
  constexpr float min_speed = 10.0f;
  constexpr float boost_force = 1500.0f;
  constexpr float steering_torque = 25.75f;
  constexpr float braking_threshold = -0.001f;
  constexpr float supersonic_turn_drag = -98.25; // TODO

  const float v_f = dot(s.v, forward());
  const float v_l = dot(s.v, left());
  const float w_u = dot(s.w, up());

  const float dir = sgn(v_f);
  const float speed = fabs(v_f);

  const float turn_damping = (-0.07186693033945346f * fabs(in.steer) +
                              -0.05545323728191764f * fabs(w_u) + 
                               0.00062552963716722f * fabs(v_l)) * v_f;

  // boosting
  if (in.boost == 1) {

    if (v_f < 0.0f) {

      return -braking_force;

    } else {

      if (v_f < driving_speed) {

        return max_speed - v_f;

      } else {

        if (v_f < max_speed) {

          return boost_accel + turn_damping;

        } else {

          // TODO 
          // this is questionable
          // TODO 
          return supersonic_turn_drag * fabs(w_u);

        }

      }

    }

  // not boosting
  } else {

    // braking
    if ((in.throttle * sgn(v_f) <= braking_threshold) && fabs(v_f) > min_speed) {

      return braking_force * sgn(v_f);

    // not braking
    } else {

      // coasting
      if (fabs(in.throttle) < throttle_threshold && fabs(v_f) > min_speed) {

        return coasting_force * sgn(v_f) + turn_damping; 

      // accelerating
      } else {

        if (fabs(v_f) > driving_speed) {

          return turn_damping;

        } else {

          return in.throttle * (throttle_force - fabs(v_f)) + turn_damping;

        }

      }

    }

  }

}

float Car::drive_force_left(const Input & in) {

  const float v_f = dot(s.v, forward());
  const float v_l = dot(s.v, left());
  const float w_u = dot(s.w, up());

  return (1380.4531378f * in.steer + 
             7.8281188f * in.throttle - 
            15.0064029f * v_l + 
           668.1208332f * w_u) * (1.0f - exp(-0.001161f * fabs(v_f)));

}

float Car::drive_torque_up(const Input & in) {

  float v_f = dot(s.v, forward());
  float w_u = dot(s.w, up());

  return 15.0f * (in.steer * kappa(fabs(v_f)) * v_f - w_u);

}

void Car::driving(const Input & in, float dt) {

  // in-plane forces
  vec3 force = drive_force_forward(in) * forward() + 
               drive_force_left(in)    * left();

  // out-of-plane torque
  vec3 torque = drive_torque_up(in) * up();

  s.v += force * dt;
  s.x += s.v * dt;

  s.w += torque * dt;
  s.o = dot(aa_rotation(s.w * dt), s.o);

}

void Car::driving_handbrake(const Input & in, float dt) {

  // TODO

}

void Car::step(Input in, float dt) {

  // driving
  if (s.on_ground) {

    if (in.jump == 1) {

      // std::cout << "Jump" << std::endl;
      jump(in, dt);

    } else {

      if (in.slide == 0) {

        // std::cout << "Driving" << std::endl;
        driving(in, dt);

      } else {

        // std::cout << "Driving (handbrake)" << std::endl;
        driving_handbrake(in, dt);

      }

    }

  // in the air
  } else {

    if (in.jump == 1 && dodge_timer < dodge_limit && can_dodge) {

      // std::cout << "Air Dodge" << std::endl;
      air_dodge(in, dt);

    } else {

      // std::cout << "Aerial Control" << std::endl;
      aerial_control(in, dt);

    }

  }

}

Car::Car() {

  s.x = vec3{0.0f, 0.0f, 0.0f};
  s.v = vec3{0.0f, 0.0f, 0.0f};
  s.w = vec3{0.0f, 0.0f, 0.0f};
  s.o = eye<3>();

  s.supersonic = false;
  s.jumped = false;
  s.double_jumped = false;
  s.on_ground = false;

  can_dodge = false;
  dodge_timer = 0.0f;

  collider.half_width = vec3{59.00368881f, 42.09970474f, 18.07953644f};
  pivot_offset = vec3{13.97565993f, 0.0f, 20.75498772f};

//  // episode 0
//  // car[[180]]
//  s.x = vec3{2558.53f, -624.871f, 17.01f};
//  s.v = vec3{-367.297f, 586.398f, 8.34f};
//  s.w = vec3{-0.004465f, -0.001741f, 1.95546f};
//  s.o = mat3{{-0.528724f, -0.84878f, -0.00486645f}, {0.848744f, -0.528746f,   0.00781196f}, {-0.00920375f, 0.0f, 0.999958f}};
//  s.supersonic = false;
//  s.jumped = false;
//  s.double_jumped = false;
//  s.on_ground = true;
//
//  Input in;
//  in.throttle = 0.0f;
//  in.steer = 0.889709f;
//  in.pitch = 0.0f;
//  in.yaw = 0.889709f;
//  in.roll = 0.0f;
//  in.jump = false;
//  in.boost = false;
//  in.slide = false;
//
//  step(in, 0.0163666f); 
//
//  std::cout << s.x << std::endl;
//  std::cout << s.v << std::endl;
//  std::cout << s.w << std::endl;

}

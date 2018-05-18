#include "linalg.h"

class Barrier {
#if 0 
  public:

    // a point on the surface
    vec3 p;

    // unit normal
    vec3 n;

    Barrier(vec3 _p, vec3 _n) {
      p = _p;
      n = normalize(_n);
    }

    double time_of_contact(state s) {

      double v_perp = dot(s.v, n);

      if (fabs(v_perp) > 1.0e-8) {
        return (R - dot(s.x - p, n)) / v_perp;
      } else {
        return -1.0;
      }

    }

    state bounce(state current) {
    
      const double Y = 2.0;
      const double mu = 0.285;
      const double C_R = 0.6;
      const double A = 0.0003;
    
      vec3 v_perp = dot(current.v, n) * n;
      vec3 v_para = current.v - v_perp;
      vec3 v_spin = R * cross(n, current.w);
      vec3 s = v_para + v_spin;
    
      double ratio = length(v_perp) / length(s);
    
      vec3 delta_v_perp = - (1.0 + C_R) * v_perp;
      vec3 delta_v_para = - fmin(1.0, Y * ratio) * mu * s;
    
      return state{
        current.x,
        current.v + delta_v_perp + delta_v_para,
        current.w + A * R * cross(delta_v_para, n)
      };
    
    }

#endif
};

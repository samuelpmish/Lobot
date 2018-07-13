#pragma once

#include "bot.cpp"
#include "Ball.h"
#include "Car.h"
#include "linalg.h"

#include <pybind11/pybind11.h>
#include <pybind11/operators.h>

#include <utility>

PYBIND11_MODULE(Lobot, m) {
  pybind11::class_<Ball>(m, "Ball")
    .def(pybind11::init<>())
    .def_readwrite("pos", &Ball::x)
    .def_readwrite("vel", &Ball::v)
    .def_readwrite("omega", &Ball::w)
    .def("step", &Ball::step);
  
  pybind11::class_<Bot>(m, "Bot")
    .def(pybind11::init<int>())
    .def("get_response", &Bot::get_response);

  pybind11::class_<vec3>(m, "vec3")
    .def(pybind11::init< float, float, float >())
    .def(pybind11::init< const vec3 & >())
    .def("__getitem__", [](const vec3 & v, size_t i){ return v[i]; })
    .def("__setitem__", [](vec3 & v, size_t i, float f){ v[i] = f; })
    .def(pybind11::self -  pybind11::self)
    .def(pybind11::self +  pybind11::self)
    .def(pybind11::self -= pybind11::self)         
    .def(pybind11::self += pybind11::self)         
    .def(float() * pybind11::self)         
    .def(pybind11::self * float());

  pybind11::class_<mat3>(m, "mat3")
    .def(pybind11::init([](float a11, float a12, float a13,
                           float a21, float a22, float a23,
                           float a31, float a32, float a33) {
      return mat3{
        {a11, a12, a13},
        {a21, a22, a23},
        {a31, a32, a33}
      };
    }))
    .def("__getitem__", [](const mat3 & A, std::pair < size_t, size_t > i){
        return A(i.first, i.second); 
    })
    .def("__setitem__", [](mat3 & A, std::pair < size_t, size_t > i, float v){
        A(i.first, i.second) = v; 
    });

  pybind11::class_<obb>(m, "obb")
    .def(pybind11::init())
    .def_readwrite("center", &obb::center)
    .def_readwrite("half_width", &obb::half_width)
    .def_readwrite("orientation", &obb::orientation);

  pybind11::class_<Input>(m, "Input")
    .def(pybind11::init<>())
    .def_readwrite("steer", &Input::steer)
    .def_readwrite("roll", &Input::roll)
    .def_readwrite("pitch", &Input::pitch)
    .def_readwrite("yaw", &Input::yaw)
    .def_readwrite("throttle", &Input::throttle)
    .def_readwrite("jump", &Input::jump)
    .def_readwrite("boost", &Input::boost)
    .def_readwrite("slide", &Input::slide);

  pybind11::class_<Car>(m, "Car")
    .def(pybind11::init<>())
    .def(pybind11::init<const Car &>())
    .def("step", &Car::step)
    .def("pitch_surface_normal", &Car::pitch_surface_normal)
    .def("bounding_box", &Car::bounding_box)
    .def("extrapolate", &Car::extrapolate)
    .def("forward", &Car::forward)
    .def("left", &Car::left)
    .def("up", &Car::up)
    .def_readwrite("target", &Car::target)
    .def_readwrite("time", &Car::time)
    .def_readwrite("ETA", &Car::ETA)
    .def_readwrite("pos", &Car::x)
    .def_readwrite("vel", &Car::v)
    .def_readwrite("omega", &Car::w)
    .def_readwrite("theta", &Car::o)
    .def_readwrite("supersonic", &Car::supersonic)
    .def_readwrite("jumped", &Car::jumped)
    .def_readwrite("double_jumped", &Car::double_jumped)
    .def_readwrite("on_ground", &Car::on_ground)
    .def_readwrite("boost", &Car::boost)
    .def_readwrite("last_input", &Car::last);

  // free functions
  m.def("dot", (float (*)(const vec3 &, const vec3 &)) &dot);
  m.def("dot", (vec3 (*)(const vec3 &, const mat3 &)) &dot);
  m.def("dot", (vec3 (*)(const mat3 &, const vec3 &)) &dot);
  m.def("dot", (mat3 (*)(const mat3 &, const mat3 &)) &dot);
  m.def("cross", (vec3 (*)(const vec3 &, const vec3 &)) &cross);

  m.def("inv", (mat3 (*)(const mat3 &)) &inv);
  m.def("euler_rotation", &euler_rotation);
  m.def("axis_rotation", &axis_rotation);
  m.def("rotation_to_axis", &rotation_to_axis);
  m.def("transpose", (mat3 (*)(const mat3 &)) &transpose);
  m.def("angle_between", (float (*)(const vec3 &, const vec3 &)) &angle_between);
  m.def("angle_between", (float (*)(const mat3 &, const mat3 &)) &angle_between);

  m.def("sgn", &sgn);
  m.def("clamp", &clamp);

  m.def("norm", (float (*)(const vec3 &)) &norm);
  m.def("normalize", (vec3 (*)(const vec3 &)) &normalize);

}

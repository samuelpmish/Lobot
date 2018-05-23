#pragma once

#include "bot.cpp"
#include "Ball.h"

#include <pybind11/pybind11.h>

PYBIND11_MODULE(Lobot, m) {
	pybind11::class_<Ball>(m, "Ball")
		.def(pybind11::init<>())
		.def_readwrite("position", &Ball::x)
		.def_readwrite("velocity", &Ball::v)
		.def_readwrite("angular_velocity", &Ball::w)
		.def("step", &Ball::step);
	
	pybind11::class_<Bot>(m, "Bot")
		.def(pybind11::init<int>())
		.def("get_response", &Bot::get_response);
}
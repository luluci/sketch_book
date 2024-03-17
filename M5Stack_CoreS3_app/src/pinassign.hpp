#pragma once

#include <cstdint>

// https://docs.m5stack.com/en/core/CoreS3

// PORT B
constexpr uint16_t PORT_B_IN = 8;
constexpr uint16_t PORT_B_OUT = 9;

// Actuator
constexpr uint16_t PIN_IR_SEND = PORT_B_OUT;
constexpr uint16_t PIN_IR_RECV = PORT_B_IN;

//
//  Project: Lidar Tank
//  main.hpp
//  Author: Arnaud Thiercelin
//  Date Created: 2018-10-10
// 

#include <unistd.h>
#include "joystick.hh"

#include <wiringPi.h>
#include <wiringPiI2C.h>

int axisEventValueToPercentage(JoystickEvent *event);
int leftAxisPercentageToCommand(int percentage);
int rightAxisPercentageToCommand(int percentage);

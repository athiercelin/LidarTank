//
//  Project: Lidar Tank
//  main.cpp
//  Author: Arnaud Thiercelin
//  Date Created: 2018-10-10
// 

#include "main.hpp"
#include "joystick.hh"

#include <wiringPi.h>
#include <wiringPiI2C.h>

int main(int argc, char** argv)
{

  //
  //  Init the Controller
  //
  
  // Create an instance of Joystick
  Joystick joystick("/dev/input/js0");

  // Ensure that it was found and that we can use it
  if (!joystick.isFound())
  {
    printf("open failed.\n");
    exit(1);
  }

  //
  //  I2C/GPIO setup
  //

  wiringPiSetupGpio();
  int motorMCUfd = wiringPiI2CSetup(0x08);
  if (motorMCUfd < 0)
    {
      std::cout << "I2C Setup Error" << std::endl;
      return 0;
    }

  

  //
  //  Main Loop
  //
  
  while (true)
  {
    // Restrict rate
    usleep(1000);

    // Attempt to sample an event from the joystick
    JoystickEvent event;
    if (joystick.sample(&event))
    {
      if (event.isButton()) {
	if (event.number == 0) {
	  printf("Button %u is %s\n",
		 event.number,
		 event.value == 0 ? "up" : "down");
	  wiringPiI2CWrite(motorMCUfd, 100);
	}
      } else if (event.isAxis()) {
	int axisNumber = event.number;

	// Neg value means pushed forward on stick axis 1 and 4
	// Positive value means pulled back.
	
	if (axisNumber == 1) { // Left Stick
	  //printf("Axis %u is at position %d\n", event.number, event.value);

	 static int command = 100; // 0
	 int newCmd = 0;
	 
	  if (event.value != 0) {
	    if (event.value > 0) {
	      newCmd = 71;
	    } else {
	      newCmd = 131;
	    }
	  } else {
	    newCmd = 100;
	  }

	  if (newCmd != command) {
	    command = newCmd;
	    printf("I2C Sending %d", command);
	    wiringPiI2CWrite(motorMCUfd, command);
	  }
	  
	} else if (axisNumber == 4) { // Right Stick
	  printf("Axis %u is at position %d\n", event.number, event.value);

	  int command = 100; // 0
	  if (event.value != 0) {
	    if (event.value > 0) {
	      command = 102;
	    } else {
	      command = 98;
	    }
	  }
	  wiringPiI2CWrite(motorMCUfd, command);
	}
	
      }
    }
  }
}

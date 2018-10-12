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

  printf("Initialize Remote Controller\n");
  
  bool joystickFound = false;
  Joystick *joystick = NULL;

  
  while (joystickFound == false) {
  
    // Create an instance of Joystick
    Joystick tmpJoystick("/dev/input/js0");

    joystick = &tmpJoystick;
    joystickFound = tmpJoystick.isFound();

    // Ensure that it was found and that we can use it
    if (!tmpJoystick.isFound())
    {
      printf("Failed finding joystick device. Will try again in 2s\n");
    //exit(1);
      sleep(2);
    }
  }

  printf("*** Remote Controller ready!\n");
  
  //
  //  I2C/GPIO setup
  //

  printf("Initializing I2C\n");
  
  wiringPiSetupGpio();
  int motorMCUfd = wiringPiI2CSetup(0x08);
  if (motorMCUfd < 0)
    {
      std::cout << "I2C Setup Error" << std::endl;
      return 0;
    }

  printf("*** I2C ready!\n");

  //
  //  Main Loop
  //
  
  while (true)
    {
      // Restrict rate
      usleep(1000);

      // Attempt to sample an event from the joystick
      JoystickEvent event;
      if (joystick->sample(&event))
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
	    printf("Axis %u is at position %d\n", event.number, event.value);
	
	    if (axisNumber == 1) { // Left Stick
	      static int command = 100; // 0
	      int newCmd = 0;
	 
	      if (event.value != 0) {
		if (event.value > 0) {
		  newCmd = 21;
		} else {
		  newCmd = 181;
		}
	      } else {
		newCmd = 100;
	      }

	      if (newCmd != command) {
		command = newCmd;
		printf("I2C Sending %d", command);
		wiringPiI2CWrite(motorMCUfd, command);
	      }
	  
	    }
	    if (axisNumber == 4) { // Right Stick
	      static int command = 100; // 0
	      int newCmd = 0;
	 
	      if (event.value != 0) {
		if (event.value > 0) {
		  newCmd = 20;
		} else {
		  newCmd = 180;
		}
	      } else {
		newCmd = 100;
	      }

	      if (newCmd != command) {
		command = newCmd;
		printf("I2C Sending %d", command);
		wiringPiI2CWrite(motorMCUfd, command);
	      }
	
	    }
	  }
	}
    }
}

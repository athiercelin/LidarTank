//
//  Project: Lidar Tank
//  main.cpp
//  Author: Arnaud Thiercelin
//  Date Created: 2018-10-10
// 

#include "main.hpp"

int main(int argc, char** argv)
{

  //
  //  Init the Controller
  //

  std::string devicePath("/dev/input/js0");

  printf("Initialize Remote Controller\n");
  Joystick *joystick = NULL;
  
  while (true) {
  
    // Create an instance of Joystick
    joystick = new Joystick(devicePath);

    if (!joystick->isFound())
    {
      printf("Failed finding joystick device. Will try again in 2s\n");
      delete joystick;
      sleep(2);
    } else {
      break;
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

	      newCmd = leftAxisPercentageToCommand(axisEventValueToPercentage(&event));
	      
	      // if (event.value != 0) {
	      // 	if (event.value > 0) {
	      // 	  newCmd = 21;
	      // 	} else {
	      // 	  newCmd = 181;
	      // 	}
	      // } else {
	      // 	newCmd = 100;
	      // }

	      if (newCmd != command) {
		command = newCmd;
		printf("I2C Sending %d", command);
		wiringPiI2CWrite(motorMCUfd, command);
	      } else {
		printf("OLD: [%d] NEW: [%d]\n", command, newCmd);
	      }
	  
	    }
	    if (axisNumber == 4) { // Right Stick
	      static int command = 100; // 0
	      int newCmd = 0;
	 
	      newCmd = rightAxisPercentageToCommand(axisEventValueToPercentage(&event));
	      // if (event.value != 0) {
	      // 	if (event.value > 0) {
	      // 	  newCmd = 20;
	      // 	} else {
	      // 	  newCmd = 180;
	      // 	}
	      // } else {
	      // 	newCmd = 100;
	      // }

	      if (newCmd != command) {
		command = newCmd;
		printf("I2C Sending %d", command);
		wiringPiI2CWrite(motorMCUfd, command);
	      } else {
		printf("OLD: [%d] NEW: [%d]\n", command, newCmd);
	      }
	
	    }
	  }
	}
    }
}


int axisEventValueToPercentage(JoystickEvent *event) {

  int eventValue = event->value;
  int percentage = 0;

  if (eventValue > 0) {
    percentage = -1 * ((float)eventValue / (float)event->MAX_AXES_VALUE) * 100;
    printf("EventVal: %d, [%d] Percentage: %d\n", eventValue, event->MAX_AXES_VALUE, percentage);
  } else if (eventValue < 0) {
    percentage = ((float) eventValue / (float)event->MIN_AXES_VALUE) * 100;
    printf("EventVal: %d, [%d] Percentage: %d\n", eventValue, event->MIN_AXES_VALUE, percentage);
  }
  return percentage; 
}

int leftAxisPercentageToCommand(int percentage) {
  int command = percentage + 100;

  if (!(command % 2) && command != 100) {
    if (command == 200) {
      command--;
    } else {
      command++;
    }
  }     
  
  return command;
}

int rightAxisPercentageToCommand(int percentage) {
  int command = percentage + 100;
  
  if (command % 2 && command != 100) {
    if (command != 200) {
      command++;
    } else {
      command--;
    }
  }     
  
  return command;
}

##
##  Project: Lidar Tank
##  File: Makefile
##  Author: Arnaud Thiercelin
##  Creation Date: 2018-10-10
##

## Tools
CC = g++

RM = rm -f

## Flags
CFLAGS = -g -Wall -std=c++0x

## Includes/Libs
INCLUDES =

LIBS = -lwiringPi

## Project
TARGET = LidarTank

SOURCES = main.cpp joystick.cc

## Compiling Steps
OBJS = $(SOURCES:.c=.o)

all: $(TARGET)
	@echo Compiling complete, launch using ./LidarTank

$(TARGET): $(OBJS)
	$(CC) $(CFLAGS) $(INCLUDES) $(LIBS) -o $(TARGET)  $(OBJS)

clean:
	$(RM) *.o *~ $(TARGET)

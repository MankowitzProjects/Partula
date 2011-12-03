PROG ?= Partula
OBJS = main.o Robot.o SensorController.o MotorController.o \
       Motor.o Sensor.o Switch.o Handle.o

all: $(PROG)

$(PROG): main.o
	g++ "./obj/lin/main.o" \
		"./obj/lin/Robot.o" \
		"./obj/lin/SensorController.o" \
		"./obj/lin/Sensor.o" \
		"./obj/lin/Switch.o" \
		"./obj/lin/MotorController.o" \
		"./obj/lin/Motor.o" \
		"./obj/lin/Handle.o"\
		-o Partula \
		-lphidget21
		
main.o: Robot.o main.cpp
	g++ -c main.cpp -o "./obj/lin/main.o"

Handle.o: Handle.cpp
	g++ -c Handle.cpp -o "./obj/lin/Handle.o"

Robot.o: Robot.cpp Robot.h SensorController.o MotorController.o Handle.o
	g++ -c Robot.cpp -o "./obj/lin/Robot.o"

SensorController.o: SensorController.cpp SensorController.h Sensor.o Switch.o
	g++ -c SensorController.cpp -o "./obj/lin/SensorController.o"

MotorController.o: MotorController.cpp MotorController.h Motor.o
	g++ -c MotorController.cpp -o "./obj/lin/MotorController.o"

Motor.o: Motor.cpp Motor.h
	g++ -c Motor.cpp -o "./obj/lin/Motor.o"

Sensor.o: Sensor.cpp Sensor.h
	g++ -c Sensor.cpp -o "./obj/lin/Sensor.o"
	
Switch.o: Switch.cpp Switch.h
	g++ -c Switch.cpp -o "./obj/lin/Switch.o"



clean:
	rm "./obj/lin/main.o" \
		"./obj/lin/Robot.o" \
		"./obj/lin/SensorController.o" \
		"./obj/lin/Sensor.o" \
		"./obj/lin/Switch.o" \
		"./obj/lin/MotorController.o" \
		"./obj/lin/Motor.o"\
		"./obj/lin/Handle.o"\
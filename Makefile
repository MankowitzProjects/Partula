PROG ?= Partula
OBJS = main.o Robot.o SensorController.o MotorController.o \
       Motor.o Sensor.o Switch.o

all: $(PROG)

$(PROG): main.o
	g++ "./obj/lin/main.o" \
		"./obj/lin/Robot.o" \
		"./obj/lin/SensorController.o" \
		"./obj/lin/Sensor.o" \
		"./obj/lin/Switch.o" \
		"./obj/lin/MotorController.o" \
		"./obj/lin/Motor.o" \
		"./obj/lin/Event.o"\
		-o Partula \
		-lphidget21
		
main.o: Robot.o main.cpp
	g++ -c main.cpp -o "./obj/lin/main.o"

Robot.o: Robot.cpp Robot.h SensorController.o MotorController.o Event.o
	g++ -c Robot.cpp -o "./obj/lin/Robot.o"

Event.o: Event.cpp Event.h SensorController.o
	g++ -c Event.cpp -o "./obj/lin/Event.o"

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
		"./obj/lin/Event.o"
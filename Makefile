PROG ?= Partula
OBJS = main.o Robot.o Action.o SensorController.o MotorController.o \
       Motor.o Sensor.o Switch.o Handle.o Event.o Pose.o Localization.o ServoController.o ServoMotor.o\

all: $(PROG)

$(PROG): main.o
	g++ "./obj/lin/main.o" \
		"./obj/lin/Robot.o" \
		"./obj/lin/SensorController.o" \
		"./obj/lin/Sensor.o" \
		"./obj/lin/Switch.o" \
		"./obj/lin/MotorController.o" \
		"./obj/lin/Motor.o" \
		"./obj/lin/Handle.o" \
		"./obj/lin/Action.o" \
		"./obj/lin/Event.o" \
		"./obj/lin/Pose.o" \
		"./obj/lin/Localization.o" \
		"./obj/lin/ServoMotor.o" \
		"./obj/lin/ServoController.o" \
		-o Partula \
		-lphidget21
		
main.o: Robot.o main.cpp
	g++ -c main.cpp -o "./obj/lin/main.o"

ServoMotor.o: ServoMotor.cpp ServoMotor.h 
	g++ -c ServoMotor.cpp -o "./obj/lin/ServoMotor.o"

ServoController.o: ServoController.cpp ServoController.h ServoMotor.o
	g++ -c ServoController.cpp -o "./obj/lin/ServoController.o"

Action.o: Action.cpp Action.h MotorController.o Pose.o
	g++ -c Action.cpp -o "./obj/lin/Action.o"

Handle.o: Handle.cpp Action.o Localization.o ServoController.o Pose.o
	g++ -c Handle.cpp -o "./obj/lin/Handle.o"

Pose.o: Pose.cpp Pose.h
	g++ -c Pose.cpp -o "./obj/lin/Pose.o"

Robot.o: Robot.cpp Robot.h SensorController.o MotorController.o Handle.o Event.o Pose.o Localization.o
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

Event.o: Event.cpp Event.h
	g++ -c Event.cpp -o "./obj/lin/Event.o"

Localization.o: Localization.cpp Localization.h Pose.o
	g++ -c Localization.cpp -o "./obj/lin/Localization.o"





clean:
	rm "./obj/lin/main.o" \
		"./obj/lin/Robot.o" \
		"./obj/lin/SensorController.o" \
		"./obj/lin/Sensor.o" \
		"./obj/lin/Switch.o" \
		"./obj/lin/MotorController.o" \
		"./obj/lin/Motor.o"\
		"./obj/lin/Handle.o"\
		"./obj/lin/Action.o"\
		"./obj/lin/Event.o"\
		"./obj/lin/Pose.o"\
		"./obj/lin/Localization.o"\
		"./obj/lin/ServoController.o"\
		"./obj/lin/ServoMotor.o"\
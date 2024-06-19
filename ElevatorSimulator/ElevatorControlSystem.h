#ifndef ELEVATORCONTROLSYSTEM_H
#define ELEVATORCONTROLSYSTEM_H

#include <QObject>
#include <QDebug>
#include <vector>
#include <QTimer>
#include <random>
#include "Elevator.h"
#include "Door.h"
#include "Display.h"
#include "Alarm.h"
#include "Sensor.h"
#include "BuildingSafetyService.h"
#include "Passenger.h"

using namespace std;

/*
This ECS acts as mediator (as a centralized design). It coordinates all elevator.
As a mediator, all elevator controls for each elevators (3 as stated in dedscription)
should be going through this ECS.
*/

class ElevatorControlSystem : public QObject {
    Q_OBJECT
    public:
        explicit ElevatorControlSystem(int numberOfElevators, QObject *parent = nullptr);
        //destructor
        ~ElevatorControlSystem();
        void allocateElevatorUp(int floor, int numberOfPassenger);
        void allocateElevatorDown(int floor, int numberOfPassenger);
        void unOverLoadElevator(int elevatorIndex);
        void handleFireEmergency();
        void openDoor(int elevatorIndex);
        void closeDoor(int elevatorIndex);
        void updateDisplay(const string& message);
        void arriveFloor(int elevatorIndex);
        void connectToBuildingSafetyService(int numberOfElevator);
        void moveToSafeFloor();
        //add elevators
        void activateAlarm();
        void engageBackupPower();
        void offBackupPower();
        bool checkIfSafeToOpenDoors(int elevatorIndex);
        void moveToFloor(int elevatorNumber, int desFloor);
        Display* getDisplay(int elevator);
        void resolvedFireEmergency();
        void handleDoorObstacle(int elevatorNumber);
        void resolveDoorObstacle(int elevatorNumber);
        void handlePowerOutage();
        void resolvePowerOutage();
        void onLoadPassenger(int, int, int);

    signals:
        void elevatorPositionChanged(int elevatorIndex, int currentFloor);

    private:
        bool emergencyStatus;
        //vector to store lists of elevators and its corresponding component
        vector<Elevator*> elevators;
        vector<Door*> doors;
        vector<Display*> displays;
        vector<Alarm*> alarms;
        vector<Sensor*> sensors;
        vector<QTimer*> updateTimers;
        BuildingSafetyService emergencyServiceCentre;
};


#endif // ELEVATORCONTROLSYSTEM_H

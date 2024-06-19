#ifndef ELEVATOR_H
#define ELEVATOR_H

#include <vector>
#include <algorithm>
#include <string>
#include "Passenger.h"
using namespace std;


class Elevator {
    public:
        Elevator(int elevatorNumber);
        ~Elevator();
        void moveToFloor(int floor);
        void openDoor();
        void closeDoor();
        void emergencyStop();
        int getCurrentFloor() const;
        int getDestinationFloor() const;
        bool isIdle() const;
        bool isDoorOpen() const;
        void boardPassenger(Passenger* passenger);
        void exitPassenger(Passenger* passenger);
        void blockDoor();
        void unBlockDoor();
        void cleanPassenger();
        void resolvedEmergency();
        void setCurrentFloor(int currentFloor);
        int passengerSize();
        Passenger* getPassenger(int i);

    private:
        int elevatorNumber;
        int currentFloor;
        int destinationFloors;
        enum Status { Idle, Moving, DoorOpening, DoorClosing, EmergencyStopped } status;
        vector<Passenger*> passengers;
        bool doorOpen;
        bool blockingDoor;
};


#endif // ELEVATOR_H

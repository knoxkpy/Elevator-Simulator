#include "Passenger.h"

int Passenger::passengerIDCounter = 0; // Initialize static counter

Passenger::Passenger(int currentFloor, int desiredFloor)
: passengerID(getNewPassengerID()), requestElevatorStatus(false), helpStatus(false), currentFloor(currentFloor), desiredFloor(desiredFloor) {}

void Passenger::createFloorRequest(int desiredFloor) {
    this->desiredFloor = desiredFloor;
}

bool Passenger::requestElevator(int currentFloor) {
    this->currentFloor = currentFloor;
    requestElevatorStatus = true;
    return true;
}


int Passenger::getCurrentFloor() const {
    return currentFloor;
}

int Passenger::getDesiredFloor() const {
    return desiredFloor;
}

int Passenger::getNewPassengerID() {
    return ++passengerIDCounter;
}

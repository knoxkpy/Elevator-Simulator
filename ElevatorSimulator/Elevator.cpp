#include "Elevator.h"

Elevator::Elevator(int elevatorNumber)
    : elevatorNumber(elevatorNumber), currentFloor(1), destinationFloors(1), status(Idle), doorOpen(false), blockingDoor(false) {}

Elevator::~Elevator() {
    for (size_t i = 0; i < passengers.size(); ++i) {
        delete passengers[i];
    }
}

void Elevator::moveToFloor(int floor) {
    if (status != DoorOpening) {
        status = Moving;
        currentFloor = floor;
        status = Idle;
    }
}

Passenger* Elevator::getPassenger(int i) {
    return passengers[i];
}

void Elevator::openDoor() {
    if (status == Idle) {
        doorOpen = true;
        status = DoorOpening;
    }
}

void Elevator::closeDoor() {
    if (!blockingDoor) { // Ensure door isn't blocked
        doorOpen = false;
        status = DoorClosing;
        status = Idle; // Once door is closed, return status to Idle
    }
}

void Elevator::emergencyStop() {
    status = EmergencyStopped;
}

void Elevator::resolvedEmergency() {
    status = DoorOpening;
    doorOpen = true;
}

int Elevator::getCurrentFloor() const {
    return currentFloor;
}

int Elevator::getDestinationFloor() const {
    return destinationFloors;
}

bool Elevator::isIdle() const {
    return status == Idle;
}

void Elevator::boardPassenger(Passenger* passenger) {
    passengers.push_back(passenger);
}

void Elevator::exitPassenger(Passenger* passenger) {
    if (doorOpen) {
        auto it = find(passengers.begin(), passengers.end(), passenger);
        if (it != passengers.end()) {
            passengers.erase(it);
        }
    }
}

void Elevator::blockDoor() {
    blockingDoor = true;
    status = DoorOpening;
    doorOpen = true;
}

void Elevator::unBlockDoor() {
    blockingDoor = false;
    status = DoorClosing;
}

void Elevator::cleanPassenger() {
    for(size_t i = 0; i < passengers.size(); ++i) {
        delete passengers[i];
    }
    passengers.clear();
}

void Elevator::setCurrentFloor(int currentFloor) {
    this->currentFloor = currentFloor;
}

bool Elevator::isDoorOpen() const {
    return doorOpen;
}

int Elevator::passengerSize() {
    return passengers.size();
}

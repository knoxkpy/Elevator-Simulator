#include "Door.h"

Door::Door(): doorStatus("Closed"), isBlock(false) {}

void Door::open() {
    doorStatus = "Opened";
}

void Door::close() {
    doorStatus = "Closed";
}

void Door::setDoorStatus(const string& status) {
    doorStatus = status;
}

string Door::getDoorStatus() {
    return doorStatus;
}

bool Door::blockDoorStatus() {
    return isBlock;
}

void Door::blockDoor() {
    isBlock = true;
}

void Door::unblockDoor() {
    isBlock = false;
}

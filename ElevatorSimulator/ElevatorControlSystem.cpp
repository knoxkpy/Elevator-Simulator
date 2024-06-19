#include "ElevatorControlSystem.h"

ElevatorControlSystem::ElevatorControlSystem(int numberOfElevators, QObject *parent) : QObject(parent), emergencyStatus(false) {
    // Initialize elevators, assuming 3 elevators for simplicity
    for (int i = 0; i < numberOfElevators; ++i) {
        elevators.push_back(new Elevator(i));
        doors.push_back(new Door());
        displays.push_back(new Display());
        alarms.push_back(new Alarm("General", i)); // Assuming a general alarm type
        sensors.push_back(new Sensor());

        //For timer
        QTimer* timer = new QTimer(this);
        timer->setSingleShot(true);
        updateTimers.push_back(timer);

    }
    emergencyServiceCentre = BuildingSafetyService();
}

ElevatorControlSystem::~ElevatorControlSystem() {
    for (auto e : elevators) delete e;
    for (auto d : doors) delete d;
    for (auto ds : displays) delete ds;
    for (auto a : alarms) delete a;
    for (auto s : sensors) delete s;
    for (auto t : updateTimers) delete t;
}

void ElevatorControlSystem::allocateElevatorDown(int floor, int numberOfPassenger) {
    int elevatorIndex = 0;
    for (auto& elevator : elevators) {
        if (elevator->isIdle() && (elevator->getCurrentFloor() >= floor)) {
            break;
        } else {
            elevatorIndex++;
        }
    }

    if (elevatorIndex >= 3) {
        qInfo() << QString("No avaliable elevator at this moment! (For going down)");
        return;
    }

    if (!elevators[elevatorIndex]->isDoorOpen()) {
        moveToFloor(elevatorIndex, floor);
        onLoadPassenger(numberOfPassenger, elevatorIndex, floor);
    }
}

void ElevatorControlSystem::allocateElevatorUp(int floor, int numberOfPassenger) {
    // Simplified allocation: Assign the first idle elevator
    int elevatorIndex = 0;
    for (auto& elevator : elevators) {
        if (elevator->isIdle() && (elevator->getCurrentFloor() <= floor)) {
            break;
        } else {
            elevatorIndex++;
        }
    }

    if (elevatorIndex >= 3) {
        qInfo() << QString("No avaliable elevator at this moment! (For going up)");
        return;
    }

    if (!elevators[elevatorIndex]->isDoorOpen()) {
        moveToFloor(elevatorIndex, floor);
        onLoadPassenger(numberOfPassenger, elevatorIndex, floor);
    }
}


void ElevatorControlSystem::onLoadPassenger(int numberOfPassenger, int elevatorIndex, int currentFloor) {
    if (!emergencyStatus) {
        random_device rd;
        mt19937 gen(rd());
        uniform_int_distribution<> dis(1, 7);

        if (elevators[elevatorIndex]->passengerSize() > 3) {
            sensors[elevatorIndex]->setOverload(true);
            doors[elevatorIndex]->open();
            alarms[elevatorIndex]->activateAlarm("Overload");
            displays[elevatorIndex]->updateDisplay("Elevator is overloaded!");
            return;
        }

        for (int i = 0; i < numberOfPassenger; ++i) {
            int randomFloor = dis(gen);
            while (randomFloor == currentFloor) {
                randomFloor = dis(gen);
            }

            if (i <= 2) {
                Passenger* newPassenger = new Passenger(currentFloor, 1);
                elevators[elevatorIndex]->boardPassenger(newPassenger);

                qInfo() << QString("Boarding passenger %1").arg(i+1);
                qInfo() << QString("Passenger Size: %1").arg(elevators[elevatorIndex]->passengerSize());
            } else {
                Passenger* newPassenger = new Passenger(currentFloor, 1);
                elevators[elevatorIndex]->boardPassenger(newPassenger);
                qInfo() << QString("Boarding passenger %1").arg(i+1);
                qInfo() << QString("Passenger Size: %1").arg(elevators[elevatorIndex]->passengerSize());

                sensors[elevatorIndex]->setOverload(true);
                doors[elevatorIndex]->open();
                alarms[elevatorIndex]->activateAlarm("Overload");
                displays[elevatorIndex]->updateDisplay("Elevator is overloaded!");
                return;
            }
        }
    }
}

void ElevatorControlSystem::unOverLoadElevator(int elevatorIndex) {
    if(sensors[elevatorIndex]->detectOverload()) {
        Passenger* goner = nullptr;
        for (int i = elevators[elevatorIndex]->passengerSize(); i >= 3; --i) {
           goner = elevators[elevatorIndex]->getPassenger(i);
           elevators[elevatorIndex]->exitPassenger(goner);
        }

        sensors[elevatorIndex]->setOverload(false);
        alarms[elevatorIndex]->deactivateAlarm();
        displays[elevatorIndex]->updateDisplay("Overload resolved!");
    }

    qInfo() << QString("The size of passengers is now %1 in elevator %2").arg(elevators[elevatorIndex]->passengerSize()).arg(elevatorIndex+1);
}

bool ElevatorControlSystem::checkIfSafeToOpenDoors(int elevatorIndex) {
    if (!elevators[elevatorIndex]->isIdle()) {
        return false;
    }

    if (sensors[elevatorIndex]->detectObstacle()) {
        return false; // It's unsafe to open doors if an obstruction is detected.
    }

    if (sensors[elevatorIndex]->detectOverload()) {
        return false;
    }

    return true;
}


void ElevatorControlSystem::handleFireEmergency() {
    if (!emergencyStatus) {
        activateAlarm();

        // In an emergency, command all elevators to perform emergency stops
        // and move to safe floors if necessary
        for (size_t i = 0; i < elevators.size(); ++i) {
            elevators[i]->emergencyStop();
        }

        // Use QTimer::singleShot for a delay before moving to safe floors
        QTimer::singleShot(2000, this, [this]() {
            moveToSafeFloor();

            // After moving to a safe floor, wait another 2 seconds
            // before updating the display
            QTimer::singleShot(2000, this, [this]() {
                updateDisplay("Fire Emergency! Please exit now!");

                // Open doors if safe to do so
                for (size_t i = 0; i < elevators.size(); ++i) {
                    if (checkIfSafeToOpenDoors(i)) {
                        openDoor(i);
                        emit elevatorPositionChanged(i, elevators[i]->getCurrentFloor());
                        elevators[i]->cleanPassenger();
                    }
                }
            });
        });

        for (size_t i = 0; i < elevators.size(); ++i) {
            connectToBuildingSafetyService(i);
        }

        emergencyServiceCentre.handleEmergency("Fire");
        emergencyStatus = true;
    }
}



void ElevatorControlSystem::resolvedFireEmergency() {
    if (emergencyStatus) {
        emergencyStatus = false;
        for (size_t i = 0; i < elevators.size(); ++i) {
            elevators[i]->resolvedEmergency();
        }
        updateDisplay("Fire Emergency Resolved!");
        emergencyServiceCentre.resetEmergencyState();
    }
}

void ElevatorControlSystem::openDoor(int elevatorIndex) {
    if(!emergencyStatus){
        doors[elevatorIndex]->open();
        elevators[elevatorIndex]->openDoor();
        displays[elevatorIndex]->updateDisplay("Doors opening");
    }
}

void ElevatorControlSystem::closeDoor(int elevatorIndex) {
    if((!emergencyStatus) && !(doors[elevatorIndex]->blockDoorStatus()) &&!(sensors[elevatorIndex]->detectOverload())) {
        doors[elevatorIndex]->close();
        elevators[elevatorIndex]->closeDoor();
        displays[elevatorIndex]->updateDisplay("Doors Closing");
    }
}

//used when fire, all other types of emergency...
void ElevatorControlSystem::updateDisplay(const string& message) {
    for (auto& display : displays) {
        display->updateDisplay(message);
    }
}

void ElevatorControlSystem::arriveFloor(int elevatorIndex) {
    // Display the arriving message and set a timer to wait before opening the door
    QString message = QString("Arrived at Floor: %1").arg(elevators[elevatorIndex]->getCurrentFloor());
    displays[elevatorIndex]->updateDisplay(message.toStdString());

    QTimer* timer = updateTimers[elevatorIndex];
    QObject::connect(timer, &QTimer::timeout, [this, elevatorIndex]() {
        elevators[elevatorIndex]->openDoor();
        openDoor(elevatorIndex);
    });
    timer->start(2000); // 2-second delay
}


void ElevatorControlSystem::moveToFloor(int elevatorIndex, int desFloor) {
    if (!emergencyStatus) {
        Elevator* elevator = elevators[elevatorIndex];

        if (elevator->isIdle()) {
            int direction = (elevator->getCurrentFloor() < desFloor) ? 1 : -1;
            while (elevator->getCurrentFloor() != desFloor) {
                int nextFloor = elevator->getCurrentFloor() + direction;

                elevator->moveToFloor(nextFloor);

                //printing the process to console for simplicity reason.
                qInfo() << ("From elevator " + std::to_string(elevatorIndex +1) + ", arriving Floor: " + std::to_string(nextFloor)).c_str();
            }
            qInfo() << "";

            emit elevatorPositionChanged(elevatorIndex, desFloor);

            if (checkIfSafeToOpenDoors(elevatorIndex)) {
                arriveFloor(elevatorIndex);
            }
        }
    }
}


void ElevatorControlSystem::connectToBuildingSafetyService(int numberOfElevator) {
    if (!emergencyStatus) {
        displays[numberOfElevator]->updateDisplay("Connected to Building Safety Service!");
        emergencyServiceCentre.handleEmergency("General Emergency");
        emergencyServiceCentre.calling911();
    }


    //assume that it got resolved.
    emergencyServiceCentre.resetEmergencyState();
}

//move To save Floor.
void ElevatorControlSystem::moveToSafeFloor() {
    for (size_t i = 0; i < elevators.size(); ++i) {
        if (elevators[i]->getCurrentFloor() < 7) {
            elevators[i]->moveToFloor(elevators[i]->getCurrentFloor() +1);
            displays[i]->updateDisplay("Moved to nearest floor:" + to_string(elevators[i]->getCurrentFloor()));
        } else {
            elevators[i]->moveToFloor(elevators[i]->getCurrentFloor());
            displays[i]->updateDisplay("Moved to nearest floor:" + to_string(elevators[i]->getCurrentFloor()));
        }
    }
}

//used when all alarm in all elevators need to be on.
void ElevatorControlSystem::activateAlarm() {
    for (auto& alarm : alarms) {
        alarm->activateAlarm("Fire");
    }
}

void ElevatorControlSystem::engageBackupPower() {
    for (size_t i = 0; i < sensors.size(); ++i) {
        if(!sensors[i]->getBackupPowerStatus()) {
            sensors[i]->setBackupPower(true);
            qInfo() << QString("Backup Power for elevator %1 is working properly")
                        .arg(i+1);
        }
    }
}

void ElevatorControlSystem::offBackupPower() {
    for (size_t i = 0; i < sensors.size(); ++i) {
        if(sensors[i]->getBackupPowerStatus()) {
            sensors[i]->setBackupPower(false);
            qInfo() << QString("Shuting down Backup Power for elevator %1.")
                        .arg(i+1);
        }
    }
}

Display* ElevatorControlSystem::getDisplay(int elevator) {
    return displays[elevator];
}

void ElevatorControlSystem::handleDoorObstacle(int elevatorNumber) {
    if (elevators[elevatorNumber]->isDoorOpen() && !doors[elevatorNumber]->blockDoorStatus()) {
        sensors[elevatorNumber]->setObstacle(true);
        elevators[elevatorNumber]->blockDoor();
        doors[elevatorNumber]->setDoorStatus("Blocked");
        doors[elevatorNumber]->blockDoor();

        openDoor(elevatorNumber);

        QTimer::singleShot(5000, this, [this, elevatorNumber]() {
            // 5seconds delay before the warning pops up.
            displays[elevatorNumber]->updateDisplay("The door is blocked!");
            alarms[elevatorNumber]->activateAlarm("Door Obstacle");
        });
    }
}

void ElevatorControlSystem::resolveDoorObstacle(int elevatorNumber) {
    if(doors[elevatorNumber]->blockDoorStatus()) {
        sensors[elevatorNumber]->setObstacle(false);
        elevators[elevatorNumber]->unBlockDoor();
        doors[elevatorNumber]->setDoorStatus("Opened");
        doors[elevatorNumber]->unblockDoor();
        alarms[elevatorNumber]->deactivateAlarm();
        displays[elevatorNumber]->updateDisplay("Door Obstacle resolved!");

        QTimer::singleShot(3000, this, [this, elevatorNumber]() {
            closeDoor(elevatorNumber);
        });
    }
}

void ElevatorControlSystem::handlePowerOutage() {
    emergencyStatus = true;
    if (emergencyStatus) {

        for (size_t i = 0; i < sensors.size(); ++i) {
            if (!sensors[i]->detectPowerOutage() && (sensors[i]->getBackupPowerStatus())) {
                sensors[i]->setPowerOutage(true);
                elevators[i]->emergencyStop();

                displays[i]->updateDisplay("Power Outage Emergency!");
                alarms[i]->activateAlarm("Power Outage");


                QTimer::singleShot(3000, this, [this, i]() {
                    if (elevators[i]->getCurrentFloor() < 7) {
                        elevators[i]->moveToFloor(elevators[i]->getCurrentFloor() +1);
                        displays[i]->updateDisplay("Moved to nearest floor:" + to_string(elevators[i]->getCurrentFloor()));
                    } else {
                        elevators[i]->moveToFloor(elevators[i]->getCurrentFloor());
                        displays[i]->updateDisplay("Moved to nearest floor:" + to_string(elevators[i]->getCurrentFloor()));
                    }
                });

                QTimer::singleShot(5000, this, [this, i]() {
                    if (checkIfSafeToOpenDoors(i)) {
                        doors[i]->open();
                        elevators[i]->openDoor();
                        displays[i]->updateDisplay("Doors opening. Please evacuate!");
                        emit elevatorPositionChanged(i, elevators[i]->getCurrentFloor());
                        elevators[i]->cleanPassenger();
                    }
                });
            } else {
                sensors[i]->setPowerOutage(true);

                displays[i]->updateDisplay("Power Outage Emergency!");
                alarms[i]->activateAlarm("Power Outage (Backup Power Shortage)");
                elevators[i]->emergencyStop();

                QTimer::singleShot(5000, this, [this, i]() {
                    displays[i]->updateDisplay("Connected to Building Safety Service!");
                    emergencyServiceCentre.handleEmergency("Power Outage (Backup Power Shortage)");
                    emergencyServiceCentre.calling911();
                });
            }
        }
    }
}

void ElevatorControlSystem::resolvePowerOutage() {
    if (emergencyStatus) {
        emergencyStatus = false;
        for (size_t i = 0; i < elevators.size(); ++i) {
            elevators[i]->resolvedEmergency();
            sensors[i]->setPowerOutage(false);
            alarms[i]->deactivateAlarm();
        }

        updateDisplay("Power Outage Resolved!");
        emergencyServiceCentre.resetEmergencyState();
    }
}

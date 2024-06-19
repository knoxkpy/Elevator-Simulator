#include "Sensor.h"

Sensor::Sensor() : obstacleDetected(false), overloadDetected(false), powerOutageDetected(false), backupPower(true) {}

bool Sensor::detectObstacle() {
    return obstacleDetected;
}

bool Sensor::detectOverload() {
    return overloadDetected;
}

bool Sensor::detectPowerOutage() {
    return powerOutageDetected;
}

void Sensor::setObstacle(bool o) {
    obstacleDetected = o;
}

void Sensor::setOverload(bool o) {
    overloadDetected = o;
}
void Sensor::setPowerOutage(bool o) {
    powerOutageDetected = o;
}

bool Sensor::getBackupPowerStatus() {
    return backupPower;
}

void Sensor::setBackupPower(bool power) {
    backupPower = power;
    if (power) {
        qInfo() << QString("set up Backup power to ON");
    } else {
        qInfo() << QString("set Backup power to OFF");
    }
}

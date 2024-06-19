#include "BuildingSafetyService.h"

BuildingSafetyService::BuildingSafetyService() {
    this -> currentEmergencyType = "";
    this -> called911 = false;
}


void BuildingSafetyService::calling911() {
    if (!called911) {
        called911 = true;
        qInfo() << QString("Message From Building Safety Service: Connecting to 911 Emergency Service!");
    }
}

void BuildingSafetyService::handleEmergency(const string& emergencyType) {
    this->currentEmergencyType = emergencyType;
    if (emergencyType == "Fire") {
        calling911();
    }
}

void BuildingSafetyService::resetEmergencyState() {
    called911 = false;
    currentEmergencyType = "";
}


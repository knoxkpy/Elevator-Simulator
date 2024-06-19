#ifndef BUILDINGSAFETYSERVICE_H
#define BUILDINGSAFETYSERVICE_H

#include <string>
#include <QDebug>
#include <vector>
using namespace std;

class BuildingSafetyService {
    public:
        BuildingSafetyService();
        void handleEmergency(const string& emergencyType);
        void calling911();
        void resetEmergencyState();

    private:
        string currentEmergencyType;
        bool called911;
};

#endif // BUILDINGSAFETYSERVICE_H

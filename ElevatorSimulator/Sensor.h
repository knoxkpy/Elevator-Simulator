#ifndef SENSOR_H
#define SENSOR_H

#include <string>
#include <QDebug>

using namespace std;

class Sensor {
    public:
        Sensor();
        bool detectObstacle();
        bool detectOverload();
        bool detectPowerOutage();
        void setObstacle(bool);
        void setOverload(bool);
        void setPowerOutage(bool);
        bool getBackupPowerStatus();
        void setBackupPower(bool power);

    private:
        bool obstacleDetected;
        bool overloadDetected;
        bool powerOutageDetected;
        bool backupPower;
};

#endif // SENSOR_H

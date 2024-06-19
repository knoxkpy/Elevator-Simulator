#ifndef ALARM_H
#define ALARM_H

#include <string>
#include <QDebug>
using namespace std;

class Alarm {
    public:
        Alarm(const string& type, int alarmNumber);
        void activateAlarm(const string& alarmType);
        void deactivateAlarm();
        bool isAlarmActive() const;
        string getAlarmType() const;

    private:
        string alarmType;
        int alarmNumber;
        bool active; // Indicates whether the alarm is active
};

#endif // ALARM_H

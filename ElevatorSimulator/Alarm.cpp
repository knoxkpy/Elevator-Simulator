#include "Alarm.h"

Alarm::Alarm(const string& type, int alarmNumber) : alarmType(type), alarmNumber(alarmNumber), active(false) {}

void Alarm::activateAlarm(const string& alarmType) {
    this->alarmType = alarmType;
    active = true;
    qInfo() << QString("ALARM From elevator %1: BEEP! Attention please! %2 Alarm is in effect!")
                .arg(this->alarmNumber)
                .arg(QString::fromStdString(this->alarmType));

}

void Alarm::deactivateAlarm() {
    active = false;
    alarmType = "";
}

bool Alarm::isAlarmActive() const {
    return active;
}

string Alarm::getAlarmType() const {
    return alarmType;
}

#ifndef DOOR_H
#define DOOR_H

#include <string>
using namespace std;

class Door {
    public:
        Door();
        void open();
        void close();
        string getDoorStatus();
        void setDoorStatus(const string& status);
        void blockDoor();
        void unblockDoor();
        bool blockDoorStatus();

    private:
        string doorStatus;
        bool isBlock;
};

#endif // DOOR_H

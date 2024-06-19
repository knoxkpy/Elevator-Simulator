#ifndef PASSENGER_H
#define PASSENGER_H

#include <string>
using namespace std;

class Passenger {
    public:
        Passenger(int currentFloor, int desiredFloor = 1); // Default desiredFloor as 1
        void createFloorRequest(int desiredFloor);
        bool requestElevator(int currentFloor);

        // Getters
        int getCurrentFloor() const;
        int getDesiredFloor() const;
        static int getNewPassengerID(); // Static function to generate unique ID

    private:
        static int passengerIDCounter; // Tracks the last assigned ID
        const int passengerID; // Unique ID for each passenger
        bool requestElevatorStatus;
        bool helpStatus;
        int currentFloor;
        int desiredFloor;
};


#endif // PASSENGER_H

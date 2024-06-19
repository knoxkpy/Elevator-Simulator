#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), ECS(new ElevatorControlSystem(3, this))
{
    ui->setupUi(this);

    // Set up connections for display updates
    for (int i = 0; i < 3; ++i) {
        connect(ECS->getDisplay(i), &Display::messageChanged, this, [this, i](const QString& message) {
            updateElevatorDisplay(i, message);
        });

        connect(ECS, &ElevatorControlSystem::elevatorPositionChanged,
                this, &MainWindow::updateElevatorPosition);
    }


    connect(ui->FirstFloor, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(0,1); });
    connect(ui->SecondFloor, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(0,2); });
    connect(ui->ThirdFloor, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(0,3); });
    connect(ui->fourthFloor, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(0,4); });
    connect(ui->FifthFloor, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(0,5); });
    connect(ui->SixthFloor, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(0,6); });
    connect(ui->SevenFloor, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(0,7); });
    connect(ui->OpenDoorButton, &QPushButton::clicked, this, [this](){ ECS->openDoor(0); });
    connect(ui->CloseDoorButton, &QPushButton::clicked, this, [this](){ ECS->closeDoor(0); });
    connect(ui->Emergency, &QPushButton::clicked, this, [this](){ ECS->connectToBuildingSafetyService(0); });

    connect(ui->FirstFloor_2, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(1,1); });
    connect(ui->SecondFloor_2, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(1,2); });
    connect(ui->ThirdFloor_2, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(1,3); });
    connect(ui->fourthFloor_2, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(1,4); });
    connect(ui->FifthFloor_2, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(1,5); });
    connect(ui->SixthFloor_2, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(1,6); });
    connect(ui->SevenFloor_2, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(1,7); });
    connect(ui->OpenDoorButton_2, &QPushButton::clicked, this, [this](){ ECS->openDoor(1); });
    connect(ui->CloseDoorButton_2, &QPushButton::clicked, this, [this](){ ECS->closeDoor(1); });
    connect(ui->Emergency_2, &QPushButton::clicked, this, [this](){ ECS->connectToBuildingSafetyService(1); });

    connect(ui->FirstFloor3, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(2,1); });
    connect(ui->SecondFloor3, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(2,2); });
    connect(ui->ThirdFloor3, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(2,3); });
    connect(ui->fourthFloor3, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(2,4); });
    connect(ui->FifthFloor3, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(2,5); });
    connect(ui->SixthFloor3, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(2,6); });
    connect(ui->SevenFloor3, &QPushButton::clicked, this, [this](){ ECS->moveToFloor(2,7); });
    connect(ui->OpenDoorButton3, &QPushButton::clicked, this, [this](){ ECS->openDoor(2); });
    connect(ui->CloseDoorButton3, &QPushButton::clicked, this, [this](){ ECS->closeDoor(2); });
    connect(ui->Emergency3, &QPushButton::clicked, this, [this](){ ECS->connectToBuildingSafetyService(2); });

    connect(ui->FireEmergency, &QPushButton::clicked, this, [this](){ ECS->handleFireEmergency(); });
    connect(ui->FireEmergencyOff, &QPushButton::clicked, this, [this](){ ECS->resolvedFireEmergency(); });

    connect(ui->blockDoorE1, &QPushButton::clicked, this, [this](){ ECS->handleDoorObstacle(0); });
    connect(ui->blockDoorE2, &QPushButton::clicked, this, [this](){ ECS->handleDoorObstacle(1); });
    connect(ui->blockDoorE3, &QPushButton::clicked, this, [this](){ ECS->handleDoorObstacle(2); });

    connect(ui->unblockDoorE1, &QPushButton::clicked, this, [this](){ ECS->resolveDoorObstacle(0); });
    connect(ui->unblockDoorE2, &QPushButton::clicked, this, [this](){ ECS->resolveDoorObstacle(1); });
    connect(ui->unblockDoorE3, &QPushButton::clicked, this, [this](){ ECS->resolveDoorObstacle(2); });

    connect(ui->onBackupPower, &QPushButton::clicked, this, [this](){ ECS->engageBackupPower(); });
    connect(ui->offBackupPower, &QPushButton::clicked, this, [this](){ ECS->offBackupPower(); });

    connect(ui->powerOutage, &QPushButton::clicked, this, [this](){ ECS->handlePowerOutage(); });
    connect(ui->resolvePowerOutage, &QPushButton::clicked, this, [this](){ ECS->resolvePowerOutage(); });

    connect(ui->upButton, &QPushButton::clicked, this, &MainWindow::onUpButtonClicked);
    connect(ui->downButton, &QPushButton::clicked, this, &MainWindow::onDownButtonClicked);

    connect(ui->unOverloadE1, &QPushButton::clicked, this, [this](){ ECS-> unOverLoadElevator(0); });
    connect(ui->unOverloadE2, &QPushButton::clicked, this, [this](){ ECS-> unOverLoadElevator(1); });
    connect(ui->unOverloadE3, &QPushButton::clicked, this, [this](){ ECS-> unOverLoadElevator(2); });

}

void MainWindow::onUpButtonClicked() {
    int currentFloor = ui->currentFloorPassenger->currentText().toInt();
    int waitingPassengers = ui->NumberOfPassenger->currentText().toInt();

    ECS -> allocateElevatorUp(currentFloor, waitingPassengers);
}

void MainWindow::onDownButtonClicked() {
    int currentFloor = ui->currentFloorPassenger->currentText().toInt();
    int waitingPassengers = ui->NumberOfPassenger->currentText().toInt();

    ECS -> allocateElevatorDown(currentFloor, waitingPassengers);
}

void MainWindow::updateElevatorDisplay(int elevatorIndex, const QString& message) {
    // Assuming you have labels named like "elevatorDisplay1", "elevatorDisplay2", etc.
    QLabel *label = findChild<QLabel*>(QString("elevatorDisplay%1").arg(elevatorIndex + 1));
    if (label) {
        label->setText(message);
    }
}

void MainWindow::updateElevatorPosition(int elevatorIndex, int currentFloor) {
    QLabel* label = findChild<QLabel*>(QString("elevatorLabel%1").arg(elevatorIndex + 1));
    if (label) {
        label->setText(QString("Current Floor: %1").arg(currentFloor));
    }
}


MainWindow::~MainWindow()
{
    delete ui;
    delete ECS;
}

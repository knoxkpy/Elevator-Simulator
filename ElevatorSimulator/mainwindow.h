#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLabel>
#include "ElevatorControlSystem.h"

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

public slots:
    void updateElevatorDisplay(int elevatorIndex, const QString& message);
    void updateElevatorPosition(int elevatorIndex, int currentFloor);
    void onUpButtonClicked();
    void onDownButtonClicked();

private:
    Ui::MainWindow *ui;
    ElevatorControlSystem *ECS;

};
#endif // MAINWINDOW_H

#include "Display.h"

Display::Display() : QObject(), currentMessage("") {}

void Display::updateDisplay(const std::string& message) {
    if (currentMessage != message) {
        currentMessage = message;
        emit messageChanged(QString::fromStdString(currentMessage));
    }
}

void Display::clearDisplay() {
    updateDisplay("");
}

string Display::getCurrentMessage() const {
    return currentMessage;
}

#ifndef DISPLAY_H
#define DISPLAY_H

#include <QObject>
#include <string>
using namespace std;

class Display : public QObject {
    Q_OBJECT

    public:
        Display();
        void updateDisplay(const string& message);
        void clearDisplay();
        string getCurrentMessage() const;

    signals:
        void messageChanged(const QString& newMessage); // Signal to emit when the message changes

    private:
        string currentMessage;
};

#endif // DISPLAY_H

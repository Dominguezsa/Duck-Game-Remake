#ifndef WrapperLobbyApp_H
#define WrapperLobbyApp_H

#include <QApplication>
#include <QMessageBox>
#include <QDebug>
#include <QObject>
#include <QEvent>
#include <QDebug>
#include <QString>

class WrapperLobbyApp : public QApplication {
    public:
        WrapperLobbyApp(int &argc, char **argv) : QApplication(argc, argv) {}

    protected:
        bool notify(QObject *receiver, QEvent *event) override;
};

#endif // WrapperLobbyApp_H

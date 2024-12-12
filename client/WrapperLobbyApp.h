#ifndef WrapperLobbyApp_H
#define WrapperLobbyApp_H

#include <QApplication>
#include <QDebug>
#include <QEvent>
#include <QMessageBox>
#include <QObject>
#include <QString>

class WrapperLobbyApp: public QApplication {
public:
    WrapperLobbyApp(int& argc, char** argv): QApplication(argc, argv) {}

protected:
    bool notify(QObject* receiver, QEvent* event) override;
};

#endif  // WrapperLobbyApp_H

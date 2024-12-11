#ifndef LOBBYWINDOW_H
#define LOBBYWINDOW_H

#include <QMainWindow>
#include <QMessageBox>

#include <string>

const char JOIN_MATCH_ACTION = 'J';
const char CREATE_MATCH_ACTION = 'C';

QT_BEGIN_NAMESPACE
namespace Ui {
    class LobbyWindow;
}
QT_END_NAMESPACE

class LobbyWindow : public QMainWindow {
    Q_OBJECT

    public:
        LobbyWindow(QWidget *parent = nullptr);
        ~LobbyWindow();

    private:
        Ui::LobbyWindow *ui;

        std::string player_name;
        std::string host_name;
        std::string port;

        // For create or join match action
        char action;
        
        // For create match action
        std::string match_name;
        int number_of_players;
    
    private:
        void validateLoginInputs();
        void validateCreateMatchInputs();

        void confirmAction();
        void createMatchAction();
        void joinMatchAction();
        void connectSignals();

        void resetLoginWidgets();

        bool tryConnectServer();
        void receiveMatchList();

        void showFailedConnectionMessage();
        void showCreateMatchMessage(bool success);
        void showJoinMatchMessage(bool success);
    
    public slots:
        void validateSelectedMatch(int index);
};
#endif // LOBBYWINDOW_H

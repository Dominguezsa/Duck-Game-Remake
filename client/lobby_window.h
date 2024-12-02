#ifndef LOBBY_WINDOW_H
#define LOBBY_WINDOW_H

#include <QComboBox>
#include <QGraphicsPixmapItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QLabel>
#include <QLineEdit>
#include <QMainWindow>
#include <QPalette>
#include <QPixmap>
#include <QPushButton>
#include <QSpinBox>
#include <QStackedWidget>
#include <QVBoxLayout>
#include <string>


#include "lobby.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class LobbyWindow: public QMainWindow {
    Q_OBJECT

signals:
    void matchCreated();
    void matchJoined();
    void lobbyClosed();

private:
    Lobby lobby;
    std::string playerName;


    // GUI components
    QStackedWidget* stackedWidget;
    QWidget* mainMenuPage;
    QWidget* createMatchPage;
    QWidget* joinMatchPage;
    QLineEdit* playerNameInput;
    QPushButton* submitNameButton;
    QPushButton* createMatchButton;
    QPushButton* joinMatchButton;
    QComboBox* mapSelector;
    QLineEdit* matchNameInput;
    QSpinBox* playerCountSpinner;
    QPushButton* createButton;
    QComboBox* matchSelector;
    QPushButton* joinButton;
    QLabel* duckCharacterLabel;

    // Private setup methods
    void setupMainMenu();
    void setupCreateMatchPage();
    void setupJoinMatchPage();
    void showErrorMessage(const QString& message);
    void applyDuckGameTheme();
    void resetCreateMatchPage();
    void resetJoinMatchPage();


public:
    explicit LobbyWindow(Socket& socket, QWidget* parent = nullptr);
    ~LobbyWindow();

private slots:
    void onPlayerNameSubmitted();
    void onCreateMatchClicked();
    void onJoinMatchClicked();
    void onMatchCreationSubmitted();
    void onMatchJoinSubmitted();
    void updateAvailableMatches();
};

#endif  // LOBBY_WINDOW_H

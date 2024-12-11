#include "LobbyWindow.h"
#include "./ui_lobbywindow.h"

LobbyWindow::LobbyWindow(std::shared_ptr<Socket>* skt, QWidget *parent) : QMainWindow(parent), ui(new Ui::LobbyWindow),
                                                                          socket(skt), lobbyProtocol(nullptr) {
    ui->setupUi(this);
    ui->centralWidget->setCurrentWidget(ui->firstLobbyScene);
    
    connectSignals();
}

void LobbyWindow::connectSignals() {
    // First lobby scene signals:
    connect(ui->playMatchButton, &QPushButton::clicked, this, [this]() {
        ui->centralWidget->setCurrentWidget(ui->loginScene);
        });

    // TO DO: createMapButton signals

    // Login scene signals:
    connect(ui->hostNameLineEdit, &QLineEdit::textChanged, this, &LobbyWindow::validateLoginInputs);
    connect(ui->serverPortLineEdit, &QLineEdit::textChanged, this, &LobbyWindow::validateLoginInputs);
    connect(ui->playerNameLineEdit, &QLineEdit::textChanged, this, &LobbyWindow::validateLoginInputs);

    connect(ui->confirmButton, &QPushButton::clicked, this, &LobbyWindow::confirmAction);

    connect(ui->joinMatchButton, &QPushButton::clicked, this, [this]() {
        this->receiveMatchList();
        ui->centralWidget->setCurrentWidget(ui->joinMatchScene);
        });
    connect(ui->createMatchButton, &QPushButton::clicked, this, [this]() {
        this->receiveMapList();
        ui->centralWidget->setCurrentWidget(ui->createMatchScene);
        });
    resetLoginWidgets();

    // Create match scene signals:
    ui->createButton->setEnabled(false);
    connect(ui->createButton, &QPushButton::clicked, this, &LobbyWindow::createMatchAction);
    connect(ui->matchNameLineEdit, &QLineEdit::textChanged, this, &LobbyWindow::validateCreateMatchInputs);
    connect(ui->numberLineEdit, &QLineEdit::textChanged, this, &LobbyWindow::validateCreateMatchInputs);
    connect(ui->mapList, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LobbyWindow::validateCreateMatchInputs);

    // Join match scene signals:
    ui->joinButton->setEnabled(false);
    connect(ui->joinButton, &QPushButton::clicked, this, &LobbyWindow::joinMatchAction);
    connect(ui->matchList, QOverload<int>::of(&QComboBox::currentIndexChanged), this, &LobbyWindow::validateSelectedMatch);
}

// --------------------- Login scene logic ---------------------

void LobbyWindow::confirmAction() {
    if (tryConnectServer()) {
        ui->joinMatchButton->setEnabled(true);
        ui->createMatchButton->setEnabled(true);

        ui->confirmButton->setEnabled(false);
        ui->hostNameLineEdit->setReadOnly(true);
        ui->serverPortLineEdit->setReadOnly(true);
        ui->playerNameLineEdit->setReadOnly(true);
    } else {
        resetLoginWidgets();
        showFailedConnectionMessage();
    }
}

void LobbyWindow::showFailedConnectionMessage() {
    QMessageBox::warning(nullptr, "Connection failed", "Please note the following and try again:\n\n"
                                  "- Make sure to enter a valid host name. (e.g. 'localhost')\n"
                                  "- Make sure to enter the correct port for an active server.\n");
}

bool LobbyWindow::tryConnectServer() {
    try {
        *this->socket = std::make_shared<Socket>(host_name.c_str(), port.c_str());
        this->lobbyProtocol = std::make_unique<LobbyProtocol>(**socket);
    } catch (const std::exception& e) {
        return false;
    }
    return true;
}

void LobbyWindow::validateLoginInputs() {
    bool valid1 = false;
    bool valid2 = false;
    bool valid3 = false;

    if (!ui->hostNameLineEdit->text().isEmpty()) {
        valid1 = true;
        host_name = ui->hostNameLineEdit->text().toStdString();
    }

    if (!ui->serverPortLineEdit->text().isEmpty()) {
        valid2 = true;
        port = ui->serverPortLineEdit->text().toStdString();
    }

    if (!ui->playerNameLineEdit->text().isEmpty()) {
        valid3 = true;
        player_name = ui->playerNameLineEdit->text().toStdString();
    }
    ui->confirmButton->setEnabled(valid1 && valid2 && valid3);
}

void LobbyWindow::resetLoginWidgets() {
    ui->hostNameLineEdit->clear();
    ui->serverPortLineEdit->clear();
    ui->playerNameLineEdit->clear();

    ui->joinMatchButton->setEnabled(false);
    ui->createMatchButton->setEnabled(false);
    ui->confirmButton->setEnabled(false);
}

// --------------------- Create match scene logic ---------------------

void LobbyWindow::createMatchAction() {
    this->action = CREATE_MATCH_ACTION;

    // TO DO: Esta validacion deberia provenir del server al intentar
    // crear una partida, pero por ahora la dejamos asi
    bool success = true;
    showCreateMatchMessage(success);

    if (success)
        close();
}

void LobbyWindow::validateCreateMatchInputs() {
    bool valid1 = false;
    bool valid2 = false;
    bool valid3 = ui->mapList->currentIndex() > 0; // A match must be selected

    if (!ui->matchNameLineEdit->text().isEmpty()) {
        valid1 = true;
        this->match_name = ui->matchNameLineEdit->text().toStdString();
    }

    bool ok_number;
    int num = ui->numberLineEdit->text().toInt(&ok_number);
    if (ok_number && num > 1) {
        valid2 = true;
        this->number_of_players = num;
    }
    ui->createButton->setEnabled(valid1 && valid2 && valid3);
}

void LobbyWindow::receiveMapList() {
    // TO DO: receive map list from server
    ui->mapList->clear();

    ui->mapList->addItem("Select a map");
    ui->mapList->setCurrentIndex(0);
    ui->mapList->addItem("Map 1");
    ui->mapList->addItem("Map 2");
}

void LobbyWindow::showCreateMatchMessage(bool success) {
    if (success) {
        QMessageBox::information(nullptr, "Match created", "The match was created successfully.");
    } else {
        QMessageBox::warning(nullptr, "Match creation failed", "The match could not be created. Please try again.");
    }
}

// --------------------- Join match scene logic ---------------------

void LobbyWindow::joinMatchAction() {
    this->action = JOIN_MATCH_ACTION;
    
    // TO DO: Esta validacion deberia provenir del server
    //        al intentar unirnos a una partida.
    bool success = true;

    showJoinMatchMessage(success);

    if (success)
        close();
}

void LobbyWindow::validateSelectedMatch(int index[[maybe_unused]]) {
    if (ui->matchList->currentIndex() != 0) {
        ui->joinButton->setEnabled(true);
    } else {
        // Disable the button if the default option is selected
        ui->joinButton->setEnabled(false);
    }
}

void LobbyWindow::receiveMatchList() {
    // TO DO: receive match list from server
    ui->matchList->clear();

    ui->matchList->addItem("Select a match");
    ui->matchList->setCurrentIndex(0);
    ui->matchList->addItem("Match 1");
    ui->matchList->addItem("Match 2");
}

void LobbyWindow::showJoinMatchMessage(bool success) {
    if (success) {
        QMessageBox::information(nullptr, "Match joined", "You have joined the match successfully.");
    } else {
        QMessageBox::warning(nullptr, "Match join failed", "The match could not be joined. Please try again.");
        ui->matchList->setCurrentIndex(0);
    }
}

// --------------------- Destructor ---------------------

LobbyWindow::~LobbyWindow() {
    delete ui;
}

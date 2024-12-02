#include "lobby_window.h"

#include <QFile>
#include <QGroupBox>
#include <QHBoxLayout>
#include <QMessageBox>
#include <QPixmap>
#include <QVBoxLayout>

LobbyWindow::LobbyWindow(Socket& socket, QWidget* parent): QMainWindow(parent), lobby(socket) {


    setWindowTitle("Duck Game Lobby");
    resize(1024, 768);

    stackedWidget = new QStackedWidget(this);
    setCentralWidget(stackedWidget);

    applyDuckGameTheme();

    setupMainMenu();
    setupCreateMatchPage();
    setupJoinMatchPage();

    stackedWidget->setCurrentWidget(mainMenuPage);  // Start with main menu
}

void LobbyWindow::applyDuckGameTheme() {
    // Load background image from resources
    QPixmap background("/var/duck_game/general/fondo.png");
    if (!background.isNull()) {  // Check if the image loads successfully
        QPalette palette;
        palette.setBrush(QPalette::Window, background.scaled(size(), Qt::IgnoreAspectRatio,
                                                             Qt::SmoothTransformation));
        setPalette(palette);
    } else {
        qWarning("Failed to load background image!");
    }

    // Duck character as a decorative element
    duckCharacterLabel = new QLabel(this);
    QPixmap duckImage("/var/duck_game/general/duckGameLobby.png");
    if (!duckImage.isNull()) {
        duckCharacterLabel->setPixmap(
                duckImage.scaled(100, 100, Qt::KeepAspectRatio, Qt::SmoothTransformation));
        duckCharacterLabel->setFixedSize(100, 100);    // Adjust as needed
        duckCharacterLabel->move(50, height() - 150);  // Position the duck character at bottom left
    } else {
        qWarning("Failed to load duck character image!");
    }

    // Button styling with hover effect
    QString buttonStyle = "QPushButton { background-color: #8B4513; color: white; border-radius: "
                          "10px; padding: 10px; font-size: 16px; }"
                          "QPushButton:hover { background-color: #A0522D; font-weight: bold; }";
    setStyleSheet(buttonStyle);
}

void LobbyWindow::setupMainMenu() {
    mainMenuPage = new QWidget();
    auto* layout = new QVBoxLayout(mainMenuPage);

    auto* nameGroup = new QGroupBox("Player Information");
    auto* nameLayout = new QVBoxLayout();

    playerNameInput = new QLineEdit();
    playerNameInput->setPlaceholderText("Enter your name");
    submitNameButton = new QPushButton("Set Name");

    nameLayout->addWidget(playerNameInput);
    nameLayout->addWidget(submitNameButton);
    nameGroup->setLayout(nameLayout);

    createMatchButton = new QPushButton("Create New Match");
    joinMatchButton = new QPushButton("Join Existing Match");

    // Ensure buttons are disabled by default
    createMatchButton->setEnabled(false);
    joinMatchButton->setEnabled(false);

    layout->addWidget(nameGroup);
    layout->addWidget(createMatchButton);
    layout->addWidget(joinMatchButton);
    layout->addStretch();

    connect(submitNameButton, &QPushButton::clicked, this, &LobbyWindow::onPlayerNameSubmitted);
    connect(createMatchButton, &QPushButton::clicked, this, &LobbyWindow::onCreateMatchClicked);
    connect(joinMatchButton, &QPushButton::clicked, this, &LobbyWindow::onJoinMatchClicked);

    stackedWidget->addWidget(mainMenuPage);
}


void LobbyWindow::setupCreateMatchPage() {
    createMatchPage = new QWidget();
    auto* layout = new QVBoxLayout(createMatchPage);

    mapSelector = new QComboBox();
    matchNameInput = new QLineEdit();
    matchNameInput->setPlaceholderText("Enter match name");

    playerCountSpinner = new QSpinBox();
    playerCountSpinner->setRange(2, 4);
    playerCountSpinner->setValue(2);

    createButton = new QPushButton("Create Match");

    layout->addWidget(new QLabel("Select Map:"));
    layout->addWidget(mapSelector);
    layout->addWidget(new QLabel("Match Name:"));
    layout->addWidget(matchNameInput);
    layout->addWidget(new QLabel("Number of Players:"));
    layout->addWidget(playerCountSpinner);
    layout->addWidget(createButton);
    layout->addStretch();

    connect(createButton, &QPushButton::clicked, this, &LobbyWindow::onMatchCreationSubmitted);

    stackedWidget->addWidget(createMatchPage);
}

void LobbyWindow::setupJoinMatchPage() {
    joinMatchPage = new QWidget();
    auto* layout = new QVBoxLayout(joinMatchPage);

    matchSelector = new QComboBox();
    joinButton = new QPushButton("Join Match");

    layout->addWidget(new QLabel("Select Match:"));
    layout->addWidget(matchSelector);
    layout->addWidget(joinButton);
    layout->addStretch();

    connect(joinButton, &QPushButton::clicked, this, &LobbyWindow::onMatchJoinSubmitted);

    stackedWidget->addWidget(joinMatchPage);
}


void LobbyWindow::onPlayerNameSubmitted() {
    QString name = playerNameInput->text().trimmed();
    if (name.isEmpty()) {
        showErrorMessage("Please enter a valid name");
        return;
    }

    // Safely set the player name
    playerName = name.toStdString();

    // Enable match creation/joining buttons
    createMatchButton->setEnabled(true);
    joinMatchButton->setEnabled(true);

    // Disable name input
    playerNameInput->setEnabled(false);
    submitNameButton->setEnabled(false);
}

void LobbyWindow::onCreateMatchClicked() {
    lobby.sendCreateCommand(playerName);
    auto maps = lobby.receiveMapList();

    mapSelector->clear();
    for (const auto& map: maps) {
        mapSelector->addItem(QString::fromStdString(map));
    }

    stackedWidget->setCurrentWidget(createMatchPage);
}

void LobbyWindow::onJoinMatchClicked() {
    lobby.sendJoinCommand(playerName);
    updateAvailableMatches();
    stackedWidget->setCurrentWidget(joinMatchPage);
}

void LobbyWindow::updateAvailableMatches() {
    matchSelector->clear();
    auto matches = lobby.receiveMatchList();
    for (const auto& match: matches) {
        matchSelector->addItem(QString::fromStdString(match));
    }
}

void LobbyWindow::onMatchCreationSubmitted() {
    QString matchName = matchNameInput->text().trimmed();
    if (matchName.isEmpty()) {
        showErrorMessage("Please enter a valid match name");
        return;
    }

    int confirmation = lobby.sendMatchCreation(playerCountSpinner->value(), matchName.toStdString(),
                                               mapSelector->currentText().toStdString());

    if (confirmation == 1) {
        QMessageBox::information(this, "Success", "Match created successfully!");
        this->close();  // Close lobby window to start game
    } else {
        showErrorMessage("Failed to create match. Please try again.");
        stackedWidget->setCurrentWidget(mainMenuPage);
    }
}


void LobbyWindow::onMatchJoinSubmitted() {
    if (matchSelector->count() == 0) {
        showErrorMessage("No matches available to join");
        return;
    }

    int confirmation = lobby.sendMatchSelection(matchSelector->currentText().toStdString());

    if (confirmation == 1) {
        QMessageBox::information(this, "Success", "Match joined successfully!");
        this->close();
    } else {
        showErrorMessage("Failed to join match. Please try again.");
        updateAvailableMatches();
    }
}


void LobbyWindow::showErrorMessage(const QString& message) {
    QMessageBox::warning(this, "Error", message);
}

LobbyWindow::~LobbyWindow() {
    lobby.quit();
    emit lobbyClosed();
}

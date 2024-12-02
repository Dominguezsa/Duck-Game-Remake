#include "EditorWindow.h"

#include "./ui_EditorWindow.h"

EditorWindow::EditorWindow(QWidget* parent):
        QMainWindow(parent),
        ui(new Ui::EditorWindow),
        gameMapScene(this),
        tiles(),
        providers(),
        selectedPixmap(),
        map_id(),
        mapCreatedMessageBox(this) {

    ui->setupUi(this);
    // Crear escena y vista
    ui->graphicsView->setScene(&gameMapScene);

    // Escena del creador de mapas:

    connect(ui->saveButton, &QPushButton::clicked, this, [this]() { saveMap(mapData); });

    // Escena del menu de inputs:

    ui->centralWidget->setCurrentWidget(ui->dataInputScene);

    connect(ui->mapNameLineEdit, &QLineEdit::textChanged, this, &EditorWindow::validateInputs);
    connect(ui->mapHeightLineEdit, &QLineEdit::textChanged, this, &EditorWindow::validateInputs);
    connect(ui->mapWidthLineEdit, &QLineEdit::textChanged, this, &EditorWindow::validateInputs);

    connect(ui->exitButton, &QPushButton::clicked, this, &QWidget::close);

    ui->createMapButton->setEnabled(false);
    connect(ui->createMapButton, &QPushButton::clicked, this, &EditorWindow::goToCreateMapScene);
}

void EditorWindow::setMapInfo(std::vector<std::vector<uint8_t>>& ids_matrix, int& width,
                              int& height, std::string& mapName, bool& map_saved) {
    this->matrix_ids = &ids_matrix;
    this->width = &width;
    this->height = &height;
    this->mapName = &mapName;
    this->mapSaved = &map_saved;
}

void EditorWindow::initMapScene() {
    matrix_ids->resize(*height, std::vector<uint8_t>(*width, 0));
    for (int row = 0; row < *height; ++row) {
        for (int col = 0; col < *width; ++col) {
            Tile* tile = new Tile(row, col, (*matrix_ids)[row][col]);
            tile->setPos(col * tile->getSize(), row * tile->getSize());
            gameMapScene.addItem(tile);
            tiles.push_back(tile);
            connect(tile, &Tile::tileClicked, this, &EditorWindow::updateTileImage);
        }
    }
    setSelectableImages();
    // Conectar la señal 'clicked' de los ImageWidget a un slot
    for (ImageWidget* provider: providers) {
        connect(provider, &ImageWidget::clicked, this, &EditorWindow::onImageProviderClicked);
    }
}

void EditorWindow::goToCreateMapScene() {
    initMapScene();
    ui->centralWidget->setCurrentWidget(ui->mapCreatorScene);
}

void EditorWindow::saveMap(std::string mapData) {
    emit mapCreated(mapData);

    mapCreatedMessageBox.setIcon(QMessageBox::Information);
    mapCreatedMessageBox.setWindowTitle("Save Successful");
    mapCreatedMessageBox.setText("The map has been successfully saved!");

    // Conectar el botón "OK" del QMessageBox a la acción de cerrar la ventana
    connect(&mapCreatedMessageBox, &QMessageBox::accepted, this, &QWidget::close);

    // Mostrar el QMessageBox
    mapCreatedMessageBox.exec();

    // Confirmamos que el mapa ha sido guardado
    *mapSaved = true;
}

void EditorWindow::setSelectableImages() {
    ui->gridLayout->setHorizontalSpacing(5);
    ui->gridLayout->setVerticalSpacing(5);

    // Platforms:
    int row = 0;
    QPixmap originalPlatformsImage(
            "/var/duck_game/map-stuff/forest/tileset-platforms-60x60.png");
    uint8_t id = 1;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            ImageWidget* imageWidget = new ImageWidget();
            QRect section(j * 60, i * 60, 60, 60);
            QPixmap sectionPixmap = originalPlatformsImage.copy(section);
            imageWidget->setImage(sectionPixmap);
            providers.push_back(imageWidget);
            ui->gridLayout->addWidget(imageWidget, i, j);
            map_id[id] = sectionPixmap;
            id++;
        }
        row++;
    }

    // Spawns/Boxes:
    QPixmap originalItemsImage("/var/duck_game/map-stuff/items-75x75.png");
    row++;
    for (int i = 0; i < 3; ++i) {
        ImageWidget* imageWidget = new ImageWidget();
        QRect section(i * 75, row, 75, 75);
        QPixmap sectionPixmap = originalItemsImage.copy(section);
        imageWidget->setImage(sectionPixmap);
        providers.push_back(imageWidget);
        ui->gridLayout->addWidget(imageWidget, row, i);
        map_id[id] = sectionPixmap;
        id++;
    }

    // Collectibles:
    row++;
    int col = 0;
    QPixmap originalCollectiblesImage("/var/duck_game/map-stuff/collectibles_36x32.png");
    for (int i = 0; i < 12; ++i) {
        if (i > 0 && i % 3 == 0) {
            row++;
        }
        ImageWidget* imageWidget = new ImageWidget();
        QRect section(i * 36, row, 36, 32);
        QPixmap sectionPixmap = originalCollectiblesImage.copy(section);
        imageWidget->setImage(sectionPixmap);
        providers.push_back(imageWidget);
        ui->gridLayout->addWidget(imageWidget, row, col++);
        if (col == 3) {
            col = 0;
        }
        map_id[id] = sectionPixmap;
        id++;
    }
}

void EditorWindow::onImageProviderClicked(ImageWidget* sender) {
    // Lógica para manejar el clic en el proveedor de imágenes
    for (ImageWidget* provider: providers) {
        provider->select(false);  // Deseleccionar todos los proveedores
    }
    sender->select(true);  // Seleccionar el proveedor que fue clickeado

    // Asignar la imagen seleccionada al QPixmap
    selectedPixmap = sender->getImage();
}

void EditorWindow::updateTileImage(int row, int col) {
    if (!selectedPixmap.isNull()) {
        // Buscar el Tile correspondiente a la fila y columna
        for (auto* item: gameMapScene.items()) {
            if (Tile* tile = dynamic_cast<Tile*>(item)) {
                if (tile->getRow() == row && tile->getCol() == col) {
                    // Asignar la imagen seleccionada al Tile
                    tile->setImage(selectedPixmap, map_id);
                    break;
                }
            }
        }
    }
}

void EditorWindow::validateInputs() {
    bool valid1 = false;
    bool valid2 = false;
    bool valid3 = false;

    bool ok1;
    int num1 = ui->mapWidthLineEdit->text().toInt(&ok1);
    if (ok1 && num1 >= 25) {
        valid1 = true;
        *width = num1;
    }
    bool ok2;
    int num2 = ui->mapHeightLineEdit->text().toInt(&ok2);
    if (ok2 && num2 >= 15) {
        valid2 = true;
        *height = num2;
    }
    // Validar tercer QLineEdit: no vacío
    if (!ui->mapNameLineEdit->text().isEmpty()) {
        valid3 = true;
        *mapName = ui->mapNameLineEdit->text().toStdString();
    }
    // Habilitar el botón solo si todas las entradas son válidas
    ui->createMapButton->setEnabled(valid1 && valid2 && valid3);
}

EditorWindow::~EditorWindow() {
    delete ui;
    for (Tile* tile: tiles) {
        delete tile;
    }
    for (ImageWidget* provider: providers) {
        delete provider;
    }
}

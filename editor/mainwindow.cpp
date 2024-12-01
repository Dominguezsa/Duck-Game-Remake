#include "mainwindow.h"
#include "./ui_mainwindow.h"

MainWindow::MainWindow(QWidget* parent) : QMainWindow(parent)
    , ui(new Ui::MainWindow), gameMapScene(this), tiles(), providers(), selectedPixmap(), map_id() {
    
    ui->setupUi(this);
    // Crear escena y vista
    ui->view->setScene(&gameMapScene);

    connect(ui->pushButton, &QPushButton::clicked, this, [this]() { saveMap(mapData); });
}

void MainWindow::initMapScene(const int& map_width, const int& map_height, 
                              std::vector<std::vector<uint8_t>>& ids_matrix) {
    for (int row = 0; row < map_height; ++row) {
        for (int col = 0; col < map_width; ++col) {
            Tile* tile = new Tile(row, col, ids_matrix[row][col]);
            tile->setPos(col * tile->getSize(), row * tile->getSize());
            gameMapScene.addItem(tile);
            tiles.push_back(tile);
            connect(tile, &Tile::tileClicked, this, &MainWindow::updateTileImage);
        }
    }
    setSelectableImages();
    // Conectar la señal 'clicked' de los ImageWidget a un slot
    for (ImageWidget* provider : providers) {
        connect(provider, &ImageWidget::clicked, this, &MainWindow::onImageProviderClicked);
    }
}

void MainWindow::saveMap(std::string mapData) {
    emit mapCreated(mapData);
    QMessageBox::information(this, "Save Successful", "The map has been successfully saved!");
}

void MainWindow::setSelectableImages() {
    ui->gridLayout->setHorizontalSpacing(5);
    ui->gridLayout->setVerticalSpacing(5);
    
    // Platforms:
    int row = 0;
    QPixmap originalPlatformsImage("/var/duck_game/data/map-stuff/forest/tileset-platforms-60x60.png");
    uint8_t id = 0;

    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            ImageWidget* imageWidget = new ImageWidget();
            QRect section(j*60, i*60, 60, 60);
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
    QPixmap originalItemsImage("/var/duck_game/data/map-stuff/items-75x75.png");
    row ++;
    for (int i = 0; i < 3; ++i) {
        ImageWidget* imageWidget = new ImageWidget();
        QRect section(i*75, row, 75, 75);
        QPixmap sectionPixmap = originalItemsImage.copy(section);
        imageWidget->setImage(sectionPixmap);
        providers.push_back(imageWidget);
        ui->gridLayout->addWidget(imageWidget, row, i);
        map_id[id] = sectionPixmap;
        id++;
    }

    // Collectibles:
    row ++;
    int col = 0;
    QPixmap originalCollectiblesImage("/var/duck_game/data/map-stuff/collectibles_36x32.png");
    for (int i = 0; i < 12; ++i) {
        if (i > 0 && i%3 == 0) {
            row++;
        }
        ImageWidget* imageWidget = new ImageWidget();
        QRect section(i*36, row, 36, 32);
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

void MainWindow::onImageProviderClicked(ImageWidget* sender) {
    // Lógica para manejar el clic en el proveedor de imágenes
    for (ImageWidget* provider : providers) {
        provider->select(false); // Deseleccionar todos los proveedores
    }
    sender->select(true); // Seleccionar el proveedor que fue clickeado

    // Asignar la imagen seleccionada al QPixmap
    selectedPixmap = sender->getImage();
}

void MainWindow::updateTileImage(int row, int col) {
    if (!selectedPixmap.isNull()) {
        // Buscar el Tile correspondiente a la fila y columna
        for (auto* item : gameMapScene.items()) {
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

MainWindow::~MainWindow()
{
    delete ui;
    for (Tile* tile : tiles) {
        delete tile;
    }
    for (ImageWidget* provider : providers) {
        delete provider;
    }
}

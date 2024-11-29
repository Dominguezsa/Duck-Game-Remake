#include "MapEditor.h"
#include <iostream>

MapEditor::MapEditor(QApplication& _app): window(), app(_app), id_matrix() {
    window.setAutoFillBackground(true); // Permitir que el fondo se pinte con el palette
    QPalette palette = window.palette();
    palette.setColor(QPalette::Window, Qt::gray); // Establecer el fondo a negro
    window.setPalette(palette);
}

int MapEditor::run() {
    // Pedir al ususaio
    std::string map_name = "Example name";
    int width = 25, height = 15;

    id_matrix.resize(height, std::vector<uint8_t>(width, 0));
    window.initMapScene(width, height, id_matrix);
    window.show();
    int return_value = app.exec();
    MapParser::parseMap(map_name, width, height, id_matrix);
    return return_value;
}

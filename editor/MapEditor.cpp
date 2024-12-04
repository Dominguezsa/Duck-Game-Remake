#include "MapEditor.h"

#include <iostream>

MapEditor::MapEditor(QApplication& _app):
        window(),
        app(_app),
        id_matrix(),
        map_width(0),
        map_height(0),
        map_name(""),
        saved_map(false) {
    window.setAutoFillBackground(true);  // Permitir que el fondo se pinte con el palette
    QPalette palette = window.palette();
    palette.setColor(QPalette::Window, Qt::gray);  // Establecer el fondo a negro
    window.setPalette(palette);
}

int MapEditor::run() {
    window.setMapInfo(id_matrix, map_width, map_height, map_name, saved_map);
    window.show();
    int exitCode = app.exec();

    if (saved_map) {
        MapParser::parseMap(map_name, map_width, map_height, id_matrix);
    }
    return exitCode;
}

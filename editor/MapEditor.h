#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include <QApplication>
#include <QLocale>
#include <QPalette>
#include <QTranslator>
#include <cstdint>
#include <string>
#include <vector>

#include "MapParser.h"
#include "mainwindow.h"

class MapEditor {
private:
    MainWindow window;
    QApplication& app;
    std::vector<std::vector<uint8_t>> id_matrix;

public:
    MapEditor(QApplication& _app);
    int run();
    ~MapEditor() = default;
};

#endif  // MAP_EDITOR_H

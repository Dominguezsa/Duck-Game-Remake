#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include "mainwindow.h"
#include "MapParser.h"
#include <string>
#include <vector>
#include <cstdint>

#include <QApplication>
#include <QLocale>
#include <QTranslator>
#include <QPalette>

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

#endif // MAP_EDITOR_H

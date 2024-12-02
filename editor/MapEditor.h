#ifndef MAP_EDITOR_H
#define MAP_EDITOR_H

#include <QApplication>
#include <QLocale>
#include <QPalette>
#include <QTranslator>
#include <cstdint>
#include <string>
#include <vector>

#include "EditorWindow.h"
#include "MapParser.h"

#define SUCCES_EXIT 0

class MapEditor {
private:
    EditorWindow window;
    QApplication& app;
    std::vector<std::vector<uint8_t>> id_matrix;
    int map_width;
    int map_height;
    std::string map_name;
    bool saved_map;

public:
    explicit MapEditor(QApplication& _app);
    int run();
    ~MapEditor() = default;
};

#endif  // MAP_EDITOR_H

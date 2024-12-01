#include <QApplication>

#include <stdio.h>

#include "MapEditor.h"

int main(int argc, char* argv[]) {
    QApplication a(argc, argv);
    MapEditor editor(a);
    return editor.run();
}

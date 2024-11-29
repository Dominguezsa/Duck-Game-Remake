#include <QApplication>
#include "MapEditor.h"
#include <stdio.h>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    MapEditor editor(a);
    return editor.run();
}

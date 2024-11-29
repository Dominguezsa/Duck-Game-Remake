#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include "ImageWidget.h"
#include <QMainWindow>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QApplication>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsRectItem>
#include <QString>
#include <QMessageBox>

#include "Tile.h"
#include <vector>
#include <string>
#include <cstdint>

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
    int width = 25, height = 15;
    void initMapScene(const int& map_width, const int& map_height, 
                      std::vector<std::vector<uint8_t>>& ids_matrix);

private:
    Ui::MainWindow *ui;
    QGraphicsScene gameMapScene;
    std::vector<Tile*> tiles;
    std::vector<ImageWidget*> providers;
    QPixmap selectedPixmap;
    std::string mapData;

    void setSelectableImages();
    void updateTileImage(int row, int col);
    


public slots:
    void saveMap(std::string mapData);

private slots:
    void onImageProviderClicked(ImageWidget* sender);
    //void onWidgetClicked(ImageWidget* widget);

signals:
    void clicked(ImageWidget* widget);
    void mapCreated(const std::string& mapData);

};
#endif // MAINWINDOW_H

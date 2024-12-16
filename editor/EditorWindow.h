#ifndef EditorWindow_H
#define EditorWindow_H

#include <QApplication>
#include <QDir>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QMainWindow>
#include <QMessageBox>
#include <QString>
#include <cstdint>
#include <map>
#include <string>
#include <vector>

#include "ImageWidget.h"
#include "Tile.h"

#define MIN_WIDTH 25
#define MIN_HEIGHT 15

QT_BEGIN_NAMESPACE
namespace Ui {
class EditorWindow;
}
QT_END_NAMESPACE

class EditorWindow: public QMainWindow {
    Q_OBJECT

public:
    explicit EditorWindow(QWidget* parent = nullptr);
    ~EditorWindow();

    void setMapInfo(std::vector<std::vector<uint8_t>>& ids_matrix, int& width, int& height,
                    std::string& mapName, bool& map_saved);

private:
    Ui::EditorWindow* ui;
    QGraphicsScene gameMapScene;
    std::vector<Tile*> tiles;
    std::vector<ImageWidget*> providers;
    QPixmap selectedPixmap;
    std::string mapData;
    std::map<uint8_t, QPixmap> map_id;
    QMessageBox mapCreatedMessageBox;
    int* width;
    int* height;
    std::string* mapName;
    bool* mapSaved;
    std::vector<std::vector<uint8_t>>* matrix_ids;


    void setSelectableImages();
    void updateTileImage(int row, int col);
    void validateInputs();
    void goToCreateMapScene();
    void initMapScene();
    void setBackGroundImage();
    bool validMap();
public slots:
    void saveMap(const std::string& mapData);

private slots:
    void onImageProviderClicked(ImageWidget* sender);

signals:
    void clicked(ImageWidget* widget);
    void mapCreated(const std::string& mapData);
};
#endif  // EditorWindow_H

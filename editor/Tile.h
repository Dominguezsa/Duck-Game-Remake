#ifndef TILE_H
#define TILE_H

#include <QBrush>
#include <QGraphicsPixmapItem>
#include <QGraphicsRectItem>
#include <QGraphicsSceneMouseEvent>
#include <QObject>
#include <QPainter>
#include <QPen>
#include <QPixmap>
#include <cstdint>

const int TILE_SIZE = 40;
const int TILE_BORDER_WIDTH = 1;

class Tile: public QObject, public QGraphicsRectItem {
    Q_OBJECT

private:
    QGraphicsPixmapItem* imageItem = nullptr;
    QPixmap imagePixmap;
    const int row;
    const int col;
    uint8_t& image_id;

public:
    Tile(int row, int col, uint8_t& id, QGraphicsItem* parent = nullptr);
    int getRow() const;
    int getCol() const;
    int getSize() const;
    void setImage(const QPixmap& pixmap);

protected:
    void mousePressEvent(QGraphicsSceneMouseEvent* event) override;
    void paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget) override;

signals:
    void tileClicked(int row, int col);
};

#endif  // TILE_H

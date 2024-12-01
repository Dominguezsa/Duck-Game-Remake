#include "Tile.h"


Tile::Tile(int row, int col, uint8_t& id, QGraphicsItem* parent):
        QObject(), QGraphicsRectItem(parent), imagePixmap(), row(row), col(col), image_id(id) {
    setRect(0, 0, TILE_SIZE, TILE_SIZE);
    setBrush(QBrush(Qt::white));  // Fondo inicial
    setFlag(QGraphicsItem::ItemIsSelectable);
    setFlag(QGraphicsItem::ItemIsMovable, false);
    setFlag(QGraphicsItem::ItemIsFocusable, false);
}

int Tile::getRow() const { return row; }

int Tile::getCol() const { return col; }

int Tile::getSize() const { return TILE_SIZE; }

void Tile::setImage(const QPixmap& pixmap, std::map<uint8_t, QPixmap>& map_id) {
    if (!pixmap.isNull() && !imagePixmap.isNull() && imagePixmap.cacheKey() == pixmap.cacheKey()) {
        // Si la imagen que se pasa es la misma que ya tiene, la vaciamos
        imagePixmap = QPixmap();  // Establecemos una imagen vacía
        image_id = 0;
    } else {
        // Si la imagen es distinta o si actualmente no hay imagen, la configuramos
        imagePixmap = pixmap;
        for (auto& [id, image]: map_id) {
            if (image.cacheKey() == pixmap.cacheKey()) {
                image_id = id;
                break;
            }
        }
    }
    update();  // Llamamos a update para redibujar el tile
}

void Tile::paint(QPainter* painter, [[maybe_unused]] const QStyleOptionGraphicsItem* option,
                 [[maybe_unused]] QWidget* widget) {
    // Siempre dibujar el borde
    QPen pen(Qt::black);              // Establece el color negro para el borde
    pen.setWidth(TILE_BORDER_WIDTH);  // Establece el grosor del borde
    painter->setPen(pen);             // Asigna el lápiz al pintor

    // Dibuja el borde alrededor del Tile
    painter->drawRect(boundingRect());  // Dibuja el rectángulo del borde

    // Calcular el área interna del Tile, sin el borde
    // Ajusta para dejar espacio para el borde
    QRectF innerRect = boundingRect().adjusted(TILE_BORDER_WIDTH, TILE_BORDER_WIDTH,
                                               -TILE_BORDER_WIDTH, -TILE_BORDER_WIDTH);

    // Si la imagen está definida, dibújala dentro del área interna
    if (!imagePixmap.isNull()) {
        // Escala la imagen para que se ajuste al área disponible del Tile sin distorsionarse
        QPixmap scaledPixmap = imagePixmap.scaled(innerRect.size().toSize(), Qt::KeepAspectRatio,
                                                  Qt::SmoothTransformation);
        painter->drawPixmap(innerRect.topLeft(),
                            scaledPixmap);  // Dibuja la imagen dentro del área ajustada
    }
}

void Tile::mousePressEvent(QGraphicsSceneMouseEvent* event) {
    emit tileClicked(row, col);
    QGraphicsRectItem::mousePressEvent(event);
}

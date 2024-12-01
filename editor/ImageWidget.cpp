#include "ImageWidget.h"

ImageWidget::ImageWidget(QWidget* parent): QLabel(parent), isSelected(false) {
    setFixedSize(50, 50);
    setStyleSheet("border: 1px solid black; background-color: lightgray;");
}

void ImageWidget::setImage(const QPixmap& pixmap) {
    this->pixmap = pixmap;
    if (!pixmap.isNull()) {
        setPixmap(pixmap.scaled(size(), Qt::KeepAspectRatio));
    } else {
        setText("Empty");
        setAlignment(Qt::AlignCenter);
    }
}

QPixmap ImageWidget::getImage() { return pixmap; }

void ImageWidget::select(bool select) {
    isSelected = select;
    if (isSelected) {
        setStyleSheet("border: 3px solid blue; background-color: lightgray;");
    } else {
        setStyleSheet("border: 1px solid black; background-color: lightgray;");
    }
}

bool ImageWidget::isSelectedState() const { return isSelected; }


void ImageWidget::mousePressEvent(QMouseEvent* event) {
    emit clicked(this, pixmap);  // Emite la imagen seleccionada
    QLabel::mousePressEvent(event);
}

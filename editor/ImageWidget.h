#ifndef IMAGE_WIDGET
#define IMAGE_WIDGET

#include <QApplication>
#include <QLabel>
#include <QMouseEvent>
#include <QPixmap>
#include <QVBoxLayout>
#include <QWidget>

class ImageWidget: public QLabel {
    Q_OBJECT

private:
    QPixmap pixmap;
    bool isSelected;

public:
    explicit ImageWidget(QWidget* parent = nullptr);
    void setImage(const QPixmap& pixmap);
    QPixmap getImage();
    void select(bool select);
    bool isSelectedState() const;

protected:
    void mousePressEvent(QMouseEvent* event) override;

signals:
    // void clicked(ImageWidget* widget);
    void clicked(ImageWidget* sender, const QPixmap& pixmap);
};

#endif  // IMAGE_WIDGET

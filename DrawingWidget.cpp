#include "DrawingWidget.h"
#include "iostream"

DrawingWidget::DrawingWidget(QWidget *parent) : QWidget(parent) {
    finishButton = new QPushButton("Finish Drawing", this);
    finishButton->move(0, 0);
    connect(finishButton, &QPushButton::clicked, this, &DrawingWidget::on_finishDrawing_clicked);
}

void DrawingWidget::eraseNearbyPoints(const QPoint &mousePos) {
    const int eraseRadius = 10;
    for (int i = points.size() - 1; i >= 0; --i) {
        if ((points[i] - mousePos).manhattanLength() < eraseRadius) {
            points.removeAt(i);
        }
    }
}

void DrawingWidget::on_finishDrawing_clicked() {
    QImage mask = getMask();
    emit drawingFinished(mask);
    this->close();
}

void DrawingWidget::setImage(const QImage &newImage) {
    image = newImage;
}

// void DrawingWidget::drawCircle(QPainter &painter, const QPoint &point) const {
//     const int radius = 15; // Set the radius of the brush
//     painter.drawEllipse(point, radius, 0); // Draw a circle at the given point
// }

void DrawingWidget::drawCircle(QPainter &painter, const QPoint &point, int radius) const {
    painter.drawEllipse(point, radius, radius); // 使用提供的半径绘制圆
}

void DrawingWidget::mousePressEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton) {
        points.push_back(event->pos()); // Add the click point
        update(); // Update the widget
    }
    else if (event->button() == Qt::RightButton) {
        if (!points.isEmpty()) {
            points.removeLast();
            update();
        }
    }
}

void DrawingWidget::mouseMoveEvent(QMouseEvent *event) {
    if (event->buttons() & Qt::LeftButton) {
        points.append(event->pos());
        update();
    } else if (event->buttons() & Qt::RightButton) {
        eraseNearbyPoints(event->pos());
        update();
    }
}

void DrawingWidget::mouseReleaseEvent(QMouseEvent *event) {
    if (event->button() == Qt::LeftButton || event->button() == Qt::RightButton) {
        update();  
    }
}

void DrawingWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    QSize scaledSize = image.size();
    scaledSize.scale(size(), Qt::KeepAspectRatio);
    xScaleFactor = (qreal)scaledSize.width() / image.width();
    yScaleFactor = (qreal)scaledSize.height() / image.height();

    painter.drawImage(QRect(QPoint(0, 0), scaledSize), image);

    painter.setPen(QPen(Qt::green, 30)); // 使用固定笔的宽度
    int radius = 15; // 固定半径
    for (const QPoint &point : points) {
        drawCircle(painter, point, radius);
    }
}

// QImage DrawingWidget::getMask() const {
//     QImage mask(image.size(), QImage::Format_ARGB32);
//     mask.fill(Qt::black);

//     QPainter painter(&mask);
//     painter.setPen(QPen(Qt::white, 30/ qMin(xScaleFactor, yScaleFactor)));

//     for (const QPoint &point : points) {
//         drawCircle(painter, QPoint(point.x() / xScaleFactor, point.y() / yScaleFactor)); // 修复回去
//     }

//     return mask;
// }

// QImage DrawingWidget::getMask() const {
//     QImage mask(image.size(), QImage::Format_ARGB32);
//     mask.fill(Qt::black);

//     QPainter painter(&mask);
//     painter.setPen(QPen(Qt::white, 30)); // 使用与 paintEvent 相同的笔宽

//     int radius = 15; // 使用与 paintEvent 相同的半径
//     for (const QPoint &point : points) {
//         drawCircle(painter, QPoint(point.x() / xScaleFactor, point.y() / yScaleFactor), radius);
//     }

//     return mask;
// }

QImage DrawingWidget::getMask() const {
    QImage mask(image.size(), QImage::Format_ARGB32);
    mask.fill(Qt::black);

    QPainter painter(&mask);
    painter.setPen(QPen(Qt::white, 30 / xScaleFactor));

    int radius = 15 / xScaleFactor; // 调整半径大小
    for (const QPoint &point : points) {
        QPoint scaledPoint(point.x() / xScaleFactor, point.y() / yScaleFactor);
        drawCircle(painter, scaledPoint, radius);
    }

    return mask;
}


// imagedisplaywidget.cpp
#include "imagedisplaywidget.h"

ImageDisplayWidget::ImageDisplayWidget(QWidget *parent) : QWidget(parent) {
}

void ImageDisplayWidget::setImageAndFeatures(const QImage& newImage, const std::vector<cv::Point2f> &features, bool flag) {
    image = newImage;
    this->features = features;
    is_train = flag;
    //update();  // Trigger a repaint
}

void ImageDisplayWidget::drawCircle(QPainter& painter, const QPoint& point) const {
    const int radius = 1; // Set the radius of the brush
    painter.drawEllipse(point, radius, 0); // Draw a circle at the given point
}

void ImageDisplayWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);

    QSize scaledSize = image.size();
    scaledSize.scale(size(), Qt::KeepAspectRatio);
    xScaleFactor = (qreal)scaledSize.width() / image.width();
    yScaleFactor = (qreal)scaledSize.height() / image.height();

    painter.drawImage(QRect(QPoint(0, 0), scaledSize), image);

    QPen pen(Qt::green);
    painter.setPen(pen);
    if (is_train) {
        for (const auto& feature : features) {
            drawCircle(painter, QPoint((feature.x + image.width() / 2.0) * xScaleFactor, (feature.y + image.height() / 2.0) * yScaleFactor)); // 修复回去
        }
    }
    else {
        for (const auto& feature : features) {
            drawCircle(painter, QPoint((feature.x) * xScaleFactor, (feature.y) * yScaleFactor)); // 修复回去
        }
    }
 
}

// imagedisplaywidget.cpp
#include "imagedisplaywidget.h"

ImageDisplayWidget::ImageDisplayWidget(QWidget *parent) : QWidget(parent) {
}

void ImageDisplayWidget::setImageAndFeatures(const cv::Mat &image, const std::vector<cv::Point2f> &features) {
    cvImage = image.clone();
    this->features = features;
    update();  // Trigger a repaint
}

void ImageDisplayWidget::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    if (!cvImage.empty()) {
        QImage qimg(cvImage.data, cvImage.cols, cvImage.rows, cvImage.step, QImage::Format_RGB888);
        painter.drawImage(rect(), qimg.rgbSwapped()); // Assumes the cv::Mat is in RGB format

        QPen pen(Qt::red);
        painter.setPen(pen);
        for (const auto &feature : features) {
            painter.drawEllipse(QPointF(feature.x, feature.y), 5, 5);
        }
    }
}

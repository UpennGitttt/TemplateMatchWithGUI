// imagedisplaywidget.h
#ifndef IMAGEDISPLAYWIDGET_H
#define IMAGEDISPLAYWIDGET_H

#include <QWidget>
#include <QPainter>
#include <opencv2/opencv.hpp>

class ImageDisplayWidget : public QWidget {
    Q_OBJECT

public:
    explicit ImageDisplayWidget(QWidget *parent = nullptr);
    void setImageAndFeatures(const QImage& newImage, const std::vector<cv::Point2f> &features, bool flag);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QImage image;
    qreal xScaleFactor;
    qreal yScaleFactor;
    bool is_train = true;
    std::vector<cv::Point2f> features;
    void drawCircle(QPainter& painter, const QPoint& point) const;

};

#endif // IMAGEDISPLAYWIDGET_H

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
    void setImageAndFeatures(const cv::Mat &image, const std::vector<cv::Point2f> &features);

protected:
    void paintEvent(QPaintEvent *event) override;

private:
    cv::Mat cvImage;
    std::vector<cv::Point2f> features;
};

#endif // IMAGEDISPLAYWIDGET_H

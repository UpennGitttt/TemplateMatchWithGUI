#ifndef DRAWINGWIDGET_H
#define DRAWINGWIDGET_H

#include <QWidget>
#include <QPainter>
#include <QMouseEvent>
#include <QVector>
#include <QImage>
#include <QPainterPath>
#include <qpushbutton.h>
#include <QGraphicsView>
#include <QGraphicsScene>
#include <QGraphicsItem>
#include <QVBoxLayout>
#include <QScrollBar>

class DrawingWidget : public QWidget {
    Q_OBJECT

public:
    explicit DrawingWidget(QWidget *parent = nullptr);
    void setImage(const QImage &newImage);
    QImage getMask() const;
    void eraseNearbyPoints(const QPoint &mousePos);

signals:
    void drawingFinished(const QImage &mask);

public slots:
    void on_finishDrawing_clicked(); // 完成绘画Mask

protected:
    void paintEvent(QPaintEvent *event) override;
    void mousePressEvent(QMouseEvent *event) override;
    void mouseMoveEvent(QMouseEvent *event) override;
    void mouseReleaseEvent(QMouseEvent *event) override;

private:
    QImage image;
    qreal xScaleFactor;
    qreal yScaleFactor;
    QPushButton *finishButton;

    QVector<QPoint> points;
    QVector<QPoint> mask_points;
    void drawCircle(QPainter &painter, const QPoint &point, int radius) const;

};

#endif // DRAWINGWIDGET_H

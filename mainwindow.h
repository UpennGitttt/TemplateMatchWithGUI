#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "lib/tempmatch.h"
#include "DrawingWidget.h"
#include "ImageDisplayWidget.h"

QT_BEGIN_NAMESPACE
namespace Ui {
class MainWindow;
}
QT_END_NAMESPACE

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void on_pushButton_uploadimg_clicked();

    void on_pushButton_getmask_clicked();

    void on_finishedDrawing(); // Slot for handling finished drawing

    void on_pushButton_exitmain_clicked();

    void on_pushButton_setCanny_clicked();

    void on_pushButton_setAngleParams_clicked();

    void on_pushButton_setPyramidLevel_clicked();

    void on_pushButton_starttrain_clicked();

    void on_pushButton_checkTempFeatures_clicked();

    void on_pushButton_uploadRunImg_clicked();

    void on_pushButton_setRunPyramidLevel_clicked();

    void on_pushButton_setOverlapRatio_clicked();

    void on_pushButton_setScoreThreshold_clicked();

    void on_pushButton_runTempMatch_clicked();

    void on_pushButton_getCenter_clicked();

    void on_pushButton_showRunRes_clicked();

    void on_pushButton_getHomogrhy_clicked();

    void on_pushButton_getscore_clicked();

    void on_pushButton_getAngle_clicked();

    void on_pushButton_writeJson_clicked();

private:
    cv::Mat train_cvimg;
    QPixmap train_qimg;
    cv::Mat mask_cvimg;

    cv::Mat run_cvimg;
    QPixmap run_qimg;
    bool is_trained{ false };
    bool is_OK{ false };
    Ui::MainWindow *ui;
    std::shared_ptr<mark::TrainTemplateMatch> trainTool;
    std::shared_ptr<mark::RunTemplateMatch> runTool;
    //std::shared_ptr<mark::TrainOutput> trainOutput;
    //std::shared_ptr<mark::RunOutput> runOutput;
    // mark::TrainTemplateMatch *train;
    DrawingWidget *drawingWidget;
    ImageDisplayWidget *imageDisplayWidget;

    void showDrawingWidget();

};
#endif // MAINWINDOW_H

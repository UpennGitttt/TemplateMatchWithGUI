#include "mainwindow.h"
#include "./ui_mainwindow.h"
#include "lib/tempmatch.h"
#include <QLabel>
#include <QFileDialog>
#include <QMessageBox>
#include <Qpainter>
#include <QInputDialog>
#include <qdebug.h>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow), drawingWidget(nullptr)
{
    ui->setupUi(this);
    trainTool = std::make_shared<mark::TrainTemplateMatch>();
    runTool = std::make_shared<mark::RunTemplateMatch>();
    //trainOutput = std::make_shared<mark::TrainOutput>();
    //runOutput = std::make_shared<mark::RunOutput>();
}

MainWindow::~MainWindow()
{
    delete ui;
    // delete drawingWidget;
    // delete train;
}

cv::Mat QImageToMat(const QImage &image) {
    switch (image.format()) {
    case QImage::Format_Grayscale8:
        return cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine()).clone();

    case QImage::Format_RGB32:
    case QImage::Format_ARGB32:
    case QImage::Format_ARGB32_Premultiplied:
    {
        cv::Mat mat(image.height(), image.width(), CV_8UC4, (void*)image.constBits(), image.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_BGRA2BGR);
        return mat;
    }

    case QImage::Format_RGB888:
    {
        QImage swapped = image.rgbSwapped();
        return cv::Mat(swapped.height(), swapped.width(), CV_8UC3, (void*)swapped.constBits(), swapped.bytesPerLine()).clone();
    }

    case QImage::Format_RGBA64:
    {
        QImage swapped = image.convertToFormat(QImage::Format_RGBA8888);
        cv::Mat mat(swapped.height(), swapped.width(), CV_8UC4, (void*)swapped.constBits(), swapped.bytesPerLine());
        cv::cvtColor(mat, mat, cv::COLOR_RGBA2BGRA);
        return mat;
    }

    case QImage::Format_Mono:
    case QImage::Format_MonoLSB:
    {
        QImage swapped = image.convertToFormat(QImage::Format_Grayscale8);
        return cv::Mat(swapped.height(), swapped.width(), CV_8UC1, (void*)swapped.constBits(), swapped.bytesPerLine()).clone();
    }

    case QImage::Format_Indexed8:
        return cv::Mat(image.height(), image.width(), CV_8UC1, (void*)image.constBits(), image.bytesPerLine()).clone();

    default:
        break;
    }
    return cv::Mat();
}

// 槽函数的实现如下 ********************************************************************************************************
void MainWindow::on_pushButton_uploadimg_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/path/to/images/", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty()) {
        QPixmap image(fileName);
        train_qimg = image;
        cv::Mat cvimg = cv::imread(fileName.toStdString(), cv::IMREAD_GRAYSCALE); // 直接读取为灰度图像
        train_cvimg = cvimg;
        if (train_cvimg.type() != CV_8UC1) {
            cv::cvtColor(train_cvimg, train_cvimg, cv::COLOR_BGR2GRAY);
        }
    }
    else{
        QMessageBox::critical(this, tr("错误标题"), tr("未能打开图片"));
    }
}

void MainWindow::on_pushButton_getmask_clicked() {
    showDrawingWidget();
}

void MainWindow::on_finishedDrawing() {
    // This function is called when the drawing is finished
    if (drawingWidget) {
        QImage mask = drawingWidget->getMask(); // Retrieve the mask from DrawingWidget
        mask_cvimg = QImageToMat(mask);

        if (mask_cvimg.type() != CV_8UC1) {
            cv::cvtColor(mask_cvimg, mask_cvimg, cv::COLOR_BGR2GRAY);
        }

        cv::imwrite("C:/Users/Smore/Desktop/mask.png", mask_cvimg);
        std::cout << "mask was written to C:/Users/Smore/Desktop/mask.png" << std::endl;
    }
}

void MainWindow::showDrawingWidget() {
    if (!train_qimg.isNull()) {
        drawingWidget = new DrawingWidget();
        drawingWidget->setAttribute(Qt::WA_DeleteOnClose);
        drawingWidget->setImage(train_qimg.toImage());
        drawingWidget->setWindowTitle("Draw Mask");
        drawingWidget->resize(train_qimg.size());
        connect(drawingWidget, &DrawingWidget::drawingFinished, this, &MainWindow::on_finishedDrawing);  // 连接信号
        drawingWidget->showMaximized();
    }else{
        QMessageBox::critical(this, tr("错误标题"), tr("图片为空"));
    }
}

void MainWindow::on_pushButton_exitmain_clicked()
{
    this->close();
}

void MainWindow::on_pushButton_setCanny_clicked()
{
    bool ok1, ok2;
    int value1 = QInputDialog::getInt(this, "Input Value", "Enter canny threshold low value:", 0, 0, 255, 1, &ok1);
    int value2 = QInputDialog::getInt(this, "Input Value", "Enter canny threshold high value:", 0, 0, 255, 1, &ok2);

    if (ok1 && ok2) {
        bool flag = trainTool->setCannyThreshold(value1, value2);
        if(flag){
            QString message = QString("First value: %1\nSecond value: %2").arg(value1).arg(value2);
            QMessageBox::information(this, "Values Entered", message);
        }else{
            QMessageBox::critical(this, "Set Error", "Set Canny threshold failed.");
        }
    } else {
        QMessageBox::warning(this, "Input Error", "Invalid input for Canny threshold.");
    }
}

void MainWindow::on_pushButton_setAngleParams_clicked()
{
    bool ok1, ok2, ok3;
    float angle_start = QInputDialog::getDouble(this, "Input Value", "Enter angle start value:", 0.0, 0.0, 359.9, 1, &ok1);
    float angle_range = QInputDialog::getDouble(this, "Input Value", "Enter angle range value:", 0.0, 0.0, 360.0, 1, &ok2);
    float angle_step = QInputDialog::getDouble(this, "Input Value", "Enter angle step value:", 0.1, 0.1, 360.0, 2, &ok3);

    if (ok1 && ok2 && ok3) {
        bool flag = trainTool->setAngleParams(angle_start, angle_range, angle_step);
        if(flag){
            QString message = QString("Angle Start: %1\nAngle Range: %2\nAngle Step: %3").arg(angle_start).arg(angle_range).arg(angle_step);
            QMessageBox::information(this, "Angle Parameters Set", message);
        } else {
            QMessageBox::critical(this, "Set Error", "Setting angle parameters failed.");
        }
    } else {
        QMessageBox::warning(this, "Input Error", "Invalid input for angle parameters.");
    }
}

void MainWindow::on_pushButton_setPyramidLevel_clicked()
{
    bool ok;
    int level = QInputDialog::getInt(this, "Input Pyramid Level", "Enter pyramid level:", 1, 1, 7, 1, &ok);

    if (ok) {
        bool flag = trainTool->setPyramidLevel(level);
        if(flag){
            QString message = QString("Pyramid level set to: %1").arg(level);
            QMessageBox::information(this, "Pyramid Level Set", message);
        } else {
            QMessageBox::critical(this, "Set Error", "Setting pyramid level failed.");
        }
    } else {
        QMessageBox::warning(this, "Input Error", "Invalid input for pyramid level.");
    }
}

void MainWindow::on_pushButton_starttrain_clicked()
{
    if (train_cvimg.empty()) {
        QMessageBox::critical(this, "Training Error", "Training image is not set.");
        return;
    }
    bool flag = trainTool->trainTemplateMatch(train_cvimg, mask_cvimg);
    if (flag) {
        is_trained = true;
        QMessageBox::information(this, "Training Success", "Training completed successfully.");
    } else {
        QMessageBox::critical(this, "Training Error", "Training failed.");
    }

}

void MainWindow::on_pushButton_checkTempFeatures_clicked()
{
    std::vector<cv::Point2f> curves = trainTool->getFMTool()->getOriginalTemplateCurve();
    QString message = QString("feature size : %1").arg(trainTool->getFMTool()->getOriginalTemplateCurve().size());

    QMessageBox::information(this, "feature size trained: ", message);
    if(curves.empty()){
        QMessageBox::warning(this, "Output Error", "No features are extracted");
        // return;
    }
    ImageDisplayWidget *displayWidget = new ImageDisplayWidget();
    displayWidget->setAttribute(Qt::WA_DeleteOnClose);
     displayWidget->setImageAndFeatures(train_qimg.toImage(), curves, true);
    displayWidget->setWindowTitle("Template Features");
    displayWidget->resize(train_qimg.size()); // 或者您可以根据需要设置其他大小
    displayWidget->showMaximized();
}


void MainWindow::on_pushButton_uploadRunImg_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this, tr("Open Image"), "/path/to/images/", tr("Image Files (*.png *.jpg *.bmp)"));
    if (!fileName.isEmpty()) {
        QPixmap image(fileName);
        run_qimg = image;
        cv::Mat cvimg = cv::imread(fileName.toStdString(), cv::IMREAD_GRAYSCALE); // 直接读取为灰度图像
        run_cvimg = cvimg;
        if (run_cvimg.type() != CV_8UC1) {
            cv::cvtColor(run_cvimg, run_cvimg, cv::COLOR_BGR2GRAY);
        }
    }
    else {
        QMessageBox::critical(this, tr("错误标题"), tr("未能打开图片"));
        return;
    }
}


void MainWindow::on_pushButton_setRunPyramidLevel_clicked()
{
    bool ok;
    int level = QInputDialog::getInt(this, "Input Pyramid Level", "Enter pyramid level:", 1, 1, 7, 1, &ok);

    if (ok) {
        bool flag = runTool->setPyramidLevel(level);
        if (flag) {
            QString message = QString("Pyramid level set to: %1").arg(level);
            QMessageBox::information(this, "Pyramid Level Set", message);
        }
        else {
            QMessageBox::critical(this, "Set Error", "Setting pyramid level failed.");
            return;
        }
    }
    else {
        QMessageBox::warning(this, "Input Error", "Invalid input for pyramid level.");
    }
}


void MainWindow::on_pushButton_setOverlapRatio_clicked()
{
    bool ok;
    float ratio = QInputDialog::getDouble(this, "Input Pyramid Level", "Enter overlap ratio level:", 0.1, 0.0, 1.0, 2, &ok);

    if (ok) {
        bool flag = runTool->setOverlapThreshold(ratio);
        if (flag) {
            QString message = QString("Overlap ratio set to: %1").arg(ratio);
            QMessageBox::information(this, "Overlap Ratio Set", message);
        }
        else {
            QMessageBox::critical(this, "Set Error", "Setting overlap ratio failed.");
            return;
        }
    }
    else {
        QMessageBox::warning(this, "Input Error", "Invalid input for overlap ratio.");
    }

}


void MainWindow::on_pushButton_setScoreThreshold_clicked()
{
    bool ok;
    int score = QInputDialog::getDouble(this, "Input Score Threshold", "Enter Score Threshold:", 0.4, 0.0, 1.0, 2, &ok);

    if (ok) {
        bool flag = runTool->setPyramidLevel(score);
        if (flag) {
            QString message = QString("Score Threshold to: %1").arg(score);
            QMessageBox::information(this, "Score Threshold Set", message);
        }
        else {
            QMessageBox::critical(this, "Set Error", "Setting Score Thresholdl Failed.");
            return;
        }
    }
    else {
        QMessageBox::warning(this, "Input Error", "Invalid input for score threshold.");
    }

}

void MainWindow::on_pushButton_runTempMatch_clicked()
{
    if (run_cvimg.empty()) {
        QMessageBox::critical(this, "Run Error", "Run image is not set.");
        return;
    }
    if(!is_trained) {
        QMessageBox::critical(this, "Run Error", "Need to train before run !");
        return;
    }
    bool flag = runTool->runTemplateMatch(run_cvimg, trainTool->getFMTool());
    if (flag) {
        is_OK = true;
        int num_found = runTool->getOuput()->getCenters().size();
        QMessageBox::information(this, "Run Success", QString("Run completed successfully, found %1 results!").arg(num_found));
    }
    else {
        QMessageBox::critical(this, "Run Error", "Run failed.");
        return;
    }
}


void MainWindow::on_pushButton_getCenter_clicked()
{
    if (!is_OK) {
        QMessageBox::critical(this, "Get Center Error", "Run Template Match failed !");
        return;
    }
    for (int i = 0; i < runTool->getOuput()->getCenters().size(); i++) {
        qDebug() << runTool->getOuput()->getCenters()[i].x << " , " << runTool->getOuput()->getCenters()[i].y;
        QString message = QString("x: %1 , y: %2").arg(runTool->getOuput()->getCenters()[i].x).arg(runTool->getOuput()->getCenters()[i].y);
        QMessageBox::information(this, "Center Result", message);
    }
}


void MainWindow::on_pushButton_showRunRes_clicked()
{
    if (!is_OK) {
        QMessageBox::critical(this, "Show Result Error", "Run Template Match failed !");
        return;
    }

    std::vector<std::vector<cv::Point2f>> curves = runTool->getOuput()->getCurves();
    if (curves.size() == 0) {
        QMessageBox::critical(this, "Show Result Error", "No features is matched !");
        return;
    }
    QString message = QString("feature size : %1").arg(runTool->getOuput()->getCenters().size());
    QMessageBox::information(this, "Num of Patterns Matched: ", message);

    ImageDisplayWidget* displayWidget = new ImageDisplayWidget();
    displayWidget->setAttribute(Qt::WA_DeleteOnClose);
    displayWidget->setImageAndFeatures(run_qimg.toImage(), curves[0], false);
    displayWidget->setWindowTitle("Template Features");
    displayWidget->resize(run_qimg.size()); 
    displayWidget->showMaximized();
}

void MainWindow::on_pushButton_getHomogrhy_clicked()
{

}


void MainWindow::on_pushButton_getscore_clicked()
{

}


void MainWindow::on_pushButton_getAngle_clicked()
{

}


void MainWindow::on_pushButton_writeJson_clicked()
{

}


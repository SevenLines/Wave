#include <core/waveprocessor.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <opencv2/opencv.hpp>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);

}

MainWindow::~MainWindow() {
    delete ui;
}

void MainWindow::on_actionProcess_triggered() {
    QImage image = ui->graphicsView->image();
    auto processor = WaveProcessor(cv::Mat(image.width(), image.height(), CV_8UC3, image.bits()));
    processor.process([this](MetaWave metaWave) {
        for (auto wave : metaWave.waves()) {
            for (auto point : wave.points()) {
                ui->graphicsView->getScene()->addEllipse(point.x, point.y, 1,1, QPen(Qt::yellow));
            }
        }
    });
}

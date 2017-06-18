#include <core/Waveprocessor.h>
#include "mainwindow.h"
#include "ui_mainwindow.h"

#include <opencv2/opencv.hpp>
#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
        QMainWindow(parent),
        ui(new Ui::MainWindow) {
    ui->setupUi(this);
}

MainWindow::~MainWindow() {
    delete ui;
}

inline cv::Mat QImageToCvMat(const QImage &inImage, bool inCloneImageData = true) {
    switch (inImage.format()) {
        // 8-bit, 4 channel
        case QImage::Format_ARGB32:
        case QImage::Format_ARGB32_Premultiplied: {
            cv::Mat mat(inImage.height(), inImage.width(),
                        CV_8UC4,
                        const_cast<uchar *>(inImage.bits()),
                        static_cast<size_t>(inImage.bytesPerLine())
            );

            return (inCloneImageData ? mat.clone() : mat);
        }

            // 8-bit, 3 channel
        case QImage::Format_RGB32:
        case QImage::Format_RGB888: {
            if (!inCloneImageData) {
                qWarning() << "ASM::QImageToCvMat() - Conversion requires cloning because we use a temporary QImage";
            }

            QImage swapped = inImage;

            if (inImage.format() == QImage::Format_RGB32) {
                swapped = swapped.convertToFormat(QImage::Format_RGB888);
            }

            swapped = swapped.rgbSwapped();

            return cv::Mat(swapped.height(), swapped.width(),
                           CV_8UC3,
                           const_cast<uchar *>(swapped.bits()),
                           static_cast<size_t>(swapped.bytesPerLine())
            ).clone();
        }

            // 8-bit, 1 channel
        case QImage::Format_Indexed8: {
            cv::Mat mat(inImage.height(), inImage.width(),
                        CV_8UC1,
                        const_cast<uchar *>(inImage.bits()),
                        static_cast<size_t>(inImage.bytesPerLine())
            );

            return (inCloneImageData ? mat.clone() : mat);
        }

        default:
            qWarning() << "ASM::QImageToCvMat() - QImage format not handled in switch:" << inImage.format();
            break;
    }

    return cv::Mat();
}

void MainWindow::on_actionProcess_triggered() {
    QImage image = ui->graphicsView->image();
    std::cout << image.format();
    auto processor = WaveProcessor(QImageToCvMat(image));
    auto skeleton = processor.process();
    ui->graphicsView->setSkeleton(skeleton);
}

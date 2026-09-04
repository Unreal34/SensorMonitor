#include "ImageViewerSubWindow.hpp"
#include <QLabel>
#include <QVBoxLayout>
#include <QCloseEvent>

ImageViewerSubWindow::ImageViewerSubWindow(QWidget *parent) : QMdiSubWindow { parent }
, mImageLabel(new QLabel(this))
{
    setAttribute(Qt::WA_DeleteOnClose);

    mImageLabel->setFixedSize(640, 480);
    mImageLabel->setAlignment(Qt::AlignCenter);

    QWidget* container = new QWidget(this);
    QVBoxLayout* layout = new QVBoxLayout(container);
    layout->addWidget(mImageLabel, 0, Qt::AlignCenter);

    setWidget(container);
    setWindowTitle(tr("Image viewer"));
}

void ImageViewerSubWindow::setImage(const QImage &newImage)
{
    Q_ASSERT(mImageLabel);
    mImageLabel->setPixmap(QPixmap::fromImage(newImage).scaled(mImageLabel->size(), Qt::KeepAspectRatio, Qt::FastTransformation));
}

void ImageViewerSubWindow::closeEvent(QCloseEvent *event)
{
    emit imageViewerCloseRequest();
    QMdiSubWindow::closeEvent(event);
}

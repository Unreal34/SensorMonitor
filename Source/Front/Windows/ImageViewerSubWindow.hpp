#ifndef IMAGEVIEWERSUBWINDOW_HPP
#define IMAGEVIEWERSUBWINDOW_HPP

#include <QMdiSubWindow>

class ImageViewerSubWindow : public QMdiSubWindow
{
    Q_OBJECT
public:
    ImageViewerSubWindow(QWidget *parent = nullptr);

public:
    void setImage(const QImage& newImage);

protected:
    virtual void closeEvent(QCloseEvent *event) override;

signals:
    void imageViewerCloseRequest();

private:
    QLabel* mImageLabel = nullptr;
};

#endif // IMAGEVIEWERSUBWINDOW_HPP

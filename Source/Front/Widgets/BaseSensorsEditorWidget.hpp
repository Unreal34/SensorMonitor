#ifndef BASESENSORSEDITORWIDGET_H
#define BASESENSORSEDITORWIDGET_H

#include <QWidget>

namespace Ui {
class BaseSensorsEditorWidget;
}

class BaseSensorsEditorWidget : public QWidget
{
    Q_OBJECT
public:
    explicit BaseSensorsEditorWidget(QWidget *parent = nullptr);
    ~BaseSensorsEditorWidget();

protected slots:
    virtual void addNewSensor() =0;
    virtual void deleteCurrentSelectedSensor() =0;

protected:
    Ui::BaseSensorsEditorWidget *ui;
};

#endif // BASESENSORSEDITORWIDGET_H

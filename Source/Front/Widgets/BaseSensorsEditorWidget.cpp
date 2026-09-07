#include "BaseSensorsEditorWidget.hpp"
#include "ui_BaseSensorsEditorWidget.h"

BaseSensorsEditorWidget::BaseSensorsEditorWidget(QWidget *parent) : QWidget { parent }
, ui(new Ui::BaseSensorsEditorWidget)
{
    ui->setupUi(this);

    connect(ui->mAdd, SIGNAL(clicked(bool)), this, SLOT(addNewSensor()));
    connect(ui->mDelete, SIGNAL(clicked(bool)), this, SLOT(deleteCurrentSelectedSensor()));
}

BaseSensorsEditorWidget::~BaseSensorsEditorWidget()
{
    delete ui;
}

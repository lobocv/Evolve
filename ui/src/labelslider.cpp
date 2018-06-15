#include "labelslider.h"
#include "ui_labelslider.h"

LabelSlider::LabelSlider(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::LabelSlider)
{
    ui->setupUi(this);
}

LabelSlider::~LabelSlider()
{
    delete ui;
}

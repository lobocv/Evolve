#include "labelslider.h"
#include "ui_labelslider.h"
#include "ecosystem.h"

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


void LabelSlider::on_min_slider_valueChanged(int value)
{
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    auto attr_name = this->ui->name_label->text().toStdString();
    ecosystem.set_attribute_limit_min(attr_name, value);
    this->ui->min_label->setText(QString(("Min :" + std::to_string(value)).c_str()));
    std::cout << "New "<< attr_name << " limits are " << ecosystem.environmental_limits_[attr_name].first << ", " << ecosystem.environmental_limits_[attr_name].second << std::endl;
}

void LabelSlider::on_max_slider_valueChanged(int value)
{
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    auto attr_name = this->ui->name_label->text().toStdString();
    ecosystem.set_attribute_limit_max(attr_name, value);
    this->ui->max_label->setText(QString(("Max :" + std::to_string(value)).c_str()));
    std::cout << "New "<< attr_name << " limits are " << ecosystem.environmental_limits_[attr_name].first << ", " << ecosystem.environmental_limits_[attr_name].second << std::endl;
}

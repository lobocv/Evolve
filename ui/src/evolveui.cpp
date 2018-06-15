#include "evolveui.h"
#include "ui_evolveui.h"
#include <iostream>

EvolveUI::EvolveUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EvolveUI)
{
    ui->setupUi(this);
}

EvolveUI::~EvolveUI()
{
    delete ui;
}

void EvolveUI::on_horizontalSlider_valueChanged(int value)
{
    std::cout << value << std::endl;
}

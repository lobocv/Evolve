#include "evolveui.h"
#include "ui_evolveui.h"
#include <iostream>

#include <evolve.h>
#include <ecosystem.h>

EvolveUI::EvolveUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EvolveUI)
{
    Evolve::init();
    ui->setupUi(this);
}

EvolveUI::~EvolveUI()
{
    delete ui;
}


void EvolveUI::on_run_button_clicked()
{
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    ecosystem.RunEpoch(ui->epoch_length_spinbox->value());
    Evolve::print_epoch_results();
}

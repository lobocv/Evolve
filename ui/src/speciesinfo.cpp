#include "speciesinfo.h"
#include "ui_speciesinfo.h"

SpeciesInfo::SpeciesInfo(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::SpeciesInfo)
{
    ui->setupUi(this);
}

SpeciesInfo::~SpeciesInfo()
{
    delete ui;
}

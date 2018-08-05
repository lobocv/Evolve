#include "evolveui.h"
#include "ui_evolveui.h"
#include <iostream>
#include <sstream>
#include <QThread>

#include <evolve.h>
#include <ecosystem.h>

EvolveUI::EvolveUI(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::EvolveUI)
{
    Evolve::init();
    ui->setupUi(this);
}

EvolveUI::~EvolveUI() { delete ui;}

void EcosystemThread::run()
{
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    while (ecosystem_running_)
    {
        ecosystem.RunEpoch(ui_->epoch_length_spinbox->value());
        ecosystem.print_epoch_results();

        // Update the number of alive and dead creatures
        std::stringstream labelstream;
        for (auto &species_pair: ecosystem.species_)
        {
            auto myspecies = species_pair.second;
            auto species_name = species_pair.first;
            auto alive = myspecies->GetAlivePopulation();
            auto deceased = myspecies->GetDeceasedPopulation();
            labelstream << species_name << "(" << alive << " / " << deceased << ")\n";
        }
        ui_->alive_dead_creatures_label->setText(QString(labelstream.str().c_str()));
        ui_->day_number_label->setText(QString(std::to_string(ecosystem.GetDay()).c_str()));

        long total_population = 0;
        for (auto &species_pair: ecosystem.species_)
        {
            total_population += species_pair.second->GetCreatures().size();
        }
        if (total_population == 0) return;
    }

}



void EvolveUI::on_run_button_clicked()
{
    if (ecosystem_thread_ != NULL && ecosystem_thread_->isRunning())
    {
        ecosystem_thread_->ecosystem_running_ = false;
        ecosystem_thread_->wait();
    } else {
        ecosystem_thread_ = std::make_shared<EcosystemThread>();
        ecosystem_thread_->ui_ = this->ui;
        ecosystem_thread_->ecosystem_running_ = true;
        ecosystem_thread_->start();
    }

}

void EvolveUI::on_temperature_min_slider_valueChanged(int value)
{
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    ecosystem.set_attribute_limit_min("Temperature Resistance", value);
    std::cout << "New Temp Resistance limits are " << ecosystem.environmental_limits_["Temperature Resistance"].first << ", " << ecosystem.environmental_limits_["Temperature Resistance"].second << std::endl;
}

void EvolveUI::on_temperature_max_slider_valueChanged(int value)
{
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    ecosystem.set_attribute_limit_max("Temperature Resistance", value);
    std::cout << "New Temp Resistance limits are " << ecosystem.environmental_limits_["Temperature Resistance"].first << ", " << ecosystem.environmental_limits_["Temperature Resistance"].second << std::endl;
}

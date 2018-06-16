#include "evolveui.h"
#include "ui_evolveui.h"
#include <iostream>
#include <thread>

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

void EvolveUI::run_ecosystem()
{
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    while (ecosystem_running_)
    {
        ecosystem.RunEpoch(ui->epoch_length_spinbox->value());
        ecosystem.print_epoch_results();

        // Update the number of alive and dead creatures
        std::string label;
        for (auto &species_pair: ecosystem.species_)
        {
            auto myspecies = species_pair.second;
            label += species_pair.first + "(" + std::to_string(myspecies->GetAlivePopulation()) + " / " + std::to_string(myspecies->GetDeceasedPopulation()) + ")\n";
        }
        ui->alive_dead_creatures_label->setText(QString(label.c_str()));

        ui->day_number_label->setText(QString(std::to_string(ecosystem.GetDay()).c_str()));
    }
}

void EvolveUI::on_run_button_clicked()
{
    if (ecosystem_thread_.joinable())
    {
        ecosystem_running_ = false;
        ecosystem_thread_.join();
    } else {
        ecosystem_running_ = true;
        ecosystem_thread_ = std::thread(&EvolveUI::run_ecosystem, this);
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

#include "evolveui.h"
#include "ui_evolveui.h"
#include "labelslider.h"
#include "ui_labelslider.h"
#include "speciesinfo.h"
#include "ui_speciesinfo.h"

#include <zmq.hpp>
#include <cstring>
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
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    for (auto &attr_pair: ecosystem.attributes_)
    {
        auto attr_name = attr_pair.first;
        auto limits = ecosystem.environmental_limits_[attr_name];
        LabelSlider *attr_slider = new LabelSlider(this);
        attr_slider->ui->name_label->setText(QString(attr_name.c_str()));
        attr_slider->ui->min_label->setText(QString(("Min :" + std::to_string(int(limits.first))).c_str()));
        attr_slider->ui->max_label->setText(QString(("Max :" + std::to_string(int(limits.second))).c_str()));
        ui->attributes_layout->addWidget(attr_slider);
    }

    for (auto &species_pair: ecosystem.species_)
    {
        SpeciesInfo *infolayout = new SpeciesInfo(this);
        auto name = species_pair.first;
        infolayout->ui->name_label->setText(QString(name.c_str()));
        ui->species_info_layout->addWidget(infolayout);
        this->speciesinfo_[name] = infolayout;
    }
}

EvolveUI::~EvolveUI() { delete ui;}

void EcosystemThread::run()
{
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    Ui::EvolveUI *ui = app->ui;

    auto socket = ecosystem.getConnection();

    while (ecosystem_running_)
    {
        ecosystem.RunEpoch(ui->epoch_length_spinbox->value());
        ecosystem.print_epoch_results();
        app->ui->day_number_label->setText(QString(std::to_string(ecosystem.GetDay()).c_str()));

        // Update the number of alive and dead creatures
        for (auto &species_pair: ecosystem.species_)
        {
            auto myspecies = species_pair.second;
            auto species_name = species_pair.first;
            auto alive = myspecies->GetAlivePopulation();
            auto deceased = myspecies->GetDeceasedPopulation();
            app->speciesinfo_[species_name]->ui->alive_label->setText(QString(std::to_string(alive).c_str()));
            app->speciesinfo_[species_name]->ui->deceased_label->setText(QString(std::to_string(deceased).c_str()));
            std::vector<std::shared_ptr<Creature>> &creatures = ecosystem.species_[species_name]->GetCreatures();
            for (auto &trait_pair: ecosystem.traits_)
            {
                auto phenotype_counter = trait_pair.second->CalculatePhenotypeStatistics(creatures);    
                std::stringstream json;
                json << "{";
                int ii=0;
                for (auto it: phenotype_counter)
                {
                    if (ii > 0) json << ", ";
                    json << "\"" << it.first << "\" : " << std::to_string(it.second);
                    ii++;
                }
                json << "}";
                auto val = json.str();
                zmq::message_t reply (val.size());
                std::cout << val << std::endl;
                std::memcpy (reply.data (), val.c_str(), val.size());
                socket->send(reply);
//                sleep(1);
            }
            
        }

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
        ecosystem_thread_->app = this;
        ecosystem_thread_->ecosystem_running_ = true;
        ecosystem_thread_->start();
    }

}

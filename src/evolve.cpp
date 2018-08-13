#include <iostream>
#include <stdlib.h> 
#include <functional>
#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/file_sinks.h"

#include "EvolveConfig.h"
#include "common.h"
#include "genetics.h"
#include "creature.h"
#include "species.h"
#include "trait.h"
#include "ecosystem.h"
#include "attribute.h"
#include <time.h> 


// Config
const auto kMySpeciesName = "Blorp";
const int kMySpeciesInitPop = 1000;
const float kMySpeciesMaleFemaleRatio = 0.5;
const int kMySpeciesChromoPairNum = 1;
const int kMySpeciesLifeExpectanceDays = 365;
const int kMySpeciesMaxOffspring = 5;
const int N_GENES = 26;
const float INTERACTION_RATE = 0.75;
const bool kEnableThresholdTightening = true;


namespace Evolve {

int init()
{


    auto console = spdlog::stdout_color_mt("console");
    std::shared_ptr<spdlog::logger> my_logger;
    try
    {
        my_logger = spdlog::basic_logger_st("file_logger", "/tmp/EvolveLog.txt");

    } catch (const spdlog::spdlog_ex& ex)
    {
        std::cout << "Log initialization failed: " << ex.what() << std::endl;
    }
    spdlog::set_level(spdlog::level::info);
    my_logger->info("       Evolve         ");
    my_logger->info("======================");
    my_logger->info("Author: Calvin Lobo");
    my_logger->info("Major Version: {0}", Evolve_VERSION_MAJOR);
    my_logger->info("Minor Version: {0}", Evolve_VERSION_MINOR);
    my_logger->info("======================");


    // Change the seed to be based off the current system time
    srand (time(NULL));
    
    // Create an Ecosystem to keep track of creatures and initialize their species
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    ecosystem.interaction_rate_ = 0.75;
    std::shared_ptr<Species> myspecies = ecosystem.RegisterSpecies(kMySpeciesName, N_GENES, kMySpeciesChromoPairNum,
                                                                   kMySpeciesMaxOffspring, kMySpeciesLifeExpectanceDays,
                                                                   kMySpeciesInitPop, kMySpeciesMaleFemaleRatio);

    try
    {
        // Create a list of traits for species in the ecosystem.
        ecosystem.RegisterContinuousTrait("Hair Length", "ABCDEFGHIJKL", {"Short Haired", "Medium-Short Haired", "Medium Haired", "Medium-Long Haired", "Long Haired"}, 10, 30);
        ecosystem.RegisterBinaryTrait("Hair Color", "D", {"Blond Haired", "Black Haired"});
        ecosystem.RegisterDiscreteTrait("Exterior Type", "LMNOP", {"Fur", "Skin", "Scales"});

        // Create a list of attributes that the traits contribute towards.
        ecosystem.RegisterAttribute("Temperature Resistance",
                                    {"Hair Color", "Hair Length", "Exterior Type",},
                                    {
                                        {{"Blond Haired", 1}, {"Black Haired", 1}},
                                        {{"Short Haired", 3}, {"Medium-Short Haired", 2}, {"Medium Haired", 1}, {"Medium-Long Haired", 0.5}, {"Long Haired", 0.25}},
                                        {{"Fur", 1}, {"Skin", 1}, {"Scales", 1}}
                                    },
                                    0.0, 1.0);

        ecosystem.RegisterAttribute("Hygene",
                                    {"Hair Length", "Exterior Type",},
                                    {
                                        {{"Short Haired", 3}, {"Medium-Short Haired", 2}, {"Medium Haired", 1}, {"Medium-Long Haired", 0.5}, {"Long Haired", 0.25}},
                                        {{"Fur", 1}, {"Skin", 2}, {"Scales", 3}}
                                    },
                                    0.0, 1.0);


    } catch (EvolveException e)
    {
        std::cout << e.what() <<std::endl;
        exit(-1);
    }

    std::cout << ecosystem << std::endl;

    // Get the first two creatures and have them reproduce
    std::vector<std::shared_ptr<Creature>> &creatures = ecosystem.species_[kMySpeciesName]->GetCreatures();
    ecosystem.openConnection("0.0.0.0", 12346);

//    int epoch_length_days =0;
//    do
//    {
//        std::cin >> epoch_length_days;
//        ecosystem.RunEpoch(epoch_length_days);
//        int day_number = ecosystem.GetDay();
//        print_epoch_results();
//        std::cout << "New Temp Resistance limits are " << ecosystem.environmental_limits_["Temperature Resistance"].first << ", " << ecosystem.environmental_limits_["Temperature Resistance"].second << std::endl;


//    } while ( epoch_length_days > 0);

    return 0;
}

}

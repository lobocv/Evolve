#include <iostream>
#include <sstream>
#include <stdlib.h> 
#include <functional>
#include <memory>
#include "spdlog/spdlog.h"
#include "spdlog/sinks/stdout_color_sinks.h"
#include "spdlog/sinks/basic_file_sink.h"

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

const char LOGFILE[] = "/tmp/EvolveLog.txt";


namespace Evolve {

int init()
{

    // Set up sinks to stdout and file
    std::vector<spdlog::sink_ptr> sinks;
    sinks.push_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
    sinks.push_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>(LOGFILE));
    // Create a logger and attach sinks to it
    auto logger = std::make_shared<spdlog::logger>("logger", begin(sinks), end(sinks));
    // Make this logger available globally
    spdlog::register_logger(logger);
    // Set the log level
    logger->set_level(spdlog::level::info);


    logger->info("       Evolve         ");
    logger->info("======================");
    logger->info("Author: Calvin Lobo");
    logger->info("Major Version: {0}", Evolve_VERSION_MAJOR);
    logger->info("Minor Version: {0}", Evolve_VERSION_MINOR);
    logger->info("======================");


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

    std::stringstream s;
    s << ecosystem;
    logger->info(s.str());

    ecosystem.openConnection("0.0.0.0", 12346);

    return 0;
}

}

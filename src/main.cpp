#include <iostream>
#include <stdlib.h> 
#include <functional>
#include <memory>

#include "EvolveConfig.h"
#include "common.h"
#include "genetics.h"
#include "creature.h"
#include "species.h"
#include "trait.h"
#include "ecosystem.h"
#include "attribute.h"
#include <time.h> 


int main()
{
    std::cout << "       Evolve         " << std::endl;
    std::cout << "======================" << std::endl;
    std::cout << "Author: Calvin Lobo" << std::endl;
    std::cout << "Major Version: " << Evolve_VERSION_MAJOR << std::endl;
    std::cout << "Minor Version: " << Evolve_VERSION_MINOR << std::endl;
    std::cout << "======================" << std::endl;

    // Config
    const auto kMySpeciesName = "Blorp";
    const int kMySpeciesInitPop = 100;
    const float kMySpeciesMaleFemaleRatio = 0.5;
    const int kMySpeciesChromoPairNum = 1;
    const int kMySpeciesLifeExpectanceDays = 365;
    const int kMySpeciesMaxOffspring = 5;
    const int N_GENES = 26;
    const float INTERACTION_RATE = 0.75;


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
        ecosystem.RegisterContinuousTrait("Hair Length", "ABCDEFGHIJKL", {"Short Haired", "Long Haired"}, 10, 30);
        ecosystem.RegisterBinaryTrait("Hair Color", "D", {"Blond Haired", "Black Haired"});
        ecosystem.RegisterDiscreteTrait("Exterior Type", "LMNOP", {"Fur", "Skin", "Feathers", "Scales"});

        // Create a list of attributes that the traits contribute towards.
        ecosystem.RegisterAttribute("Temperature Resistance", {"Hair Color", "Hair Length", "Exterior Type",}, {{1, 1}, {1, 1}, {1, 1, 1, 1},}, 0.25, 1.0);

    } catch (EvolveException e)
    {
        std::cout << e.what() <<std::endl;
        exit(-1);
    }

    std::cout << "List of Species" << std::endl;
    std::cout << "===============" << std::endl;
    for (auto species: ecosystem.species_)
    {
        std::cout << *species.second << std::endl;
    }
    std::cout << std::endl;
    std::cout << "List of Traits" << std::endl;
    std::cout << "===============" << std::endl;
    for (auto trait: ecosystem.traits_)
    {
        std::cout << *trait.second << std::endl;
    }
    std::cout << "===============" << std::endl;

    // Get the first two creatures and have them reproduce
    std::vector<std::shared_ptr<Creature>> &creatures = ecosystem.species_[kMySpeciesName]->get_creatures();

    int& day_number = ecosystem.get_day();
    int epoch_length_days =0;
    do
    {
        std::cin >> epoch_length_days;
        ecosystem.RunEpoch(epoch_length_days);

        std::cout << "Number of alive creatures after " << day_number << " days = " << myspecies->get_alive_population() << std::endl;
        std::cout << "Number of deceased creatures after " << day_number << " days = " << myspecies->get_deceased_population() << std::endl;

        if (creatures.size() > 0)
        {
            for (auto &trait_pair: ecosystem.traits_)
            {
                auto trait = trait_pair.second;
                auto stat = trait->CalculateStatistics(creatures);
                std::cout << *trait << " Mean = " << stat.first << std::endl;
                std::cout << *trait << " Standard Deviation = " << stat.second << std::endl;
            }
        }
        ecosystem.environmental_limits_["Temperature Resistance"].first *= 1.02;
//        ecosystem.environmental_limits_["Temperature Resistance"].second /= 1.01;
        std::cout << "New Temp Resistance limits are " << ecosystem.environmental_limits_["Temperature Resistance"].first << ", " << ecosystem.environmental_limits_["Temperature Resistance"].second << std::endl;


    } while ( epoch_length_days > 0);

    return 0;
}

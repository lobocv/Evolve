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


    // Create a list of traits for species in the ecosystem.
    ecosystem.RegisterContinuousTrait("Hair Length", "ABCDEF", 10, 30);
    ecosystem.RegisterContinuousTrait("Body Fat Percentage", "GHIKJ", 0, 100);
    ecosystem.RegisterDiscreteTrait("Hair Color", "D");

    // Create a list of attributes that the traits contribute towards.
    ecosystem.RegisterAttribute("Temperature Resistance", {"Hair Length", "Body Fat Percentage"}, {{5}, {3}} );


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

        std::cout << "N CREATURES =" << creatures.size() << std::endl;
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
    } while ( epoch_length_days > 0);

    return 0;
}

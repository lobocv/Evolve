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
    std::shared_ptr<Species> myspecies = ecosystem.RegisterSpecies(kMySpeciesName, N_GENES, kMySpeciesChromoPairNum, kMySpeciesMaxOffspring,
                                                                   kMySpeciesInitPop, kMySpeciesMaleFemaleRatio);


    // Create a list of traits for species in the ecosystem.
    ecosystem.RegisterContinuousTrait("Hair Length", "ABCDEF", 10, 30);
    ecosystem.RegisterDiscreteTrait("Hair Color", "D");

    // Create a list of attributes that the traits contribute towards.
    ecosystem.RegisterAttribute("Temperature Resistance", {"Hair Length"});


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
        for (int epoch_day=0; epoch_day < epoch_length_days; epoch_day++, day_number++)
        {
            /*
             * Implement stochastic interaction events here
             *
            */

            if (std::rand() % 100 <= 100 * INTERACTION_RATE && creatures.size() > 0)
            {

                int c1_id = std::rand() % creatures.size();
                int c2_id = std::rand() % creatures.size();

                auto c1 = creatures[c1_id];
                auto c2 = creatures[c2_id];
                try
                {
                    Creature::Reproduce(c1, c2);
                    std::cout << *c1 << " and " << *c2 << " are reproducing." << std::endl;
                } catch (CannotProcreateError) {
                    std::cout << *c1 << " and " << *c2 << " failed to reproduce." << std::endl;
                }

            }

            std::vector<std::shared_ptr<Creature>>::iterator it = creatures.begin();
            while (it != creatures.end())
            {
                int age = (day_number - (*it)->get_birth_date());
                std::cout << "Age of " << **it << " is " << age << std::endl;
                if (age > kMySpeciesLifeExpectanceDays) {
                    it = creatures.erase(it);
                } else {
                    it++;
                }
            }


        }

        std::cout << "N CREATURES =" << creatures.size() << std::endl;
        std::cout << "Number of alive creatures after " << day_number << " days = " << myspecies->get_alive_population() << std::endl;
        std::cout << "Number of deceased creatures after " << day_number << " days = " << myspecies->get_deceased_population() << std::endl;

        if (creatures.size() > 0)
        {
            // Hair Length stats
            std::shared_ptr<ContinuousTrait> hair_length = std::static_pointer_cast<ContinuousTrait>(ecosystem.traits_["Hair Length"]);
            auto hair_length_stats = hair_length->CalculateStatistics(creatures);
            std::cout << "Hair Length Mean = " << hair_length_stats.first << std::endl;
            std::cout << "Hair Length Standard Deviation = " << hair_length_stats.second << std::endl;

            std::shared_ptr<DiscreteTrait> hair_color = std::static_pointer_cast<DiscreteTrait>(ecosystem.traits_["Hair Color"]);
            auto hair_color_stats = hair_color->CalculateStatistics(creatures);
            std::cout << "Hair Color Mean = " << hair_color_stats.first << std::endl;
            std::cout << "Hair Color Standard Deviation = " << hair_color_stats.second << std::endl;

        }
    } while ( epoch_length_days > 0);


    auto child = *creatures[2];
    auto &m = *child.get_father();
    auto &f = *child.get_mother();
    return 0;
}

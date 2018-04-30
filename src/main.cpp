#include <iostream>
#include <stdlib.h> 
#include <functional>
#include <memory>

#include "EvolveConfig.h"
#include "genetics.h"
#include "creature.h"
#include "common.h"
#include "helpers.h"
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
    const int kMySpeciesInitPop = 2;
    const float kMySpeciesMaleFemaleRatio = 0.5;
    const int kMySpeciesChromoPairNum = 1;
    const int N_GENES = 26;
    const float INTERACTION_RATE = 0.2;


    // Change the seed to be based off the current system time
    srand (time(NULL));
    
    // Create an Ecosystem to keep track of creatures and initialize their species
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    std::shared_ptr<Species> myspecies = ecosystem.RegisterSpecies(kMySpeciesName, N_GENES, kMySpeciesChromoPairNum, kMySpeciesInitPop, kMySpeciesMaleFemaleRatio);

    // Create a list of traits for species in the ecosystem.
    ecosystem.RegisterContinuousTrait("Height", "ABCDEF", 100, 300);
    ecosystem.RegisterDiscreteTrait("Hair Color", "D");

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
    Creature::Reproduce(creatures[0], creatures[1]);

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

            if (std::rand() % 100 <= 100 * INTERACTION_RATE)
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
        }
        std::cout << "Number of alive creatures after " << day_number << " days = " << myspecies->get_alive_population() << std::endl;
    } while ( epoch_length_days > 0);


    for (const auto &c: creatures)
    {
        std::cout << *c << std::endl;
        c->print();
        c->print_traits();
        std::cout << std::endl;
    }

    auto child = *creatures[2];
    auto &m = *child.get_father();
    auto &f = *child.get_mother();
    return 0;
}

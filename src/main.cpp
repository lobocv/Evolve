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
    for (auto species: ecosystem.species_) {
        std::cout << *species.second << std::endl;
    }
    std::cout << std::endl;
    std::cout << "List of Traits" << std::endl;
    std::cout << "===============" << std::endl;
    for (auto trait: ecosystem.traits_) {
        std::cout << *trait.second << std::endl;
    }
    std::cout << "===============" << std::endl;

    // Get the first two creatures and have them reproduce
    auto &creatures = ecosystem.species_[kMySpeciesName]->get_creatures();
    Creature::Reproduce(creatures[0], creatures[1]);

    std::cout << "Alive = " << myspecies->get_alive_population() << std::endl;
    for (const auto &c: creatures) {
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

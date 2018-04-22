#include <SFML/Graphics.hpp>

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
    const int kMySpeciesChromoPairNum = 1;
    const int N_GENES = 26;

    // Change the seed to be based off the current system time
    srand (time(NULL));

    // Create a registry of creatures and initialize their species
    SpeciesRegistry &registery = SpeciesRegistry::GetRegistry();
    Species &myspecies = registery.RegisterSpecies(kMySpeciesName, N_GENES, kMySpeciesChromoPairNum, kMySpeciesInitPop);

    std::cout << "List of Species" << std::endl;
    std::cout << "===============" << std::endl;
    for (auto species: registery.species_) {
        std::cout << species.second << std::endl;
    }
    std::cout << "===============" << std::endl;
   
    int ii=0;
    
    std::vector<Creature> const &creatures = registery.species_["Blorp"].get_creatures();
    
    Creature c1 = creatures[0];
    Creature c2 = creatures[1];

    c1.Reproduce(c2);

    std::cout << "Alive = " << myspecies.get_alive_population() << std::endl;
    
    for (const auto &c: creatures) {
        std::cout << c << std::endl;
        c.print();
        std::cout << std::endl;
    }


    return 0;
}

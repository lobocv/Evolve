#include <SFML/Graphics.hpp>

#include <iostream>
#include <stdlib.h> 
#include <functional>
#include <memory>

#include "EvolveConfig.h"
#include "gene.h"
#include "common.h"
#include <time.h> 



int main()
{
    std::cout << "Evolve" << std::endl;
    std::cout << "Major Version: " << Evolve_VERSION_MAJOR << std::endl;
    std::cout << "Minor Version: " << Evolve_VERSION_MINOR << std::endl;
    std::cout << "Author: Calvin Lobo" << std::endl;
    std::cout << "===============" << std::endl;

    // Change the seed to be based off the current system time
    srand (time(NULL));
    

    // Create a creature with a random genotype.
    Creature creature(2);
    int ii=0;
    for (auto chromosome_pair: creature.get_chromosomes()){
        std::cout << "Chromosome Pair #" << (ii+1) << std::endl;
        std::cout << chromosome_pair.first << std::endl;
        std::cout << chromosome_pair.second << std::endl;
        ii++;
    }


    return 0;
}

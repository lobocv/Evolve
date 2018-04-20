#include <SFML/Graphics.hpp>

#include <iostream>
#include <stdlib.h> 
#include <functional>
#include <memory>

#include "EvolveConfig.h"
#include "gene.h"




int main()
{
    std::cout << "Major Version: " << Evolve_VERSION_MAJOR << std::endl;
    std::cout << "Minor Version: " << Evolve_VERSION_MINOR << std::endl;
    std::cout << "===============" << std::endl;

    std::set<char> genecodes = {'a' ,'b', 'c', 'd', 'e', 'f'};
    std::pair<Chromosome, Chromosome> chromosome_pair = Chromosome::MakePair(genecodes);

    std::vector<Gene> gvect = chromosome_pair.first.genes;

    std::cout << chromosome_pair.first << std::endl;
    std::cout << chromosome_pair.second << std::endl;




    return 0;
}

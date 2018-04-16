#include <SFML/Graphics.hpp>

#include <iostream>
#include <stdlib.h> 
#include <functional>
#include <memory>

#include "EvolveConfig.h"
#include "phenotype.h"
#include "gene.h"




int main()
{
    std::vector<std::pair<std::string, char>> relationships;
    relationships.push_back( std::make_pair(std::string("complete"), 'A') );
    relationships.push_back( std::make_pair(std::string("complete"), 'B') );

    Phenotype brown_eyes(relationships);

    std::cout << "Major Version: " << Evolve_VERSION_MAJOR << std::endl;
    std::cout << "Minor Version: " << Evolve_VERSION_MINOR << std::endl;


    return 0;
}

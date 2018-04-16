#include <SFML/Graphics.hpp>

#include <iostream>
#include <stdlib.h> 
#include <functional>
#include <memory>

#include "EvolveConfig.h"

enum DominanceType {Incomplete, Codominant};

class Allele {
    float dominance; // Governs how dominant this allele is over allele's of the same genotype.

};


class Gene {

    char name;
    std::pair<Allele, Allele> alleles;
    DominanceType dominance_type;
    float expressivity; // Governs the strength of the phenotype

};





int main()
{

    std::cout << "Major Version: " << Evolve_VERSION_MAJOR << std::endl;
    std::cout << "Minor Version: " << Evolve_VERSION_MINOR << std::endl;


    return 0;
}

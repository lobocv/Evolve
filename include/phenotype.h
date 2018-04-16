#ifndef __Phenotype
#define __Phenotype

#include <vector>
#include <string>
#include "dominance.h"

struct AlleleRelationship; // Forward declaration


class Phenotype {
    std::string name;
    std::vector<AlleleRelationship> gene_relationships;

public:
    float get_expressivity();

    Phenotype(std::vector<std::pair<std::string, char>>);
};

#endif
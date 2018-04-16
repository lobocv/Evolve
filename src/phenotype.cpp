#include "dominance.h"
#include "phenotype.h"
#include <vector>
#include <string>
#include <iostream>

Phenotype::Phenotype(std::vector<std::pair<std::string, char>> relationships) {

    
    for (auto cls: relationships) {
        if (cls.first == "complete") {
            gene_relationships.push_back(CompleteDominance(this, cls.second));
        } else if (cls.first == "imcomplete") {
            std::cout << "Not supported yet" << std::endl;
        } else if (cls.first == "codominant") {
            std::cout << "Not supported yet" << std::endl;
        } else {
            // TODO: Raise error
        }

    }
};

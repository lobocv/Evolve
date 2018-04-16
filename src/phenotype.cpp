#include "dominance.h"
#include "phenotype.h"
#include <vector>
#include <string>
#include <iostream>

Phenotype::Phenotype(std::vector<std::pair<std::string, char>> relationships) {

    
    for (auto cls: relationships) {
        if (cls.first == "complete") {
            std::cout << cls.first << std::endl;
        } else if (cls.first == "imcomplete") {
            std::cout << cls.first << std::endl;
        } else if (cls.first == "codominant") {
            std::cout << cls.first << std::endl;
        } else {
            // TODO: Raise error
        }

    }
};

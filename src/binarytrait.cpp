#include "trait.h"


BinaryTrait::BinaryTrait(std::string name, std::string gene_code, std::vector<std::string> phenotypes) : DiscreteTrait(name, gene_code, phenotypes)
{
    if (phenotypes.size() != 2)
    {
        throw InvalidTraitParameterError("Binary traits must have only 2 phenotypes. " + std::to_string(phenotypes.size()) + " were given.");
    } else if (gene_code.size() != 1)
    {
        throw InvalidTraitParameterError("Binary traits must be dependent on only one gene. " + std::to_string(gene_code.size()) + " were given.");
    }

    genevectors_[toupper(gene_code[0])] = {2, 0};
    genevectors_[tolower(gene_code[0])] = {0, 1};
}

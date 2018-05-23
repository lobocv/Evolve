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

    InitializeGenevectors();
}


void BinaryTrait::InitializeGenevectors()
{
    // Set the genevector of the upper-case allele to a larger magnitude
    // than the lower-case allele in order to represent a complete-dominance
    // relationship of the binary trait.
    // The values were chosen such that the normalized trait vector has
    // "nice" looking numbers.

    genevectors_[toupper(gene_codes_[0])] = {4, 0};   // dominant
    genevectors_[tolower(gene_codes_[0])] = {0, 3};   // recessive

}

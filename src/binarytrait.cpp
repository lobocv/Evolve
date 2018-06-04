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
}


void BinaryTrait::InitializeGenevectors()
{
    // Set the genevector of the upper-case allele to a larger magnitude
    // than the lower-case allele in order to represent a complete-dominance
    // relationship of the binary trait.
    // The values were chosen such that the normalized trait vector has
    // "nice" looking numbers.

    gene_phenovectors_[toupper(gene_codes_[0])] = {1, 0};   // dominant
    gene_phenovectors_[tolower(gene_codes_[0])] = {0, 1};   // recessive

}


std::string BinaryTrait::ValueToPhenotype(Phenovector trait_vec)
{
    // If there is any sign of the dominant allele, return the dominant phenotype.
    int phenotype_index = (trait_vec[DominantPhenotypeIndex_] > 0) ? DominantPhenotypeIndex_ : RecessivePhenotypeIndex_;
    return phenotypes_[phenotype_index];
}


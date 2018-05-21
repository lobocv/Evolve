#include <ostream>
#include "trait.h"
#include <memory>
#include <algorithm>

BinaryTrait::BinaryTrait(std::string name, std::string genes, std::vector<std::string> phenotypes) : DiscreteTrait(name, genes, phenotypes)
{
    if (phenotypes.size() != 2)
    {
        throw InvalidTraitParameterError("Binary traits must have only 2 phenotypes. " + std::to_string(phenotypes.size()) + " were given.");
    } else if (genes.size() != 1)
    {
        throw InvalidTraitParameterError("Binary traits must be dependent on only one gene. " + std::to_string(genes.size()) + " were given.");
    }
}

float BinaryTrait::CalculateValue(const Genome &genome)
{
    auto dom_rec_ratio = GetAlleleRatio(gene_codes_, genome);
    float outcome = dom_rec_ratio.first > 0 ? 1.0 : 0.0;
//    std::cout << name_ << "=" << outcome << " (N_DOM=" << dom_rec_ratio.first << ", N_REC=" << dom_rec_ratio.second << ")" << std::endl;
    return outcome;
}

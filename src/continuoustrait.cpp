#include <ostream>
#include "trait.h"
#include <memory>
#include <algorithm>

/*
    ContinuousTrait
*/
ContinuousTrait::ContinuousTrait(std::string name, std::string genes, std::vector<std::string> phenotypes, float min, float max) : Trait(name, genes, phenotypes), min_(min), max_(max)
{
    if (gene_codes_.length() < 2)
    {
        throw InvalidTraitParameterError("ContinuousTrait must be represented by a polygene (more than 2 genes).");
    }
}

/**
 * @brief Create the phenospace that the genes alleles will reside in.
 * Each eigenvector in the phenospace corresponds to a specific phenotype
 */
void ContinuousTrait::InitializePhenospace()
{

    int N_col = 1;
    phenotype_vectors_ = {{1}};
    InitializeGenevectors();

}

void ContinuousTrait::InitializeGenevectors()
{
    // Make all upper-case alleles point towards one phenovector and
    // all lower-case alleles point towards the other. The sum of
    // all of these vectors gives the ratio of dominant to recessive
    // alleles which follows a normal distribution.
    // This normal distribution is then used to find the physical
    // value of the continuous trait.

    for (auto c: gene_codes_)
    {
        gene_phenovectors_[toupper(c)] = {1};
        gene_phenovectors_[tolower(c)] = {-1};
    }
}

std::string ContinuousTrait::ValueToPhenotype(Phenovector trait_vec)
{
    auto normalized_value = 0.5*(trait_vec[0] / (gene_phenovectors_.size()) + 1);
    int index = std::round((phenotypes_.size()-1) * normalized_value);
    return phenotypes_[index];
}


float ContinuousTrait::ApplyWeighting(PhenotypeWeights weights, Phenovector trait_vec)
{
    // Find the value in the continuous interval [0, 1]
    auto normalized_value = 0.5*(trait_vec[0] / (gene_phenovectors_.size()) + 1);
    // Each phenotype is a section of the continuous interval [0, 1], these form bins
    // Find the value of the trait normalized by it's bin size.
    auto bin_size = 1.0 / phenotypes_.size();
    auto bin_normalized_value = std::fmod(normalized_value, bin_size) / bin_size;
    auto phenotype = ValueToPhenotype(trait_vec);
    return weights[phenotype] * bin_normalized_value;
}

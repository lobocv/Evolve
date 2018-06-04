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
    // Calculate the normalized value [-1, 1] for the trait
    auto normalized_value = trait_vec[0] / gene_phenovectors_.size();

    float full_range = (max_ - min_);
    float mean_value = 0.5*(max_ + min_);
    auto value = normalized_value * (0.5 * full_range)  + mean_value;
    int index = std::round(value);
    return phenotypes_[index];


}

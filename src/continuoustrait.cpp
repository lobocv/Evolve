#include <ostream>
#include "trait.h"
#include <memory>
#include <algorithm>

/*
    ContinuousTrait
*/
ContinuousTrait::ContinuousTrait(std::string name, std::string genes, std::vector<std::string> n_phenotypes, float min, float max) : Trait(name, genes, n_phenotypes), min_(min), max_(max)
{
    if (gene_codes_.length() < 2)
    {
        throw InvalidTraitParameterError("ContinuousTrait must be represented by a polygene (more than 2 genes).");
    }
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
        genevectors_[toupper(c)] = {1, 0};
        genevectors_[tolower(c)] = {0, 1};
    }
}



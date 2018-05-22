#include <ostream>
#include "trait.h"
#include <memory>
#include <algorithm>

DiscreteTrait::DiscreteTrait(std::string name, std::string genes, std::vector<std::string> phenotypes) : Trait(name, genes, phenotypes)
{
    InitializePhenospace();
}


std::weak_ptr<TraitWeighting> DiscreteTrait::MakeWeighting(std::vector<float> weights)
{
    if (weights.size() != phenotypes_.size()) {
        throw InvalidAttributeParameterError("DiscreteTraitWeighting '" + name_ + "' must have a " +
                                             std::to_string(phenotypes_.size()) + " weight values. " +
                                             "Got " + std::to_string(weights.size()) + " weight(s) instead.");
    }
    auto ptr = std::shared_ptr<TraitWeighting>(new DiscreteTraitWeighting(weights));
    weights_.push_back(ptr);
    return ptr;
}

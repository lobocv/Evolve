#include "ecosystem.h"
#include "species.h"
#include "trait.h"
#include "attribute.h"

/*
    Ecosystem (Singleton)
*/

Ecosystem& Ecosystem::GetEcosystem()
{
    // static Ecosystem ecosystem;  <-- not usually a good idea to make static

    // static shared ptr will delete when main() exits
    // since desstructor is private, we have to give it a functor class which is able to call the destructor
    static std::shared_ptr<Ecosystem> eco( new Ecosystem() , Ecosystem::EcosystemDeleter() );
    return *eco;
}

std::shared_ptr<Species> Ecosystem::RegisterSpecies(std::string species_name, int chromosome_length,
                                                    int n_chromosome_pairs, int max_offspring,
                                                    int initial_population, float male_female_ratio)
{
    int num_males = male_female_ratio * initial_population;
    int num_females = initial_population - num_males;
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    auto new_species = std::make_shared<Species>(species_name, chromosome_length, n_chromosome_pairs, max_offspring);
    new_species->InitializeCreatures(num_males, num_females);
    ecosystem.species_[species_name] = new_species;
    return new_species;
}

void Ecosystem::RegisterTrait(std::shared_ptr<Trait> trait)
{
    std::cout << "TRAIT REGISTERED " << trait->get_name() << std::endl;
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    ecosystem.traits_[trait->get_name()] = trait;

}

void Ecosystem::RegisterDiscreteTrait(std::string trait_name, std::string gene_codes)
{
    auto trait = new DiscreteTrait(trait_name, gene_codes);
    std::shared_ptr<Trait> trait_shared(trait);
    RegisterTrait(trait_shared);
}

void Ecosystem::RegisterContinuousTrait(std::string trait_name, std::string gene_codes, float min, float max)
{
    auto trait = new ContinuousTrait(trait_name, gene_codes, min, max);
    std::shared_ptr<Trait> trait_shared(trait);
    RegisterTrait(trait_shared);

}

void Ecosystem::RegisterAttribute(std::string attr_name, std::vector<std::string> traits)
{
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    std::vector<std::shared_ptr<Trait>> traitVec;
    for (auto trait_name: traits)
    {
        auto it = ecosystem.traits_.find(trait_name);
        if (it != ecosystem.traits_.end())
        {
            traitVec.push_back(it->second);
        } else {
            throw InvalidAttributeParameterError();
        }
    }

    auto attr = new Attribute(attr_name, traitVec);
    std::shared_ptr<Attribute> attr_shared(attr);
    ecosystem.attributes_[attr_name] = attr_shared;
}

int& Ecosystem::get_day() {return day_;}

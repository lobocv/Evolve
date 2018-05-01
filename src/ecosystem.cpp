#include "ecosystem.h"
#include "trait.h"

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
                                                    int n_chromosome_pairs, int initial_population, float male_female_ratio)
{
    int num_males = male_female_ratio * initial_population;
    int num_females = initial_population - num_males;
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    auto new_species = std::make_shared<Species>(species_name, chromosome_length, n_chromosome_pairs);
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

int& Ecosystem::get_day() {return day_;}

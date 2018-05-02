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
                                                    int life_expectancy_days,
                                                    int initial_population, float male_female_ratio)
{
    int num_males = male_female_ratio * initial_population;
    int num_females = initial_population - num_males;
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    auto new_species = std::make_shared<Species>(species_name, chromosome_length, n_chromosome_pairs, max_offspring, life_expectancy_days);
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

void Ecosystem::RunEpoch(int number_of_days)
{
    for (int epoch_day=0; epoch_day < number_of_days; epoch_day++, day_++)
    {
        /*
         * Implement stochastic interaction events here
         *
        */
        for (auto &species : species_)
        {
            auto creatures = species.second->get_creatures();
            if (std::rand() % 100 <= 100 * interaction_rate_ && creatures.size() > 0)
            {

                int c1_id = std::rand() % creatures.size();
                int c2_id = std::rand() % creatures.size();

                auto c1 = creatures[c1_id];
                auto c2 = creatures[c2_id];
                try
                {
                    Creature::Reproduce(c1, c2);
                    std::cout << *c1 << " and " << *c2 << " are reproducing." << std::endl;
                } catch (CannotProcreateError) {
                    std::cout << *c1 << " and " << *c2 << " failed to reproduce." << std::endl;
                }

            }

            std::vector<std::shared_ptr<Creature>>::iterator it = creatures.begin();
            while (it != creatures.end())
            {
                int age = (day_ - (*it)->get_birth_date());
                std::cout << "Age of " << **it << " is " << age << std::endl;
                if (age > species.second->life_expectancy_days_) {
                    it = creatures.erase(it);
                } else {
                    it++;
                }
            }
        }


    }
}

int& Ecosystem::get_day() {return day_;}



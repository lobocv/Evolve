#include "ecosystem.h"
#include "species.h"
#include "trait.h"
#include "attribute.h"

#include <algorithm>

/**
    Ecosystem (Singleton)

    Return a new ecosystem as a static shared ptr that will delete when main() exits.
    Since the destructor is private, we have to give it a functor class which is able to call the destructor.
*/
Ecosystem& Ecosystem::GetEcosystem()
{
    static std::shared_ptr<Ecosystem> eco( new Ecosystem() , Ecosystem::EcosystemDeleter() );
    return *eco;
}

/**
 * @brief Create and register a new species into the Ecosystem.
 * @param species_name
 * @param chromosome_length
 * @param n_chromosome_pairs
 * @param max_offspring
 * @param life_expectancy_days
 * @param initial_population
 * @param male_female_ratio
 * @return
 */
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

/**
 * @brief Register a trait for all species within the ecosystem. This method is called indirectly.
 * @param trait
 */
void Ecosystem::RegisterTrait(std::shared_ptr<Trait> trait)
{
    std::cout << "TRAIT REGISTERED " << trait->get_name() << std::endl;
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    ecosystem.traits_[trait->get_name()] = trait;

}

/**
 * @brief Create and register a new DiscreteTrait for all species in the ecosystem.
 * @param trait_name
 * @param gene_codes
 */
void Ecosystem::RegisterDiscreteTrait(std::string trait_name, std::string gene_codes, std::vector<std::string> phenotypes)
{
    auto trait = new DiscreteTrait(trait_name, gene_codes, phenotypes);
    std::shared_ptr<Trait> trait_shared(trait);
    RegisterTrait(trait_shared);
}

/**
 * @brief Create and register a new BinaryTrait for all species in the ecosystem.
 * @param trait_name
 * @param gene_codes
 */
void Ecosystem::RegisterBinaryTrait(std::string trait_name, std::string gene_code, std::vector<std::string> phenotypes)
{
    auto trait = new BinaryTrait(trait_name, gene_code, phenotypes);
    std::shared_ptr<Trait> trait_shared(trait);
    RegisterTrait(trait_shared);
}

///**
// * @brief Create and register a new ContinuousTrait for all species in the ecosystem.
// * @param trait_name
// * @param gene_codes
// */
//void Ecosystem::RegisterContinuousTrait(std::string trait_name, std::string gene_codes, std::vector<std::string> phenotypes, float min, float max)
//{
//    auto trait = new ContinuousTrait(trait_name, gene_codes, phenotypes, min, max);
//    std::shared_ptr<Trait> trait_shared(trait);
//    RegisterTrait(trait_shared);

//}

/**
 * @brief Create and register a new attribute for all species in the ecosystem.
 * @param attr_name
 * @param traits
 * @param weights
 * @param min
 * @param max
 */
void Ecosystem::RegisterAttribute(std::string attr_name, std::vector<std::string> traits, std::vector<std::vector<float>> weightValues, float min, float max)
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
            throw InvalidAttributeParameterError("No trait by the name of " + trait_name + " has been registered.");
        }
    }

    float weight_sum = 0;
    // Determine the maximum weight from all the phenotypes described by the traits.
    for (auto trait_weight: weightValues)
    {
        weight_sum += *std::max_element(trait_weight.begin(), trait_weight.end());;
    }
    // Normalize all the weights and create weight objects.
    auto normalizer = [weight_sum](float w) {return w/ weight_sum;}; // lambda function that normalizes the weights.
    std::vector<std::weak_ptr<TraitWeighting>> weights;
    int ii = 0;
    for (auto &w: weightValues)
    {
        std::transform(w.begin(), w.end(), w.begin(), normalizer);
        auto weight_obj = traitVec[ii]->MakeWeighting(w);
        weights.push_back(weight_obj);
        ii++;
    }

    auto attr = new Attribute(attr_name, traitVec, weights);
    std::shared_ptr<Attribute> attr_shared(attr);
    ecosystem.attributes_[attr_name] = attr_shared;
    ecosystem.environmental_limits_[attr_name] = std::make_pair(min, max);
}

/**
 * @brief Simulate the passing of a number of days (known as an epoch). During this time creatures will interact (reproduce / fight), grow and and die.
 * @param number_of_days
 */
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
            auto &creatures = species.second->get_creatures();
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
                } catch (CannotProcreateError e) {
                    std::cout << *c1 << " and " << *c2 << " failed to reproduce because " << e.what() << std::endl;
                }

            }

            std::vector<std::shared_ptr<Creature>>::iterator it = creatures.begin();
            while (it != creatures.end())
            {
                int age = (day_ - (*it)->get_birth_date());
//                std::cout << "Age of " << **it << " is " << age << std::endl;
                (*it)->print_traits();
                bool creature_survives = true;
                for (auto attr: attributes_)
                {
                    auto attr_value = attr.second->CalculateValue(**it);
                    auto attr_limit = environmental_limits_[attr.second->get_name()];
                    std::cout << "Attribute " << attr.first << " = " << attr_value << std::endl;
                    creature_survives = attr_value >= attr_limit.first && attr_value <= attr_limit.second;
                }
                if (age > species.second->life_expectancy_days_ || !creature_survives)
                {
                    it = creatures.erase(it);
                } else {
                    it++;
                }
            }
        }


    }
}

/**
 * @brief Get the absolute day number in the ecosystem simulation.
 * @return
 */
int& Ecosystem::get_day() {return day_;}



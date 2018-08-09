#include "ecosystem.h"
#include "species.h"
#include "trait.h"
#include "attribute.h"
#include <sstream>
#include <algorithm>

#include "json.hpp"

using json = nlohmann::json;


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
 * @brief Create an out-bound connection that will transmit details of the ecosystem after each epoch.
 * @param host
 * @param port
 * @return
 */
std::shared_ptr<zmq::socket_t> Ecosystem::openConnection(std::string host, int port)
{
    zmq_context_ = std::make_shared<zmq::context_t>(1);
    socket_ = std::make_shared<zmq::socket_t>(*zmq_context_, ZMQ_PUB);
    std::stringstream url;
    url << "tcp://" << host << ":" << port;
    socket_->bind(url.str().c_str());

    return socket_;
}

std::shared_ptr<zmq::socket_t> Ecosystem::getConnection() {return socket_;}

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
    std::cout << "TRAIT REGISTERED " << trait->GetName() << std::endl;
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    trait->InitializePhenospace();
    ecosystem.traits_[trait->GetName()] = trait;

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

/**
 * @brief Create and register a new ContinuousTrait for all species in the ecosystem.
 * @param trait_name
 * @param gene_codes
 */
void Ecosystem::RegisterContinuousTrait(std::string trait_name, std::string gene_codes, std::vector<std::string> phenotypes, float min, float max)
{
    auto trait = new ContinuousTrait(trait_name, gene_codes, phenotypes, min, max);
    std::shared_ptr<Trait> trait_shared(trait);
    RegisterTrait(trait_shared);

}

/**
 * @brief Create and register a new attribute for all species in the ecosystem.
 * @param attr_name
 * @param traits
 * @param weights
 * @param min
 * @param max
 */
void Ecosystem::RegisterAttribute(std::string attr_name, std::vector<std::string> traits, std::vector<PhenotypeWeights> weight_vectors, float min, float max)
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

    // Check that the number of weight vectors matches the number of phenotypes
    if (weight_vectors.size() != traits.size())
    {
        throw InvalidAttributeParameterError("The number of weight vectors (" + std::to_string(weight_vectors.size()) + ") does not match " +
                                             "the number of traits (" + std::to_string(traits.size()) + ").");
    }

    float weight_sum = 0;
    int ii=0;
    // Determine the maximum weight from all the phenotypes described by the traits.
    for (auto trait_weight: weight_vectors)
    {
        if (trait_weight.size() != traitVec[ii]->phenotypes_.size())
        {
            throw InvalidAttributeParameterError("The number of weights (" + std::to_string(trait_weight.size()) + ") in trait '" +
                                                 traits[ii] + "' for attribute '" + attr_name + "' does not match. Requires " +
                                                 std::to_string(traitVec[ii]->phenotypes_.size()) + "weights.");
        }
        // Check that all the phenotypes in the trait have weights associated with them.
        std::set<std::string> set1, set2;
        std::vector<std::string> setdiff;
        std::for_each(trait_weight.begin(), trait_weight.end(), [&set1](const PhenotypeWeights::value_type &pair) {set1.insert(pair.first);});
        std::for_each(traitVec[ii]->phenotypes_.begin(), traitVec[ii]->phenotypes_.end(), [&set2](const std::string &s) {set2.insert(s);});
        std::set_difference(set2.begin(), set2.end(), set1.begin(), set1.end(), std::inserter(setdiff, setdiff.begin()));
        if (setdiff.size() > 0)
        {
            std::string errormsg = "Not all phenotypes have been assigned weights: ";
            for (int kk=0; kk < setdiff.size(); kk++)
            {
                errormsg += "'" + setdiff[kk] + "'";
                if (kk != setdiff.size()-1 && setdiff.size() > 1) { errormsg += ", ";}

            }
            errormsg += '.';
            throw InvalidAttributeParameterError(errormsg);
        }
        weight_sum += std::max_element(trait_weight.begin(),
                                        trait_weight.end(),
                                        [](const PhenotypeWeights::value_type &pair1,const PhenotypeWeights::value_type &pair2) {return pair1.second < pair2.second;}
                                        )->second;
        ii++;
    }
    // Normalize all the weights.
    auto normalizer = [weight_sum](PhenotypeWeights::value_type &pair) {return pair.second /= weight_sum;}; // lambda function that normalizes the weights.
    for (auto &w: weight_vectors)
    {
        std::for_each(w.begin(), w.end(), normalizer);
    }

    auto attr = new Attribute(attr_name, traitVec, weight_vectors);
    std::shared_ptr<Attribute> attr_shared(attr);
    ecosystem.attributes_[attr_name] = attr_shared;
    ecosystem.environmental_limits_[attr_name] = std::make_pair(min, max);
}

/**
 * @brief Get the absolute day number in the ecosystem simulation.
 * @return
 */
int Ecosystem::GetDay() {return day_;}


std::ostream &operator<<(std::ostream &stream, const Ecosystem &ecosystem)
{
    std::cout << "List of Species" << std::endl;
    std::cout << "===============" << std::endl;
    for (auto species: ecosystem.species_)
    {
        std::cout << *species.second << std::endl;
    }
    std::cout << std::endl;
    std::cout << "List of Traits" << std::endl;
    std::cout << "===============" << std::endl;
    for (auto trait: ecosystem.traits_)
    {
        std::cout << *trait.second << std::endl;
    }
    std::cout << "===============" << std::endl;

    // Get the first two creatures and have them repr
}

void Ecosystem::set_attribute_limit_min(std::string attribute, int value)
{
    float norm_value = value / 100.0;
    auto max = environmental_limits_[attribute].second;
    if ( norm_value >= 0 && norm_value <= max) environmental_limits_[attribute].first = norm_value;
}

void Ecosystem::set_attribute_limit_max(std::string attribute, int value)
{
    float norm_value = value / 100.0;
    auto min = environmental_limits_[attribute].first;
    if ( norm_value >= min && norm_value <= 1) environmental_limits_[attribute].second = norm_value;
}

void Ecosystem::print_epoch_results()
{
    auto socket = getConnection();
    json payload;

    for (auto &species_pair: species_)
    {
        auto speciesname = species_pair.first;
        auto species = species_pair.second;
        std::vector<std::shared_ptr<Creature>> &creatures = species_[speciesname]->GetCreatures();

        std::cout << "Number of alive creatures after " << day_ << " days = " << species->GetAlivePopulation() << std::endl;
        std::cout << "Number of deceased creatures after " << day_ << " days = " << species->GetDeceasedPopulation() << std::endl;

        if (creatures.size() > 0)
        {
            for (auto &trait_pair: traits_)
            {
                auto trait = trait_pair.second;
                  auto phenotype_counter = trait->CalculatePhenotypeStatistics(creatures);
                  std::cout << *trait << " : ";
                  for (auto it: phenotype_counter)
                  {
                      payload["phenotypes"][speciesname][it.first] = it.second;
                      std::cout << it.first  << " = " << it.second << ", ";
                  }
                  std::cout << std::endl;
            }
        }

        std::stringstream jsonstrstream;
        jsonstrstream << payload;
        std::string payloadstr = jsonstrstream.str();
        zmq::message_t reply (payloadstr.size());
        std::cout << payloadstr << std::endl;
        std::memcpy (reply.data (), payloadstr.c_str(), payloadstr.size());
        socket->send(reply);
    }

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
            auto &creatures = species.second->GetCreatures();
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
                int age = (day_ - (*it)->GetBirthDate());
//                std::cout << "Age of " << **it << " is " << age << std::endl;
                (*it)->PrintTraits();
                bool creature_survives = true;
                for (auto attr: attributes_)
                {
                    auto attr_value = attr.second->CalculateValue(**it);
                    auto attr_limit = environmental_limits_[attr.second->GetName()];
                    std::cout << "Attribute " << attr.first << " = " << attr_value << std::endl;
                    creature_survives &= attr_value >= attr_limit.first && attr_value <= attr_limit.second;
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



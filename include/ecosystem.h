#ifndef ECOSYSTEM_H
#define ECOSYSTEM_H

#include <map>
#include <set>
#include <iostream>
#include <memory>
#include "creature.h"
#include "species.h"
#include "attribute.h"

// Singleton
class Ecosystem
{
public:
    float interaction_rate_ = 0.5;
    std::map<std::string, std::shared_ptr<Species>> species_;
    std::map<std::string, std::shared_ptr<Trait>> traits_;
    std::map<std::string, std::shared_ptr<Attribute>> attributes_;
    std::map<std::string, std::pair<float, float>> environmental_limits_;

    static Ecosystem& GetEcosystem();
    std::shared_ptr<Species> RegisterSpecies(std::string species_name, int chromosome_length, int n_chromosome_pairs, int max_offspring,
                                             int life_expectancy_days, int initial_population, float male_female_ratio);
    void RegisterDiscreteTrait(std::string name, std::string gene_codes, std::vector<std::string> phenotypes);
    void RegisterBinaryTrait(std::string name, std::string gene_code, std::vector<std::string> phenotypes);
    void RegisterContinuousTrait(std::string name, std::string gene_codes, std::vector<std::string> phenotypes, float min, float max);
    void RegisterAttribute(std::string attr_name, std::vector<std::string> traits, std::vector<PhenotypeWeights> weights, float min, float max);

    void RunEpoch(int number_of_days);
    int GetDay();
    void set_attribute_limit_min(std::string attribute, int value);
    void set_attribute_limit_max(std::string attribute, int value);
    void print_epoch_results();
private:
    int day_ = 0;
    void RegisterTrait(std::shared_ptr<Trait> trait);
    Ecosystem(){}
    ~Ecosystem(){}
    Ecosystem(Ecosystem &c){}
    void operator=(Ecosystem &c){}

    class EcosystemDeleter
    {
    public:
        void operator()(Ecosystem * p ) { delete p; }
    };

    friend class EcosystemDeleter;
};

std::ostream &operator<<(std::ostream &stream, const Ecosystem &ecosystem);

#endif // ECOSYSTEM_H

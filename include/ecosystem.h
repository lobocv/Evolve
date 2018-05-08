#ifndef ECOSYSTEM_H
#define ECOSYSTEM_H

#include <map>
#include <iostream>
#include <memory>
#include "creature.h"
#include "species.h"
#include "attribute.h"

// Singleton
class Ecosystem {
    int day_ = 0;

public:
    float interaction_rate_ = 0.5;
    std::map<std::string, std::shared_ptr<Species>> species_;
    std::map<std::string, std::shared_ptr<Trait>> traits_;
    std::map<std::string, std::shared_ptr<Attribute>> attributes_;
    std::map<std::string, std::pair<float, float>> environmental_limits_;

    static Ecosystem& GetEcosystem();
    std::shared_ptr<Species> RegisterSpecies(std::string species_name, int chromosome_length, int n_chromosome_pairs, int max_offspring,
                                             int life_expectancy_days, int initial_population, float male_female_ratio);
    void RegisterDiscreteTrait(std::string name, std::string gene_codes, int n_phenotypes);
    void RegisterContinuousTrait(std::string name, std::string gene_codes, int n_phenotypes, float min, float max);
    void RegisterAttribute(std::string attr_name, std::vector<std::string> traits, std::vector<std::vector<float>> weights, float min, float max);
    int& get_day();
    void RunEpoch(int number_of_days);
private:
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

#endif // ECOSYSTEM_H

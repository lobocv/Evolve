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
    std::map<std::string, std::shared_ptr<Species>> species_;
    std::map<std::string, std::shared_ptr<Trait>> traits_;
    std::map<std::string, std::shared_ptr<Attribute>> attributes_;

    static Ecosystem& GetEcosystem();
    std::shared_ptr<Species> RegisterSpecies(std::string species_name, int chromosome_length, int n_chromosome_pairs, int max_offspring,
                                             int initial_population, float male_female_ratio);
    void RegisterTrait(std::shared_ptr<Trait> trait);
    void RegisterDiscreteTrait(std::string name, std::string gene_codes);
    void RegisterContinuousTrait(std::string name, std::string gene_codes, float min, float max);
    void RegisterAttribute(std::string attr_name, std::vector<std::string> traits);
    int& get_day();
private:
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

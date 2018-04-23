#ifndef __Creature
#define __Creature

#include <vector>
#include <map>
#include <ostream>
#include "common.h"
#include "genetics.h"

class Species {

    int n_chromosome_pairs_;
    int genotype_length_;
    std::string name_;
    std::vector<Creature> creatures_;
    int alive_=0;
    int deceased_=0;
    
public:
    Species() = default;
    Species(std::string name, int genotype_length, int n_chromosome_pairs);
    void InitializeCreatures(int n_males, int n_females);
    Creature& AddCreature(Sex, Genome);
    const std::string get_name() const;
    const int get_n_chromosome_pairs() const;
    std::vector<Creature> &get_creatures();
    const int get_alive_population() const;
    const int get_deceased_population() const;
    
};


class Creature {
    Species& species_;
    Genome genome_;
    Sex sex_;
    int id_;
    Creature* mother_;
    Creature* father_;
public:
    Creature() = default;
    Creature(Species& species, Sex sex);
    void Reproduce(Creature& other);    
    friend Creature& Species::AddCreature(Sex, Genome);
    friend void Species::InitializeCreatures(int, int);
    const Genome& get_genome() const;
    const int get_id() const;
    const Sex get_sex() const;
    Creature* get_father() const;
    Creature* get_mother() const;
    Species& get_species() const;
    void print() const;
};


// Singleton
class Ecosystem {
public:
    static Ecosystem& GetRegistry();
    std::map<std::string, Species> species_;
    std::map<std::string, Trait> traits_;
    Species& RegisterSpecies(std::string species_name, int chromosome_length, int n_chromosome_pairs, int initial_population, float male_female_ratio);
    void RegisterTrait(std::string name, GeneSequence genes);
private:
    Ecosystem(){};
    ~Ecosystem(){};
    Ecosystem(Ecosystem &c){};
    void operator=(Ecosystem &c){};
};

#endif
#ifndef __Creature
#define __Creature

#include <vector>
#include <map>
#include <ostream>
#include "common.h"
#include "genetics.h"
#include <memory>
class Species {

    int n_chromosome_pairs_;
    int genotype_length_;
    std::string name_;
    std::vector< std::shared_ptr<Creature> > creatures_;
    int alive_=0;
    int deceased_=0;
    
public:
    Species() = default;
    Species(std::string name, int genotype_length, int n_chromosome_pairs);
    void InitializeCreatures(int n_males, int n_females);
    std::shared_ptr<Creature> AddCreature(Sex, Genome);
    const std::string get_name() const;
    const int get_n_chromosome_pairs() const;
    std::vector<std::shared_ptr<Creature> > & get_creatures();
    const int get_alive_population() const;
    const int get_deceased_population() const;
    
};


class Creature {
    Species& species_;
    Genome genome_;
    Sex sex_;
    int id_;
    std::weak_ptr<Creature> mother_;
    std::weak_ptr<Creature> father_;
public:
    Creature() = default;
    Creature(Species& species, Sex sex);
    static std::shared_ptr<Creature> Reproduce(std::shared_ptr<Creature> creature1, std::shared_ptr<Creature> creature2);    
    friend std::shared_ptr<Creature> Species::AddCreature(Sex, Genome);
    friend void Species::InitializeCreatures(int, int);
    const Genome& get_genome() const;
    const int get_id() const;
    const Sex get_sex() const;
    std::shared_ptr<Creature> get_father() const;
    std::shared_ptr<Creature> get_mother() const;
    Species& get_species() const;
    void print() const;
    void print_traits();
};


// Singleton
class Ecosystem {
public:
    static Ecosystem& GetRegistry();
    std::map<std::string, std::shared_ptr<Species>> species_;
    std::map<std::string, Trait> traits_;
    std::shared_ptr<Species> RegisterSpecies(std::string species_name, int chromosome_length, int n_chromosome_pairs, int initial_population, float male_female_ratio);
    void RegisterTrait(std::string name, GeneSequence genes);
private:
    Ecosystem(){};
    ~Ecosystem(){};
    Ecosystem(Ecosystem &c){};
    void operator=(Ecosystem &c){};
};

#endif

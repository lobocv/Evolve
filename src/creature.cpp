#include "creature.h"
#include "genetics.h"
#include "common.h"
#include <ctype.h>
#include <iostream>
#include <cstdlib>

/*
    Creature
*/
Creature::Creature(Species& species, Sex sex) : species_(species), sex_(sex){};

void Creature::print() const {
    int chromo_num=0;
    for (auto &chromosome_pair: get_genome()){
        std::cout << "Chromosome Pair #" << (chromo_num+1) << ":" << std::endl;
        std::cout << chromosome_pair.first << std::endl;
        std::cout << chromosome_pair.second << std::endl;
        chromo_num++;
    }
};

std::shared_ptr<Creature> Creature::Reproduce(std::shared_ptr<Creature> creature1, std::shared_ptr<Creature> creature2)
{
    const int N_offspring = rand() % (MAX_OFFSPRING+1);
    Chromosome child_chromo1, child_chromo2;
    // TODO: Check creatures are of reproductive age.
    Species &species = creature1->get_species();

    // Determine which creature is male (father) and which is female (mother)    
    std::shared_ptr<Creature>& father = (creature1->get_sex() == Male) ? creature1 : creature2;
    std::shared_ptr<Creature>& mother = (creature2->get_sex() == Male) ? creature1 : creature2;
    // Throw an error if trying to mate two of the same sex.

    if (mother->get_sex() == father->get_sex()) {
        throw(CannotProcreateError());
    }

    Genome male_chromo_pairs = creature1->get_genome();
    Genome female_chromo_pairs = creature2->get_genome();
    Genome child_genome;

    // Iterate through the chromosome pairs. If both creatures are the same 
    // species, they necessarily have the same number of chromosome pairs
    for (int chromo_pair_num=0; chromo_pair_num < male_chromo_pairs.size(); chromo_pair_num++) {
        // Get the parents chromosomes
        Chromosome& male_chromo1 = male_chromo_pairs[chromo_pair_num].first;
        Chromosome& male_chromo2 = male_chromo_pairs[chromo_pair_num].second;
        Chromosome& female_chromo1 = female_chromo_pairs[chromo_pair_num].first;
        Chromosome& female_chromo2 = female_chromo_pairs[chromo_pair_num].second;
        // Role the dice and take one chromosome from each parent to create a new
        
        child_chromo1 = (FlipCoin()) ? male_chromo1 : male_chromo2;
        child_chromo2 = (FlipCoin()) ? female_chromo1 : female_chromo2;
        child_genome.push_back(std::make_pair(child_chromo1, child_chromo2));
    }

    // Add the creature to the species
    Sex sex_of_child = Sex(FlipCoin()) ;
    std::shared_ptr<Creature> child = species.AddCreature(sex_of_child, child_genome);    
    child->mother_ = mother;
    child->father_ = father;
    return child;
};

const int Creature::get_id() const { return id_;};
Species &Creature::get_species() const { return species_;};
const Genome &Creature::get_genome() const { return genome_;}
const Sex Creature::get_sex() const { return sex_;};
std::shared_ptr<Creature> Creature::get_father() const {return father_.lock();}
std::shared_ptr<Creature> Creature::get_mother() const {return mother_.lock();}
void Creature::print_traits() {
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    std::cout << "Trait Values" << std::endl;
    for (auto trait: ecosystem.traits_) {
        float value = trait.second->CalculateValue(genome_);
    }
}


/*
    Species
*/
Species::Species(std::string name, int genotype_length, int n_chromosome_pairs) 
    : name_(name), genotype_length_(genotype_length), n_chromosome_pairs_(n_chromosome_pairs) {};

const std::string Species::get_name() const {return name_;};
const int Species::get_alive_population() const { return alive_;};
const int Species::get_deceased_population() const { return deceased_;};
const int Species::get_n_chromosome_pairs() const{return n_chromosome_pairs_;};

std::vector< std::shared_ptr<Creature> >& Species::get_creatures() {
    return creatures_;
};

std::shared_ptr<Creature> Species::AddCreature(Sex sex, Genome genome) {
    std::shared_ptr<Creature> creature = std::make_shared<Creature>(*this, sex);
    for (int chromo_num=0; chromo_num < this->get_n_chromosome_pairs(); chromo_num++) {
        creature->genome_ = genome;
        creature->id_ = get_alive_population() + get_deceased_population();
    }
    // Add the creature to the list of recorded creatures in the species
    creatures_.push_back(creature);
    alive_++;
    return creature;
}

void Species::InitializeCreatures(int n_males, int n_females) {
    std::string gene_sequence;
    for (int chromo_num=0; chromo_num < genotype_length_; chromo_num++) {
        gene_sequence.push_back('A' + chromo_num);
    }        

    int males_created =0;
    Sex sex_of_child;
    // For each creature
    for (int creature_num=0; creature_num < (n_males+n_females); creature_num++) {
        if (males_created < n_males) {
             sex_of_child = Male;
             males_created++;
        } else {
            sex_of_child = Female;
        }
        
        Genome genome;
        // Create all the chromosomes for the new creature
        for (int chromo_num=0; chromo_num < this->get_n_chromosome_pairs(); chromo_num++) {
            auto chromosome_pair = Chromosome::MakeRandomPair(gene_sequence);
            genome.push_back(chromosome_pair);
        }
        // Add the creature to the species
        AddCreature(sex_of_child, genome);
    }
};


/*
    Ecosystem (Singleton)
*/

Ecosystem& Ecosystem::GetEcosystem() {
    static Ecosystem ecosystem;
    return ecosystem;
};

std::shared_ptr<Species> Ecosystem::RegisterSpecies(std::string species_name, int chromosome_length, int n_chromosome_pairs, int initial_population, float male_female_ratio) {
    int num_males = male_female_ratio * initial_population;
    int num_females = initial_population - num_males;
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    auto new_species = std::make_shared<Species>(species_name, chromosome_length, n_chromosome_pairs);
    new_species->InitializeCreatures(num_males, num_females);
    ecosystem.species_[species_name] = new_species;
    return new_species;
};

void Ecosystem::RegisterTrait(std::shared_ptr<Trait> trait) {
    std::cout << "TRAIT REGISTERED " << trait->get_name() << std::cout;
    Ecosystem &ecosystem = Ecosystem::GetEcosystem();
    ecosystem.traits_[trait->get_name()] = trait;

}

void Ecosystem::RegisterDiscreteTrait(std::string trait_name, std::string gene_codes) {
    auto trait = new DiscreteTrait(trait_name, gene_codes);
    std::shared_ptr<Trait> trait_shared(trait);
    RegisterTrait(trait_shared);
}

void Ecosystem::RegisterContinuousTrait(std::string trait_name, std::string gene_codes, float min, float max) {
    auto trait = new ContinuousTrait(trait_name, gene_codes, min, max);
    std::shared_ptr<Trait> trait_shared(trait);
    RegisterTrait(trait_shared);

}

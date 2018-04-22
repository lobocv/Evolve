#include "creature.h"
#include "genetics.h"
#include "common.h"
#include <ctype.h>
#include <iostream>
#include <cstdlib>

/*
    Creature
*/
Creature::Creature(Species& species) : species_(species) {};

void Creature::print() const {
    int chromo_num=0;
    for (auto &chromosome_pair: get_chromosomes()){
        std::cout << "Chromosome Pair #" << (chromo_num+1) << ":" << std::endl;
        std::cout << chromosome_pair.first << std::endl;
        std::cout << chromosome_pair.second << std::endl;
        chromo_num++;
    }
};

void Creature::Reproduce(const Creature& c1, const Creature& c2){
    const int N_offspring = rand() % (MAX_OFFSPRING+1);
    Chromosome c1_chromo1, c1_chromo2, c2_chromo1, c2_chromo2, child_chromo1, child_chromo2;
    ChromosomePair childpair;
    // TODO: Check creatures are of the same species (allowed to mate)
    // TODO: Check sex of the creatures are opposite
    // TODO: Check creatures are of reproductive age.
    Species species = c1.get_species();

    std::vector<ChromosomePair> c1_chromo_pairs = c1.get_chromosomes();
    std::vector<ChromosomePair> c2_chromo_pairs = c2.get_chromosomes();

    // Iterate through the chromosome pairs. If both creatures are the same 
    // species, they necessarily have the same number of chromosome pairs
    for (int creature_num=0; creature_num < c1_chromo_pairs.size(); creature_num++) {
        // Get the parents chromosomes
        c1_chromo1 = c1_chromo_pairs[creature_num].first;
        c1_chromo2 = c1_chromo_pairs[creature_num].second;
        c2_chromo1 = c2_chromo_pairs[creature_num].first;
        c2_chromo2 = c2_chromo_pairs[creature_num].second;
        for (int gene_num=0; gene_num < c1_chromo1.get_length(); gene_num++){
            // Role the dice and take one chromosome from each parent to create a new
            int rand_binary = std::rand() % 2;
            child_chromo1 = (rand_binary) ? c1_chromo1 : c2_chromo1;
            child_chromo2 = (rand_binary) ? c1_chromo2 : c2_chromo2;
            childpair = std::make_pair(child_chromo1, child_chromo2);
        }   
    }

};

const int Creature::get_id() const { return id_;};
const Species &Creature::get_species() const { return species_;};
const std::vector<ChromosomePair> &Creature::get_chromosomes() const { return chromosomes_;}

/*
    Species
*/
Species::Species(std::string name, int genotype_length, int n_chromosome_pairs) 
    : name_(name), genotype_length_(genotype_length), n_chromosome_pairs_(n_chromosome_pairs) {};

const std::string Species::get_name() const {return name_;};
const int Species::get_alive_population() const { return alive_;};
const int Species::get_deceased_population() const { return deceased_;};
const int Species::get_n_chromosome_pairs() const{return n_chromosome_pairs_;};

const std::vector<Creature>& Species::get_creatures() const {
    return creatures_;};

void Species::AddCreatures(int number) {
    std::string gene_sequence;

    for (int chromo_num=0; chromo_num < N_GENES; chromo_num++) {
        gene_sequence.push_back('A' + chromo_num);
    }        

    // For each creature
    for (int creature_num=0; creature_num < number; creature_num++) {
        Creature creature(*this);
        
        // Create all the chromosomes for the new creature
        for (int chromo_num=0; chromo_num < this->get_n_chromosome_pairs(); chromo_num++) {
            auto chromosome_pair = Chromosome::MakeRandomPair(gene_sequence);
            creature.chromosomes_.push_back(chromosome_pair);
            creature.id_ = get_alive_population() + get_deceased_population();
        }
        // Add the creature to the list of recorded creatures in the species
        creatures_.push_back(creature);
        alive_++;

    }
};


/*
    Creature Registry (Singleton)
*/

CreatureRegistry& CreatureRegistry::GetRegistry() {
    static CreatureRegistry registry;
    return registry;
};

void CreatureRegistry::GenerateCreatures(std::string species_name, int number) {
    CreatureRegistry &registery = CreatureRegistry::GetRegistry();
    Species &species = registery.species_[species_name];
    species.AddCreatures(number);
};

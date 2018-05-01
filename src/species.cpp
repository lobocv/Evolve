#include "common.h"
#include "creature.h"
#include "species.h"
#include "genetics.h"
#include "trait.h"
#include "ecosystem.h"
#include <ctype.h>
#include <iostream>
#include <cstdlib>

/*
    Species
*/
Species::Species(std::string name, int genotype_length, int n_chromosome_pairs, int max_offspring)
    : name_(name), genotype_length_(genotype_length), n_chromosome_pairs_(n_chromosome_pairs), max_offspring_(max_offspring) {}

const std::string Species::get_name() const {return name_;}
const int Species::get_alive_population() const { return alive_;}
const int Species::get_deceased_population() const { return deceased_;}
const int Species::get_n_chromosome_pairs() const{return n_chromosome_pairs_;}
std::vector<std::shared_ptr<Creature>>& Species::get_creatures() {return creatures_;}

std::shared_ptr<Creature> Species::AddCreature(Sex sex, Genome genome)
{
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

void Species::InitializeCreatures(int n_males, int n_females)
{
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
}


bool operator==(const Species &lhs, const Species &rhs) {return lhs.name_ == rhs.name_;}
bool operator!=(const Species &lhs, const Species &rhs) {return !(lhs == rhs);}

std::ostream &operator<<(std::ostream &stream, const Species &obj)
{
    stream << "Species " << obj.get_name();
    return stream;
}

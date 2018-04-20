#include "gene.h"
#include <ctype.h>
#include <iostream>


Gene::Gene(char name) : name{name} {};

Gene::Gene(){};

char Gene::get_name() const {
    return name;
}

Chromosome::Chromosome(){};

Chromosome::Chromosome(std::set<char> genecodes){

    for (auto genecode: genecodes) {
        Gene g(genecode);
        genes.push_back(g);
    }
};

// Create a pair of chromosomes for the given gene codes. 
// This creates one chromosome containing all upper-case alleles of the gene
// and one chromosome with all lower-case alleles of the gene
std::pair<Chromosome, Chromosome> Chromosome::MakePair(std::set<char> genecodes) {
    std::set<char> uppercase, lowercase;
    for (auto genecode: genecodes) {
        uppercase.insert((char) toupper(genecode));
        lowercase.insert((char) tolower(genecode));
    }
    Chromosome uppcase_case_chromosome(uppercase);
    Chromosome lower_case_chromosome(lowercase);

    return std::make_pair(uppcase_case_chromosome, lower_case_chromosome);
};

std::ostream &operator<< (std::ostream &stream, const Chromosome &c) {
    auto it = c.genes.begin();
    for (it; it != c.genes.end(); it++) {
        stream << (*it).get_name();    
    }
    return stream;
};
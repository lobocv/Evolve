#ifndef __EVOLVE_COMMON
#define __EVOLVE_COMMON

#include <utility>

const int N_GENES = 26;
const int MAX_OFFSPRING = 3;

// Forward Declarations
class Chromosome;
class Creature;
class Species;

// Typedefs

typedef std::pair<Chromosome, Chromosome> ChromosomePair;

#endif
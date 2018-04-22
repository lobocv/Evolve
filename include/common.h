#ifndef __EVOLVE_COMMON
#define __EVOLVE_COMMON

#include <utility>
#include <vector>
#include <cstdlib>

const int MAX_OFFSPRING = 3;

// Forward Declarations
class Chromosome;
class Creature;
class Species;

// Typedefs

typedef std::pair<Chromosome, Chromosome> ChromosomePair;
typedef std::vector<ChromosomePair> Genome;

enum Sex{Male, Female};


// Exceptions

class CannotProcreateError {};

inline bool FlipCoin() {return std::rand() % 2;}

#endif
# Evolve
Creature evolution simulator

## Motivation

I started this project as a way to get a broad yet deep understanding of the C++ language. To me, the theory of
evolution is one of the most interesting scientific discoveries of all time and hence why I chose to create
this evolution simulator. The complex and chaotic domain of this project gives avenues for learning many
different aspects of the C++ language. As I develop this project, I aim to document my journey and list
the concepts in which I've utilized.

#### Polymorphism:
* Traits come in several forms such as discrete and continuous. They both expose a common interface
through the base Trait class and their closely related classes for trait weighting and phenotypes.

#### Smart Pointers
* The Ecosystem is responsible for managing and storing all of the species that evolve within it.
* The Species is responsible for managing and storing all of the creatures within it.
* Creatures are created on the heap and can be allocated (born) and deallocated (killed).

#### Operator Overloading
* In order to see the results of the simulations, most classes implement ostream &operator<< functions.

#### Singleton
* The Ecosystem is a singleton class that always returns a static reference to it's own sole instantiation.

#### Friendship
* Creation of certain objects can only be done via certain methods so that they can be registered with the Ecosystem.
* Some ostream& operator<< methods are marked as friend so that they can access private members.



## Terminology


#### Creature
* An individual organism of a Species which has it's own genotype. This genotype contributes
genes to it's offspring and is used to evaluate the magnitude of Traits. The magnitudes of the
traits can fall into certain bins (or ranges) that categorize the trait's phenotype.

#### Attributes
* Descriptors of an organism that govern how fit the organism is for survival.
Attributes are contributed by weighting the significance of a subset of traits. Their value, in
conjunction with environmental factors, decide if the organism lives or dies.

#### Trait
* A feature of a species that can either be discrete or continuous. It's exact value is computed based
on the rules of the specific trait and the genome of the Creature.

#### Phenotype
* A bin of a particular trait. For a binary trait, there are two phenotypes. For continuous
or discrete non-binary traits, there can be many phenotypes. In the discrete case, each
phenotype describes a vector in phenospace.

#### Phenospace
* A vector space that spans possible phenotypes. Phenotypes are represented as eigen-vectors in the vector-space
The phenospace need not be singular, however if it is not singular, the linearly dependent vectors
must be a negative scaling of one another. This signifies anti-correlation of the corresponding phenotypes.

#### Gene
* The building blocks of genetic Traits. When the gene has information about multiple traits
it is said to be  pleiotropic. If a trait has influence from multiple genes, those genes are said to be
polygenic. These two concepts are not mutually exclusive. An example of a polygene group is a group of alleles that
when observed in high numbers, cause the organism to be tall. An example of pleiotropic gene is a gene who's occurance
affects both the height and weight of an organism simultaneously.
       
#### Allele
* A specific variant of a gene. For simplicity, we limit the number of alleles per gene to 2.
We call these alleles dominant (upper-case, eg "A") or recessive (lower-case, eg "a") for lack of better terminology.
The relationship between dominant and recessive allele is not necessarily complete-dominance, in fact, this is only
the case for binary discrete traits.

#### Chromosome
* Contains many genes and distributes those genes to offspring during reproduction


### Polygenic vs Pleitropic Genes
Poylgenic Genes
![alt text](https://github.com/lobocv/Evolve/blob/master/polygene.jpg?raw=true "Polygene")
Pleiotropic Genes
![alt text](https://github.com/lobocv/Evolve/blob/master/pleiotropy.jpg?raw=true "Pleiotropy")


### List of Traits
Traits are specific features of an organism that are derived from it's genome. They contribute to an organisms Attributes.

Continuous Traits
------------------
- Colour   
- Height
- Weight

Discrete Binary Traits
-----------------------
- Nocturnal
- Handedness
- Colorblind
- Hair length 
- Bipedal


Discrete Non-Binary Traits
---------------------------
- Exterior Type (skin, feathers, scales, fur, wool)
- Eye color (blue, green, brown, grey)
- Number of Appendages
- Appendage Weapon (None, Claws, Hooves, Flippers)
- Diet (Herbivore, Carnivore, Omnivore)


### List of Attributes

- Strength
- Intelligence
- Agility
- Hearing
- Vision
- Smell
- Sociability
- Reproductivity


### Future Plans

The great thing about this project is that it can be almost endlessly expanded upon. Some neat
features that I would like to one day implement are:
- Environmental Factors: Interact with creatures to decide whether they are fit enough for survival.
- Calendar: Creatures would reproduce at certain intervals, have a life extectancy. Environment can have seasons.
- Multi-threaded: Parallelize some of the code
- Data Publishing: Expose creature statistics via a socket so that other programs can visualize the evolution.

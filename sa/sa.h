typedef enum { MAX, MIN } EXTREME_MODE;
typedef enum { NO_MEMORY, WITH_MEMORY } MEMORY_MODE;
typedef enum { LEFT, RIGHT } CMP_RESULT;
typedef void* (*saFunc)(const void*);
typedef void* (*saFuncTDependent)(const void*, const double);
typedef double (*saScalarFunc)(const void*, const void*);
typedef CMP_RESULT (*saPredicate)(const void*, const void*);

/**
 * Structure instance containing the algorithm's initial conditions and operaton modes
*/
typedef struct {
    /* The first guess for the optimization solution */
    void* guess;
    /* The simulated annealing temperature control parameter */
    double temperature;
    /* The cooldown coefficient used to decrease the temperature */
    double cooldown;
    /* Number of iterations per epochs */
    unsigned int epoch_iters;
    /* Number of epochs for the algorithm to run */
    unsigned int epochs;
    /* Determines whether the algorithm will seek a maximum or a minimum*/
    EXTREME_MODE emode;
    /* Determines whether the algorithm will remember the best solution found so far */
    MEMORY_MODE mem_mode;

} sa_config_t;
/**
 * Structure instance containing the optimized function's domain configuration for the algorithm
*/
typedef struct {
    /* A function that should be able to select a neighbouring solution space object
       in a stochastic way, dependent on the current temperature value */
    saFuncTDependent neighbour;
} sa_domain_config_t;
/**
 * Structure instance containing the optimized function's codomain configuration for the algorithm
*/
typedef struct {
    /* A comparing function that should be able to generate a CMP_RESULT comparison result 
       based on a given pair of optimized function codomain objects */
    saPredicate comparer;
    /* A metric function that should be able to give the algoritm an idea
       of how far the 2 given objects are from each other in the optimized function's codomain space */
    saScalarFunc metric;
} sa_codomain_config_t;

/**
 * @brief The core general function used to run the simulated annealing metaheuristic algorithm.
 * @param f The function to be optimized
 * @param config Structure instance containing algorithm's initial conditions and operation modes
 * and iterations/epochs count
 * @param domainConfig Structure instance containing f function's domain configuration for the algorithm
 * @param codomainConfig Structure instance containing f function's codomain configuration for the algorithm
 * @returns The best or the final solution from domain space, depending on the algorithm's memory configuration
*/
void* sa_extreme(const saFunc f,
                 const sa_config_t config,
                 const sa_domain_config_t domainConfig,
                 const sa_codomain_config_t codomainConfig);

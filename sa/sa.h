//#define sa_type(domainType, codomainType);

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
    /* The algorithmic Boltzmann k constant */
    double boltzmann_k;
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
    //size_t extent;
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
    //size_t extent;
} sa_codomain_config_t;
/**
 * Structure instance containing statistics for the simulated annealing algorithm runtime
*/
typedef struct {
    unsigned long int solution_improved_times;
    unsigned long int solution_worsened_times;
    unsigned long int best_solution_updated_times;
} sa_stat_t;

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

/**
 * @brief Get simulated annealing algorithm runtime statistics
*/
sa_stat_t get_sa_stat();

typedef struct {
    unsigned char* blobDomainPtr;
    unsigned char* blobCodomainPtr;
    //size_t blobUnitSize;
    unsigned long domainExtent;
    unsigned long codomainExtent;
} _blob;
_blob _blob_loc(const _blob* blob);

/**
 * @brief Macro for setting domain & codomain types for the simualted annealing algorithm
 * @param domainType Optimized function domain datatype
 * @param codomainType Optimized function codomain datatype
*/
#define sa_type(domainType, codomainType) ({ \
    static unsigned char domainStorage[sizeof(domainType) << 1]; \
    static unsigned char codomainStorage[sizeof(domainType) << 1]; \
    static _blob blob; \
    blob.blobDomainPtr = domainStorage; \
    blob.blobCodomainPtr = codomainStorage; \
    blob.domainExtent = sizeof(domainType); \
    blob.codomainExtent = sizeof(codomainType); \
    _blob_loc(&blob); \
})

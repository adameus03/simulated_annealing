#include <math.h>
#include <stdio.h> //neccessary?
#include <stdlib.h>
#include <time.h>
#include "sa.h"

typedef enum { WORSE, STAY } SA_DECISION;

/**
 * @brief Decides whether to accept a worse candidate or stay with the current solution.
 * The decision is made in the random manner, but using the Boltzmann formula.
 * @param solutionMeasure Optimized function's value for the current solution
 * @param candidateMeasure Optimized function's value for the solution candidate
 * @param temperature Represents the current temperature value in the SA algorithm
 * @param metric Represents the metric used to calculate "distance" 
 * between the solution space objects. For real numbers, you can use |a-b|
 * @returns WORSE or STAY according to the stochastic Boltzmann selection 
*/

SA_DECISION sa_decide(const void* solutionMeasure, 
                      const void* candidateMeasure, 
                      const double temperature,
                      const saScalarFunc metric) {
    static double probabilityWorse;
    double metricValue = metric(solutionMeasure, candidateMeasure);
    probabilityWorse = exp(- metricValue / (0.01 * temperature));
    srand((unsigned int)time(NULL));
    double randomValue = (double)rand() / RAND_MAX;
    if (randomValue < probabilityWorse) {
        return WORSE;
    }
    return STAY;

}

typedef enum { _S, _SM, _BS, _BSM } _blobID;
typedef enum { _READ, _WRITE } _blobAccess;

/*size_t _blob_usz(const size_t sz) {
    static size_t _sz;
    if (sz) 
    {
        _sz = sz;
    }
    return _sz;

}*/

/*typedef struct {
    unsigned char* blobDomainPtr;
    unsigned char* blobCodomainPtr;
    //size_t blobUnitSize;
    size_t domainExtent;
    size_t codomainExtent;
} _blob;*/


_blob _blob_loc(const _blob* blob) {
    static _blob __blob;
    if (blob == NULL) {
        return __blob;
    }
    else {
        __blob = *blob;
    }
}

/*static unsigned char* __domainStorageHead;
static unsigned char* __codomainStorageHead;
static size_t __domainUsz;
static size_t __codomainUsz;*/
//

/*void _blob_init() {
    static _blob blob;
    blob.blobDomainPtr = __domainStorageHead;
    blob.blobCodomainPtr = __codomainStorageHead;
    blob.domainExtent = __domainUsz;
    blob.codomainExtent = __codomainUsz;
    printf("domainExtent: %d; codomainExtent: %d\n", blob.domainExtent, blob.codomainExtent);
    //blob;
    _blob_loc(&blob);
}*/

void* _blob_access(const void* dataPtr, const _blobID blobId, const _blobAccess blobAccess) {
    //JQuery na spokojnie jeszcze zdążymy dodać
    if (blobId > 3 || blobId < 0) {
        return NULL;
    }

    //static const size_t blobUnitSize = sizeof(*dataPtr);//HERE
    //const size_t blobUnitSize = _blob_usz(0);
    //const size_t blobFullSize = blobUnitSize << 2;
    //static unsigned char blob[sizeof(*dataPtr)] = {0};
    static _blob blob;
    blob = _blob_loc(NULL);
    static size_t blobDomainUnitSize;
    blobDomainUnitSize = blob.domainExtent;
    static size_t blobCodomainUnitSize;
    blobCodomainUnitSize = blob.codomainExtent;
    /*static size_t blobDomainFullSize;
    blobDomainFullSize = (blobDomainUnitSize << 1);
    static size_t blobCodomainFullSize;
    blobCodomainFullSize = (blobCodomainUnitSize << 1);*/

    static unsigned char* blobPtr;
    static size_t blobCurrentUnitSize;
    static size_t blobUnitIndex;
    if (blobId == _S || blobId == _BS) {
        blobPtr = blob.blobDomainPtr;
        blobCurrentUnitSize = blob.domainExtent;
        blobUnitIndex = blobId == _BS;
    }
    else {
        blobPtr = blob.blobCodomainPtr;
        blobCurrentUnitSize = blob.codomainExtent;
        blobUnitIndex = blobId == _BSM;
    }
    

    switch (blobAccess) {
        case _READ:
            //return (void*)(blob.blobPtr + blobId * blobUnitSize);
            return (void*)(blobPtr + blobUnitIndex * blobCurrentUnitSize);
        case _WRITE:
            unsigned char* ucDataPtr = (unsigned char*)dataPtr;
            for (size_t i = 0; i < blobCurrentUnitSize; i++) {
                //blob.blobPtr[blobId * blobUnitSize + i] = ucDataPtr[i];
                blobPtr[blobUnitIndex * blobCurrentUnitSize + i] = ucDataPtr[i];
            }
            return NULL;
        default:
            return NULL;
    }
    
}

void* _blob_read(const _blobID blobId) {
    return _blob_access(NULL, blobId, _READ);
}

void _blob_write(const void* dataPtr, const _blobID blobId) {
    _blob_access(dataPtr, blobId, _WRITE);
}


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
                 const sa_codomain_config_t codomainConfig) { //posible to rewrite using a typed macro
    //_blob bp = sa_type("", "");
    //_blob_loc(&bp);
    //_blob_init();
    _blob_write(config.guess, _S);
    void* solution = _blob_read(_S); // {{instead return address from _blob_write and remove _blob_read? (cuts no of lines in half)}}
    _blob_write(f(solution), _SM);
    void* solutionMeasure = _blob_read(_SM);
    double temperature = config.temperature;
    _blob_write(solution, _BS);
    void* bestSolution = _blob_read(_BS);
    _blob_write(solutionMeasure, _BSM);
    void* bestSolutionMeasure = _blob_read(_BSM);    

    for (unsigned int epoch = 0; epoch < config.epochs; epoch++) {
        printf("[%d / %d] T: %4.4f solutionMeasure: %4.2f; bestSolutionMeasure: %4.2f\n", epoch, config.epochs, temperature, *(double*)solutionMeasure, *(double*)bestSolutionMeasure);
        for (unsigned int iter = 0; iter < config.epoch_iters; iter++) {
            static void* candidate;
            candidate = domainConfig.neighbour(solution, temperature);
            static void* candidateMeasure;
            candidateMeasure = f(candidate);

            static CMP_RESULT comparerResult;
            //printf("%dcandidateMeasure: %4.2f; solutionMeasure: %4.2f; bestSolutionMeasure: %4.2f\n", *(double*)candidateMeasure, *(double*)solutionMeasure, *(double*)bestSolutionMeasure);
            comparerResult = codomainConfig.comparer(candidateMeasure, solutionMeasure);
            if ((config.emode == MAX && comparerResult == LEFT)
                || (config.emode == MIN && comparerResult == RIGHT)
                || sa_decide(solutionMeasure, candidateMeasure, temperature, codomainConfig.metric)
            ) {
                /*solution = candidate;
                solutionMeasure = candidateMeasure;*/
                _blob_write(candidate, _S); //{{solutionWrite?}}
                _blob_write(candidateMeasure, _SM); //{{solutionMeasureWrite?}}
                if (config.mem_mode == WITH_MEMORY
                    && codomainConfig.comparer(solutionMeasure, bestSolutionMeasure) == LEFT) {
                    //bestSolution = solution;
                    //bestSolutionMeasure = solutionMeasure;
                    _blob_write(solution, _BS);
                    _blob_write(solutionMeasure, _BSM);
                }
            }
        }
        temperature *= config.cooldown;
    }
    switch (config.mem_mode) {
        case WITH_MEMORY:
            return bestSolution;
        case NO_MEMORY:
            return solution;
        default:
            return NULL;
    }
}

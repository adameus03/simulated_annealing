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
                      const double k,
                      const saScalarFunc metric) {
    static double probabilityWorse;
    double metricValue = metric(solutionMeasure, candidateMeasure);
    probabilityWorse = exp(- metricValue / (k * temperature));
    double randomValue = (double)rand() / RAND_MAX;
    if (randomValue < probabilityWorse) {
        return WORSE;
    }
    return STAY;
}

typedef enum { _S, _SM, _BS, _BSM } _blobID;
typedef enum { _READ, _WRITE } _blobAccess;


_blob _blob_loc(const _blob* blob) {
    static _blob __blob;
    if (blob == NULL) {
        return __blob;
    }
    else {
        __blob = *blob;
        return __blob;
    }
}

void* _blob_access(const void* dataPtr, const _blobID blobId, const _blobAccess blobAccess) {
    if (blobId > 3 || blobId < 0) {
        return NULL;
    }
    static _blob blob;
    blob = _blob_loc(NULL);
    /*static size_t blobDomainUnitSize;
    blobDomainUnitSize = blob.domainExtent;
    static size_t blobCodomainUnitSize;
    blobCodomainUnitSize = blob.codomainExtent;*/

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
            return (void*)(blobPtr + blobUnitIndex * blobCurrentUnitSize);
        case _WRITE:
            unsigned char* ucDataPtr = (unsigned char*)dataPtr;
            for (size_t i = 0; i < blobCurrentUnitSize; i++) {
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

void _random_guard() {
    static unsigned char guard = 0x1;
    if (guard) {
        guard = 0x0;
        srand((unsigned int)time(NULL));   
    }
}


///<debug>
typedef struct {
    double x;
    double y;
} r2_cart_t;
///</debug>

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
    _random_guard();
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
        printf("[%d / %d] T: %4.4f solutionMeasure: %4.4f; bestSolutionMeasure: %4.4f\n", epoch, config.epochs, temperature, *(double*)solutionMeasure, *(double*)bestSolutionMeasure);
        for (unsigned int iter = 0; iter < config.epoch_iters; iter++) {
            static void* candidate;
            candidate = domainConfig.neighbour(solution, temperature);

            static void* candidateMeasure;
            candidateMeasure = f(candidate);

            //printf("{Probe %d/%d}\n", iter, config.epoch_iters); //debug
            //printf("Candidate: x=%4.3f, y=%4.3f  ==> %4.4f\n", ((r2_cart_t*)candidate)->x, ((r2_cart_t*)candidate)->y, *(double*)candidateMeasure);//debug
            //printf("Current solution: x=%4.3f, y=%4.3f ==> %4.4f\n", ((r2_cart_t*)solution)->x, ((r2_cart_t*)solution)->y, *(double*)solutionMeasure);
            //printf("Current bestSolution: x=%4.3f, y=%4.3f ==> %4.4f\n", ((r2_cart_t*)bestSolution)->x, ((r2_cart_t*)bestSolution)->y, *(double*)bestSolutionMeasure);

            static CMP_RESULT comparerResult;
            comparerResult = codomainConfig.comparer(candidateMeasure, solutionMeasure);

            if ((config.emode == MAX && comparerResult == LEFT)
                || (config.emode == MIN && comparerResult == RIGHT)
                || (sa_decide(solutionMeasure, candidateMeasure, temperature, config.boltzmann_k, codomainConfig.metric) == WORSE)
            ) {
                _blob_write(candidate, _S); //{{solutionWrite?}}
                _blob_write(candidateMeasure, _SM); //{{solutionMeasureWrite?}}
                if (config.mem_mode == WITH_MEMORY) {
                    comparerResult = codomainConfig.comparer(solutionMeasure, bestSolutionMeasure);
                    if ((config.emode == MAX && comparerResult == LEFT)
                        || (config.emode == MIN && comparerResult == RIGHT)
                    ) {
                        _blob_write(solution, _BS);
                        _blob_write(solutionMeasure, _BSM);
                    }
                }
            }
        }
        temperature *= config.cooldown;
    }
    
    /*for(int i=0; i<10; i++){
        printf("%4.2f\n", ((double)rand())/RAND_MAX*5); //>>>> ?????
    }*/
    switch (config.mem_mode) {
        case WITH_MEMORY:
            return bestSolution;
        case NO_MEMORY:
            return solution;
        default:
            return NULL;
    }
}

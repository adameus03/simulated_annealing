#include "../sa/sa_r2_r_basic.h"

typedef enum { VERBOSE, QUIET } BATCH_VERBOSITY_MODE;

void batch(const char* batch_name, const unsigned long int runTimes, 
           const saFunc f, const sa_config_t config, const r2_cart_rect_constraint_t constraint, const BATCH_VERBOSITY_MODE verbose);
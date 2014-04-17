/** @file quintessence.h Documented includes for quintessence part */

#ifndef __QUINTESSENCE__
#define __QUINTESSENCE__

#include "common.h"
// #include "quadrature.h"
// #include "growTable.h"
// #include "arrays.h"
// #include "dei_rkck.h"
// #include "parser.h"

/**
 * HERE BE COMMENTS
 */

struct quintessence
{
  /** @name - quintessence parameters
   *
   * The quintessence structure should contain all the quintessence related functions.
   * This should facilitate the incorporation of further models.
   * Each model would then be contained in a separate file
   */
  
  //@{

  double lambda_scf;
  double alpha_scf;
  
  
  //@}
  
};

#endif
/** @file quintessence.h Documented includes for quintessence part */

#ifndef __QUINTESSENCE__
#define __QUINTESSENCE__

#include "common.h"
// #include "quadrature.h"
// #include "growTable.h"
// #include "arrays.h"
// #include "dei_rkck.h"
// #include "parser.h"

// enum spatial_curvature {flat,open,closed};

/**
 * HERE BE COMMENTS
 */

struct quintessence
{
  /** @name - input parameters initialized by user in input module
   *  (all other quantitites are computed in this module, given these parameters
   *   and the content of the 'precision' structure)
   *
   * The background cosmological parameters listed here form a parameter
   * basis which is directly usable by the background module. Nothing
   * prevents from defining the input cosmological parameters
   * differently, and to pre-process them into this format, using the input
   * module (this might require iterative calls of background_init()
   * e.g. for dark energy or decaying dark matter). */
  
  //@{

  double param_test; /**< \f$ H_0 \f$ : Hubble parameter (in fact, [H_0/c]) in \f$ Mpc^{-1} \f$ */
  
  
  //@}
  
};

#endif
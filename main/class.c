/** @file class.c 
 * Julien Lesgourgues, 17.04.2011    
 */
 
#include "class.h"

int main(int argc, char **argv) {

  struct precision pr;        /* for precision parameters */
  struct background ba;       /* for cosmological background */
  struct thermo th;           /* for thermodynamics */
  struct perturbs pt;         /* for source functions */
  struct transfers tr;        /* for transfer functions */
  struct primordial pm;       /* for primordial spectra */
  struct spectra sp;          /* for output spectra */
  struct nonlinear nl;        /* for non-linear spectra */
  struct lensing le;          /* for lensed spectra */
  struct output op;           /* for output files */
  ErrorMsg errmsg;            /* for error messages */

  if (input_init_from_arguments(argc, argv,&pr,&ba,&th,&pt,&tr,&pm,&sp,&nl,&le,&op,errmsg) == _FAILURE_) {
    printf("\n\nError running input_init_from_arguments \n=>%s\n",errmsg); 
    return _FAILURE_;
  }

  //Tune the scalar field parameters
  if (tune_scalar_field_parameters(&pr,&ba) == _FAILURE_) {
    printf("\n\nError running background_init \n=>%s\n",ba.error_message);
    return _FAILURE_;
  }

  if (background_init(&pr,&ba) == _FAILURE_) {
    printf("\n\nError running background_init \n=>%s\n",ba.error_message);
    return _FAILURE_;
  }

  if (thermodynamics_init(&pr,&ba,&th) == _FAILURE_) {
    printf("\n\nError in thermodynamics_init \n=>%s\n",th.error_message);
    return _FAILURE_;
  }
  

  if (perturb_init(&pr,&ba,&th,&pt) == _FAILURE_) {
    printf("\n\nError in perturb_init \n=>%s\n",pt.error_message);
    return _FAILURE_;
  }
  
//   //added in order to extract the background functions
//   if (output_init(&ba,&pt,&sp,&nl,&le,&op) == _FAILURE_) {
//     printf("\n\nError in output_init \n=>%s\n",op.error_message);
//     return _FAILURE_;
//   }    
// 
//   return _SUCCESS_; // stop at the background_free  
  
  if (primordial_init(&pr,&pt,&pm) == _FAILURE_) {
    printf("\n\nError in primordial_init \n=>%s\n",pm.error_message);
    return _FAILURE_;
  }

  if (transfer_init(&pr,&ba,&th,&pt,&tr) == _FAILURE_) {
    printf("\n\nError in transfer_init \n=>%s\n",tr.error_message);
    return _FAILURE_;
  }

  if (spectra_init(&pr,&ba,&pt,&tr,&pm,&sp) == _FAILURE_) {
    printf("\n\nError in spectra_init \n=>%s\n",sp.error_message);
    return _FAILURE_;
  }

  if (nonlinear_init(&pr,&ba,&th,&pt,&tr,&pm,&sp,&nl) == _FAILURE_) {
    printf("\n\nError in nonlinear_init \n=>%s\n",nl.error_message);
    return _FAILURE_;
  }

  if (lensing_init(&pr,&pt,&sp,&nl,&le) == _FAILURE_) {
    printf("\n\nError in lensing_init \n=>%s\n",le.error_message);
    return _FAILURE_;
  }

  if (output_init(&ba,&pt,&sp,&nl,&le,&op) == _FAILURE_) {
    printf("\n\nError in output_init \n=>%s\n",op.error_message);
    return _FAILURE_;
  }

  /****** all calculations done, now free the structures ******/

  if (lensing_free(&le) == _FAILURE_) {
    printf("\n\nError in lensing_free \n=>%s\n",le.error_message);
    return _FAILURE_;
  }

  if (nonlinear_free(&nl) == _FAILURE_) {
    printf("\n\nError in nonlinear_free \n=>%s\n",nl.error_message);
    return _FAILURE_;
  }

  if (spectra_free(&sp) == _FAILURE_) {
    printf("\n\nError in spectra_free \n=>%s\n",sp.error_message);
    return _FAILURE_;
  }

  if (transfer_free(&tr) == _FAILURE_) {
    printf("\n\nError in transfer_free \n=>%s\n",tr.error_message);
    return _FAILURE_;
  }

  if (primordial_free(&pm) == _FAILURE_) {
    printf("\n\nError in primordial_free \n=>%s\n",pm.error_message);
    return _FAILURE_;
  }

  if (perturb_free(&pt) == _FAILURE_) {
    printf("\n\nError in perturb_free \n=>%s\n",pt.error_message);
    return _FAILURE_;
  }

  if (thermodynamics_free(&th) == _FAILURE_) {
    printf("\n\nError in thermodynamics_free \n=>%s\n",th.error_message);
    return _FAILURE_;
  }

  if (background_free(&ba) == _FAILURE_) {
    printf("\n\nError in background_free \n=>%s\n",ba.error_message);
    return _FAILURE_;
  }

  return _SUCCESS_;

}



int tune_scalar_field_parameters(
		                 struct precision * ppr,
		                 struct background * pba
		                 )  {
  if (!pba->has_scf) {
    printf(" no scalar field, skipping tune algorithm \n");
    return _SUCCESS_; 
  }
  
  /** The parameter to be tuned can be defined as a preprocessor variable */

#define _TUNE_PARAM_ pba->scf_lambda
  
  int iter=0, maximum_iter = 100; //maximum number of iterations
  double scf_lambda_min = -1;
  double scf_lambda_max = 10*_TUNE_PARAM_;
  double Omega0_scf_try = 0.;//absurd value
  double tolerance = 1e-4; //Need to pass a preccision argument

  pba->scf_is_tuned = _FALSE_; /*scalar field not tuned. Useful to avoid unnecesary output NOTE: perhaps trade for a temporary background verbose value */
  
  while (fabs(pba->Omega0_scf-Omega0_scf_try) > tolerance){
    
    _TUNE_PARAM_=0.5*(scf_lambda_max+scf_lambda_min);
    
 //   printf(" Tuning lambda = %e, Omega_0_try = %f \n",_TUNE_PARAM_, Omega0_scf_try);
    
    if (background_init(ppr,pba) == _FAILURE_) {
      printf("\n\nError running background_init with exp_quint, lambda = %e \n=>%s\n",_TUNE_PARAM_,pba->error_message);
      /*changed from pba.error_message to pba->error_message*/
      return _FAILURE_;
    }
    
    Omega0_scf_try = 
    pba->background_table[(pba->bt_size-1)*pba->bg_size+pba->index_bg_rho_scf]/
    pba->background_table[(pba->bt_size-1)*pba->bg_size+pba->index_bg_rho_crit];
    /* this line returns the real value of Omega0_scf infered from the real evolution of the fields */
    
    if (Omega0_scf_try > pba->Omega0_scf)
      scf_lambda_min = _TUNE_PARAM_;
    else
      scf_lambda_max = _TUNE_PARAM_;
    /* note: the above four lines will find the correct value by bisection only if Omega0_scf_try is a monotonous function of sc_lambda. I hope that this is the case for the model we are after. It might not be the case in general. Then, one needs to think more. E.g. maybe one should pass values of scf_lambda_min and scf_lambda_max chosen not by chance, but knowning that they define the interval inside which the function Omega0_scf(scf_lambda) is monotonous. Moreover, depending on the fact that the function Omega0_scf(scf_lambda) is growing or decreasing, you may need to invert "min" and "max" inside the four lines above: I let you think about it. 
    MZ: changed min/max
     */
    
    if (background_free(pba) == _FAILURE_) {
      printf("\n\nError in background_free \n=>%s\n",pba->error_message);
      return _FAILURE_;
    }
    /* note: it is important to call background_free now, otherwise we could not call background_init again */
    
    if (iter > maximum_iter) {
      printf("\n\nError in tune_scalar_field_parameters, too many iterations \n=>%s\n",pba->error_message);
      return _FAILURE_;
    }
    iter ++;
    
  } /* end of bisection loop */
  
    pba->scf_is_tuned = _TRUE_;
  
  return _SUCCESS_;

}


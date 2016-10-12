/*
 * ViennaSetup.cpp
 *
 *  Created on: 22.09.2016
 *      Author: Mmann
 */

#include "VrnaHandler.h"
#include "general.h"

#include <iostream>

extern "C" {
	#include <ViennaRNA/energy_const.h>
	#include <ViennaRNA/read_epars.h>
}



////////////////////////////////////////////////////////////////////////////

VrnaHandler::
VrnaHandler( double temperature, const std::string * const vrnaParamFile )
	:
	model()
{

	// init parameters from file if needed
	if (vrnaParamFile != NULL) {
		// read parameters from file
		read_parameter_file( vrnaParamFile->c_str() );
	}


	// get default model data
	vrna_md_set_default( &model );

	// set model details according to command line arguments

	model.temperature = temperature;
//	model.temperature = this->temperature;
//	  double  temperature;                  /**<  @brief  The temperature used to scale the thermodynamic parameters */
//	  double  betaScale;                    /**<  @brief  A scaling factor for the thermodynamic temperature of the Boltzmann factors */
	model.dangles = 2;
//	  int     dangles;                      /**<  @brief  Specifies the dangle model used in any energy evaluation (0,1,2 or 3) */
//	  int     special_hp;                   /**<  @brief  Include special hairpin contributions for tri, tetra and hexaloops */
	model.noLP = 0;
//	  int     noLP;                         /**<  @brief  Only consider canonical structures, i.e. no 'lonely' base pairs */
	model.noGU = 0;
//	  int     noGU;                         /**<  @brief  Do not allow GU pairs */
	model.noGUclosure = 0;
//	  int     noGUclosure;                  /**<  @brief  Do not allow loops to be closed by GU pair */
//	  int     logML;                        /**<  @brief  Use logarithmic scaling for multi loops */
	model.circ = 0;
//	  int     circ;                         /**<  @brief  Assume RNA to be circular instead of linear */
	model.gquad = 0;
//	  int     gquad;                        /**<  @brief  Include G-quadruplexes in structure prediction */
//	  int     canonicalBPonly;              /**<  @brief  remove non-canonical bp's from constraint structures  */
//	  int     uniq_ML;                      /**<  @brief  Flag to ensure unique multibranch loop decomposition during folding */
//	  int     energy_set;                   /**<  @brief  Specifies the energy set that defines set of compatible base pairs */
//	  int     backtrack;                    /**<  @brief  Specifies whether or not secondary structures should be backtraced */
//	  char    backtrack_type;               /**<  @brief  Specifies in which matrix to backtrack */
//	  int     compute_bpp;                  /**<  @brief  Specifies whether or not backward recursions for base pair probability (bpp) computation will be performed */
//	  char    nonstandards[64];             /**<  @brief  contains allowed non standard bases */
	model.max_bp_span = -1;
//	  int     max_bp_span;                  /**<  @brief  maximum allowed base pair span */
//	  int     min_loop_size;                /**<  @brief  Minimum size of hairpin loops */
	model.window_size = -1;
//	  int     window_size;                  /**<  @brief  Size of the sliding window for locally optimal structure predition */
//	  int     oldAliEn;                     /**<  @brief  Use old alifold energy model */
//	  int     ribo;                         /**<  @brief  Use ribosum scoring table in alifold energy model */
//	  double  cv_fact;                      /**<  @brief  Covariance scaling factor for consensus structure prediction */
//	  double  nc_fact;                      /**<  @brief  Scaling factor to weight covariance contributions of non-canonical pairs */
//	  double  sfact;                        /**<  @brief  Scaling factor for partition function scaling */
//	  int     rtype[8];                     /**<  @brief  Reverse base pair type array */
//	  short   alias[MAXALPHA+1];            /**<  @brief  alias of an integer nucleotide representation */
//	  int     pair[MAXALPHA+1][MAXALPHA+1]; /**<  @brief  Integer representation of a base pair */


	// might need to be called to broadcast model changes globally
	//vrna_md_defaults_reset(model);

}

////////////////////////////////////////////////////////////////////////////

VrnaHandler::
~VrnaHandler() {
}

////////////////////////////////////////////////////////////////////////////




vrna_md_t
VrnaHandler::
getModel( int max_bp_span, int window_size ) const
{
	// the sub model to be returned
	vrna_md_t subModel;

	// TODO replace by vrna_model_copy() as soon as available (v2.3.0)
	// copy default model
	// vrna_model_copy(&subModel, &model);
	
    // copy over general model
    memcpy(&subModel, &model, sizeof(vrna_md_t));
    memcpy(subModel.rtype, &(model.rtype[0]), 8 * sizeof(int));
    memcpy(subModel.alias, &(model.alias[0]), (MAXALPHA + 1) * sizeof(short));
//    memset(subModel.alias, 0, (MAXALPHA + 1) * sizeof(short));
    for(int i = 0;i <= MAXALPHA; i++) {
      memcpy(subModel.pair[i], &(subModel.pair[i]), (MAXALPHA + 1) * sizeof(int));
//      memset(subModel.pair[i], 0, (MAXALPHA + 1) * sizeof(int));
    }
//    vrna_md_update( &subModel );

    // set specific parameters
    subModel.max_bp_span = max_bp_span;
    subModel.window_size = window_size;

	// return new model
	return subModel;
}

////////////////////////////////////////////////////////////////////////////

double
VrnaHandler::
getRT( ) const
{
	return ((model.temperature+K0)*GASCONST/1000.0);
}


////////////////////////////////////////////////////////////////////////////



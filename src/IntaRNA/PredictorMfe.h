
#ifndef INTARNA_PREDICTORMFE_H_
#define INTARNA_PREDICTORMFE_H_


#include "IntaRNA/Predictor.h"

#include "IntaRNA/IndexRangeList.h"

#include <list>
#include <utility>

namespace IntaRNA {

/**
 * Generic Predictor interface for MFE interaction computation to avoid
 * code redundancy
 *
 * @author Martin Mann
 *
 */
class PredictorMfe : public Predictor {


public:

	/**
	 * Construction call the super constructor
	 *
	 * @param energy the interaction energy handler
	 * @param output the output handler to report optimal interactions to
	 * @param predTracker the prediction tracker to be used or NULL if no
	 *         tracking is to be done; if non-NULL, the tracker gets deleted
	 *         on this->destruction.
	 */
	PredictorMfe( const InteractionEnergy & energy
				, OutputHandler & output
				, PredictionTracker * predTracker );

	virtual ~PredictorMfe();

protected:


	//! access to the interaction energy handler of the super class
	using Predictor::energy;

	//! access to the output handler of the super class
	using Predictor::output;

	//! access to the prediction tracker of the super class
	using Predictor::predTracker;

	// TODO provide all data structures as arguments to make predict() call threadsafe

	//! list of interactions
	typedef std::list<Interaction> InteractionList;

	//! mfe interaction boundaries
	InteractionList mfeInteractions;

	//! index ranges of reported interactions to identify non-overlapping
	//! interactions (first = seq1, second = seq2)
	//! NOTE: the indices for seq2 are reversed
	std::pair< IndexRangeList, IndexRangeList > reportedInteractions;

	//! minimal stacking energy
	const E_type minStackingEnergy;
	//! minimal interaction initiation energy
	const E_type minInitEnergy;
	//! minimal dangling end energy
	const E_type minDangleEnergy;
	//! minimal interaction end energy
	const E_type minEndEnergy;

	/**
	 * Initializes the global energy minimum storage
	 *
	 * @param outConstraint constrains the interactions reported to the output handler
	 */
	virtual
	void
	initOptima( const OutputConstraint & outConstraint );

	/**
	 * updates the global optimum to be the mfe interaction if needed
	 *
	 * @param i1 the index of the first sequence interacting with i2
	 * @param j1 the index of the first sequence interacting with j2
	 * @param i2 the index of the second sequence interacting with i1
	 * @param j2 the index of the second sequence interacting with j1
	 * @param energy the energy of the interaction
	 * @param isHybridE whether or not the given energy is only the
	 *        hybridization energy (init+loops) or the total interaction energy
	 */
	virtual
	void
	updateOptima( const size_t i1, const size_t j1
				, const size_t i2, const size_t j2
				, const E_type energy
				, const bool isHybridE );


	/**
	 * Fills a given interaction (boundaries given) with the according
	 * hybridizing base pairs.
	 * Note, the
	 * @param interaction IN/OUT the interaction to fill
	 * @param outConstraint constrains the interactions reported to the output handler
	 */
	virtual
	void
	traceBack( Interaction & interaction, const OutputConstraint & outConstraint  ) = 0;


	/**
	 * Identifies the next best interaction with an energy equal to or higher
	 * than the given interaction. The new interaction will not overlap any
	 * index range stored in reportedInteractions.
	 *
	 * @param curBest IN/OUT the current best interaction to be replaced with one
	 *        of equal or higher energy not overlapping with any reported
	 *        interaction so far; an interaction with energy E_INF is set, if
	 *        there is no better interaction left
	 */
	virtual
	void
	getNextBest( Interaction & curBest ) = 0;

	/**
	 * Calls for the stored mfe and suboptimal solutions traceBack(i)
	 * and pushes the according interactions to the output handler.
	 * For non-overlapping interaction enumeration, getNextBest() is called
	 * iteratively.
	 *
	 * @param outConstraint constrains the interactions reported to the output handler
	 */
	virtual
	void
	reportOptima( const OutputConstraint & outConstraint );

};

} // namespace

#endif /* INTARNA_PREDICTORMFE_H_ */

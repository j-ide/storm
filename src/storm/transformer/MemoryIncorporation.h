#pragma once

#include <memory>

#include "storm/logic/Formula.h"
#include "storm/logic/MultiObjectiveFormula.h"
#include "storm/storage/memorystructure/MemoryStructure.h"

namespace storm {
namespace transformer {

/*!
 * Incorporates Memory into the state space of the given model, that is
 * the resulting model is the crossproduct of of the given model plus
 * some type of memory structure
 */
template<class SparseModelType>
class MemoryIncorporation {
    typedef typename SparseModelType::ValueType ValueType;
    typedef typename SparseModelType::RewardModelType RewardModelType;
    typedef std::function<storm::storage::BitVector(storm::logic::Formula const&)> CheckFormulaCallback;

   public:
    /*!
     * Incorporates memory that stores whether a 'goal' state has already been reached. This supports operatorformulas whose subformula is
     * a (bounded-) until formula, eventually formula, or a globally formula. Total reward formulas and cumulative reward formulas will be ignored.
     */
    static std::shared_ptr<SparseModelType> incorporateGoalMemory(SparseModelType const& model,
                                                                  std::vector<std::shared_ptr<storm::logic::Formula const>> const& formulas,
                                                                  CheckFormulaCallback const& formulaChecker);

    /*!
     * Incorporates a memory structure where the nondeterminism of the model decides which successor state to choose.
     */
    static std::shared_ptr<SparseModelType> incorporateFullMemory(SparseModelType const& model, uint64_t memoryStates);

    /*!
     * Incorporates a memory structure where the nondeterminism of the model can increment a counter.
     */
    static std::shared_ptr<SparseModelType> incorporateCountingMemory(SparseModelType const& model, uint64_t memoryStates);
};
}  // namespace transformer
}  // namespace storm

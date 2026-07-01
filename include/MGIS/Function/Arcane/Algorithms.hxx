/*!
 * \file   MGIS/Function/Arcane/Algorithms.hxx
 * \brief
 * \author Thomas Helfer
 * \date   28/06/2026
 * \copyright (C) Copyright Thomas Helfer 2018.
 * Use, modification and distribution are subject
 * to one of the following licences:
 * - GNU Lesser General Public License (LGPL), Version 3.0. (See accompanying
 *   file LGPL-3.0.txt)
 * - CECILL-C,  Version 1.0 (See accompanying files
 *   CeCILL-C_V1-en.txt and CeCILL-C_V1-fr.txt).
 */

#ifndef LIB_MGIS_FUNCTION_ARCANE_ALGORITHMS_HXX
#define LIB_MGIS_FUNCTION_ARCANE_ALGORITHMS_HXX

#include "arccore/common/accelerator/RunQueue.h"

#include "MGIS/Config.hxx"
#include "MGIS/AbstractErrorHandler.hxx"
#include "MGIS/Function/SpaceConcept.hxx"
#include "MGIS/Function/EvaluatorConcept.hxx"
#include "MGIS/Function/FunctionConcept.hxx"

namespace mgis::function {

  /*!
   * \brief assign the evaluator to a function
   * \param[in] ctx: execution context
   * \param[in] q: queue on which assignement is run
   * \param[in] lhs: left hand side
   * \param[in] e: right hand side
   */
  template <typename FunctionType, EvaluatorConcept EvaluatorType>
  [[nodiscard]] bool assign(AbstractErrorHandler&,
                            Arcane::RunQueue&,
                            FunctionType&,
                            const EvaluatorType)  //
      requires(
          ((LinearElementSpaceConcept<evaluator_space<EvaluatorType>>) ||
           (LinearQuadratureSpaceConcept<evaluator_space<EvaluatorType>>)) &&
          std::same_as<function_space<FunctionType>,
                       evaluator_space<EvaluatorType>>);

}  // end of namespace mgis::function

#include "MGIS/Function/Arcane/Algorithms.ixx"

#endif /* LIB_MGIS_FUNCTION_ARCANE_ALGORITHMS_HXX */

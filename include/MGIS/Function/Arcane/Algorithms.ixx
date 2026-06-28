/*!
 * \file   MGIS/Function/Arcane/Algorithms.ixx
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

#ifndef LIB_MGIS_FUNCTION_ARCANE_ALGORITHMS_IXX
#define LIB_MGIS_FUNCTION_ARCANE_ALGORITHMS_IXX

#include "arccore/accelerator/RunCommandLoop.h"
#include "MGIS/Function/Algorithms.hxx"

namespace mgis::function {

  template <typename FunctionType, EvaluatorConcept EvaluatorType>
  bool assign(AbstractErrorHandler& ctx,
              Arcane::RunQueue& q,
              FunctionType& f,
              const EvaluatorType e)  //
      requires(
          ((LinearElementSpaceConcept<evaluator_space<EvaluatorType>>) ||
           (LinearQuadratureSpaceConcept<evaluator_space<EvaluatorType>>)) &&
          std::same_as<function_space<FunctionType>,
                       evaluator_space<EvaluatorType>>) {
    using Space = std::decay_t<decltype(getSpace(f))>;
    using space_size_type = typename SpaceTraits<Space>::size_type;
    using value_type = std::invoke_result_t<FunctionType, size_type>;
    using result_type = std::invoke_result_t<const EvaluatorType, size_type>;
    constexpr auto use_direct_assignement =
        requires(value_type & v1, const result_type& v2) {
      v1 = v2;
    };
    //
    const auto& space = getSpace(f);
    if (!areEquivalent(space, getSpace(e))) {
      return ctx.registerErrorMessage("unmatched spaces");
    }
    if (getNumberOfComponents(f) != getNumberOfComponents(e)) {
      return ctx.registerErrorMessage("unmatched number of components");
    }
    if (!check(ctx, e)) {
      return false;
    }
    //
    auto command = Arcane::Accelerator::makeCommand(q);
    auto bounds = Arcane::SimpleForLoopRanges<1, space_size_type>{getSpaceSize(f)};
    //
    if constexpr (LightweightViewConcept<FunctionType>) {
      if constexpr (use_direct_assignement) {
        auto fct = [f, e](const space_size_type i) mutable {
          f(i) = e(i);
        };
        Arcane::Accelerator::run(command, bounds, fct);
      } else {
        auto fct = [f, e](const space_size_type i) mutable {
          ::mgis::function::internals::assign_value(f(i), e(i));
        };
        Arcane::Accelerator::run(command, bounds, fct);
      }
    } else {
      auto v = view(f);
      if constexpr (use_direct_assignement) {
        auto fct = [v, e](const space_size_type i) mutable {
          v(i) = e(i);
        };
        Arcane::Accelerator::run(command, bounds, fct);
      } else {
        auto fct = [v, e](const space_size_type i) mutable {
          ::mgis::function::internals::assign_value(v(i), e(i));
        };
        Arcane::Accelerator::run(command, bounds, fct);
      }
    }
    return true;
  }  // end of assign

}  // end of namespace mgis::function

#endif /* LIB_MGIS_FUNCTION_ARCANE_ALGORITHMS_HXX */

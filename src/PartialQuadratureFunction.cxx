/*!
 * \file   src/PartialQuadratureFunction.cxx
 * \brief
 * \author Thomas Helfer
 * \date   08/08/2026
 */

#include "MGIS/Function/Algorithms.hxx"
#include "MGIS/Function/EvaluatorConcept.hxx"
#include "MGIS/Arcane/PartialQuadratureFunction.hxx"

namespace mgis::arcane {

  static_assert(
      mgis::function::EvaluatorConcept<SubPartialQuadratureFunctionView<true>>);

  static_assert(mgis::function::EvaluatorConcept<
                SubPartialQuadratureFunctionView<false>>);

  static_assert(
      mgis::function::EvaluatorConcept<PartialQuadratureFunctionView<true>>);

  static_assert(
      mgis::function::EvaluatorConcept<PartialQuadratureFunctionView<false>>);

  static_assert(mgis::function::ElementFunctionConcept<
                SubPartialQuadratureFunctionView<true>>);

  static_assert(mgis::function::QuadratureFunctionConcept<
                SubPartialQuadratureFunctionView<true>>);

  static_assert(mgis::function::ElementFunctionConcept<
                PartialQuadratureFunctionView<true>>);

  static_assert(mgis::function::QuadratureFunctionConcept<
                PartialQuadratureFunctionView<true>>);

  static_assert(
      mgis::function::ElementFunctionConcept<PartialQuadratureFunction>);

  static_assert(
      mgis::function::QuadratureFunctionConcept<PartialQuadratureFunction>);

  bool SubPartialQuadratureFunctionViewBase::check(
      mgis::AbstractErrorHandler& ctx,
      const Arcane::Integer size) const noexcept {
    if (this->data_begin < 0) {
      return ctx.registerErrorMessage("invalid value of the initial offset");
    }
    if (this->data_size <= 0) {
      return ctx.registerErrorMessage("invalid value of the data size");
    }
    if (this->data_begin + this->data_size > size) {
      return ctx.registerErrorMessage(
          "range of the subview exceeds the size of the underlying partial "
          "quadrature view");
    }
    return true;
  }  // end of check

  PartialQuadratureFunctionBase::PartialQuadratureFunctionBase(
      std::shared_ptr<PartialQuadratureSpace> s,
      const Arcane::String& n,
      const Arcane::Integer c)
      : qspace(s),
        variable(Arcane::VariableBuildInfo(
            &(s->getMutableDoFFamilyInterfaceReference()), n)) {
    this->variable.resize(c);
  }  // end ofPartialQuadratureFunctionBase

  PartialQuadratureFunction::PartialQuadratureFunction(
      std::shared_ptr<PartialQuadratureSpace> s,
      const Arcane::String& n,
      const Arcane::Integer c)
      : PartialQuadratureFunctionBase(s, n, c),
        PartialQuadratureFunctionView(*(this->qspace),
                                      this->variable.asArray()) {
  }  // end of PartialQuadratureFunction

}  // end of namespace mgis::arcane
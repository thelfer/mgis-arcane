/*!
 * \file   src/PartialQuadratureFunction.cxx
 * \brief    
 * \author Thomas Helfer
 * \date   08/08/2026
 */

#include "MGIS/Function/Algorithms.hxx"
#include "MGIS/Function/EvaluatorConcept.hxx"
#include "MGIS/Function/FixedSizeModifier.hxx"
#include "MGIS/Arcane/PartialQuadratureFunction.hxx"

namespace mgis::arcane {

  static_assert(
      mgis::function::EvaluatorConcept<PartialQuadratureFunctionView<true>>);

  static_assert(
      mgis::function::EvaluatorConcept<PartialQuadratureFunctionView<false>>);

  static_assert(
      mgis::function::ElementFunctionConcept<PartialQuadratureFunction>);

  static_assert(
      mgis::function::QuadratureFunctionConcept<PartialQuadratureFunction>);

  PartialQuadratureFunctionBase::PartialQuadratureFunctionBase(
      std::shared_ptr<PartialQuadratureSpace> s,
      const Arcane::String& n,
      const Arcane::Integer c)
      : qspace(s),
        variable(Arcane::VariableBuildInfo(&(s->getMutableDoFFamilyReference()),
                                           n)) {
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

  void test(mgis::Context& ctx,
            PartialQuadratureFunction& f,
            PartialQuadratureFunction& f2) {
    using namespace mgis::function;
    // //    auto v = ::mgis::function::view<1>(f);
    auto v2 = ::mgis::function::view<1>(view(f));
    assign(ctx, f, v2 | multiply_by_scalar(3));
  }

} // end of namespace mgis::arcane
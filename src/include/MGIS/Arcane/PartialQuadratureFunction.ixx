/*!
 * \file   MGIS/Arcane/PartialQuadratureFunction.ixx
 * \brief
 * \author Thomas Helfer
 * \date   08/08/2026
 */

#ifndef LIB_MGIS_ARCANE_PARTIALQUADRATUREFUNCTION_IXX
#define LIB_MGIS_ARCANE_PARTIALQUADRATUREFUNCTION_IXX

namespace mgis::arcane {

  const PartialQuadratureSpace& PartialQuadratureFunctionBase::getSpace()
      const noexcept {
    return *(this->qspace);
  }  // end of getSpace

  template <bool is_mutable>
  PartialQuadratureFunctionView<is_mutable>::PartialQuadratureFunctionView(
      const PartialQuadratureSpace& s, DataType d)
      : qspace_ptr(&s),
        qspace_view(s),
        values(d) {}  // end of PartialQuadratureFunctionView

  template <bool is_mutable>
  PartialQuadratureFunctionView<is_mutable>::operator DataType&() noexcept {
    return this->values;
  }

  template <bool is_mutable>
  PartialQuadratureFunctionView<is_mutable>::operator const DataType&()
      const noexcept {
    return this->values;
  }

  template <bool is_mutable>
  const PartialQuadratureSpace&
  PartialQuadratureFunctionView<is_mutable>::getSpace() const noexcept {
    return *(this->qspace_ptr);
  }  // end of getSpace

  template <bool is_mutable>
  Arcane::ArrayView<real> PartialQuadratureFunctionView<is_mutable>::operator()(
      const Arcane::Integer e) noexcept requires(is_mutable) {
    return this->values[e];
  }  // end of operator()

  template <bool is_mutable>
  Arcane::ConstArrayView<real>
  PartialQuadratureFunctionView<is_mutable>::operator()(
      const Arcane::Integer e) const noexcept {
    return this->values[e];
  }  // end of operator()

  //
  template <bool is_mutable>
  Arcane::ArrayView<real> PartialQuadratureFunctionView<is_mutable>::operator()(
      const Arcane::Integer c, const Arcane::Integer q) noexcept
      requires(is_mutable) {
    return this->operator()(getQuadraturePointOffset(this->qspace_view, c, q));
  }  // end of operator()

  template <bool is_mutable>
  Arcane::ConstArrayView<real>
  PartialQuadratureFunctionView<is_mutable>::operator()(
      const Arcane::Integer c, const Arcane::Integer q) const noexcept {
    return this->operator()(getQuadraturePointOffset(this->qspace_view, c, q));
  }  // end of operator()

  template <bool is_mutable>
  const real* PartialQuadratureFunctionView<is_mutable>::data() const noexcept {
    return this->values.data();
  }  // end of data

  inline auto PartialQuadratureFunction::view() noexcept {
    return static_cast<PartialQuadratureFunctionView<true>&>(*this);
  }  // end of view

  inline auto PartialQuadratureFunction::view() const noexcept {
    return PartialQuadratureFunctionView<false>(*(this->qspace),
                                                this->variable.asArray());
  }  // end of view

  template <bool is_mutable>
  constexpr bool check(
      mgis::AbstractErrorHandler&,
      const PartialQuadratureFunctionView<is_mutable>&) noexcept {
    return true;
  }  // end of check

  template <bool is_mutable>
  inline const PartialQuadratureSpace& getSpace(
      const PartialQuadratureFunctionView<is_mutable>& f) noexcept {
    return f.getSpace();
  }  // end of getSpace

  template <bool is_mutable>
  inline mgis::size_type getNumberOfComponents(
      const PartialQuadratureFunctionView<is_mutable>& f) noexcept {
    return static_cast<
               const PartialQuadratureFunctionView<is_mutable>::DataType&>(f)
        .dim2Size();
  }  // end of getNumberOfComponents

  inline PartialQuadratureFunctionView<true> view(
      PartialQuadratureFunction& f) noexcept {
    return f.view();
  }

  inline PartialQuadratureFunctionView<false> view(
      const PartialQuadratureFunction& f) noexcept {
    return f.view();
  }  // end of view

  template <Arcane::Integer N, bool is_mutable>
  auto view(PartialQuadratureFunctionView<is_mutable>& f) noexcept
      requires(N > 0) {
    return ::mgis::function::view<static_cast<::mgis::size_type>(N)>(f);
  }

  template <Arcane::Integer N, bool is_mutable>
  auto view(const PartialQuadratureFunctionView<is_mutable>& f) noexcept
      requires(N > 0) {
    return ::mgis::function::view<static_cast<::mgis::size_type>(N)>(f);
  }

}  // end of namespace mgis::arcane

#endif /* LIB_MGIS_ARCANE_PARTIALQUADRATUREFUNCTION_HXX */
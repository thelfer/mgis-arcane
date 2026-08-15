/*!
 * \file   MGIS/Arcane/PartialQuadratureFunction.ixx
 * \brief
 * \author Thomas Helfer
 * \date   08/08/2026
 */

#ifndef LIB_MGIS_ARCANE_PARTIALQUADRATUREFUNCTION_IXX
#define LIB_MGIS_ARCANE_PARTIALQUADRATUREFUNCTION_IXX

namespace mgis::arcane {

  //  Implementation of PartialQuadratureFunctionBase's methods

  const PartialQuadratureSpace& PartialQuadratureFunctionBase::getSpace()
      const noexcept {
    return *(this->qspace);
  }  // end of getSpace

  inline PartialQuadratureFunctionBase::operator Arcane::
      VariableDoFArrayReal&() noexcept {
    return this->variable;
  }

  inline PartialQuadratureFunctionBase::operator const Arcane::
      VariableDoFArrayReal&() const noexcept {
    return this->variable;
  }

  //  Implementation of PartialQuadratureFunctionViewBase's methods

  template <bool is_mutable>
  PartialQuadratureFunctionViewBase<is_mutable>::
      PartialQuadratureFunctionViewBase(const PartialQuadratureSpace& s,
                                        DataType d)
      : qspace_ptr(&s),
        qspace_view(s),
        values(d) {}  // end of PartialQuadratureFunctionView

  template <bool is_mutable>
  PartialQuadratureFunctionViewBase<is_mutable>::operator DataType&() noexcept {
    return this->values;
  }

  template <bool is_mutable>
  PartialQuadratureFunctionViewBase<is_mutable>::operator const DataType&()
      const noexcept {
    return this->values;
  }

  template <bool is_mutable>
  const PartialQuadratureSpace&
  PartialQuadratureFunctionViewBase<is_mutable>::getSpace() const noexcept {
    return *(this->qspace_ptr);
  }  // end of getSpace

  //  Implementation of PartialQuadratureFunctionView's methods

  template <bool is_mutable>
  real* PartialQuadratureFunctionView<is_mutable>::data(
      ::mgis::attributes::UnsafeAttribute,
      const Arcane::Integer e) requires(is_mutable) {
    const auto s = this->values.dim2Size();
    return this->values.unguardedBasePointer() + (e * s);
  }  // end of data

  template <bool is_mutable>
  real* PartialQuadratureFunctionView<is_mutable>::data(
      ::mgis::attributes::UnsafeAttribute,
      const Arcane::Integer c,
      const Arcane::Integer q) requires(is_mutable) {
    const auto o = getQuadraturePointOffset(this->qspace_view, c, q);
    return this->data(::mgis::unsafe, o);
  }  // end of data

  template <bool is_mutable>
  const real* PartialQuadratureFunctionView<is_mutable>::data(
      ::mgis::attributes::UnsafeAttribute, const Arcane::Integer e) const {
    const auto s = this->values.dim2Size();
    return Arcane::ConstArray2View<real>(this->values).data() + (e * s);
  }  // end of data

  template <bool is_mutable>
  const real* PartialQuadratureFunctionView<is_mutable>::data(
      ::mgis::attributes::UnsafeAttribute,
      const Arcane::Integer c,
      const Arcane::Integer q) const {
    const auto o = getQuadraturePointOffset(this->qspace_view, c, q);
    return this->data(::mgis::unsafe, o);
  }  // end of data

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

  //  Implementation of SubPartialQuadratureFunctionViewBase's methods

  SubPartialQuadratureFunctionViewBase::SubPartialQuadratureFunctionViewBase(
      const Description d)
      : data_begin(d.begin),
        data_size(d.size) {}  // end of SubPartialQuadratureFunctionView

  //  Implementation of SubPartialQuadratureFunctionView's methods

  template <bool is_mutable>
  SubPartialQuadratureFunctionView<is_mutable>::
      SubPartialQuadratureFunctionView(const PartialQuadratureSpace& space,
                                       DataType data,
                                       const Description d)
      : SubPartialQuadratureFunctionViewBase(d),
        PartialQuadratureFunctionViewBase<is_mutable>(space, data) {
  }  // end of SubPartialQuadratureFunctionView

  template <bool is_mutable>
  bool SubPartialQuadratureFunctionView<is_mutable>::check(
      mgis::AbstractErrorHandler& ctx) const noexcept {
    return SubPartialQuadratureFunctionViewBase::check(ctx,
                                                       this->values.dim2Size());
  }  // end of check

  template <bool is_mutable>
  Arcane::ArrayView<real>
  SubPartialQuadratureFunctionView<is_mutable>::operator()(
      const Arcane::Integer e) noexcept requires(is_mutable) {
    return this->values[e].subView(this->data_begin, this->data_size);
  }  // end of operator()

  template <bool is_mutable>
  Arcane::ConstArrayView<real>
  SubPartialQuadratureFunctionView<is_mutable>::operator()(
      const Arcane::Integer e) const noexcept {
    return this->values[e].subConstView(this->data_begin, this->data_size);
  }  // end of operator()

  template <bool is_mutable>
  Arcane::ArrayView<real>
  SubPartialQuadratureFunctionView<is_mutable>::operator()(
      const Arcane::Integer c, const Arcane::Integer q) noexcept
      requires(is_mutable) {
    return this->operator()(getQuadraturePointOffset(this->qspace_view, c, q));
  }  // end of operator()

  template <bool is_mutable>
  Arcane::ConstArrayView<real>
  SubPartialQuadratureFunctionView<is_mutable>::operator()(
      const Arcane::Integer c, const Arcane::Integer q) const noexcept {
    return this->operator()(getQuadraturePointOffset(this->qspace_view, c, q));
  }  // end of operator()

  template <bool is_mutable>
  const real* SubPartialQuadratureFunctionView<is_mutable>::data()
      const noexcept {
    return this->values.data();
  }  // end of data

  template <bool is_mutable>
  real* SubPartialQuadratureFunctionView<is_mutable>::data(
      ::mgis::attributes::UnsafeAttribute,
      const Arcane::Integer e) requires(is_mutable) {
    const auto s = this->values.dim2Size();
    return this->values.unguardedBasePointer() + (e * s) + this->data_begin;
  }  // end of :data

  template <bool is_mutable>
  real* SubPartialQuadratureFunctionView<is_mutable>::data(
      mgis::attributes::UnsafeAttribute unsafe,
      const Arcane::Integer c,
      const Arcane::Integer q) requires(is_mutable) {
    return this->data(unsafe,
                      getQuadraturePointOffset(this->qspace_view, c, q));
  }  // end of data

  template <bool is_mutable>
  const real* SubPartialQuadratureFunctionView<is_mutable>::data(
      mgis::attributes::UnsafeAttribute, const Arcane::Integer e) const {
    const auto s = this->values.dim2Size();
    return this->values.unguardedBasePointer() + (e * s) + this->data_begin;
  }  // end of data

  template <bool is_mutable>
  const real* SubPartialQuadratureFunctionView<is_mutable>::data(
      mgis::attributes::UnsafeAttribute unsafe,
      const Arcane::Integer c,
      const Arcane::Integer q) const {
    return this->data(unsafe,
                      getQuadraturePointOffset(this->qspace_view, c, q));
  }  // end of data

  //  Implementation of PartialQuadratureFunctionView's methods

  inline auto PartialQuadratureFunction::view() noexcept {
    return static_cast<PartialQuadratureFunctionView<true>&>(*this);
  }  // end of view

  inline auto PartialQuadratureFunction::view() const noexcept {
    return PartialQuadratureFunctionView<false>(*(this->qspace),
                                                this->variable.asArray());
  }  // end of view

  // free functions

  template <bool is_mutable>
  constexpr bool check(
      mgis::AbstractErrorHandler&,
      const PartialQuadratureFunctionView<is_mutable>&) noexcept {
    return true;
  }  // end of check

  template <bool is_mutable>
  bool check(mgis::AbstractErrorHandler& e,
             const SubPartialQuadratureFunctionView<is_mutable>& f) noexcept {
    return f.check(e);
  }  // end of check

  template <bool is_mutable>
  const PartialQuadratureSpace& getSpace(
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

}  // end of namespace mgis::arcane

#endif /* LIB_MGIS_ARCANE_PARTIALQUADRATUREFUNCTION_HXX */
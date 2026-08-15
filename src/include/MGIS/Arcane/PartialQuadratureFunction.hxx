/*!
 * \file   MGIS/Arcane/PartialQuadratureFunction.hxx
 * \brief
 * \author Thomas Helfer
 * \date   08/08/2026
 */

#ifndef LIB_MGIS_ARCANE_PARTIALQUADRATUREFUNCTION_HXX
#define LIB_MGIS_ARCANE_PARTIALQUADRATUREFUNCTION_HXX

#include <memory>
#include "arcane/core/VariableTypes.h"
#include "MGIS/Function/Function.hxx"
#include "MGIS/Function/FixedSizeModifier.hxx"
#include "MGIS/Arcane/PartialQuadratureSpace.hxx"

namespace Arcane {

  template <typename DataType>
  inline void assign_value(ArrayView<DataType> lhs,
                           const ArrayView<DataType>& rhs) {
    std::copy(rhs.begin(), rhs.end(), lhs.begin());
  }

  template <typename DataType>
  inline void assign_value(ArrayView<DataType> lhs,
                           ConstArrayView<DataType>& rhs) {
    std::copy(rhs.begin(), rhs.end(), lhs.begin());
  }

  template <typename DataType>
  inline void assign_value(ArrayView<DataType> lhs, const DataType& rhs) {
    lhs[0] = rhs;
  }

}  // namespace Arcane

namespace mgis::arcane {

  struct PartialQuadratureFunctionBase {
    //
    explicit operator Arcane::VariableDoFArrayReal&() noexcept;
    explicit operator const Arcane::VariableDoFArrayReal&() const noexcept;
    //
    [[nodiscard]] const PartialQuadratureSpace& getSpace() const noexcept;

   protected:
    /*!
     *
     */
    PartialQuadratureFunctionBase(std::shared_ptr<PartialQuadratureSpace>,
                                  const Arcane::String&,
                                  const Arcane::Integer);
    //! \brief partial quadrature space
    std::shared_ptr<PartialQuadratureSpace> qspace;
    //! \brief Arcane's variable
    Arcane::VariableDoFArrayReal variable;
  };  // end of struct PartialQuadratureFunctionBase

  template <bool is_mutable>
  struct PartialQuadratureFunctionViewBase {
    //
    using DataType = std::conditional_t<is_mutable,
                                        Arcane::Array2View<real>,
                                        Arcane::ConstArray2View<real>>;
    //
    PartialQuadratureFunctionViewBase(const PartialQuadratureSpace&, DataType);
    //
    PartialQuadratureFunctionViewBase(
        PartialQuadratureFunctionViewBase&&) noexcept = default;
    PartialQuadratureFunctionViewBase(
        const PartialQuadratureFunctionViewBase&) noexcept = default;
    /*!
     * \return the underlying space
     *
     * \note this method shall not be used on the device
     */
    [[nodiscard]] const PartialQuadratureSpace& getSpace() const noexcept;
    //
    [[nodiscard]] explicit operator DataType&() noexcept;
    [[nodiscard]] explicit operator const DataType&() const noexcept;

   protected:
    /*!
     * \brief pointer to the quadrature space
     *
     * \note this pointer shall not be used on the device
     */
    const PartialQuadratureSpace* const qspace_ptr;
    //! \brief view to partial quadrature space
    const PartialQuadratureSpaceView qspace_view;
    //! \brief view to the data of the partial quadrature function data
    DataType values;
  };  // end of struct PartialQuadratureFunctionViewBase

  /*!
   * \brief base class for the SubPartialQuadratureFunctionView
   *
   * \note This class has been introduced for the check method to be
   * implented in a source file (error handling and check functions
   * shall are better not be inlined). This class is not meant to be used by an
   * end-user. It's constructor is declared protected on purpose.
   */
  struct SubPartialQuadratureFunctionViewBase {
    //
    struct Description {
      const Arcane::Integer begin;
      const Arcane::Integer size;
    };
    //
    [[nodiscard]] Arcane::Integer getDataSize() const noexcept;

   protected:
    SubPartialQuadratureFunctionViewBase(const Description);
    //
    [[nodiscard]] bool check(mgis::AbstractErrorHandler&,
                             const Arcane::Integer) const noexcept;
    //
    Arcane::Integer data_begin;
    Arcane::Integer data_size;
  };

  template <bool is_mutable = true>
  struct [[nodiscard]] SubPartialQuadratureFunctionView
      : SubPartialQuadratureFunctionViewBase,
        PartialQuadratureFunctionViewBase<is_mutable> {
    //
    using DataType =
        typename PartialQuadratureFunctionViewBase<is_mutable>::DataType;
    //
    SubPartialQuadratureFunctionView(const PartialQuadratureSpace&,
                                     DataType,
                                     const Description);
    //
    SubPartialQuadratureFunctionView(
        SubPartialQuadratureFunctionView&&) noexcept = default;
    SubPartialQuadratureFunctionView(
        const SubPartialQuadratureFunctionView&) noexcept = default;
    //
    [[nodiscard]] bool check(mgis::AbstractErrorHandler&) const noexcept;
    //
    [[nodiscard]] Arcane::ArrayView<real> operator()(
        const Arcane::Integer) noexcept requires(is_mutable);
    [[nodiscard]] Arcane::ConstArrayView<real> operator()(
        const Arcane::Integer) const noexcept;
    //
    [[nodiscard]] Arcane::ArrayView<real> operator()(
        const Arcane::Integer, const Arcane::Integer) noexcept
        requires(is_mutable);
    [[nodiscard]] Arcane::ConstArrayView<real> operator()(
        const Arcane::Integer, const Arcane::Integer) const noexcept;
    //! \brief return a pointer to the underlying data
    [[nodiscard]] const real* data() const noexcept;
    /*!
     * \return the data associated with an integration point
     * \param[in] o: offset associated with the integration point
     */
    [[nodiscard]] real* data(::mgis::attributes::UnsafeAttribute,
                             const Arcane::Integer) requires(is_mutable);
    /*!
     * \return the data associated with an integration point
     * \param[in] e: element index
     * \param[in] i: quadrature point index
     */
    [[nodiscard]] real* data(mgis::attributes::UnsafeAttribute,
                             const Arcane::Integer,
                             const Arcane::Integer) requires(is_mutable);
    /*!
     * \return the data associated with an integration point
     * \param[in] o: offset associated with the integration point
     */
    [[nodiscard]] const real* data(mgis::attributes::UnsafeAttribute,
                                   const Arcane::Integer) const;
    /*!
     * \return the data associated with an integration point
     * \param[in] e: element index
     * \param[in] i: quadrature point index
     */
    [[nodiscard]] const real* data(mgis::attributes::UnsafeAttribute,
                                   const Arcane::Integer,
                                   const Arcane::Integer) const;
  };  // end of struct SubPartialQuadratureFunctionView

  template <bool is_mutable = true>
  struct [[nodiscard]] PartialQuadratureFunctionView
      : PartialQuadratureFunctionViewBase<is_mutable> {
    //
    using PartialQuadratureFunctionViewBase<
        is_mutable>::PartialQuadratureFunctionViewBase;
    //
    PartialQuadratureFunctionView(PartialQuadratureFunctionView&&) noexcept =
        default;
    PartialQuadratureFunctionView(
        const PartialQuadratureFunctionView&) noexcept = default;

    //
    [[nodiscard]] Arcane::ArrayView<real> operator()(
        const Arcane::Integer) noexcept requires(is_mutable);
    [[nodiscard]] Arcane::ConstArrayView<real> operator()(
        const Arcane::Integer) const noexcept;
    //
    [[nodiscard]] Arcane::ArrayView<real> operator()(
        const Arcane::Integer, const Arcane::Integer) noexcept
        requires(is_mutable);
    [[nodiscard]] Arcane::ConstArrayView<real> operator()(
        const Arcane::Integer, const Arcane::Integer) const noexcept;
    //! \brief return a pointer to the underlying data
    [[nodiscard]] const real* data() const noexcept;
    /*!
     * \return the data associated with an integration point
     * \param[in] o: offset associated with the integration point
     */
    [[nodiscard]] real* data(::mgis::attributes::UnsafeAttribute,
                             const Arcane::Integer) requires(is_mutable);
    /*!
     * \return the data associated with an integration point
     * \param[in] e: element index
     * \param[in] i: quadrature point index
     */
    [[nodiscard]] real* data(mgis::attributes::UnsafeAttribute,
                             const Arcane::Integer,
                             const Arcane::Integer) requires(is_mutable);
    /*!
     * \return the data associated with an integration point
     * \param[in] o: offset associated with the integration point
     */
    [[nodiscard]] const real* data(mgis::attributes::UnsafeAttribute,
                                   const Arcane::Integer) const;
    /*!
     * \return the data associated with an integration point
     * \param[in] e: element index
     * \param[in] i: quadrature point index
     */
    [[nodiscard]] const real* data(mgis::attributes::UnsafeAttribute,
                                   const Arcane::Integer,
                                   const Arcane::Integer) const;
    /*!
     * \return a sub view
     * \param[in] d: description of the subview
     * \note a every view and evaluators, the `check` method shall be called on
     * the returned object before using it.
     */
    SubPartialQuadratureFunctionView<is_mutable> view(
        typename SubPartialQuadratureFunctionViewBase::Description) noexcept;
    /*!
     * \return a sub view
     * \param[in] d: description of the subview
     * \note a every view and evaluators, the `check` method shall be called on
     * the returned object before using it.
     */
    SubPartialQuadratureFunctionView<false> view(
        typename SubPartialQuadratureFunctionViewBase::Description)
        const noexcept;
  };

  //
  template <bool is_mutable>
  [[nodiscard]] constexpr bool check(
      mgis::AbstractErrorHandler&,
      const PartialQuadratureFunctionView<is_mutable>&) noexcept;

  template <bool is_mutable>
  [[nodiscard]] bool check(
      mgis::AbstractErrorHandler&,
      const SubPartialQuadratureFunctionView<is_mutable>&) noexcept;

  template <bool is_mutable>
  [[nodiscard]] const PartialQuadratureSpace& getSpace(
      const PartialQuadratureFunctionView<is_mutable>&) noexcept;

  template <bool is_mutable>
  [[nodiscard]] mgis::size_type getNumberOfComponents(
      const PartialQuadratureFunctionView<is_mutable>&) noexcept;

  template <bool is_mutable>
  [[nodiscard]] const PartialQuadratureSpace& getSpace(
      const SubPartialQuadratureFunctionView<is_mutable>&) noexcept;

  template <bool is_mutable>
  [[nodiscard]] mgis::size_type getNumberOfComponents(
      const SubPartialQuadratureFunctionView<is_mutable>&) noexcept;

  struct [[nodiscard]] PartialQuadratureFunction
      : PartialQuadratureFunctionBase,
        PartialQuadratureFunctionView<true> {
    /*!
     *
     */
    PartialQuadratureFunction(std::shared_ptr<PartialQuadratureSpace>,
                              const Arcane::String&,
                              const Arcane::Integer);
    //
    using PartialQuadratureFunctionView<true>::view;
    [[nodiscard]] auto view() noexcept;
    [[nodiscard]] auto view() const noexcept;
    //
    using PartialQuadratureFunctionView<true>::getSpace;
  };  // end of struct PartialQuadratureFunction

  [[nodiscard]] PartialQuadratureFunctionView<true> view(
      PartialQuadratureFunction&) noexcept;

  [[nodiscard]] PartialQuadratureFunctionView<false> view(
      const PartialQuadratureFunction&) noexcept;

}  // end of namespace mgis::arcane

namespace mgis::function {

  template <bool is_mutable>
  struct LightweightViewTraits<
      mgis::arcane::PartialQuadratureFunctionView<is_mutable>>
      : std::true_type {};

  template <bool is_mutable>
  struct LightweightViewTraits<
      mgis::arcane::SubPartialQuadratureFunctionView<is_mutable>>
      : std::true_type {};

  namespace internals {

    template <>
    struct FunctionResultTypeTraits<Arcane::ArrayView<real>> {
      static constexpr auto is_specialized = true;
    };

  }  // end of namespace internals

}  // end of namespace mgis::function

#include "MGIS/Arcane/PartialQuadratureFunction.ixx"

#endif /* LIB_MGIS_ARCANE_PARTIALQUADRATUREFUNCTION_HXX */

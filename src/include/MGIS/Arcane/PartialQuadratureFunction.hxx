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

namespace mgis::function::internals {

  inline void assign_value(Arcane::ArrayView<real> lhs, const real& rhs) {
    lhs[0] = rhs;
  }

  inline void assign_value(Arcane::ArrayView<real> lhs, const auto& rhs) {
    std::copy(rhs.begin(), rhs.end(), lhs.begin());
  }

}  // end of namespace mgis::function::internals

#include "MGIS/Function/Function.hxx"
#include "MGIS/Arcane/PartialQuadratureSpace.hxx"

namespace mgis::function::internals{

  template <>
  struct FunctionResultTypeTraits<Arcane::ArrayView<real>> {
    static constexpr auto is_specialized = true;
  };

  template <>
  struct FunctionResultTypeTraits<Arcane::ConstArrayView<real>> {
    static constexpr auto is_specialized = true;
  };

} // end of mgis::function::internals

namespace Arcane {

  template <typename DataType>
  inline void assign_value(ArrayView<DataType> lhs,
                           const ArrayView<DataType>& rhs) {
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

  template <bool is_mutable = true>
  struct PartialQuadratureFunctionView {
    //
    using DataType = std::conditional_t<is_mutable,
                                        Arcane::Array2View<real>,
                                        Arcane::ConstArray2View<real>>;
    //
    PartialQuadratureFunctionView(const PartialQuadratureSpace&,
                                  DataType);
    //
    PartialQuadratureFunctionView(PartialQuadratureFunctionView&&) noexcept = default;
    PartialQuadratureFunctionView(
        const PartialQuadratureFunctionView&) noexcept = default;
    /*!
     * \return the underlying space
     *
     * \note this method shall not be used on the device
     */
    [[nodiscard]] const PartialQuadratureSpace& getSpace() const noexcept;
    //
    [[nodiscard]] Arcane::ArrayView<real> operator()(const Arcane::Integer) noexcept
        requires(is_mutable);
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
  };  // end of struct PartialQuadratureFunctionView

    //
  template <bool is_mutable>
  [[nodiscard]] constexpr bool check(
      mgis::AbstractErrorHandler&,
      const PartialQuadratureFunctionView<is_mutable>&) noexcept;

  template <bool is_mutable>
  [[nodiscard]] const PartialQuadratureSpace& getSpace(
      const PartialQuadratureFunctionView<is_mutable>&) noexcept;

  template <bool is_mutable>
  [[nodiscard]] mgis::size_type getNumberOfComponents(
      const PartialQuadratureFunctionView<is_mutable>&) noexcept;

} // end of namespace mgis::arcane

namespace mgis::function{

  template <bool is_mutable>
  struct LightweightViewTraits<
      mgis::arcane::PartialQuadratureFunctionView<is_mutable>>
      : std::true_type {};

} // end of namespace mgis::function

namespace mgis::arcane{

  struct PartialQuadratureFunction : PartialQuadratureFunctionBase,
                                     PartialQuadratureFunctionView<true> {
    /*!
     *
     */
    PartialQuadratureFunction(std::shared_ptr<PartialQuadratureSpace>,
                              const Arcane::String&,
                              const Arcane::Integer);
    //
    [[nodiscard]] auto view() noexcept;
    [[nodiscard]] auto view() const noexcept;
    //
    using PartialQuadratureFunctionView<true>::getSpace;
  };  // end of struct PartialQuadratureFunction

  [[nodiscard]] PartialQuadratureFunctionView<true> view(
      PartialQuadratureFunction&) noexcept;

  [[nodiscard]] PartialQuadratureFunctionView<false> view(
      const PartialQuadratureFunction&) noexcept;

} // end of namespace mgis::arcane

#include "MGIS/Arcane/PartialQuadratureFunction.ixx"

#endif /* LIB_MGIS_ARCANE_PARTIALQUADRATUREFUNCTION_HXX */

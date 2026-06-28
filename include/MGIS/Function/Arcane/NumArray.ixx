/*!
 * \file   MGIS/Function/Arcane/NumArray.ixx
 * \brief
 * \author Thomas Helfer
 * \date   27/06/2026
 * \copyright (C) Copyright Thomas Helfer 2018.
 * Use, modification and distribution are subject
 * to one of the following licences:
 * - GNU Lesser General Public License (LGPL), Version 3.0. (See accompanying
 *   file LGPL-3.0.txt)
 * - CECILL-C,  Version 1.0 (See accompanying files
 *   CeCILL-C_V1-en.txt and CeCILL-C_V1-fr.txt).
 */

#ifndef LIB_MGIS_FUNCTION_ARCANE_NUMARRAY_IXX
#define LIB_MGIS_FUNCTION_ARCANE_NUMARRAY_IXX

#include "MGIS/Function/BasicLinearSpace.hxx"
#include "MGIS/Function/Function.hxx"
#include "MGIS/Function/StridedCoalescedMemoryAccessFunctionViewBase.hxx"
#include "MGIS/Function/TFEL/Tensors.hxx"

namespace mgis::function::internals {

  template <mgis::size_type N, typename RightLayoutType>
  auto right_layout_view_impl(
      Arcane::NumArray<mgis::real, Arcane::MDDim2, RightLayoutType> &a) noexcept
      requires(std::is_base_of_v<Arcane::RightLayout, RightLayoutType>) {
    const auto e0 = static_cast<size_type>(a.extent0());
    const auto e1 = static_cast<size_type>(a.extent1());
    auto space = BasicLinearSpace(a.extent0());
    auto data = std::span<mgis::real>(a.to1DSpan().data(), e0 * e1);
    if constexpr (N == mgis::dynamic_extent) {
      return FunctionView<BasicLinearSpace, FunctionDataLayoutDescription{}, true>{
          space, data, e1};
    } else {
      if (e1 != N) {
        ContractViolationHandler ctx;
        ctx.registerErrorMessage(
            "right_layout_view_impl: invalid number of components");
      }
      return FunctionView<BasicLinearSpace,
                          FunctionDataLayoutDescription{.data_size = N, .data_stride = N},
                          true>{space, data};
    }
  }  // end of view

  template <mgis::size_type N, typename RightLayoutType>
  auto right_layout_view_impl(
      const Arcane::NumArray<mgis::real, Arcane::MDDim2, RightLayoutType>
          &a) noexcept
      requires(std::is_base_of_v<Arcane::RightLayout, RightLayoutType>) {
    const auto e0 = static_cast<size_type>(a.extent0());
    const auto e1 = static_cast<size_type>(a.extent1());
    auto space = BasicLinearSpace(e0);
    auto data = std::span<const mgis::real>(a.to1DSpan().data(), e0 * e1);
    if constexpr (N == mgis::dynamic_extent) {
      return FunctionView<BasicLinearSpace, FunctionDataLayoutDescription{}, false>{
          space, data, e1};
    } else {
      if (e1 != N) {
        ContractViolationHandler ctx;
        ctx.registerErrorMessage(
            "right_layout_view_impl: invalid number of components");
      }
      return FunctionView<BasicLinearSpace,
                          FunctionDataLayoutDescription{.data_size = N, .data_stride = N},
                          false>{space, data};
    }
  }  // end of view

  template <mgis::size_type N, typename LeftLayoutType>
  auto left_layout_view_impl(
      Arcane::NumArray<mgis::real, Arcane::MDDim2, LeftLayoutType> &a) noexcept
      requires((std::is_base_of_v<Arcane::LeftLayout, LeftLayoutType>)&&  //
               (N != mgis::dynamic_extent)) {
    const auto e0 = static_cast<size_type>(a.extent0());
    const auto e1 = static_cast<size_type>(a.extent1());
    auto space = BasicLinearSpace(e0);
    auto data = std::span<mgis::real>(a.to1DSpan().data(), e0 * e1);
    if (e1 != N) {
      ContractViolationHandler ctx;
      ctx.registerErrorMessage(
          "right_layout_view_impl: invalid number of components");
    }
    return StridedCoalescedMemoryAccessTensorView<
        BasicLinearSpace, tfel::math::fsarray<N, mgis::real>, true>{space,
                                                                    data};
  }  // end of view

  template <mgis::size_type N, typename LeftLayoutType>
  auto left_layout_view_impl(
      const Arcane::NumArray<mgis::real, Arcane::MDDim2, LeftLayoutType>
          &a) noexcept
      requires((std::is_base_of_v<Arcane::LeftLayout, LeftLayoutType>)&&  //
               (N != mgis::dynamic_extent)) {
    const auto e0 = static_cast<size_type>(a.extent0());
    const auto e1 = static_cast<size_type>(a.extent1());
    auto space = BasicLinearSpace(e0);
    auto data = std::span<const mgis::real>(a.to1DSpan().data(), e0 * e1);
    if (e1 != N) {
      ContractViolationHandler ctx;
      ctx.registerErrorMessage(
          "right_layout_view_impl: invalid number of components");
    }
    return StridedCoalescedMemoryAccessTensorView<
        BasicLinearSpace, tfel::math::fsarray<N, mgis::real>, false>{space,
                                                                     data};
  }  // end of view

}  // end of namespace mgis::function::internals

namespace mgis::function {

  template <typename LayoutPolicy>
  auto view(
      Arcane::NumArray<mgis::real, Arcane::MDDim1, LayoutPolicy> &a) noexcept {
    auto space = BasicLinearSpace(a.extent0());
    return FunctionView<BasicLinearSpace, FunctionDataLayoutDescription{1, 1},
                        true>{
        space, std::span<mgis::real>(a._internalData(), a.extent0())};
  }  // end of view

  template <typename LayoutPolicy>
  auto const_view(
      Arcane::NumArray<mgis::real, Arcane::MDDim1, LayoutPolicy> &a) noexcept {
    auto space = BasicLinearSpace(a.extent0());
    return FunctionView<BasicLinearSpace, FunctionDataLayoutDescription{1, 1},
                        false>{
        space, std::span<mgis::real>(a._internalData(), a.extent0())};
  }  // end of view

  template <typename LayoutPolicy>
  auto view(const Arcane::NumArray<mgis::real, Arcane::MDDim1, LayoutPolicy>
                &a) noexcept {
    auto space = BasicLinearSpace(a.extent0());
    return FunctionView<BasicLinearSpace, FunctionDataLayoutDescription{1, 1},
                        false>{
        space, std::span<mgis::real>(a._internalData(), a.extent0())};
  }  // end of view

  template <mgis::size_type N>
  auto view(Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::RightLayout>
                &a) noexcept {
    return internals::right_layout_view_impl<N>(a);
  }  // end of view

  template <mgis::size_type N>
  auto const_view(
      Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::RightLayout>
          &a) noexcept {
    return internals::right_layout_view_impl<N>(
        static_cast<const Arcane::NumArray<mgis::real, Arcane::MDDim2,
                                           Arcane::RightLayout> &>(a));
  }  // end of view

  template <mgis::size_type N>
  auto view(
      const Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::RightLayout>
          &a) noexcept {
    return internals::right_layout_view_impl<N>(a);
  }  // end of view

  template <mgis::size_type N>
  auto view(Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::LeftLayout>
                &a) noexcept {
    return internals::left_layout_view_impl<N>(a);
  }  // end of view

  template <mgis::size_type N>
  auto const_view(
      Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::LeftLayout>
          &a) noexcept requires(N != mgis::dynamic_extent) {
    return internals::left_layout_view_impl<N>(
        static_cast<const Arcane::NumArray<mgis::real, Arcane::MDDim2,
                                           Arcane::LeftLayout> &>(a));
  }  // end of view

  template <mgis::size_type N>
  auto view(
      const Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::LeftLayout>
          &a) noexcept requires(N != mgis::dynamic_extent) {
    return internals::left_layout_view_impl<N>(a);
  }  // end of view

  template <mgis::size_type N>
  auto view(Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::DefaultLayout>
                &a) noexcept {
    static_assert(
        std::is_base_of_v<Arcane::RightLayout, Arcane::DefaultLayout>);
    return internals::right_layout_view_impl<N>(a);
  }  // end of view

  template <mgis::size_type N>
  auto const_view(
      Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::DefaultLayout>
          &a) noexcept {
    static_assert(
        std::is_base_of_v<Arcane::RightLayout, Arcane::DefaultLayout>);
    return internals::right_layout_view_impl<N>(
        static_cast<const Arcane::NumArray<mgis::real, Arcane::MDDim2,
                                           Arcane::DefaultLayout> &>(a));
  }  // end of view

  template <mgis::size_type N>
  auto view(
      const Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::DefaultLayout>
          &a) noexcept {
    static_assert(
        std::is_base_of_v<Arcane::RightLayout, Arcane::DefaultLayout>);
    return internals::right_layout_view_impl<N>(a);
  }  // end of view

}  // end of namespace mgis::function

#endif /* LIB_MGIS_FUNCTION_ARCANE_NUMARRAY_IXX */

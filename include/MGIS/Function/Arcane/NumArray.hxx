/*!
 * \file   MGIS/Function/Arcane/NumArray.hxx
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

#ifndef LIB_MGIS_FUNCTION_ARCANE_NUMARRAY_HXX
#define LIB_MGIS_FUNCTION_ARCANE_NUMARRAY_HXX

#include "arccore/common/NumArray.h"
#include "MGIS/Config.hxx"

namespace mgis::function {

  template <typename LayoutPolicy>
  auto view(
      Arcane::NumArray<mgis::real, Arcane::MDDim1, LayoutPolicy> &) noexcept;

  template <typename LayoutPolicy>
  auto const_view(
      Arcane::NumArray<mgis::real, Arcane::MDDim1, LayoutPolicy> &) noexcept;

  template <typename LayoutPolicy>
  auto view(const Arcane::NumArray<mgis::real, Arcane::MDDim1, LayoutPolicy>
                &) noexcept;

  template <mgis::size_type N = mgis::dynamic_extent>
  [[nodiscard]] auto view(
      Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::RightLayout>
          &) noexcept;

  template <mgis::size_type N = mgis::dynamic_extent>
  [[nodiscard]] auto const_view(
      Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::RightLayout>
          &) noexcept;

  template <mgis::size_type N = mgis::dynamic_extent>
  [[nodiscard]] auto view(
      const Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::RightLayout>
          &) noexcept;

  template <mgis::size_type N = mgis::dynamic_extent>
  [[nodiscard]] auto view(
      Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::LeftLayout>
          &) noexcept;

  template <mgis::size_type N>
  [[nodiscard]] auto const_view(
      Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::LeftLayout>
          &) noexcept requires(N != mgis::dynamic_extent);

  template <mgis::size_type N>
  [[nodiscard]] auto view(
      const Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::LeftLayout>
          &) noexcept requires(N != mgis::dynamic_extent);

  template <mgis::size_type N = mgis::dynamic_extent>
  [[nodiscard]] auto view(
      Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::DefaultLayout>
          &) noexcept;

  template <mgis::size_type N = mgis::dynamic_extent>
  [[nodiscard]] auto const_view(
      Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::DefaultLayout>
          &) noexcept;

  template <mgis::size_type N = mgis::dynamic_extent>
  [[nodiscard]] auto view(
      const Arcane::NumArray<mgis::real, Arcane::MDDim2, Arcane::DefaultLayout>
          &) noexcept;

}  // end of namespace mgis::function

#include "MGIS/Function/Arcane/NumArray.ixx"

#endif /* LIB_MGIS_FUNCTION_ARCANE_NUMARRAY_HXX */

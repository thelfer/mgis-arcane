/*!
 * \file   MGIS/Arcane/PartialQuadratureSpace.hxx
 * \brief
 * \author Thomas Helfer
 * \date   07/08/2026
 */

#ifndef LIB_MGIS_ARCANE_PARTIALQUADRATURESPACE_HXX
#define LIB_MGIS_ARCANE_PARTIALQUADRATURESPACE_HXX

#include "arccore/trace/TraceAccessor.h"
#include "arcane/core/IDoFFamily.h"
#include "arcane/core/IIndexedIncrementalItemConnectivity.h"
#include "arcane/core/IndexedItemConnectivityView.h"

#include "MGIS/Config.hxx"
#include "MGIS/Function/SpaceConcept.hxx"

namespace mgis::arcane {

  //! \brief a view on a partial partial quadrature
  struct PartialQuadratureSpaceView {
    [[nodiscard]] const Arcane::ItemVectorView &getCellGroupItems()
        const noexcept;
    [[nodiscard]] const Arcane::ItemVectorView &getDoFFamilyItems() const;
    [[nodiscard]] Arcane::IndexedCellDoFConnectivityView
    getCellDoFConnectivity() const;

   protected:
    Arcane::ItemVectorView dof_family_view;
    Arcane::ItemVectorView cells_view;
    Arcane::IndexedCellDoFConnectivityView cell_dof_connectivity_view;
  };

  /*!
   * \brief a partial quadrature space based on `Arcane::IDoFFamily`
   */
  struct PartialQuadratureSpace : public Arcane::TraceAccessor,
                                  public PartialQuadratureSpaceView {
    explicit PartialQuadratureSpace(Arcane::ITraceMng &,
                                    const Arcane::CellGroup &,
                                    const Arcane::String &);
    //
    [[nodiscard]] const Arcane::CellGroup &getCellGroup() const noexcept;
    //
    [[nodiscard]] const Arcane::IItemFamily &getDoFFamily() const;
    //
    [[nodiscard]] Arcane::IItemFamily &getMutableDoFFamilyReference();

   private:
    const Arcane::CellGroup &cells;
    Arcane::Ref<Arcane::IIndexedIncrementalItemConnectivity>
        cell_dof_connectivity;
    Arcane::IItemFamily *dof_family = nullptr;
  };

  [[nodiscard]] Arcane::Integer getSpaceSize(
      const PartialQuadratureSpaceView &) noexcept;

  [[nodiscard]] Arcane::Integer getNumberOfCells(
      const PartialQuadratureSpaceView &) noexcept;

  [[nodiscard]] Arcane::Integer getNumberOfQuadraturePoints(
      const PartialQuadratureSpaceView, const size_type) noexcept;

  [[nodiscard]] Arcane::Integer getQuadraturePointOffset(
      const PartialQuadratureSpaceView &,
      const Arcane::Integer,
      const Arcane::Integer) noexcept;

  [[nodiscard]] bool areEquivalent(const PartialQuadratureSpace&,
                                   const PartialQuadratureSpace&) noexcept;

}  // namespace mgis::arcane

namespace mgis::function {

  template <>
  struct SpaceTraits<::mgis::arcane::PartialQuadratureSpace> {
    // integration points are indexed from 0 to the space size
    static constexpr auto linear_element_indexing = true;
    // integration points are indexed from 0 to the number of cells
    static constexpr auto linear_cell_indexing = true;
    using size_type = Arcane::Integer;
    using element_index_type = Arcane::Integer;
    using cell_index_type = Arcane::Integer;
    using quadrature_point_index_type = Arcane::Integer;
  };

}  // end of namespace mgis::function

#include "MGIS/Arcane/PartialQuadratureSpace.ixx"

#endif /* LIB_MGIS_ARCANE_PARTIALQUADRATURESPACE_HXX */

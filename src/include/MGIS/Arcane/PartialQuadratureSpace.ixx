/*!
 * \file   MGIS/Arcane/PartialQuadratureSpace.ixx
 * \brief
 * \author Thomas Helfer
 * \date   07/08/2026
 */

#ifndef LIB_MGIS_ARCANE_PARTIALQUADRATURESPACE_IXX
#define LIB_MGIS_ARCANE_PARTIALQUADRATURESPACE_IXX

#include "arcane/core/ItemGroup.h"

namespace mgis::arcane {

  inline const Arcane::ItemVectorView &
  PartialQuadratureSpaceView::getCellGroupItems() const noexcept {
    return this->cells_view;
  }  // end of getCellGroupItems

  inline const Arcane::ItemVectorView &
  PartialQuadratureSpaceView::getDoFFamilyItems() const {
    return this->dof_family_view;
  }  // end of getDoFFamilyItems

  inline Arcane::IndexedCellDoFConnectivityView
  PartialQuadratureSpaceView::getCellDoFConnectivity() const {
    return this->cell_dof_connectivity_view;
    ;
  }  // end of getCellDoFConnectivity

  inline const Arcane::IItemFamily &
  PartialQuadratureSpace::getDoFFamilyInterface() const {
    return *(this->dof_family_interface);
  }  // end of getDoFFamilyInterface

  inline Arcane::IItemFamily &
  PartialQuadratureSpace::getMutableDoFFamilyInterfaceReference() {
    return *(this->dof_family_interface);
  }  // end of getMutableDoFFamilyInterfaceReference

  inline const Arcane::IDoFFamily &PartialQuadratureSpace::getDoFFamily()
      const {
    return *(this->dof_family);
  }  // end of getDoFFamily

  inline Arcane::IDoFFamily &
  PartialQuadratureSpace::getMutableDoFFamilyReference() {
    return *(this->dof_family);
  }  // end of getMutableDoFFamilyReference

  inline const Arcane::CellGroup &PartialQuadratureSpace::getCellGroup()
      const noexcept {
    return this->cells;
  }

  inline Arcane::Integer getSpaceSize(
      const PartialQuadratureSpaceView &s) noexcept {
    return s.getDoFFamilyItems().size();
  }  // end of getSpaceSize

  inline Arcane::Integer getNumberOfCells(
      const PartialQuadratureSpaceView &s) noexcept {
    return s.getCellDoFConnectivity().nbSourceItem();
  }  // end of getNumberOfCells

  inline Arcane::Integer getNumberOfQuadraturePoints(
      const PartialQuadratureSpace s, const Arcane::Integer e) noexcept {
    const auto id = s.getCellGroupItems()[e];
    return s.getCellDoFConnectivity().nbItem(id);
  }  // end of getNumberOfQuadraturePoints

  inline Arcane::Integer getQuadraturePointOffset(
      const PartialQuadratureSpaceView &s,
      const Arcane::Integer e,
      const Arcane::Integer q) noexcept {
    const auto id =
        Arcane::ItemLocalIdT<Arcane::Cell>(s.getCellGroupItems()[e].localId());
    const auto &c = s.getCellDoFConnectivity();
    return c.items(id)[q];
  }  // end of getQuadraturePointOffset

  inline bool areEquivalent(const PartialQuadratureSpace &s1,
                            const PartialQuadratureSpace &s2) noexcept {
    return (&(s1.getDoFFamily())) == (&(s2.getDoFFamily()));
  }  // end of areEquivalent

}  // namespace mgis::arcane

#endif /* LIB_MGIS_ARCANE_PARTIALQUADRATURESPACE_IXX */

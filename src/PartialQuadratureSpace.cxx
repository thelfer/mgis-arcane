/*!
 * \file   PartialQuadratureSpace.cxx
 * \brief
 * \author Thomas Helfer
 * \date   07/08/2026
 */

#include "arcane/core/IMesh.h"
#include "arcane/core/IParallelMng.h"
#include "arcane/core/ItemGroup.h"
#include "arcane/core/IItemConnectivityInfo.h"
#include "arcane/core/IIncrementalItemConnectivity.h"
#include "arcane/core/IIndexedIncrementalItemConnectivityMng.h"
#include "MGIS/Arcane/PartialQuadratureSpace.hxx"

namespace mgis::arcane {

  static_assert(
      mgis::function::LinearElementSpaceConcept<PartialQuadratureSpace>,
      "PartialQuadratureSpace does not statisfy the "
      "LinearQuadratureSpaceConcept");

  static_assert(
      mgis::function::LinearQuadratureSpaceConcept<PartialQuadratureSpace>,
      "PartialQuadratureSpace does not statisfy the "
      "LinearQuadratureSpaceConcept");

  PartialQuadratureSpace::PartialQuadratureSpace(
      Arcane::ITraceMng &tm,
      const Arcane::CellGroup &c,
      const Arcane::String &name)
      : TraceAccessor(&tm), cells(c) {
    using namespace Arcane;
    IMesh *mesh = this->cells.mesh();
    IItemFamily *cell_family = this->cells.itemFamily();
    IItemFamily *dof_family_interface =
        mesh->findItemFamily(Arcane::IK_DoF, name, true);
    IDoFFamily *dof_family =
        ARCANE_CHECK_POINTER(dof_family_interface->toDoFFamily());
    this->dof_family = dof_family_interface;

    // Get the maximum number of nodes of cells across all sub-domains.
    // This will be used to compute the uniqueId of the DoF and make sure they
    // are always the same.
    Int32 max_nb_dof_per_cell =
        cell_family->globalConnectivityInfos()->maxNodePerItem();
    info() << "Create DoF family '" << name
           << "' for cells of group '" << this->cells.name() << "'";
    info() << "MAX_NB_DOF_PER_CELL=" << max_nb_dof_per_cell;

    // Create the DoFs
    UniqueArray<Int64> uids(this->cells.size() * max_nb_dof_per_cell);
    {
      Integer dof_index = 0;
      // Use a mask to make sure the uniqueId() of the dof
      // can not be negative if we multiply the uniqueId().
      const UInt64 uid_mask = (1 << 28) - 1;
      ENUMERATE_(Cell, icell, this->cells) {
        Cell cell = *icell;
        Int32 nb_dof_per_cell = cell.nbNode();
        Int64 cell_unique_id = cell.uniqueId().asInt64();
        for (Integer i = 0; i < nb_dof_per_cell; ++i) {
          uids[dof_index] =
              (cell_unique_id & uid_mask) * max_nb_dof_per_cell + i;
          ++dof_index;
        }
      }
      uids.resize(dof_index);
    }

    Int32UniqueArray dof_lids(uids.size());
    dof_family->addDoFs(uids, dof_lids);
    dof_family->endUpdate();
    info() << "NB_DOF=" << dof_family->allItems().size();

    // Create Cell -> DoF connectivity.
    this->cell_dof_connectivity =
        mesh->indexedConnectivityMng()->findOrCreateConnectivity(
            mesh->cellFamily(), this->dof_family, name + "DoFCell");
    auto *cn = this->cell_dof_connectivity->connectivity();
    {
      Integer dof_index = 0;
      ENUMERATE_(Cell, icell, this->cells) {
        Cell cell = *icell;
        Int32 nb_dof_per_cell = cell.nbNode();
        for (Integer i = 0; i < nb_dof_per_cell; ++i) {
          cn->addConnectedItem(cell, DoFLocalId(dof_lids[dof_index]));
          ++dof_index;
        }
      }
    }
    info() << "End build Dofs";

    IndexedCellDoFConnectivityView cell_dof(this->cell_dof_connectivity->view());
    {
      // Set the owners of the DoF.
      // It is only used when using message passing (i.e MPI)
      IParallelMng *pm = mesh->parallelMng();
      Int32 my_rank = pm->commRank();
      DoFInfoListView dofs_view(this->dof_family);
      ENUMERATE_(Cell, icell, mesh->allCells()) {
        Cell cell = *icell;
        Int32 cell_owner = cell.owner();
        for (DoFLocalId dof : cell_dof.dofs(cell)) {
          dofs_view[dof].mutableItemBase().setOwner(cell_owner, my_rank);
        }
      }
      this->dof_family->notifyItemsOwnerChanged();
      this->dof_family->computeSynchronizeInfos();
    }
    // creating views
    this->cells_view = this->cells.view();
    this->dof_family_view = this->dof_family->view();
    this->cell_dof_connectivity_view = this->cell_dof_connectivity->view();
  }  // end of PartialQuadratureSpace

}  // end of namespace mgis::arcane

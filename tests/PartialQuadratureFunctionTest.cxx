/*!
 * \file   MGIS/Arcane/PartialQuadratureSpace.hxx
 * \brief
 * \author Thomas Helfer, Gilles Grospellier
 * \date   13/08/2026
 */

#include <cstdlib>
#include "arcane/launcher/ArcaneLauncher.h"
#include "arcane/core/IDoFFamily.h"
#include "arcane/core/IIncrementalItemConnectivity.h"
#include "arcane/core/IIndexedIncrementalItemConnectivity.h"
#include "arcane/core/IIndexedIncrementalItemConnectivityMng.h"
#include "arcane/core/IItemConnectivityInfo.h"
#include "arcane/core/IMesh.h"
#include "arcane/core/IParallelMng.h"
#include "arcane/core/ISubDomain.h"
#include "arcane/core/IndexedItemConnectivityView.h"
#include "arcane/core/ItemGroup.h"
#include "arcane/core/MeshReaderMng.h"
#include "arcane/core/VariableTypes.h"
#include "arcane/utils/Exception.h"
#include "arcane/utils/FatalErrorException.h"
#include "arcane/utils/ITraceMng.h"
#include "arcane/utils/Real3.h"
#include "TFEL/Tests/TestCase.hxx"
#include "TFEL/Tests/TestProxy.hxx"
#include "TFEL/Tests/TestManager.hxx"
#include "MGIS/Function/TFEL/Tensors.hxx"
#include "MGIS/Arcane/PartialQuadratureFunction.hxx"

struct PartialQuadratureFunctionTest final : public tfel::tests::TestCase {
  PartialQuadratureFunctionTest()
      : tfel::tests::TestCase("MGIS/Arcane", "PartialQuadratureFunctionTest") {
  }  // end of PartialQuadratureFunctionTest
  tfel::tests::TestResult execute() override {
    this->initialize();
    this->test1();
    this->test2();
    return this->result;
  }  // end of execute
 private:
  void initialize() {
    using namespace Arcane;
    this->launcher =
        StandaloneSubDomain(ArcaneLauncher::createStandaloneSubDomain(
            "PartialQuadratureFunctionTest.arc"));
    auto *sd = this->launcher.subDomain();
    auto *mesh = sd->defaultMesh();
    auto *cell_family = mesh->cellFamily();
    const auto nb_cell = mesh->nbCell();
    // We suppose the mesh is a bar with Y varying from 0.0 to 1.0.
    // The cells from 0.0 to 0.5 are triangles and cells from 0.5 to 1
    // are quadrangles. The cells from 0.0 to 0.65 will be in group Mat1 and
    // the remaining cells will be in group Mat2.
    VariableNodeReal3 &nodes_coordinates = mesh->nodesCoordinates();
    UniqueArray<Int32> mat1_cells_id;
    UniqueArray<Int32> mat2_cells_id;
    ENUMERATE_(Cell, icell, cell_family->allItems()) {
      Cell cell = *icell;
      Real3 cell_center;
      for (Node node : cell.nodes()) {
        cell_center += nodes_coordinates[node];
      }
      cell_center /= cell.nbNode();
      if (cell_center.x < 0.65) {
        mat1_cells_id.add(cell.localId());
      } else {
        mat2_cells_id.add(cell.localId());
      }
    }
    // create the groups
    this->mat1 = cell_family->createGroup("Mat1", mat1_cells_id);
    this->mat2 = cell_family->createGroup("Mat2", mat2_cells_id);
  }  // end of initialize

  // manipulation of scalar fields
  // using standard MGIS/Function's algorithms (CPU version only)
  void test1() {
    using namespace mgis::function;
    using namespace mgis::arcane;
    auto ctx = ::mgis::Context{};
    // Get the trace class to display messages
    auto &tm = *(this->launcher.traceMng());
    // creating the partial quadrature space
    auto space = std::make_shared<PartialQuadratureSpace>(
        tm, this->mat1, "Mat1StandardQuadrature");
    // creating partial quadrature functions
    auto f = PartialQuadratureFunction(space, "f", 1);
    auto f2 = PartialQuadratureFunction(space, "f2", 1);
    TFEL_TESTS_ASSERT(getNumberOfComponents(f) == 1);
    TFEL_TESTS_ASSERT(getNumberOfComponents(f2) == 1);
    // value access through Arcane::ArrayView
    for (Arcane::Integer i = 0; i != getSpaceSize(f2.getSpace()); ++i) {
      f2(i)[0] = 2;
    }
    // creating mutable scalar view
    auto v = ::mgis::arcane::view<1>(f);
    // creating immutable scalar view matching MGIS/Function EvaluatorConcept
    auto v2 = ::mgis::arcane::view<1>(
        static_cast<const PartialQuadratureFunction &>(f2));
    TFEL_TESTS_ASSERT(getNumberOfComponents(v) == 1);
    TFEL_TESTS_ASSERT(getNumberOfComponents(v2) == 1);
    //
    const auto ok = assign(ctx, v, v2 | multiply_by_scalar(3));
    TFEL_TESTS_ASSERT(ok);
    if (!ok) {
      return;
    }
    auto eps = Arcane::Real{};
    for (Arcane::Integer i = 0; i != getSpaceSize(v.getSpace()); ++i) {
      eps = std::max(std::abs(v(i) - 6), eps);
    }
    TFEL_TESTS_ASSERT(eps < Arcane::Real{1e-14});
  }
  //
  void test2() {
    using namespace mgis::function;
    using namespace mgis::arcane;
    auto ctx = ::mgis::Context{};
    // Get the trace class to display messages
    auto &tm = *(this->launcher.traceMng());
    // creating the partial quadrature space
    auto space = std::make_shared<PartialQuadratureSpace>(
        tm, this->mat1, "Mat1StandardQuadrature");
    auto f = PartialQuadratureFunction(space, "f", 6);
    // creating mutable tensor view
    auto v = f | as_stensor<3>;
    TFEL_TESTS_ASSERT(v.check(ctx));
    for (Arcane::Integer i = 0; i != getSpaceSize(v.getSpace()); ++i) {
      v(i) = tfel::math::stensor<3>::Id();
    }
    //
    const auto &vdof = static_cast<const Arcane::VariableDoFArrayReal &>(f);
    auto eps = Arcane::Real{};
    ENUMERATE_(Arcane::DoF, idof, space->getDoFFamily().allItems()) {
      const auto dof = *idof;
      const auto dof_value = vdof[dof];
      auto leps = Arcane::Real{};
      for (Arcane::Integer i = 0; i != 6; ++i) {
        if (i < 3) {
          leps += std::abs(dof_value[i] - 1);
        } else {
          leps += std::abs(dof_value[i]);
        }
      }
      eps = std::max(leps, eps);
    }
    TFEL_TESTS_ASSERT(eps < Arcane::Real{1e-14});
  }  // end of test2

  //
  Arcane::StandaloneSubDomain launcher;
  Arcane::CellGroup mat1;
  Arcane::CellGroup mat2;
};

TFEL_TESTS_GENERATE_PROXY(PartialQuadratureFunctionTest,
                          "PartialQuadratureFunctionTest");

/* coverity [UNCAUGHT_EXCEPT]*/
int main(int argc, char *argv[]) {
  Arcane::CommandLineArguments cmd_line_args(&argc, &argv);
  Arcane::ArcaneLauncher::init(cmd_line_args);
  //
  auto &m = tfel::tests::TestManager::getTestManager();
  m.addTestOutput(std::cout);
  m.addXMLTestOutput("PartialQuadratureFunctionTest.xml");
  return m.execute().success() ? EXIT_SUCCESS : EXIT_FAILURE;
}

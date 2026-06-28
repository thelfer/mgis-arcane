#include <cstdlib>
#include <vector>
#include <iostream>

#include "arccore/common/ExceptionUtils.h"
#include "arccore/common/accelerator/Memory.h"
#include "arccore/common/accelerator/RunQueue.h"
#include "arccore/common/accelerator/Runner.h"
#include "arccore/accelerator/AcceleratorInitializer.h"
#include "MGIS/Function/Arcane/NumArray.hxx"
#include "MGIS/Function/Arcane/Algorithms.hxx"

void f() {
  auto a = Arcane::NumArray<double, Arcane::MDDim2>(12, 6);
  std::cout << "right layout\n";
  std::cout << &(a(0, 0)) << "\n"
            << &(a(0, 1)) << '\n'
            << &(a(0, 0)) + 1 << "\n"
            << a.to1DSpan().data() + 1 << " \n"
            << &(a(0, 2)) << '\n'
            << &(a(0, 0)) + 2 << "\n"
            << a.to1DSpan().data() + 2 << " \n"
            << &(a(1, 0)) << '\n'
            << &(a(0, 0)) + 6 << "\n"
            << a.to1DSpan().data() + 6 << " \n";
  std::cout << "left layout\n";
  auto b = Arcane::NumArray<double, Arcane::MDDim2, Arcane::LeftLayout>(12, 16);
  std::cout << &(b(0, 0)) << "\n"
            << &(b(0, 1)) << '\n'
            << &(b(0, 0)) + 12 << "\n"
            << b.to1DSpan().data() + 12 << "\n"
            << &(b(0, 2)) << '\n'
            << &(b(0, 0)) + 24 << "\n"
            << b.to1DSpan().data() + 24 << "\n"
            << &(b(1, 0)) << '\n'
            << &(b(0, 0)) + 1 << "\n"
            << b.to1DSpan().data() + 1 << "\n";
}  // end of f

void f2() {
  using namespace mgis;
  using namespace mgis::function;
  auto a = Arcane::NumArray<real, Arcane::MDDim1>(12);
  a.fill(13);
  auto b = Arcane::NumArray<real, Arcane::MDDim1>(12);
  auto bv = view(b);
  const auto ok = view(a) | multiply_by_scalar(2) | bv;
  for (const auto &v : b.to1DSpan()) {
    std::cout << v << " ";
  }
  std::cout << '\n';
}  // end of f2

void f3() {
  using namespace mgis;
  using namespace mgis::function;
  //
  auto a = Arcane::NumArray<real, Arcane::MDDim2>(12, 6);
  auto av = view<6>(a);
  auto a_t = av | as_stensor<3>;
  for (size_type i = 0; i != 12; ++i) {
    a_t(i) = i * tfel::math::stensor<3u, real>::Id();
  }
  //
  auto b = Arcane::NumArray<real, Arcane::MDDim1>(12);
  auto bv = view(b);
  const auto ok = view(a) | as_stensor<3> | multiply_by_scalar(2) | trace | bv;
  for (const auto &v : b.to1DSpan()) {
    std::cout << v << " ";
  }
  std::cout << '\n';
}

void f4() {
  using namespace mgis;
  using namespace mgis::function;
  //
  auto a = Arcane::NumArray<real, Arcane::MDDim2, Arcane::RightLayout>(12, 6);
  auto av = view<6>(a);
  for (mgis::size_type i = 0; i != 12; ++i) {
    auto value = av(i);
    for (mgis::size_type j = 0; j != 6; ++j) {
      value[j] = i + 2 * j;
    }
  }
  //
  for (Arcane::Int32 i = 0; i != 12; ++i) {
    for (Arcane::Int32 j = 0; j != 6; ++j) {
      std::cout << a(i, j) << ' ';
    }
    std::cout << '\n';
  }
}

void f5() {
  using namespace mgis;
  using namespace mgis::function;
  //
  auto a = Arcane::NumArray<real, Arcane::MDDim2, Arcane::LeftLayout>(12, 6);
  auto av = view<6>(a);
  for (mgis::size_type i = 0; i != 12; ++i) {
    auto value = av(i);
    for (mgis::size_type j = 0; j != 6; ++j) {
      value[j] = i + 2 * j;
    }
  }
  //
  for (Arcane::Int32 i = 0; i != 12; ++i) {
    for (Arcane::Int32 j = 0; j != 6; ++j) {
      std::cout << a(i, j) << ' ';
    }
    std::cout << '\n';
  }
}

void f6() {
  using namespace mgis;
  using namespace mgis::function;
  auto ctx = Context{};
  auto initializer = Arcane::Accelerator::AcceleratorInitializer{false, 64};
  auto runner = Arcane::Runner{initializer.executionPolicy()};
  auto queue = Arcane::Accelerator::makeQueue(runner);
  auto a = Arcane::NumArray<real, Arcane::MDDim1>(
      12, Arcane::eMemoryResource::UnifiedMemory);
  auto b = Arcane::NumArray<real, Arcane::MDDim1>(
      12, Arcane::eMemoryResource::UnifiedMemory);
  for (mgis::size_type i = 0; i != 12; ++i) {
    b(i) = i;
  }
  const auto op = view(b) | multiply_by_scalar(3);
  auto av = view(a);
  const auto ok = assign(ctx, queue, av, op);
  for (Arcane::Int32 i = 0; i != 12; ++i) {
    std::cout << a(i) << ' ';
  }
  std::cout << '\n';
}

int main() { return Arcane::ExceptionUtils::callWithTryCatch(f6); }

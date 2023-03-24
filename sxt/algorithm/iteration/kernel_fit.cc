#include "sxt/algorithm/iteration/kernel_fit.h"

#include "sxt/base/error/assert.h"
#include "sxt/base/num/divide_up.h"
#include "sxt/execution/kernel/kernel_dims.h"

namespace sxt::algi {
//--------------------------------------------------------------------------------------------------
// fit_iteration_kernel
//--------------------------------------------------------------------------------------------------
xenk::kernel_dims fit_iteration_kernel(unsigned int n) noexcept {
  SXT_DEBUG_ASSERT(n > 0);
  if (n <= 4) {
    return {
        .num_blocks = n,
        .block_size = xenk::block_size_t::v1,
    };
  }
  if (n <= 8) {
    return {
        .num_blocks = 2,
        .block_size = xenk::block_size_t::v4,
    };
  }
  if (n <= 16) {
    return {
        .num_blocks = 2,
        .block_size = xenk::block_size_t::v8,
    };
  }
  if (n <= 32) {
    return {
        .num_blocks = 2,
        .block_size = xenk::block_size_t::v16,
    };
  }
  if (n <= 64) {
    return {
        .num_blocks = 2,
        .block_size = xenk::block_size_t::v32,
    };
  }

  // Fix block size at 32 and increase blocks up to 32
  if (n <= 32 * 32) {
    return {
        .num_blocks = basn::divide_up(n, 32u),
        .block_size = xenk::block_size_t::v32,
    };
  }

  // Fix block size at 64 and increase blocks up to 128
  if (n <= 128 * 64) {
    return {
        .num_blocks = basn::divide_up(n, 64u),
        .block_size = xenk::block_size_t::v64,
    };
  }

  // Fix block size and use a formula to derive the other numbers
  unsigned max_blocks = 256;
  auto block_size = xenk::block_size_t::v128;
  auto num_iters = basn::divide_up(n, static_cast<unsigned>(block_size) * max_blocks) *
                   static_cast<unsigned>(block_size);
  auto num_blocks = basn::divide_up(n, num_iters);

  return {
      .num_blocks = num_blocks,
      .block_size = block_size,
  };
}
} // namespace sxt::algi

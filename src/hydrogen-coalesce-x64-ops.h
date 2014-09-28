// Copyright 2014 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#ifndef V8_HYDROGEN_COALESCE_X64_OPS_
#define V8_HYDROGEN_COALESCE_X64_OPS_

#include "src/hydrogen.h"

namespace v8 {
namespace internal {


class HCoalesceX64Operations : public HPhase {
 public:
  explicit HCoalesceX64Operations(HGraph* graph)
      : HPhase("H_Coalesce X64 operations", graph) { }

  void Run();

 private:
  void CoalesceMuls(HInstruction* instr);
};


} }  // namespace v8::internal

#endif  // V8_HYDROGEN_COALESCE_X64_OPS_

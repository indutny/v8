// Copyright 2014 the V8 project authors. All rights reserved.
// Use of this source code is governed by a BSD-style license that can be
// found in the LICENSE file.

#include "src/hydrogen-coalesce-x64-ops.h"

namespace v8 {
namespace internal {


void HCoalesceX64Operations::CoalesceMuls(HInstruction* instr) {
  if (!instr->IsMul())
    return;

  // int26 * int26 could be perfomed in 64bit field, if all of the uses do
  // support integer inputs
  HMul* mul = HMul::cast(instr);
  if (!mul->representation().IsDouble())
    return;
  if (!mul->left()->representation().IsInteger32())
    return;
  if (!mul->right()->representation().IsInteger32())
    return;

  for (HUseIterator it(mul->uses()); !it.Done(); it.Advance()) {
    HValue* use = it.value();
    if (use->IsSimulate())
      continue;
    if (use->IsBitwise()) {
      // Ok-ish
    } else if (use->IsDiv() &&
               use->CheckUsesForFlag(HValue::kTruncatingToInt32)) {
      // Ok-ish, if all div's uses are truncated and right side is a constant
      HDiv* div = HDiv::cast(use);
      if (!div->right()->IsConstant())
        return;

      // The constant should be an integer
      HConstant* c = HConstant::cast(div->right());
      if (c->HasDoubleValue()) {
        if (c->DoubleValue() != std::floor(c->DoubleValue()))
          return;
      } else {
        return;
      }
    } else  {
      // No luck
      return;
    }
  }

  mul->ChangeRepresentation(Representation::Integer32());
  mul->SetFlag(HValue::kUint64Output);

  for (HUseIterator it(mul->uses()); !it.Done(); it.Advance()) {
    HValue* use = it.value();
    if (use->IsSimulate())
      continue;
    if (use->IsDiv())
      use->ChangeRepresentation(Representation::Integer32());
  }
}


void HCoalesceX64Operations::Run() {
  const ZoneList<HBasicBlock*>* blocks(graph()->blocks());
  for (int i = 0; i < blocks->length(); ++i) {
    // Process phi instructions first.
    const HBasicBlock* block(blocks->at(i));
    const ZoneList<HPhi*>* phis = block->phis();
    for (int j = 0; j < phis->length(); j++) {
    }

    // Process normal instructions.
    for (HInstruction* current = block->first(); current != NULL; ) {
      HInstruction* next = current->next();

      CoalesceMuls(current);

      current = next;
    }
  }
}


} }  // namespace v8::internal

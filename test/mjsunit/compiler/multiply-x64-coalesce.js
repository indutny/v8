// Copyright 2014 the V8 project authors. All rights reserved.
// Redistribution and use in source and binary forms, with or without
// modification, are permitted provided that the following conditions are
// met:
//
//     * Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//     * Redistributions in binary form must reproduce the above
//       copyright notice, this list of conditions and the following
//       disclaimer in the documentation and/or other materials provided
//       with the distribution.
//     * Neither the name of Google Inc. nor the names of its
//       contributors may be used to endorse or promote products derived
//       from this software without specific prior written permission.
//
// THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
// "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
// LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
// A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
// OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
// SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
// LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
// DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
// THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
// (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
// OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

// Flags: --allow-natives-syntax --no-use-inlining

// -----------------------------------------------------------------------------

function RefMul(a, b) {
  var ai = a | 0;
  var bi = b | 0;

  var r = ai * bi;
  var hi = (r / 0x4000000) | 0;
  var lo = r & 0x3ffffff;

  return hi + '/' + lo;
}
%NeverOptimizeFunction(RefMul);

function MulX64(a, b) {
  var ai = a | 0;
  var bi = b | 0;

  var r = ai * bi;
  var hi = (r / 0x4000000) | 0;
  var lo = r & 0x3ffffff;

  return hi + '/' + lo;
}

MulX64(1, 2);
MulX64(0xffff, 0xdead);

var values = [
  [ 1, 2 ],
  [ 0xffff, 0xdead ],
  [ 0x3ffffff, 0x3ffffff ],

  // Will deoptimize
  [ -1, 2 ],
  [ 0xfffffff, 0xfffffff ]
]

values.forEach(function(pair) {
  %OptimizeFunctionOnNextCall(MulX64);
  assertEquals(RefMul(pair[0], pair[1]), MulX64(pair[0], pair[1]));
});

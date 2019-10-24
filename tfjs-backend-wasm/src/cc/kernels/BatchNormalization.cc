/* Copyright 2019 Google Inc. All Rights Reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 * ===========================================================================*/

#ifdef __EMSCRIPTEN__
#include <emscripten.h>
#endif

#include <math.h>
#include <vector>

#include "src/cc/backend.h"
#include "src/cc/util.h"

template <class T>
void batch_norm_impl(T* x_buf, int x_size, T* mean_buf, int mean_size,
                     T* variance_buf, int variance_size, T* offset_buf,
                     int offset_size, T* scale_buf, int scale_size,
                     T* variance_epsilon_buf, int variance_epsilon_size,
                     T* out_buf) {
  int size = x_size;
  for (int i = 0; i < size; ++i) {
    out_buf[i] = x_buf[i];
  }
}
// Templates need explicit instantiation when implemented in a .cc file.
template void batch_norm_impl<float>(float* x_buf, int x_size, float* mean_buf,
                                     int mean_size, float* variance_buf,
                                     int variance_size, float* offset_buf,
                                     int offset_size, float* scale_buf,
                                     int scale_size,
                                     float* variance_epsilon_buf,
                                     int variance_epsilon_size, float* out_buf);

namespace tfjs {
namespace wasm {
// We use C-style API to interface with Javascript.
extern "C" {

#ifdef __EMSCRIPTEN__
EMSCRIPTEN_KEEPALIVE
#endif
void BatchNormalization(int x_id, int mean_id, int variance_id, int offset_id,
                        int scale_id, int variance_epsilon_id, int out_id) {
  const auto x_info = backend::get_tensor_info(x_id);
  const auto mean_info = backend::get_tensor_info(mean_id);
  const auto variance_info = backend::get_tensor_info(variance_id);
  const auto offset_info = backend::get_tensor_info(offset_id);
  const auto scale_info = backend::get_tensor_info(scale_id);
  const auto variance_epsilon_info =
      backend::get_tensor_info(variance_epsilon_id);
  const auto out_info = backend::get_tensor_info(out_id);
  batch_norm_impl(x_info.buf.f32, x_info.size, mean_info.buf.f32,
                  mean_info.size, variance_info.buf.f32, variance_info.size,
                  offset_info.buf.f32, offset_info.size, scale_info.buf.f32,
                  scale_info.size, variance_epsilon_info.buf.f32,
                  variance_epsilon_info.size, out_info.buf.f32);
}

}  // namespace wasm
}  // namespace wasm
}  // namespace tfjs

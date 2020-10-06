/* Copyright 2020 The TensorFlow Authors. All Rights Reserved.

Licensed under the Apache License, Version 2.0 (the "License");
you may not use this file except in compliance with the License.
You may obtain a copy of the License at

    http://www.apache.org/licenses/LICENSE-2.0

Unless required by applicable law or agreed to in writing, software
distributed under the License is distributed on an "AS IS" BASIS,
WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
See the License for the specific language governing permissions and
limitations under the License.
==============================================================================*/

// Reference implementation of timer functions.  Platforms are not required to
// implement these timer methods, but they are required to enable profiling.

// On platforms that have a POSIX stack or C library, it can be written using
// methods from <sys/time.h> or clock() from <time.h>.

// To add an equivalent function for your own platform, create your own
// implementation file, and place it in a subfolder with named after the OS
// you're targeting. For example, see the Cortex M bare metal version in
// tensorflow/lite/micro/bluepill/micro_time.cc or the mbed one on
// tensorflow/lite/micro/mbed/micro_time.cc.

#include "tensorflow/lite/micro/micro_time.h"

#define DWT_CONTROL             (*((volatile uint32_t*)0xE0001000))
/*!< DWT Control register */
#define DWT_CYCCNTENA_BIT       (1UL<<0)
/*!< CYCCNTENA bit in DWT_CONTROL register */
#define DWT_CYCCNT              (*((volatile uint32_t*)0xE0001004))
/*!< DWT Cycle Counter register */
#define DEMCR                   (*((volatile uint32_t*)0xE000EDFC))
#define LAR                   (*((volatile uint32_t*)0xE0001FB0))
/*!< DEMCR: Debug Exception and Monitor Control Register */
#define TRCENA_BIT              (1UL<<24)
/*!< Trace enable bit in DEMCR register */

namespace tflite {

constexpr int kClocksPerSecond = 216e6;

  // Reference implementation of the ticks_per_second() function that's required
  // for a platform to support Tensorflow Lite for Microcontrollers profiling.
  // This returns 0 by default because timing is an optional feature that builds
  // without errors on platforms that do not need it.Z
  int32_t ticks_per_second() { return kClocksPerSecond; }
  // Reference implementation of the GetCurrentTimeTicks() function that's
  // required for a platform to support Tensorflow Lite for Microcontrollers
  // profiling. This returns 0 by default because timing is an optional feature
  // that builds without errors on platforms that do not need it.
  int32_t GetCurrentTimeTicks() {
      return DWT_CYCCNT;
  }

  void StartTimer() {
      DWT_CYCCNT = 0;
  }

  void InitTimer() {
    DEMCR |= TRCENA_BIT;
    LAR = 0xC5ACCE55;
    DWT_CONTROL |= DWT_CYCCNTENA_BIT;
    DWT_CYCCNT = 0;
  }

}  // namespace tflite

/**
 * Copyright (c) 2016-present, Facebook, Inc.
 * All rights reserved.
 *
 * This source code is licensed under the MIT license found in the
 * LICENSE file in the root directory of this source tree.
 */

#include <iomanip>
#include <sstream>
#include <iostream>
#include <queue>
#include <stdexcept>
#include "args.h"
#include "autotune.h"
#include "fasttext.h"

using namespace fasttext;

int main(int argc, char** argv) {
  FastText fasttext;
  fasttext.loadModel("/home/geb/PycharmProjects/auxiliary-discriminator/serving/models/model-128.bin");
  std::istringstream in("abcdefg");
  std::vector<std::pair<real, std::string>> predictions;
  int32_t k = 1;
  real threshold = 0.0;
  fasttext.predictLine(in, predictions, k, threshold);
  for (const auto& prediction : predictions) {
    std::cout << prediction.second;
    std::cout << " " << prediction.first << std::endl;
  }
  return 0;
}

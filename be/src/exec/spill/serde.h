// Copyright 2021-present StarRocks, Inc. All rights reserved.
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     https://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#pragma once

#include "column/vectorized_fwd.h"
#include "common/status.h"
#include "common/statusor.h"
#include "exec/spill/block_manager.h"
#include "util/raw_container.h"

namespace starrocks::spill {
class SpilledOptions;

enum class SerdeType {
    BY_COLUMN,
};

struct SerdeContext {
    std::string serialize_buffer;
    raw::RawString compress_buffer;
};

// Serde is used to serialize and deserialize spilled data.
class Serde {
public:
    virtual ~Serde() = default;

    // serialize chunk and append the serialized data into block
    virtual Status serialize(SerdeContext& ctx, const ChunkPtr& chunk, BlockPtr block) = 0;
    // deserialize data from block, return the chunk after deserialized
    virtual StatusOr<ChunkUniquePtr> deserialize(SerdeContext& ctx, const BlockPtr block) = 0;
};
using SerdePtr = std::shared_ptr<Serde>;

StatusOr<SerdePtr> create_serde(SpilledOptions* options);
} // namespace starrocks::spill
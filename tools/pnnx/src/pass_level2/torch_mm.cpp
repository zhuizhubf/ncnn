// Copyright 2023 Tencent
// SPDX-License-Identifier: BSD-3-Clause

#include "pass_level2.h"

namespace pnnx {

class torch_mm : public GraphRewriterPass
{
public:
    const char* match_pattern_graph() const
    {
        return R"PNNXIR(7767517
4 3
pnnx.Input              input_0     0 1 input
pnnx.Input              input_1     0 1 mat2
aten::mm                op_0        2 1 input mat2 out
pnnx.Output             output      1 0 out
)PNNXIR";
    }

    const char* type_str() const
    {
        return "torch.mm";
    }
};

REGISTER_GLOBAL_PNNX_GRAPH_REWRITER_PASS(torch_mm, 90)

} // namespace pnnx

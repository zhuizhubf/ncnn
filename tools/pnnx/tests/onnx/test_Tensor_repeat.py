# Copyright 2024 Tencent
# SPDX-License-Identifier: BSD-3-Clause

import torch
import torch.nn as nn
import torch.nn.functional as F

class Model(nn.Module):
    def __init__(self):
        super(Model, self).__init__()

    def forward(self, x, y, z):
        x = x.repeat(1, 2, 3)
        x = x.repeat(2, 3, 4)
        y = y.repeat(1, 2, 1, 4)
        y = y.repeat(3, 4, 5, 1)
        z = z.repeat(1, 2, 3, 1, 5)
        z = z.repeat(2, 3, 3, 1, 1)
        return x, y, z

def test():
    net = Model()
    net.eval()

    torch.manual_seed(0)
    x = torch.rand(1, 3, 16)
    y = torch.rand(1, 5, 9, 11)
    z = torch.rand(14, 8, 5, 9, 10)

    a = net(x, y, z)

    # export onnx
    torch.onnx.export(net, (x, y, z), "test_Tensor_repeat.onnx")

    # onnx to pnnx
    import os
    os.system("../../src/pnnx test_Tensor_repeat.onnx inputshape=[1,3,16],[1,5,9,11],[14,8,5,9,10]")

    # pnnx inference
    import test_Tensor_repeat_pnnx
    b = test_Tensor_repeat_pnnx.test_inference()

    for a0, b0 in zip(a, b):
        if not torch.equal(a0, b0):
            return False
    return True

if __name__ == "__main__":
    if test():
        exit(0)
    else:
        exit(1)

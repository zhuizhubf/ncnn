# Copyright 2021 Tencent
# SPDX-License-Identifier: BSD-3-Clause

import torch
import torch.nn as nn
import torch.nn.functional as F
from packaging import version

class Model(nn.Module):
    def __init__(self):
        super(Model, self).__init__()

        self.w2 = nn.Parameter(torch.rand(12, 6, 4))
        self.b2 = nn.Parameter(torch.rand(12))
        self.w3 = nn.Parameter(torch.rand(6, 4, 3))

    def forward(self, x, w0, w1, b1, y):
        x = F.conv1d(x, w0, None, stride=2, padding=1)
        if version.parse(torch.__version__) < version.parse('1.9'):
            x = F.conv1d(x, w1, b1, stride=1, padding=1, dilation=2, groups=2)
        else:
            x = F.conv1d(x, w1, b1, stride=1, padding='same', dilation=2, groups=2)

        y = F.conv1d(y, self.w2, self.b2, stride=2, padding=2)
        y = F.conv1d(y, self.w3, None, stride=2, padding=1, groups=3)
        return x, y

def test():
    net = Model().half().float()
    net.eval()

    torch.manual_seed(0)
    x = torch.rand(1, 12, 52)
    w0 = torch.rand(16, 12, 3)
    w1 = torch.rand(16, 8, 5)
    b1 = torch.rand(16)
    y = torch.rand(1, 6, 25)

    a0, a1 = net(x, w0, w1, b1, y)

    # export torchscript
    mod = torch.jit.trace(net, (x, w0, w1, b1, y))
    mod.save("test_F_conv1d.pt")

    # torchscript to pnnx
    import os
    os.system("../../src/pnnx test_F_conv1d.pt inputshape=[1,12,52],[16,12,3],[16,8,5],[16],[1,6,25]")

    # ncnn inference
    import test_F_conv1d_ncnn
    b0, b1 = test_F_conv1d_ncnn.test_inference()

    return torch.allclose(a0, b0, 1e-4, 1e-4) and torch.allclose(a1, b1, 1e-4, 1e-4)

if __name__ == "__main__":
    if test():
        exit(0)
    else:
        exit(1)

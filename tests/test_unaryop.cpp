// Copyright 2020 Tencent
// SPDX-License-Identifier: BSD-3-Clause

#include "testutil.h"

#define OP_TYPE_MAX 20

static int op_type = 0;

static int test_unaryop(const ncnn::Mat& _a)
{
    ncnn::Mat a = _a;
    if (op_type == 2 || op_type == 3)
    {
        // large dynamic range for floor ceil
        for (int i = 0; i < a.total(); i++)
        {
            a[i] *= 1000;
        }
    }
    if (op_type == 5 || op_type == 6 || op_type == 8 || op_type == 17)
    {
        // value must be positive for sqrt rsqrt log
        Randomize(a, 0.001f, 2.f);
    }
    if (op_type == 11 || op_type == 12 || op_type == 13)
    {
        // smaller range for tan asin acos
        Randomize(a, -1.f, 1.f);
    }
#if __powerpc__
    // nearbyintf produces wrong result in halfway cases, why ?
    // too troublesome to resolve the compiler or qemu problem
    // so just skip them   --- nihui
    if (op_type == 18)
    {
        // drop 0.4 ~ 0.6
        for (int i = 0; i < a.total(); i++)
        {
            float v = a[i];
            float vv = fabs(v - (int)v);
            while (vv > 0.4f && vv < 0.6f)
            {
                v = RandomFloat(-15, 15);
                vv = fabs(v - (int)v);
            }
            a[i] = v;
        }
    }
#endif // __powerpc__

    ncnn::ParamDict pd;
    pd.set(0, op_type);

    std::vector<ncnn::Mat> weights(0);

    int ret = test_layer("UnaryOp", pd, weights, a);
    if (ret != 0)
    {
        fprintf(stderr, "test_unaryop failed a.dims=%d a=(%d %d %d %d) op_type=%d\n", a.dims, a.w, a.h, a.d, a.c, op_type);
    }

    return ret;
}

static int test_unaryop_0()
{
    return 0
           || test_unaryop(RandomMat(11, 3, 2, 16))
           || test_unaryop(RandomMat(10, 2, 2, 12))
           || test_unaryop(RandomMat(6, 1, 5, 13));
}

static int test_unaryop_1()
{
    return 0
           || test_unaryop(RandomMat(11, 7, 16))
           || test_unaryop(RandomMat(10, 4, 12))
           || test_unaryop(RandomMat(6, 5, 13));
}

static int test_unaryop_2()
{
    return 0
           || test_unaryop(RandomMat(12, 16))
           || test_unaryop(RandomMat(10, 12))
           || test_unaryop(RandomMat(14, 15));
}

static int test_unaryop_3()
{
    return 0
           || test_unaryop(RandomMat(128))
           || test_unaryop(RandomMat(12))
           || test_unaryop(RandomMat(15));
}

int main()
{
    SRAND(7767517);

    for (op_type = 0; op_type < OP_TYPE_MAX; op_type++)
    {
        int ret = 0
                  || test_unaryop_0()
                  || test_unaryop_1()
                  || test_unaryop_2()
                  || test_unaryop_3();

        if (ret != 0)
            return ret;
    }

    return 0;
}

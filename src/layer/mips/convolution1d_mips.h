// Copyright 2021 Tencent
// SPDX-License-Identifier: BSD-3-Clause

#ifndef LAYER_CONVOLUTION1D_MIPS_H
#define LAYER_CONVOLUTION1D_MIPS_H

#include "convolution1d.h"

namespace ncnn {

class Convolution1D_mips : public Convolution1D
{
public:
    Convolution1D_mips();

    virtual int create_pipeline(const Option& opt);
    virtual int destroy_pipeline(const Option& opt);

    virtual int forward(const Mat& bottom_blob, Mat& top_blob, const Option& opt) const;

    virtual int forward(const std::vector<Mat>& bottom_blobs, std::vector<Mat>& top_blobs, const Option& opt) const;

public:
    // packn
    Mat weight_data_packed;
};

} // namespace ncnn

#endif // LAYER_CONVOLUTION1D_MIPS_H

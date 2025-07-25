// Copyright 2019 BUG1989
// Copyright 2021 Tencent
// SPDX-License-Identifier: BSD-3-Clause

#include "requantize.h"
#include "fused_activation.h"

namespace ncnn {

static inline signed char float2int8(float v)
{
    int int32 = static_cast<int>(round(v));
    if (int32 > 127) return 127;
    if (int32 < -127) return -127;
    return (signed char)int32;
}

Requantize::Requantize()
{
    one_blob_only = true;
    support_inplace = false;
}

int Requantize::load_param(const ParamDict& pd)
{
    scale_in_data_size = pd.get(0, 1);
    scale_out_data_size = pd.get(1, 1);
    bias_data_size = pd.get(2, 0);
    activation_type = pd.get(3, 0);
    activation_params = pd.get(4, Mat());

    return 0;
}

int Requantize::load_model(const ModelBin& mb)
{
    scale_in_data = mb.load(scale_in_data_size, 1);
    if (scale_in_data.empty())
        return -100;

    scale_out_data = mb.load(scale_out_data_size, 1);
    if (scale_out_data.empty())
        return -100;

    if (bias_data_size)
    {
        bias_data = mb.load(bias_data_size, 1);
        if (bias_data.empty())
            return -100;
    }

    return 0;
}

static void requantize(const int* intptr, signed char* ptr, float scale_in, float bias, float scale_out, int activation_type, const Mat& activation_params, int size)
{
    for (int i = 0; i < size; i++)
    {
        float v = *intptr * scale_in + bias;
        v = activation_ss(v, activation_type, activation_params);
        *ptr = float2int8(v * scale_out);
        intptr++;
        ptr++;
    }
}

int Requantize::forward(const Mat& bottom_blob, Mat& top_blob, const Option& opt) const
{
    const int dims = bottom_blob.dims;
    const int w = bottom_blob.w;
    const int h = bottom_blob.h;
    const int channels = bottom_blob.c;

    if (dims == 1)
    {
        top_blob.create(w, (size_t)1u, opt.blob_allocator);
        if (top_blob.empty())
            return -100;

        // assert scale_in_data_size == 1
        // assert bias_data_size == 0 || bias_data_size == 1
        // assert scale_out_data_size == 1

        const int* intptr = bottom_blob;
        signed char* ptr = top_blob;

        const float scale_in = scale_in_data[0];
        const float bias = bias_data_size == 0 ? 0.f : bias_data[0];
        const float scale_out = scale_out_data[0];

        requantize(intptr, ptr, scale_in, bias, scale_out, activation_type, activation_params, w);
    }

    if (dims == 2)
    {
        top_blob.create(w, h, (size_t)1u, opt.blob_allocator);
        if (top_blob.empty())
            return -100;

        #pragma omp parallel for num_threads(opt.num_threads)
        for (int i = 0; i < h; i++)
        {
            const int* intptr = bottom_blob.row<const int>(i);
            signed char* ptr = top_blob.row<signed char>(i);

            const float scale_in = scale_in_data_size == 1 ? scale_in_data[0] : scale_in_data[i];
            const float bias = bias_data_size == 0 ? 0.f : bias_data_size == 1 ? bias_data[0] : bias_data[i];
            const float scale_out = scale_out_data_size == 1 ? scale_out_data[0] : scale_out_data[i];

            requantize(intptr, ptr, scale_in, bias, scale_out, activation_type, activation_params, w);
        }
    }

    if (dims == 3)
    {
        top_blob.create(w, h, channels, (size_t)1u, opt.blob_allocator);
        if (top_blob.empty())
            return -100;

        #pragma omp parallel for num_threads(opt.num_threads)
        for (int q = 0; q < channels; q++)
        {
            const int* intptr = bottom_blob.channel(q);
            signed char* ptr = top_blob.channel(q);

            const float scale_in = scale_in_data_size == 1 ? scale_in_data[0] : scale_in_data[q];
            const float bias = bias_data_size == 0 ? 0.f : bias_data_size == 1 ? bias_data[0] : bias_data[q];
            const float scale_out = scale_out_data_size == 1 ? scale_out_data[0] : scale_out_data[q];

            requantize(intptr, ptr, scale_in, bias, scale_out, activation_type, activation_params, w * h);
        }
    }

    return 0;
}

} // namespace ncnn

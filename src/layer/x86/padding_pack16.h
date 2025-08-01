// Copyright 2022 Tencent
// SPDX-License-Identifier: BSD-3-Clause

static void padding_constant_pack16_avx512(const Mat& src, Mat& dst, int top, int bottom, int left, int right, const __m512& v)
{
    const float* ptr = src;
    float* outptr = dst;
    int top_size = top * dst.w;
    int bottom_size = bottom * dst.w;

    // fill top
    for (int y = 0; y < top_size; y++)
    {
        _mm512_store_ps(outptr, v);
        outptr += 16;
    }
    // fill center
    for (int y = 0; y < src.h; y++)
    {
        for (int x = 0; x < left; x++)
        {
            _mm512_store_ps(outptr, v);
            outptr += 16;
        }
        for (int x = 0; x < src.w; x++)
        {
            _mm512_store_ps(outptr, _mm512_load_ps(ptr));
            ptr += 16;
            outptr += 16;
        }
        for (int x = 0; x < right; x++)
        {
            _mm512_store_ps(outptr, v);
            outptr += 16;
        }
    }
    // fill top
    for (int y = 0; y < bottom_size; y++)
    {
        _mm512_store_ps(outptr, v);
        outptr += 16;
    }
}

static void padding_replicate_pack16_avx512(const Mat& src, Mat& dst, int top, int bottom, int left, int right)
{
    const float* ptr = src;
    float* outptr = dst;

    // fill top
    for (int y = 0; y < top; y++)
    {
        const float* ptr0 = ptr;
        __m512 _p = _mm512_load_ps(ptr0);
        for (int x = 0; x < left; x++)
        {
            _mm512_store_ps(outptr, _p);
            outptr += 16;
        }
        for (int x = 0; x < src.w; x++)
        {
            _p = _mm512_load_ps(ptr0);
            _mm512_store_ps(outptr, _p);
            ptr0 += 16;
            outptr += 16;
        }
        for (int x = 0; x < right; x++)
        {
            _mm512_store_ps(outptr, _p);
            outptr += 16;
        }
    }
    // fill center
    for (int y = 0; y < src.h; y++)
    {
        __m512 _p = _mm512_load_ps(ptr);
        for (int x = 0; x < left; x++)
        {
            _mm512_store_ps(outptr, _p);
            outptr += 16;
        }
        for (int x = 0; x < src.w; x++)
        {
            _p = _mm512_load_ps(ptr);
            _mm512_store_ps(outptr, _p);
            ptr += 16;
            outptr += 16;
        }
        for (int x = 0; x < right; x++)
        {
            _mm512_store_ps(outptr, _p);
            outptr += 16;
        }
    }
    // fill bottom
    ptr -= src.w * 16;
    for (int y = 0; y < bottom; y++)
    {
        const float* ptr0 = ptr;
        __m512 _p = _mm512_load_ps(ptr0);
        for (int x = 0; x < left; x++)
        {
            _mm512_store_ps(outptr, _p);
            outptr += 16;
        }
        for (int x = 0; x < src.w; x++)
        {
            _p = _mm512_load_ps(ptr0);
            _mm512_store_ps(outptr, _p);
            ptr0 += 16;
            outptr += 16;
        }
        for (int x = 0; x < right; x++)
        {
            _mm512_store_ps(outptr, _p);
            outptr += 16;
        }
    }
}

static void padding_reflect_pack16_avx512(const Mat& src, Mat& dst, int top, int bottom, int left, int right)
{
    const float* ptr = src;
    float* outptr = dst;

    // fill top
    ptr += top * src.w * 16;
    for (int y = 0; y < top; y++)
    {
        const float* ptr0 = ptr;
        for (int x = 0; x < left; x++)
        {
            __m512 _p = _mm512_load_ps(ptr0 + (left - x) * 16);
            _mm512_store_ps(outptr, _p);
            outptr += 16;
        }
        for (int x = 0; x < src.w; x++)
        {
            __m512 _p = _mm512_load_ps(ptr0);
            _mm512_store_ps(outptr, _p);
            ptr0 += 16;
            outptr += 16;
        }
        for (int x = 0; x < right; x++)
        {
            __m512 _p = _mm512_load_ps(ptr0 - 32 - x * 16);
            _mm512_store_ps(outptr, _p);
            outptr += 16;
        }
        ptr -= src.w * 16;
    }
    // fill center
    for (int y = 0; y < src.h; y++)
    {
        for (int x = 0; x < left; x++)
        {
            __m512 _p = _mm512_load_ps(ptr + (left - x) * 16);
            _mm512_store_ps(outptr, _p);
            outptr += 16;
        }
        for (int x = 0; x < src.w; x++)
        {
            __m512 _p = _mm512_load_ps(ptr);
            _mm512_store_ps(outptr, _p);
            ptr += 16;
            outptr += 16;
        }
        for (int x = 0; x < right; x++)
        {
            __m512 _p = _mm512_load_ps(ptr - 32 - x * 16);
            _mm512_store_ps(outptr, _p);
            outptr += 16;
        }
    }
    // fill bottom
    ptr -= 2 * src.w * 16;
    for (int y = 0; y < bottom; y++)
    {
        const float* ptr0 = ptr;
        for (int x = 0; x < left; x++)
        {
            __m512 _p = _mm512_load_ps(ptr0 + (left - x) * 16);
            _mm512_store_ps(outptr, _p);
            outptr += 16;
        }
        for (int x = 0; x < src.w; x++)
        {
            __m512 _p = _mm512_load_ps(ptr0);
            _mm512_store_ps(outptr, _p);
            ptr0 += 16;
            outptr += 16;
        }
        for (int x = 0; x < right; x++)
        {
            __m512 _p = _mm512_load_ps(ptr0 - 32 - x * 16);
            _mm512_store_ps(outptr, _p);
            outptr += 16;
        }
        ptr -= src.w * 16;
    }
}

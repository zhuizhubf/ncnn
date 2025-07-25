// Copyright 2021 Tencent
// SPDX-License-Identifier: BSD-3-Clause

static void conv7x7s2_pack1ton_rvv(const Mat& bottom_blob, Mat& top_blob, const Mat& kernel, const Mat& _bias, const Option& opt)
{
    const int packn = csrr_vlenb() / 4;
    const size_t vl = __riscv_vsetvl_e32m1(packn);

    int w = bottom_blob.w;
    int inch = bottom_blob.c;

    int outw = top_blob.w;
    int outh = top_blob.h;
    int outch = top_blob.c;

    const int tailstep = w - 2 * outw + w;

    const float* bias = _bias;

    #pragma omp parallel for num_threads(opt.num_threads)
    for (int p = 0; p < outch; p++)
    {
        Mat out0 = top_blob.channel(p);

        vfloat32m1_t _bias0 = bias ? __riscv_vle32_v_f32m1(bias + p * packn, vl) : __riscv_vfmv_v_f_f32m1(0.f, vl);
        out0.fill(_bias0);

        for (int q = 0; q < inch; q++)
        {
            float* outptr0 = out0;

            const Mat img0 = bottom_blob.channel(q);

            const float* r0 = img0.row(0);
            const float* r1 = img0.row(1);
            const float* r2 = img0.row(2);
            const float* r3 = img0.row(3);
            const float* r4 = img0.row(4);
            const float* r5 = img0.row(5);
            const float* r6 = img0.row(6);

            const float* kptr = kernel.channel(p).row(q);

            int i = 0;

            for (; i < outh; i++)
            {
                int j = 0;
                for (; j + 7 < outw; j += 8)
                {
                    vfloat32m1_t _sum0 = __riscv_vle32_v_f32m1(outptr0, vl);
                    vfloat32m1_t _sum1 = __riscv_vle32_v_f32m1(outptr0 + packn, vl);
                    vfloat32m1_t _sum2 = __riscv_vle32_v_f32m1(outptr0 + packn * 2, vl);
                    vfloat32m1_t _sum3 = __riscv_vle32_v_f32m1(outptr0 + packn * 3, vl);
                    vfloat32m1_t _sum4 = __riscv_vle32_v_f32m1(outptr0 + packn * 4, vl);
                    vfloat32m1_t _sum5 = __riscv_vle32_v_f32m1(outptr0 + packn * 5, vl);
                    vfloat32m1_t _sum6 = __riscv_vle32_v_f32m1(outptr0 + packn * 6, vl);
                    vfloat32m1_t _sum7 = __riscv_vle32_v_f32m1(outptr0 + packn * 7, vl);

                    vfloat32m1_t _k00 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k01 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k02 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k03 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k04 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k05 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k06 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[0], _k00, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r0[2], _k00, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r0[4], _k00, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r0[6], _k00, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r0[8], _k00, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r0[10], _k00, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r0[12], _k00, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r0[14], _k00, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[1], _k01, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r0[3], _k01, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r0[5], _k01, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r0[7], _k01, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r0[9], _k01, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r0[11], _k01, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r0[13], _k01, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r0[15], _k01, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[2], _k02, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r0[4], _k02, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r0[6], _k02, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r0[8], _k02, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r0[10], _k02, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r0[12], _k02, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r0[14], _k02, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r0[16], _k02, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[3], _k03, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r0[5], _k03, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r0[7], _k03, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r0[9], _k03, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r0[11], _k03, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r0[13], _k03, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r0[15], _k03, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r0[17], _k03, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[4], _k04, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r0[6], _k04, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r0[8], _k04, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r0[10], _k04, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r0[12], _k04, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r0[14], _k04, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r0[16], _k04, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r0[18], _k04, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[5], _k05, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r0[7], _k05, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r0[9], _k05, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r0[11], _k05, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r0[13], _k05, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r0[15], _k05, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r0[17], _k05, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r0[19], _k05, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[6], _k06, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r0[8], _k06, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r0[10], _k06, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r0[12], _k06, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r0[14], _k06, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r0[16], _k06, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r0[18], _k06, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r0[20], _k06, vl);

                    vfloat32m1_t _k10 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k11 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k12 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k13 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k14 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k15 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k16 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[0], _k10, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r1[2], _k10, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r1[4], _k10, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r1[6], _k10, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r1[8], _k10, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r1[10], _k10, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r1[12], _k10, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r1[14], _k10, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[1], _k11, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r1[3], _k11, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r1[5], _k11, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r1[7], _k11, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r1[9], _k11, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r1[11], _k11, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r1[13], _k11, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r1[15], _k11, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[2], _k12, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r1[4], _k12, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r1[6], _k12, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r1[8], _k12, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r1[10], _k12, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r1[12], _k12, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r1[14], _k12, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r1[16], _k12, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[3], _k13, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r1[5], _k13, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r1[7], _k13, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r1[9], _k13, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r1[11], _k13, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r1[13], _k13, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r1[15], _k13, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r1[17], _k13, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[4], _k14, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r1[6], _k14, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r1[8], _k14, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r1[10], _k14, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r1[12], _k14, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r1[14], _k14, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r1[16], _k14, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r1[18], _k14, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[5], _k15, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r1[7], _k15, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r1[9], _k15, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r1[11], _k15, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r1[13], _k15, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r1[15], _k15, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r1[17], _k15, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r1[19], _k15, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[6], _k16, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r1[8], _k16, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r1[10], _k16, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r1[12], _k16, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r1[14], _k16, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r1[16], _k16, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r1[18], _k16, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r1[20], _k16, vl);

                    vfloat32m1_t _k20 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k21 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k22 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k23 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k24 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k25 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k26 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[0], _k20, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r2[2], _k20, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r2[4], _k20, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r2[6], _k20, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r2[8], _k20, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r2[10], _k20, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r2[12], _k20, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r2[14], _k20, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[1], _k21, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r2[3], _k21, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r2[5], _k21, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r2[7], _k21, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r2[9], _k21, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r2[11], _k21, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r2[13], _k21, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r2[15], _k21, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[2], _k22, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r2[4], _k22, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r2[6], _k22, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r2[8], _k22, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r2[10], _k22, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r2[12], _k22, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r2[14], _k22, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r2[16], _k22, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[3], _k23, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r2[5], _k23, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r2[7], _k23, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r2[9], _k23, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r2[11], _k23, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r2[13], _k23, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r2[15], _k23, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r2[17], _k23, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[4], _k24, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r2[6], _k24, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r2[8], _k24, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r2[10], _k24, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r2[12], _k24, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r2[14], _k24, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r2[16], _k24, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r2[18], _k24, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[5], _k25, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r2[7], _k25, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r2[9], _k25, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r2[11], _k25, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r2[13], _k25, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r2[15], _k25, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r2[17], _k25, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r2[19], _k25, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[6], _k26, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r2[8], _k26, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r2[10], _k26, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r2[12], _k26, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r2[14], _k26, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r2[16], _k26, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r2[18], _k26, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r2[20], _k26, vl);

                    vfloat32m1_t _k30 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k31 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k32 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k33 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k34 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k35 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k36 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[0], _k30, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r3[2], _k30, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r3[4], _k30, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r3[6], _k30, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r3[8], _k30, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r3[10], _k30, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r3[12], _k30, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r3[14], _k30, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[1], _k31, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r3[3], _k31, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r3[5], _k31, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r3[7], _k31, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r3[9], _k31, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r3[11], _k31, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r3[13], _k31, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r3[15], _k31, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[2], _k32, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r3[4], _k32, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r3[6], _k32, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r3[8], _k32, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r3[10], _k32, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r3[12], _k32, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r3[14], _k32, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r3[16], _k32, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[3], _k33, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r3[5], _k33, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r3[7], _k33, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r3[9], _k33, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r3[11], _k33, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r3[13], _k33, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r3[15], _k33, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r3[17], _k33, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[4], _k34, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r3[6], _k34, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r3[8], _k34, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r3[10], _k34, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r3[12], _k34, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r3[14], _k34, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r3[16], _k34, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r3[18], _k34, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[5], _k35, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r3[7], _k35, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r3[9], _k35, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r3[11], _k35, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r3[13], _k35, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r3[15], _k35, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r3[17], _k35, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r3[19], _k35, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[6], _k36, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r3[8], _k36, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r3[10], _k36, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r3[12], _k36, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r3[14], _k36, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r3[16], _k36, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r3[18], _k36, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r3[20], _k36, vl);

                    vfloat32m1_t _k40 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k41 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k42 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k43 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k44 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k45 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k46 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[0], _k40, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r4[2], _k40, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r4[4], _k40, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r4[6], _k40, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r4[8], _k40, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r4[10], _k40, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r4[12], _k40, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r4[14], _k40, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[1], _k41, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r4[3], _k41, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r4[5], _k41, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r4[7], _k41, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r4[9], _k41, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r4[11], _k41, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r4[13], _k41, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r4[15], _k41, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[2], _k42, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r4[4], _k42, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r4[6], _k42, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r4[8], _k42, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r4[10], _k42, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r4[12], _k42, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r4[14], _k42, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r4[16], _k42, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[3], _k43, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r4[5], _k43, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r4[7], _k43, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r4[9], _k43, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r4[11], _k43, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r4[13], _k43, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r4[15], _k43, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r4[17], _k43, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[4], _k44, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r4[6], _k44, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r4[8], _k44, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r4[10], _k44, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r4[12], _k44, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r4[14], _k44, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r4[16], _k44, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r4[18], _k44, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[5], _k45, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r4[7], _k45, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r4[9], _k45, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r4[11], _k45, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r4[13], _k45, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r4[15], _k45, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r4[17], _k45, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r4[19], _k45, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[6], _k46, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r4[8], _k46, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r4[10], _k46, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r4[12], _k46, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r4[14], _k46, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r4[16], _k46, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r4[18], _k46, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r4[20], _k46, vl);

                    vfloat32m1_t _k50 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k51 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k52 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k53 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k54 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k55 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k56 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[0], _k50, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r5[2], _k50, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r5[4], _k50, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r5[6], _k50, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r5[8], _k50, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r5[10], _k50, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r5[12], _k50, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r5[14], _k50, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[1], _k51, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r5[3], _k51, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r5[5], _k51, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r5[7], _k51, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r5[9], _k51, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r5[11], _k51, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r5[13], _k51, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r5[15], _k51, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[2], _k52, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r5[4], _k52, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r5[6], _k52, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r5[8], _k52, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r5[10], _k52, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r5[12], _k52, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r5[14], _k52, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r5[16], _k52, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[3], _k53, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r5[5], _k53, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r5[7], _k53, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r5[9], _k53, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r5[11], _k53, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r5[13], _k53, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r5[15], _k53, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r5[17], _k53, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[4], _k54, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r5[6], _k54, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r5[8], _k54, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r5[10], _k54, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r5[12], _k54, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r5[14], _k54, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r5[16], _k54, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r5[18], _k54, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[5], _k55, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r5[7], _k55, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r5[9], _k55, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r5[11], _k55, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r5[13], _k55, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r5[15], _k55, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r5[17], _k55, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r5[19], _k55, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[6], _k56, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r5[8], _k56, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r5[10], _k56, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r5[12], _k56, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r5[14], _k56, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r5[16], _k56, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r5[18], _k56, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r5[20], _k56, vl);

                    vfloat32m1_t _k60 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k61 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k62 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k63 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k64 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k65 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k66 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr -= packn * 42;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[0], _k60, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r6[2], _k60, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r6[4], _k60, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r6[6], _k60, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r6[8], _k60, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r6[10], _k60, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r6[12], _k60, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r6[14], _k60, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[1], _k61, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r6[3], _k61, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r6[5], _k61, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r6[7], _k61, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r6[9], _k61, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r6[11], _k61, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r6[13], _k61, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r6[15], _k61, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[2], _k62, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r6[4], _k62, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r6[6], _k62, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r6[8], _k62, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r6[10], _k62, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r6[12], _k62, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r6[14], _k62, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r6[16], _k62, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[3], _k63, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r6[5], _k63, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r6[7], _k63, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r6[9], _k63, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r6[11], _k63, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r6[13], _k63, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r6[15], _k63, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r6[17], _k63, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[4], _k64, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r6[6], _k64, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r6[8], _k64, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r6[10], _k64, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r6[12], _k64, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r6[14], _k64, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r6[16], _k64, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r6[18], _k64, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[5], _k65, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r6[7], _k65, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r6[9], _k65, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r6[11], _k65, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r6[13], _k65, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r6[15], _k65, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r6[17], _k65, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r6[19], _k65, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[6], _k66, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r6[8], _k66, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r6[10], _k66, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r6[12], _k66, vl);
                    _sum4 = __riscv_vfmacc_vf_f32m1(_sum4, r6[14], _k66, vl);
                    _sum5 = __riscv_vfmacc_vf_f32m1(_sum5, r6[16], _k66, vl);
                    _sum6 = __riscv_vfmacc_vf_f32m1(_sum6, r6[18], _k66, vl);
                    _sum7 = __riscv_vfmacc_vf_f32m1(_sum7, r6[20], _k66, vl);

                    __riscv_vse32_v_f32m1(outptr0, _sum0, vl);
                    __riscv_vse32_v_f32m1(outptr0 + packn, _sum1, vl);
                    __riscv_vse32_v_f32m1(outptr0 + packn * 2, _sum2, vl);
                    __riscv_vse32_v_f32m1(outptr0 + packn * 3, _sum3, vl);
                    __riscv_vse32_v_f32m1(outptr0 + packn * 4, _sum4, vl);
                    __riscv_vse32_v_f32m1(outptr0 + packn * 5, _sum5, vl);
                    __riscv_vse32_v_f32m1(outptr0 + packn * 6, _sum6, vl);
                    __riscv_vse32_v_f32m1(outptr0 + packn * 7, _sum7, vl);

                    outptr0 += packn * 8;

                    r0 += 16;
                    r1 += 16;
                    r2 += 16;
                    r3 += 16;
                    r4 += 16;
                    r5 += 16;
                    r6 += 16;
                }
                for (; j + 3 < outw; j += 4)
                {
                    vfloat32m1_t _sum0 = __riscv_vle32_v_f32m1(outptr0, vl);
                    vfloat32m1_t _sum1 = __riscv_vle32_v_f32m1(outptr0 + packn, vl);
                    vfloat32m1_t _sum2 = __riscv_vle32_v_f32m1(outptr0 + packn * 2, vl);
                    vfloat32m1_t _sum3 = __riscv_vle32_v_f32m1(outptr0 + packn * 3, vl);

                    vfloat32m1_t _k00 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k01 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k02 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k03 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k04 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k05 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k06 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[0], _k00, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r0[2], _k00, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r0[4], _k00, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r0[6], _k00, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[1], _k01, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r0[3], _k01, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r0[5], _k01, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r0[7], _k01, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[2], _k02, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r0[4], _k02, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r0[6], _k02, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r0[8], _k02, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[3], _k03, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r0[5], _k03, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r0[7], _k03, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r0[9], _k03, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[4], _k04, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r0[6], _k04, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r0[8], _k04, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r0[10], _k04, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[5], _k05, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r0[7], _k05, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r0[9], _k05, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r0[11], _k05, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[6], _k06, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r0[8], _k06, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r0[10], _k06, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r0[12], _k06, vl);

                    vfloat32m1_t _k10 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k11 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k12 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k13 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k14 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k15 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k16 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[0], _k10, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r1[2], _k10, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r1[4], _k10, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r1[6], _k10, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[1], _k11, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r1[3], _k11, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r1[5], _k11, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r1[7], _k11, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[2], _k12, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r1[4], _k12, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r1[6], _k12, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r1[8], _k12, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[3], _k13, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r1[5], _k13, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r1[7], _k13, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r1[9], _k13, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[4], _k14, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r1[6], _k14, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r1[8], _k14, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r1[10], _k14, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[5], _k15, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r1[7], _k15, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r1[9], _k15, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r1[11], _k15, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[6], _k16, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r1[8], _k16, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r1[10], _k16, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r1[12], _k16, vl);

                    vfloat32m1_t _k20 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k21 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k22 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k23 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k24 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k25 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k26 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[0], _k20, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r2[2], _k20, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r2[4], _k20, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r2[6], _k20, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[1], _k21, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r2[3], _k21, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r2[5], _k21, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r2[7], _k21, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[2], _k22, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r2[4], _k22, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r2[6], _k22, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r2[8], _k22, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[3], _k23, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r2[5], _k23, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r2[7], _k23, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r2[9], _k23, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[4], _k24, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r2[6], _k24, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r2[8], _k24, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r2[10], _k24, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[5], _k25, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r2[7], _k25, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r2[9], _k25, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r2[11], _k25, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[6], _k26, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r2[8], _k26, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r2[10], _k26, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r2[12], _k26, vl);

                    vfloat32m1_t _k30 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k31 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k32 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k33 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k34 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k35 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k36 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[0], _k30, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r3[2], _k30, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r3[4], _k30, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r3[6], _k30, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[1], _k31, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r3[3], _k31, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r3[5], _k31, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r3[7], _k31, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[2], _k32, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r3[4], _k32, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r3[6], _k32, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r3[8], _k32, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[3], _k33, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r3[5], _k33, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r3[7], _k33, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r3[9], _k33, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[4], _k34, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r3[6], _k34, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r3[8], _k34, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r3[10], _k34, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[5], _k35, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r3[7], _k35, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r3[9], _k35, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r3[11], _k35, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[6], _k36, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r3[8], _k36, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r3[10], _k36, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r3[12], _k36, vl);

                    vfloat32m1_t _k40 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k41 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k42 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k43 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k44 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k45 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k46 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[0], _k40, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r4[2], _k40, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r4[4], _k40, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r4[6], _k40, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[1], _k41, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r4[3], _k41, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r4[5], _k41, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r4[7], _k41, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[2], _k42, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r4[4], _k42, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r4[6], _k42, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r4[8], _k42, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[3], _k43, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r4[5], _k43, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r4[7], _k43, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r4[9], _k43, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[4], _k44, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r4[6], _k44, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r4[8], _k44, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r4[10], _k44, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[5], _k45, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r4[7], _k45, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r4[9], _k45, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r4[11], _k45, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[6], _k46, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r4[8], _k46, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r4[10], _k46, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r4[12], _k46, vl);

                    vfloat32m1_t _k50 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k51 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k52 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k53 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k54 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k55 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k56 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[0], _k50, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r5[2], _k50, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r5[4], _k50, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r5[6], _k50, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[1], _k51, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r5[3], _k51, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r5[5], _k51, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r5[7], _k51, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[2], _k52, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r5[4], _k52, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r5[6], _k52, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r5[8], _k52, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[3], _k53, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r5[5], _k53, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r5[7], _k53, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r5[9], _k53, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[4], _k54, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r5[6], _k54, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r5[8], _k54, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r5[10], _k54, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[5], _k55, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r5[7], _k55, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r5[9], _k55, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r5[11], _k55, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[6], _k56, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r5[8], _k56, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r5[10], _k56, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r5[12], _k56, vl);

                    vfloat32m1_t _k60 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k61 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k62 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k63 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k64 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k65 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k66 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr -= packn * 42;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[0], _k60, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r6[2], _k60, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r6[4], _k60, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r6[6], _k60, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[1], _k61, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r6[3], _k61, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r6[5], _k61, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r6[7], _k61, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[2], _k62, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r6[4], _k62, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r6[6], _k62, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r6[8], _k62, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[3], _k63, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r6[5], _k63, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r6[7], _k63, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r6[9], _k63, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[4], _k64, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r6[6], _k64, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r6[8], _k64, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r6[10], _k64, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[5], _k65, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r6[7], _k65, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r6[9], _k65, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r6[11], _k65, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[6], _k66, vl);
                    _sum1 = __riscv_vfmacc_vf_f32m1(_sum1, r6[8], _k66, vl);
                    _sum2 = __riscv_vfmacc_vf_f32m1(_sum2, r6[10], _k66, vl);
                    _sum3 = __riscv_vfmacc_vf_f32m1(_sum3, r6[12], _k66, vl);

                    __riscv_vse32_v_f32m1(outptr0, _sum0, vl);
                    __riscv_vse32_v_f32m1(outptr0 + packn, _sum1, vl);
                    __riscv_vse32_v_f32m1(outptr0 + packn * 2, _sum2, vl);
                    __riscv_vse32_v_f32m1(outptr0 + packn * 3, _sum3, vl);

                    outptr0 += packn * 4;

                    r0 += 8;
                    r1 += 8;
                    r2 += 8;
                    r3 += 8;
                    r4 += 8;
                    r5 += 8;
                    r6 += 8;
                }
                for (; j < outw; j++)
                {
                    vfloat32m1_t _sum0 = __riscv_vle32_v_f32m1(outptr0, vl);

                    vfloat32m1_t _k00 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k01 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k02 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k03 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k04 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k05 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k06 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[0], _k00, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[1], _k01, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[2], _k02, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[3], _k03, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[4], _k04, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[5], _k05, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r0[6], _k06, vl);

                    vfloat32m1_t _k10 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k11 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k12 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k13 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k14 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k15 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k16 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[0], _k10, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[1], _k11, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[2], _k12, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[3], _k13, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[4], _k14, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[5], _k15, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r1[6], _k16, vl);

                    vfloat32m1_t _k20 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k21 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k22 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k23 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k24 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k25 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k26 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[0], _k20, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[1], _k21, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[2], _k22, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[3], _k23, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[4], _k24, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[5], _k25, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r2[6], _k26, vl);

                    vfloat32m1_t _k30 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k31 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k32 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k33 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k34 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k35 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k36 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[0], _k30, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[1], _k31, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[2], _k32, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[3], _k33, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[4], _k34, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[5], _k35, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r3[6], _k36, vl);

                    vfloat32m1_t _k40 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k41 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k42 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k43 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k44 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k45 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k46 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[0], _k40, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[1], _k41, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[2], _k42, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[3], _k43, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[4], _k44, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[5], _k45, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r4[6], _k46, vl);

                    vfloat32m1_t _k50 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k51 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k52 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k53 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k54 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k55 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k56 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr += packn * 7;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[0], _k50, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[1], _k51, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[2], _k52, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[3], _k53, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[4], _k54, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[5], _k55, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r5[6], _k56, vl);

                    vfloat32m1_t _k60 = __riscv_vle32_v_f32m1(kptr, vl);
                    vfloat32m1_t _k61 = __riscv_vle32_v_f32m1(kptr + packn, vl);
                    vfloat32m1_t _k62 = __riscv_vle32_v_f32m1(kptr + packn * 2, vl);
                    vfloat32m1_t _k63 = __riscv_vle32_v_f32m1(kptr + packn * 3, vl);
                    vfloat32m1_t _k64 = __riscv_vle32_v_f32m1(kptr + packn * 4, vl);
                    vfloat32m1_t _k65 = __riscv_vle32_v_f32m1(kptr + packn * 5, vl);
                    vfloat32m1_t _k66 = __riscv_vle32_v_f32m1(kptr + packn * 6, vl);

                    kptr -= packn * 42;

                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[0], _k60, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[1], _k61, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[2], _k62, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[3], _k63, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[4], _k64, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[5], _k65, vl);
                    _sum0 = __riscv_vfmacc_vf_f32m1(_sum0, r6[6], _k66, vl);

                    __riscv_vse32_v_f32m1(outptr0, _sum0, vl);

                    outptr0 += packn;

                    r0 += 2;
                    r1 += 2;
                    r2 += 2;
                    r3 += 2;
                    r4 += 2;
                    r5 += 2;
                    r6 += 2;
                }

                r0 += tailstep;
                r1 += tailstep;
                r2 += tailstep;
                r3 += tailstep;
                r4 += tailstep;
                r5 += tailstep;
                r6 += tailstep;
            }
        }
    }
}

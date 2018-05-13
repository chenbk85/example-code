// @author visaodumuc
// g++ -std=c++11 -O3 mandelbrot_tbb_avx2.cpp -o mandelbrot_tbb_avx2 -I/opt/intel/tbb/include -L/opt/intel/tbb/lib/intel64/gcc4.7 -ltbb -mavx2
#include <chrono>
#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <iostream>

#include <immintrin.h>

#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>

int main(int argc, const char **argv) {
    std::size_t loop_count = 512;    // atoi(argv[5]);
    float x_min = -2.0;              // atof(argv[1]);
    float x_max = 2.0;               // atof(argv[2]);
    float y_min = -2.0;              // atof(argv[3]);
    float y_max = 2.0;               // atof(argv[4]);
    std::size_t x_resolution = 2048; // atoi(argv[6]);
    std::size_t y_resolution = 2048; // atoi(argv[7]);
    float threshold = 4.0;
    if (argc >= 8) {
        x_min = atof(argv[1]);
        x_max = atof(argv[2]);
        y_min = atof(argv[3]);
        y_max = atof(argv[4]);
        x_resolution = atoi(argv[5]);
        y_resolution = atoi(argv[6]);
        loop_count = atoi(argv[7]);
    } else {
        std::cout << "use default arguments" << std::endl;
    }
    std::cout << "x=[" << x_min << ";" << x_max << "] y=[" << y_min << ";"
              << y_max << "]"
              << " resolution=[" << x_resolution << ";" << y_resolution << "]"
              << " depth=" << loop_count << std::endl;
    std::size_t image_size = x_resolution * y_resolution;
    uint16_t *image = new uint16_t[image_size];
    memset(image, 0xff, image_size * sizeof(uint16_t));
    // x(n+1) = x(n) ^ 2 + c
    // x_n_1_r = x_r^2 - x_i^2 + c_r
    // x_n_1_i = 2*x_r*x_i + c_i
    //
    tbb::task_scheduler_init init;
    auto now = std::chrono::high_resolution_clock::now();
    float x_diff = (x_max - x_min) / x_resolution;
    float y_diff = (y_max - y_min) / y_resolution;
    tbb::parallel_for(
        tbb::blocked_range<std::size_t>(0, y_resolution),
        [&](const tbb::blocked_range<std::size_t> &range) {
            __m256 const_2 = _mm256_set1_ps(2);
            __m256 const_threshold = _mm256_set1_ps(threshold);
            std::size_t loop[8] = {0};
            for (std::size_t y_idx = range.begin(); y_idx != range.end();
                 y_idx++) {
                __m256 y = _mm256_set1_ps(y_max - y_diff * y_idx);
                for (std::size_t x_idx = 0; x_idx < x_resolution; x_idx += 8) {
                    float x_0 = x_min + x_diff * x_idx;
                    __m256 x = _mm256_set_ps(
                        x_0, x_0 + x_diff, x_0 + x_diff * 2, x_0 + x_diff * 3,
                        x_0 + x_diff * 4, x_0 + x_diff * 5, x_0 + x_diff * 6,
                        x_0 + x_diff * 7);
                    //
                    __m256 v_i = _mm256_set1_ps(0);
                    __m256 v_r = _mm256_set1_ps(0);
                    std::size_t n = 0;
                    for (std::size_t i = 0; i < 8; i++) {
                        loop[i] = 0;
                    }
                    for (n = 0; n < loop_count; n++) {
                        __m256 v_n_r = _mm256_mul_ps(v_r, v_r);
                        __m256 tmp = _mm256_mul_ps(v_i, v_i);
                        v_n_r = _mm256_sub_ps(v_n_r, tmp);
                        v_n_r = _mm256_add_ps(v_n_r, x);

                        __m256 v_n_i = _mm256_mul_ps(v_r, v_i);
                        v_n_i = _mm256_mul_ps(v_n_i, const_2);
                        v_n_i = _mm256_add_ps(v_n_i, y);

                        __m256 v_2 = _mm256_mul_ps(v_n_r, v_n_r);
                        tmp = _mm256_mul_ps(v_n_i, v_n_i);
                        v_2 = _mm256_add_ps(v_2, tmp);

                        tmp = _mm256_cmp_ps(v_2, const_threshold, _CMP_GE_OQ);
                        uint32_t mask = _mm256_movemask_ps(tmp);
                        for (std::size_t i = 0; i < 8; i++) {
                            if ((mask & (1 << i)) && loop[i] == 0) {
                                loop[i] = n;
                            }
                        }
                        if ((mask & 0x0FF) == 0x0FF) {
                            break;
                        }
                        v_r = v_n_r;
                        v_i = v_n_i;
                    }
                    for (std::size_t i = 0; i < 8; i++) {
                        image[y_idx * y_resolution + x_idx + i] = loop[7 - i];
                    }
                } // end for x
            }     // end for y
        });
    auto diff = std::chrono::duration_cast<std::chrono::milliseconds>(
                    std::chrono::high_resolution_clock::now() - now)
                    .count();
    std::cout << "time = " << diff << " [ms]" << std::endl;

    //
    /* Open the file and write the header. */
    const char *filename = "mandelbrot.ppm";
    FILE *fp = fopen(filename, "wb");
    /*write ASCII header to the file*/
    fprintf(fp,
            "P6\n# Mandelbrot, xmin=%lf, xmax=%lf, ymin=%lf, ymax=%lf, "
            "maxiter=%d\n",
            x_min, x_max, y_min, y_max, loop_count, x_resolution, y_resolution,
            loop_count);
    fprintf(fp, "%d\n%d\n%d\n", x_resolution, y_resolution,
            loop_count < 256 ? 256 : loop_count);

    for (std::size_t idx = 0; idx < image_size; idx++) {
        uint16_t n = image[idx];
        unsigned char color[6] = {0};
        color[0] = n >> 8;
        color[1] = n & 255;
        // color[2] = n >> 8;
        // color[3] = n & 255;
        // color[4] = n >> 8;
        // color[5] = n & 255;
        fwrite(color, 6, 1, fp);
    }

    fclose(fp);

    delete[] image;
    return 0;
}

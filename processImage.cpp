#include <iostream>
// #include <vector>
// #include <string>
//#include <unistd.h>
#include "CImg.h"
using namespace std;
using namespace cimg_library;

#include <windows.h>

void usleep(__int64 usec) {
    HANDLE timer;
    LARGE_INTEGER ft;
    ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval (negative value indicates relative time)
    timer = CreateWaitableTimer(NULL, TRUE, NULL);
    SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
    WaitForSingleObject(timer, INFINITE);
    CloseHandle(timer);
}


CImg<unsigned char> normalize(const CImg<unsigned char>& img) {
    int width = img.width();
    int height = img.height();
    CImg<unsigned char> tmp(width, height, 1, 1, 0);
    float min = 10000; float max = 0;

    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            if (img(r, c) < min) min = img(r, c);
            if (img(r, c) > max) max = img(r, c);
        }
    }

    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            float ratio = (float)(img(r, c) - min) / (max - min);
            tmp(r, c) = ratio * 255;
            if (r >= 250 && r <= 255 && c >= 250 && c <= 255)
                std::cout << ratio * 255 << " ";
        }
        // std::cout << "\n";
    }

    return tmp;
}

CImg<unsigned char> calcGradientMagnitude(const CImg<unsigned char>& sobel_x,
    const CImg<unsigned char>& sobel_y) {
    int width = sobel_x.width();
    int height = sobel_x.height();
    CImg<unsigned char> tmp(width, height, 1, 1, 0);

    for (int r = 0; r < height; ++r) {
        for (int c = 0; c < width; ++c) {
            tmp(r, c) =
                sqrt(sobel_x(r, c) * sobel_x(r, c) + sobel_y(r, c) * sobel_x(r, c));
            // if (tmp(r, c) > 255)
            //   tmp(r, c) = 255;
        }
    }

    return tmp;
}

CImg<unsigned char> processImage(const CImg<unsigned char>& img, int type = 0)
{
    int width = img.width();
    int height = img.height();
    // std::cout << width << " " << height << std::endl;
    CImg<unsigned char> tmp(width, height, 1, 1, 0);
    // CImg<unsigned char> tmp2(width, height, 1, 1, 0);

    // filter mask
    // identity // CMU homework kernel
    // int mask[3][3] = {{0,0,0},{0,1,0},{0,0,0}}; //{{1,3,1},{3,5,3},{1,3,1}};
    // int mask[3][3] = {{-1,0,1},{-2,0,2},{-1,0,1}}; //{{1,3,1},{3,5,3},{1,3,1}};//horizontal Sobel kernel
    int mask[3][3] = { {-1,-2,1},{0,0,0},{1,2,1} }; //{{1,3,1},{3,5,3},{1,3,1}};//vertical Sobel kernel

    if (type == 1) {    //Gy
        mask[0][0] = -1;
        mask[0][1] = -2;
        mask[0][2] = 1;
        mask[1][0] = 0;
        mask[1][1] = 0;
        mask[1][2] = 0;
        mask[2][0] = 1;
        mask[2][1] = 2;
        mask[2][2] = 1;
    }
    if (type == 0) {    //Gx
        mask[0][0] = -1;
        mask[0][1] = 0;
        mask[0][2] = 1;
        mask[1][0] = -2;
        mask[1][1] = 0;
        mask[1][2] = 2;
        mask[2][0] = -1;
        mask[2][1] = 0;
        mask[2][2] = 1;
    }

    // edge detection kernel (wiki)
    // int mask[3][3] = {{-1,-1,-1},{-1,8,-1},{-1,-1,-1}}; //{{1,3,1},{3,5,3},{1,3,1}};
    // ridge detect kernel
    // int mask[3][3] = {{0,-1,0},{-1,4,-1},{0,-1,0}}; //{{1,3,1},{3,5,3},{1,3,1}};
    // Box blur
    // int mask[3][3] = {{1/9,1/9,1/9},{1/9,1/9,1/9},{1/9,1/9,1/9}}; //{{1,3,1},{3,5,3},{1,3,1}};
    // Gaussian blur 5x5
    // int mask[5][5] = {{1,4,6,4,1},{4,16,24,16,4},{6,24,36,24,6},{4,16,24,16,4},{1,4,6,4,1}};
    // for(int i = 0; i < sizeof(mask)/sizeof(mask[0]); i++) {
    //         for(int j = 0; j < sizeof(mask)/sizeof(mask[0]); j++) {
    //             mask[i][j] *= 1/256;
    //         }
    // }
    // int mask[5][5] = {{0,0,0,0,0},{0,0,0,0,0},{0,0,1,0,0},{0,0,0,0,0},{0,0,0,0,0}};
    // numrows = sizeof(mask)/sizeof(mask[0]);
    // numcols = sizeof(mask[0])/sizeof(mask[0][0]);

    int m = sizeof(mask) / sizeof(mask[0]) * 0.5;
    // cout << "m=" << m << endl;
    // int size = sizeof(mask)/sizeof(int);
    // cout << width << " " << height << endl;
    for (int r = 0; r < height; ++r)
    {
        for (int c = 0; c < width; ++c)
        {
            for (int i = 0; i < sizeof(mask) / sizeof(mask[0]); i++) {
                for (int j = 0; j < sizeof(mask) / sizeof(mask[0]); j++) {
                    if (r + i - m > 0 && r + i - m < height && c + j - m > 0 && c + j - m < width)
                        tmp(r, c) += img(r + i - m, c + j - m) * mask[i][j]; // 1+1 for 5x5; 1 for 
                    //if ((int)tmp(r, c, 0, 0) > 128) std:cout << (int)tmp(r, c, 0, 0) << " ";
                }
            }
            // tmp(r+m,c+m) /= 256;
        }
    }
    // CImg<unsigned char> tmp2(width, height, 1, 1, 0);
    // tmp2 = img;
    // return img;
    return tmp;
}

int processImageMain()
{

    // CImg<unsigned char> img("/home/jaccobs/bin/projects/learnCPP/cimg_project/lena_128.png");
    CImg<unsigned char> img("lena_512.png");

    // img.display("My first CImg code");
    CImgDisplay disp_orig(img.width(), img.height(),"Original");
    disp_orig.display(img); // use display, not render (see tutorial.cpp inside CImg examples folder)


    CImg<unsigned char> grayscale_img(img, img.width(), img.height(), 1, 1);
    CImgDisplay dispGray(img.width(), img.height(),"Gray");
    dispGray.display(grayscale_img);


    CImg<unsigned char> sobel_x = processImage(grayscale_img, 0);
    CImg<unsigned char> sobel_y = processImage(grayscale_img, 1);
    CImg<unsigned char> gradient_magnitude = calcGradientMagnitude(sobel_x, sobel_y);
    gradient_magnitude = normalize(gradient_magnitude);
    CImgDisplay dispSobel(img.width(), img.height(),"Sobel");
    dispSobel.display(gradient_magnitude);

    // CImg<unsigned char> processedImage2(128,128,1,1);// = processImage(grayscale_img, 0);
    // processedImage2.fill(0);

    // grayscale_img = processImage(grayscale_img, 0);
    // // grayscale_img = processImage(grayscale_img, 1);
    // dispGray.render(grayscale_img);

    while (!disp_orig.is_closed() && !dispGray.is_closed() && !dispSobel.is_closed())
    {
        //usleep(100000);
        // disp.display(img2);
        if (disp_orig.is_resized())disp_orig.resize().display(img);
        if (dispGray.is_resized()) dispGray.resize().display(grayscale_img);
        if (dispSobel.is_resized()) dispSobel.resize().display(gradient_magnitude);
        // Temporize event loop
        cimg::wait(20);
    }
    // disp.display(img);



    return 0;
}

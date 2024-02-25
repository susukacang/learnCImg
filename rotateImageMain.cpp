#include <iostream>
// #include <vector>
// #include <string>
#include <unistd.h>
#include "CImg.h"
using namespace std;
using namespace cimg_library;


CImg<unsigned char> rotateImage(const CImg<unsigned char> &img, int type = 0)
{
    int width = img.width();
    int height = img.height();
    CImg<unsigned char> tmp(width, height, 1, 3, 0);
    // cout << width << " " << height << endl;
    for (int r = 0; r < height; ++r)
    {
        for (int c = 0; c < width; ++c)
        {
            int x, y;
            switch (type)
            {

            // 0 deg
            case 0:
                x = r;
                y = c;
                break;

            // 180 deg
            case 1:
                y = (width - 1) - c;
                x = (height - 1) - r;
                break;

            // X-axis mirror
            // int x = r;
            // int y = (width-1) - c;

            // Y-axis mirror
            // int y = c;
            // int x = (height-1) - r;

            // 90 deg CCW
            case 2:
                y = r;
                x = (width - 1) - c;
                break;

            // 90 deg CW
            case 3:
                x = c;
                y = (height - 1) - r;
                break;

                // default:
            }

            /* code */
            const unsigned int
                val_red = img(x, y, 0),
                val_green = img(x, y, 1),
                val_blue = img(x, y, 2);
            const unsigned target_color[3] = {val_red, val_green, val_blue};

            // if (r == 32) cout << val_red << val_green << val_blue << "\t";

            // tmp(r,c) = img(c,r);
            // tmp(r,c) = img(c,r);
            // tmp(r,c) = img(c,r);
            // tmp(r,c,0) = val_red;
            // tmp(r,c,1) = val_green;
            // tmp(r,c,2) = val_blue;

            tmp.draw_point(r, c, target_color);
            // i++;
        }

        /* code */
    }
    // tmp = img;
    return tmp;
}

int rotateImageMain()
{


    // CImg<unsigned char> img(640,400,1,3);
    // img.fill(0);
    // unsigned char purple[] = {255,0,255};
    // img.draw_text(100,100,"Hello World", purple);
    // img.display("My first CImg code");

    // CImg<unsigned char> img("/home/jaccobs/bin/projects/learnCPP/cimg_project/Lena-Soederberg-256x256-JPEG-image-77-Kbytes.jpg");
    CImg<unsigned char> img("/home/jaccobs/bin/projects/learnCPP/cimg_project/lena_128.png");

    // int height = img.height;
    // int width = img.width;
    // for(int r=0;r<height;r++) {
    //     for(int c=0;r<width;c++) {
    //         img(c,r,0,0);
    //     }
    // }

    // img.display("My first CImg code");
    CImgDisplay disp_orig(img.width(), img.height());
    disp_orig.render(img);
    // CImg<float> img2(320,320);
    // cimg_for(img2, ptr, float) {*ptr = 0;}
    // img2.display("My second CImg code");

    // CImgDisplay first_disp(img, "My first CImg code"), second_disp(img2, "My second CImg code");
    // first_disp.render();
    // second_disp.render();
    // while (!first_disp.is_closed() && !second_disp.is_closed()) {
    //     first_display.wait();
    //     second_display.wait();
    // }
    CImgDisplay disp1(img.width(), img.height());
    CImg<unsigned char> rotatedImage1 = rotateImage(img, 1);
    disp1.render(rotatedImage1);

    CImgDisplay disp2(img.width(), img.height());
    CImg<unsigned char> rotatedImage2 = rotateImage(img, 2);
    disp2.render(rotatedImage2);

    CImgDisplay disp3(img.width(), img.height());
    CImg<unsigned char> rotatedImage3 = rotateImage(img, 3);
    disp3.render(rotatedImage3);



    CImgDisplay dispBig(img.width()*2, img.height()*2);
    // CImg<unsigned char> rotatedImage3 = rotateImage(img, 3);
    dispBig.render(rotatedImage3);

    while (!disp1.is_closed() && !disp2.is_closed() && !disp3.is_closed() && !disp_orig.is_closed())
    {
        usleep(100000);
        // disp.display(img2);
    }
    // disp.display(img);

    return 0;
}
#include <iostream>
#include <vector>
#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

class Pixel{
    public:
        int r,g,b,a;
        Pixel(int R,int G, int B,int A)
        {
            r = R;
            g = G;
            b = B;
            a = A;
        }
};


class Img{
    private:
        int width, height, channels;
        std::vector<std::vector<Pixel>> PixelArray;
    
    public:
        //constructor
        Img(){};

        //image loader constructor
        Img(std::string file)
        {
            Load(file);
        }

        //load png image from file
        void Load(std::string file)
        {
            this->PixelArray = {};
            //load image
	        auto img = stbi_load(file.c_str(), &width, &height, &channels, 1);

            //loop through pixels and add them to array
            for(int y = 0; y != height;y++)
            {
                std::vector<Pixel> xArr;
                for(int x = 0; x != width; x++)
                {
	                unsigned char* pixelOffset = img + (x + width * y) * channels;
                    xArr.push_back(Pixel(pixelOffset[0],pixelOffset[1],pixelOffset[2],channels >= 4 ? pixelOffset[3] : 0xff));
                }
                PixelArray.push_back(xArr);
            }

        }

        //returns rray of pixels (arr[y][x])
        std::vector<std::vector<Pixel>> GetArray()
        {
            return PixelArray;
        }

        int GetHeight()
        {
            return height;
        }

        int GetWidth()
        {
            return width;
        }

        int GetChannels()
        {
            return channels;
        }

};
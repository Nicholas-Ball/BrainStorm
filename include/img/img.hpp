#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

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

        //convert pixel array to stb image
        stbi_uc* AtoS()
        {
            //allocate
            unsigned char* img = malloc(this->width * this->height * this->channels);
            if (img == NULL)
            {
                std::cout << "Couldn't allocate image" << std::endl;
                exit(1);
            }

            for (int i = 0; i != PixelArray.size(); i++)
            {
                *img = (uint8_t)();
             }
        }
    
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
            if (img == NULL)
            {
                std::cout << "Cant't read/find image" << std::endl;
                exit(1);
            }

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

            stbi_image_free(img);
        }



        //save png image to file (or creates one if one doesn't exist)
        void Save(std::string file)
        {
            stbi_write_png(file.c_str(),this->width,this->height,this->channels,)
        }

        //returns array of pixels (arr[y][x])
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
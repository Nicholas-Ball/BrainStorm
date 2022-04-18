#include <iostream>
#include <vector>

#define STB_IMAGE_IMPLEMENTATION
#include "stb/stb_image.h"

#define STB_IMAGE_WRITE_IMPLEMENTATION
#include "stb/stb_image_write.h"

class Img{
    private:
        int width, height, channels;
        uint8_t* pixels;

    
    public:
        
        //constructor
        Img(){
            
        };

        //constructor
        Img(int x, int y, int comp){
            width = x;
            height = y;
            comp = channels;

            pixels = new uint8_t[height * width * channels];
        };

        //image loader constructor
        Img(std::string file)
        {
            Load(file);
        }

        ~Img() {
            free(pixels);
        }

        //load image from file
        void Load(std::string file)
        {
            //load image
	        unsigned char *img = stbi_load(file.c_str(), &this->width, &this->height, &this->channels, 0);

            if (img == NULL)
            {
                std::cout << "Cant't read/find image" << std::endl;
                exit(1);
            }

            pixels = img;
        }

        //get color value at a pixel
        int GetColorValue(int x, int y, int channel)
        {
            unsigned char* pixelOffset = pixels + (x + (width*y)) * channels;
            return pixelOffset[channel];
        }


        //set color value at a pixel
        void SetColorValue(int x, int y, int channel,int value)
        {
            *((pixels + (x + (width * y)) * channels) + channel) = (unsigned char)value;
        }



        //save image to file png (or creates one if one doesn't exist)
        void Save(std::string file)
        {
            stbi_write_png(file.c_str(), width, height, channels, pixels, width * channels);
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
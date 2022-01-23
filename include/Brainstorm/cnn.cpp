#include "brainstorm.hpp"
#include "../img/img.hpp"


//append scan to scan list
void Brainstorm::CNN::AddScan(std::vector<std::vector<double>> scan)
{
    this->Scans.push_back(scan);
}

//generate network and pass matrix for feed forward network
void Brainstorm::CNN::Generate(std::vector<int> matrix)
{
    //create feed forward network
    Brainstorm::FeedForward ff;
    ff.Generate(matrix);
    ff.SetType(Brainstorm::Types::RELU);

    //set network
    this->FFNetwork = ff;
}

//run cnn network
void Brainstorm::CNN::Run(Img image)
{
    //array of pooled data
    std::vector<std::vector<std::vector<double>>> pooled;

    //get image as pixel array
    auto arr  = image.GetArray();
    
    std::vector<std::vector<double>> r;
    std::vector<std::vector<double>> g;
    std::vector<std::vector<double>> b;

    //break image into r,g, and b arrays
    for(int y = 0; y != image.GetHeight();y++)
    {
        std::vector<double> rLine;
        std::vector<double> gLine;
        std::vector<double> bLine;
        for(int x = 0; x != image.GetWidth();x++)
        {
            //add pixel to array
            rLine.push_back(arr[y][x].r);
            gLine.push_back(arr[y][x].g);
            bLine.push_back(arr[y][x].b);
        }
        //add pixel line to image array
        r.push_back(rLine);
        g.push_back(gLine);
        b.push_back(bLine);
    }

    //add r,g,and b images as pooled data
    pooled.push_back(r);
    pooled.push_back(g);
    pooled.push_back(b);

    //preform image scans
    for(int i = 0; i != this->Pool;i++)
    {
        std::vector<std::vector<std::vector<double>>> temp;

        //preform convolution

        //loop through pooled data
        for(int p = 0; p != pooled.size();p++)
        {
            //loop through scans
            for(int s = 0; s != this->Scans.size();s++)
            {
                
            }
        }
    }
    
} 

//get output of network
std::vector<double> Brainstorm::CNN::GetOuput()
{

}

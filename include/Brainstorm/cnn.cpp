#include "brainstorm.hpp"


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

        //preform convolution

        
        std::vector<std::vector<std::vector<double>>> temp;

        //loop through pooled data
        for(int p = 0; p != pooled.size();p++)
        {
            //loop through scans
            for(int s = 0; s != this->Scans.size();s++)
            {
                std::vector<std::vector<double>> data;
                //loop through y axis of pooled data
                for(int y = 0; y != pooled[p].size() - this->Scans[s].size()-1; y++)
                {
                    std::vector<double> xLine;
                    //loop through x axis of pooled data
                    for(int x = 0; x != pooled[p][y].size() - this->Scans[s][0].size();x++)
                    {
                        double calculation = 0;

                        //calculate scan position

                        //loop through scan calulations 
                        for(int yPixel = 0; yPixel != this->Scans[s].size();yPixel++)
                        {
                            for(int xPixel = 0; xPixel != this->Scans[s][yPixel].size();xPixel++)
                            {
                                calculation += pooled[p][y][x] * this->Scans[s][yPixel][xPixel];
                            }
                        }

                        xLine.push_back(calculation);
                    }
                    data.push_back(xLine);
                }
                temp.push_back(data);
            }
        }

        //Max Pool scanned data

        std::vector<std::vector<std::vector<double>>> tempPool;

        

        //loop through scanned data
        for(int s = 0; s != temp.size();s++)
        {
            
            std::vector<std::vector<double>> maxPooled;
            //loop y axis
            for(int y = 0; y != temp[s].size()-1;y++)
            {
                std::vector<double> xAxis;
                //loop x axis
                for(int x = 0; x != temp[s][y].size()-1;x++)
                {
                    double num = temp[s][y][x];

                    if(temp[s][y][x+1] > num)
                    {
                        num = temp[s][y][x+1];
                    }

                    if(temp[s][y+1][x] > num)
                    {
                        num = temp[s][y+1][x];
                    }

                    if(temp[s][y+1][x+1] > num)
                    {
                        num = temp[s][y+1][x+1];
                    }

                    xAxis.push_back(num);
                }
                maxPooled.push_back(xAxis);
            }
            tempPool.push_back(maxPooled);
        }

        pooled = tempPool;
    }

		std::vector<double> flat;

		//std::vector<std::vector<std::vector<double>>> pooled;

		//flatten scanned data
		for(int p = 0; p != pooled.size();p++)
		{
			for(int y = 0; y != pooled[p].size();y++)
			{
				for(int x = 0; x != pooled[p][y].size();x++)
				{
					//add to flattened data
					flat.push_back(pooled[p][y][x]);
				}
			}
		}

		//run network
		this->FFNetwork.Run(flat);
    
} 

//get output of network
std::vector<double> Brainstorm::CNN::GetOuput()
{

}

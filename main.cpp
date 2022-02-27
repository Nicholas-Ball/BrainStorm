#include <iostream>
#include <fstream>
#include "include/img/img.hpp"



int main() {
	Img i("hill.jpg");

	i.Save("hill1.png");

	std::cout << i.GetColorValue(100, 100, 0) << std::endl;
	std::cout << i.GetColorValue(100, 100, 1) << std::endl;
	std::cout << i.GetColorValue(100, 100, 2) << std::endl;

	for (int x = 0; x != i.GetWidth(); x++)
	{
		i.SetColorValue(x, 0, 3, 255);
	}
	i.Save("hill2.png");
} 
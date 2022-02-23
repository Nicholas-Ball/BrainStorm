#include <iostream>
#include <fstream>
#include "include/img/img.hpp"



int main() {
	Img i("dice.png");
	std::cout << i.GetWidth() << std::endl;

} 
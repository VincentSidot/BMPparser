#include <iostream>
#include <string>
#include "bmp.h"



char message[] = "Samuel il suce des queus";
char rep[255];

int main(int argc, char** argv)
{
	File::copy("C:\\Users\\Vincent\\source\\repos\\BMPparser\\BMPparser\\doggo.bmp", "C:\\Users\\Vincent\\source\\repos\\BMPparser\\BMPparser\\doggo.bmp.bak");
	BMP test("C:\\Users\\Vincent\\source\\repos\\BMPparser\\BMPparser\\doggo.bmp");
	

	test.HideMessage(message);
	size_t out = test.ReadMessage(rep);

	std::cout << out << std::endl;
	std::cout << rep << std::endl;



	std::cin.ignore().get();
	return 0;
}
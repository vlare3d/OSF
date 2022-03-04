#include "OsfConfig.h"
#include "osfEncoder.h"

#include "BmpParser.h"

#include <iostream>
#include <stdio.h>
#include <fstream>

const int _sliceLayerCount = 348;

using namespace std;

// raw file is model preview image show in the touch screen.
// after removed the bmp's header data, you can get the raw data
IconImg readRawFile(string filePath)
{
	IconImg img;
	std::ifstream t;
	int length;
	t.open(filePath, ios::in | ios::binary);
	t.seekg(0, std::ios::end);
	length = static_cast<int>(t.tellg());
	t.seekg(0, std::ios::beg);
	char* buffer = new char[length];
	t.read(buffer, length);
	t.close();

	img.data = (unsigned char*)buffer;
	img.len = length;

	return img;
}
IconImg readRawFileFromBmp(string filePath)
{
    int ignore_len = 54;
    IconImg img;
    std::ifstream t;
    int length;
    t.open(filePath, ios::in | ios::binary);
    t.seekg(0, std::ios::end);
    length = static_cast<int>(t.tellg());
    t.seekg(ignore_len, std::ios::beg);
    char* buffer = new char[length-ignore_len];
    t.read(buffer, length-ignore_len);
    t.close();

    img.data = (unsigned char*)buffer;
    img.len = length-ignore_len;

    return img;
}

int main()
{
	OsfConfig config;
	BmpParser bmpParser;

    osfEncoder parser;
	parser.setConfig(config);
	// img:RGB565,raw; 148*80;300*140;208*116;404*240
    // This step is optional. If your print file does not have a 3D preview image,
    // you do not need to load it, which does not affect printing
	parser.addIconImg(readRawFileFromBmp("./imgs/raw/img1.bmp"),
					  readRawFileFromBmp("./imgs/raw/img2.bmp"),
					  readRawFileFromBmp("./imgs/raw/img3.bmp"),
					  readRawFileFromBmp("./imgs/raw/img4.bmp"));

	std::vector<unsigned char> sourceData;
	for (int i = 0; i < _sliceLayerCount; i++) {
		sourceData.clear();
		// bmp:8bit or 24bit
		string imgpath = "./imgs/bmps/" + std::to_string(i) + ".bmp";
		bmpParser.getData(imgpath, sourceData);
		parser.addData(sourceData, i);
	}
	
    //out put the .osf format file
	parser.saveOsfFile("./Comoression.osf");
	
	system("pause");
	return 0;
}

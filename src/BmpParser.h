#pragma once

#include <string>
#include <vector>
#include <windows.h>

class BmpParser {
public:
	bool readBmp(char *bmpName);

	void getData(std::string imgPath, std::vector<unsigned char>& data);

private:
	unsigned char *_bmpBuf = nullptr;
	RGBQUAD* _pColorTable;
	int _imgWidth;
	int _imgHeight;
	int _biBitCount;
};


#include "BmpParser.h"

#include <iostream>

using namespace std;

bool BmpParser::readBmp(char *bmpName)
{
	FILE *fp = fopen(bmpName, "rb");
	if (fp == 0)
		return false;

	fseek(fp, sizeof(BITMAPFILEHEADER), 0);
	BITMAPINFOHEADER head;
	fread(&head, sizeof(BITMAPINFOHEADER), 1, fp);
	_imgWidth = head.biWidth;
	_imgHeight = head.biHeight;
	_biBitCount = head.biBitCount;
	int lineByte = (_imgWidth * _biBitCount / 8 + 3) / 4 * 4;

	if (_biBitCount == 8) {
		_pColorTable = new RGBQUAD[256];
		fread(_pColorTable, sizeof(RGBQUAD), 256, fp);
	}
	_bmpBuf = new unsigned char[lineByte * _imgHeight];
	fread(_bmpBuf, 1, lineByte * _imgHeight, fp);
	fclose(fp);

	return true;
}

void BmpParser::getData(std::string imgPath, std::vector<unsigned char>& data)
{
	if (!readBmp(const_cast<char*>(imgPath.c_str())))
		return;

	int linebyte1 = (_imgWidth * _biBitCount / 8 + 3) / 4 * 4;
	int count_pixel = 0;

	if (_biBitCount == 8) {
		for (int i = _imgHeight - 1; i >= 0; i--) {
			for (int j = 0; j < _imgWidth; j++) {
				int index = *(_bmpBuf + i * linebyte1 + j);
				unsigned char color = (_pColorTable[index].rgbRed + _pColorTable[index].rgbGreen + _pColorTable[index].rgbBlue ) / 3;
				data.push_back(color);
				count_pixel++;
			}
		}
	}

	if (_biBitCount == 24) {
		for (int i = _imgHeight - 1; i >= 0; i--) {
			for (int j = 0; j < _imgWidth; j++) {
				data.push_back(*(_bmpBuf + i * linebyte1 + j * 3));
				count_pixel++;
			}
		}
	}
	delete[]_bmpBuf;
	if (_biBitCount == 8)
		delete[]_pColorTable;
}


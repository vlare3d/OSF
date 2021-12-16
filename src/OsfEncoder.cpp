#include "OsfEncoder.h"

#include <algorithm>
#include <iostream>
#include <fstream>
#include <windows.h>
//#pragma comment(lib, "wsock32.lib")

void osfEncoder::setConfig(OsfConfig config)
{
	_config.resolutionX = config.resolutionX;
	_config.resolutionY = config.resolutionY;
	_config.envelopX = config.envelopX;
	_config.envelopY = config.envelopY;
	_config.envelopZ = config.envelopZ;
	_config.layerThickness = config.layerThickness;
	_config.antialiasingLevel = config.antialiasingLevel;
	_config.antialiasing = config.antialiasing;
	_config.pixelPrecision = config.pixelPrecision;
	_config.xMirrored = config.xMirrored;
	_config.yMirrored = config.yMirrored;
	_config.baseLayerCount = config.baseLayerCount;
	_config.transitionLayerCount = config.transitionLayerCount;
	_config.transitionType = 0;
	_config.exposureTime = config.exposureTime * 1000;
	_config.baseExposureTime = config.baseExposureTime * 1000;
	_config.supportDelay = config.supportDelay * 1000;
	_config.baseSupportDelay = config.baseSupportDelay * 1000;
	_config.supportExposureSwitch = config.supportExposureSwitch;

	_config.transitionLayerDelay = config.transitionLayerDelay * 1000;
	_config.liftingDelayBefore = config.liftingDelayBefore * 1000;
	_config.liftingDelayAfter = config.liftingDelayAfter * 1000;
	_config.retractDelay = config.retractDelay * 1000;
	_config.baseLiftingDistanceLS = config.baseLiftingDistanceLS;
	_config.baseLiftingDistance = config.baseLiftingDistance;
	_config.liftingDistanceLS = config.liftingDistanceLS;
	_config.liftingDistance = config.liftingDistance;
	_config.baseRetractDistanceLS = config.baseRetractDistanceLS;
	_config.baseRetractDistance = config.baseRetractDistance;
	_config.retractDistanceLS = config.retractDistanceLS;
	_config.retractDistance = config.retractDistance;
	_config.curvatureType = config.curvatureType;
	_config.baseLiftingSpeedStart = config.baseLiftingSpeedStart;
	_config.baseLiftingSpeedLS = config.baseLiftingSpeedLS;
	_config.baseLiftingSpeed = config.baseLiftingSpeed;
	_config.baseLiftingCurvature = config.baseLiftingCurvature;
	_config.liftingSpeedStart = config.liftingSpeedStart;
	_config.liftingSpeedLS = config.liftingSpeedLS;
	_config.liftingSpeed = config.liftingSpeed;
	_config.liftingCurvature = config.liftingCurvature;
	_config.baseRetractSpeedStart = config.baseRetractSpeedStart;
	_config.baseRetractSpeedLS = config.baseRetractSpeedLS;
	_config.baseRetractSpeed = config.baseRetractSpeed;
	_config.baseRetractCurvature = config.baseRetractCurvature;
	_config.retractSpeedStart = config.retractSpeedStart;
	_config.retractSpeedLS = config.retractSpeedLS;
	_config.retractSpeed = config.retractSpeed;
	_config.retractCurvature = config.retractCurvature;

	_config.baseLightPwm = 255;
	_config.lightPwm = 255;
	_config.isGray = false;
	_config.isDistortion = false;
}

void osfEncoder::addData(const DlpData& data, const unsigned int& id)
{
	std::vector<PointSet> pointSets;
	raw2Points(data, pointSets);

	SliceData lr;
	lr.layerId = id;
	lr.pointsWithSupport = std::move(pointSets);

	_SliceDataVec.push_back(std::move(lr));
}

void osfEncoder::addData(const DlpData &data, const DlpData &dataSupport, const unsigned int &id)
{
	std::vector<PointSet> htPointSets, htPointSetsSupport;
	raw2Points(data, htPointSets);

	bool noneZero = std::any_of(dataSupport.begin(), dataSupport.end(), [](unsigned char gray) { return (gray != 0); });
	if (noneZero)
		raw2Points(dataSupport, htPointSetsSupport);

	SliceData lr;
	lr.layerId = id;
	lr.pointsWithSupport = std::move(htPointSets);
	lr.pointsSupport = std::move(htPointSetsSupport);

	_SliceDataVec.push_back(std::move(lr));
}

void osfEncoder::addIconImg(const IconImg& img1, const IconImg &img2, const IconImg &img3, const IconImg &img4)
{
	_img1 = img1;
	_img2 = img2;
	_img3 = img3;
	_img4 = img4;
}

void of4B(std::ofstream &out, int n)
{
	int input = htonl(n);
	out.write((char*)(&input), 4);
}

void of2B(std::ofstream &out, short s)
{
	int input = htons(s);
	out.write((char*)(&input), 2);
}

void osfEncoder::saveOsfFile(const std::string& filename)
{
	int parameterCount = 1;

	std::ofstream out;
	out.open(filename, std::ios::binary);

	unsigned int headLen = _img1.len + _img2.len + _img3.len + _img4.len + 145;
	unsigned char logarithmic = 2; 
	of4B(out, headLen);
	of2B(out, _version);
	out << (unsigned char)logarithmic;

	
	_3BOut(out, _img1.len); 
	out.write((const char*)_img1.data, _img1.len);
	_3BOut(out, _img2.len); 
	out.write((const char*)_img2.data, _img2.len);
	_3BOut(out, _img3.len); 
	out.write((const char*)_img3.data, _img3.len);
	_3BOut(out, _img4.len); 
	out.write((const char*)_img4.data, _img4.len);

	of2B(out, _config.resolutionX);
	of2B(out, _config.resolutionY);
	unsigned short pixelPrecision = static_cast<unsigned short>(round(_config.envelopX / _config.resolutionX * 1000 * 100));
	of2B(out, pixelPrecision);
	unsigned char mirrored = ((_config.xMirrored ? 0x01 : 0x00) | (_config.yMirrored ? 0x10 : 0x00));
	out << (unsigned char)(mirrored)
		<< (unsigned char)(_config.baseLightPwm)
		<< (unsigned char)(_config.lightPwm)
		<< (unsigned char)(_config.isGray)
		<< (unsigned char)(_config.isDistortion)
		<< (unsigned char)(_config.supportExposureSwitch);
	of4B(out, static_cast<int>(_SliceDataVec.size()));
	of2B(out, parameterCount);

	of4B(out, static_cast<int>(_SliceDataVec.size() - 1));
	int thickness = static_cast<int>(_config.layerThickness * 1000 * 100);
	_3BOut(out, thickness);
	out << (unsigned char)_config.baseLayerCount;
	_3BOut(out, _config.exposureTime / 10);
	_3BOut(out, _config.baseExposureTime / 10);
	_3BOut(out, _config.supportDelay / 10);
	_3BOut(out, _config.baseSupportDelay / 10);
	out << (unsigned char)_config.transitionLayerCount
		<< (unsigned char)_config.transitionType;
	_3BOut(out, _config.transitionLayerDelay / 10);
	_3BOut(out, _config.liftingDelayBefore / 10);
	_3BOut(out, _config.liftingDelayAfter / 10);
	_3BOut(out, _config.retractDelay / 10);
	_3BOut(out, static_cast<int>(_config.baseLiftingDistanceLS * 1000));
	_3BOut(out, static_cast<int>(_config.baseLiftingDistance * 1000));
	_3BOut(out, static_cast<int>(_config.liftingDistanceLS * 1000));
	_3BOut(out, static_cast<int>(_config.liftingDistance * 1000));
	_3BOut(out, static_cast<int>(_config.baseRetractDistanceLS * 1000));
	_3BOut(out, static_cast<int>(_config.baseRetractDistance * 1000));
	_3BOut(out, static_cast<int>(_config.retractDistanceLS * 1000));
	_3BOut(out, static_cast<int>(_config.retractDistance * 1000));
	out << (unsigned char)(_config.curvatureType);
	of2B(out, static_cast<short>(_config.baseLiftingSpeedStart));
	of2B(out, static_cast<short>(_config.baseLiftingSpeedLS));
	of2B(out, static_cast<short>(_config.baseLiftingSpeed));

	out << (unsigned char)(_config.baseLiftingCurvature);
	of2B(out, static_cast<short>(_config.liftingSpeedStart));
	of2B(out, static_cast<short>(_config.liftingSpeedLS));
	of2B(out, static_cast<short>(_config.liftingSpeed));

	out << (unsigned char)(_config.liftingCurvature);
	of2B(out, static_cast<short>(_config.baseRetractSpeedStart));
	of2B(out, static_cast<short>(_config.baseRetractSpeedLS));
	of2B(out, static_cast<short>(_config.baseRetractSpeed));
	out << (unsigned char)(_config.baseRetractCurvature);
	of2B(out, static_cast<short>(_config.retractSpeedStart));
	of2B(out, static_cast<short>(_config.retractSpeedLS));
	of2B(out, static_cast<short>(_config.retractSpeed));
	out << (unsigned char)(_config.retractCurvature);

	// 20byte reserved
	for (int i = 0; i < 20; i++)
		out << (unsigned char)0x00;

	out << (unsigned char)0x00;

	std::sort(_SliceDataVec.begin(), _SliceDataVec.end(), [](const SliceData &data1, const SliceData &data2) {
		return data1.layerId < data2.layerId; });

	std::cout << "the total  size is " << _SliceDataVec.size() << std::endl;
	for (int i = 0; i < _SliceDataVec.size(); i++) {
		if (_SliceDataVec[i].pointsSupport.size() == 0)
			points2Stream(out, _SliceDataVec[i].pointsWithSupport, HeadType::WithSupport);
		else {
			points2Stream(out, _SliceDataVec[i].pointsWithSupport, HeadType::SupportOnly);
			points2Stream(out, _SliceDataVec[i].pointsSupport, HeadType::WithSupport);
		}
	}

	out.close();
}

void osfEncoder::_3BOut(std::ofstream &out, int data)
{
	unsigned char* p = (unsigned char*)&data;
	out << (unsigned char)p[2] << (unsigned char)p[1] << (unsigned char)p[0];
}

void osfEncoder::points2Stream(std::ofstream &out, const std::vector<PointSet> &htPointSets, HeadType headType)
{
	if (htPointSets.size() == 0)
		return;

	int width = _config.resolutionX;

	int startPixelCount = 0, endPixelCount = 0;
	int startPointSetsNum = 0, endPointSetsNum = 0; //
	for (int i = 0; i < static_cast<int>(htPointSets.size()); i++) {
		if (htPointSets[i].gray != 0 && startPointSetsNum == 0)
			startPointSetsNum = i;

		if (htPointSets[i].gray != 0)
			endPointSetsNum = i;
	}
	for (int i = 0; i < endPointSetsNum; i++) {
		if (i < startPointSetsNum)
			startPixelCount += htPointSets[i].count;
	}

	for (int j = endPointSetsNum + 1; j < static_cast<int>(htPointSets.size()); j++) {
		endPixelCount += htPointSets[j].count;
	}

	int startX = startPixelCount % width;
	int startY = startPixelCount / width;

	int endX = endPixelCount % width;

	int totalSize = endPointSetsNum - startPointSetsNum + 1 + (startX == 0 ? 0 : 1) + (endX == 0 ? 0 : 1);

	if (headType == HeadType::WithSupport)
		out << (unsigned char)0x0D << (unsigned char)0x0A;
	else
		out << (unsigned char)0x0D << (unsigned char)0x0B;
	of4B(out, totalSize);
	of2B(out, startY);
	if (startX > 0) {
		singleSet2Stream(out, 0x00, startX);
	}

	for (int i = startPointSetsNum; i < endPointSetsNum + 1; i++) {
		singleSet2Stream(out, htPointSets[i].gray, htPointSets[i].count);
	}

	if (endX > 0) {
		singleSet2Stream(out, 0x00, endX);
	}
}

void osfEncoder::singleSet2Stream(std::ofstream &out, int gray, int count)
{
	if (count > 1) {
		out << (unsigned char)(gray | 1);
		int pixelCount = static_cast<int>(floor(log2(count)) + 1);
		if (pixelCount <= 7) {
			out << (unsigned char)count;
		} else if (pixelCount <= 14) {
			out << (unsigned char)((2 << 6) + (count >> 8));
			out << (unsigned char)count;
		} else if (pixelCount <= 21) {
			out << (unsigned char)((6 << 5) + (count >> 16));
			out << (unsigned char)(count >> 8);
			out << (unsigned char)count;
		} else {
			out << (unsigned char)((14 << 4) + (count >> 24));
			out << (unsigned char)(count >> 16);
			out << (unsigned char)(count >> 8);
			out << (unsigned char)count;
		}
	}
	else {
		out << (unsigned char)(gray & ~1);
	}
}

void osfEncoder::raw2Points(const DlpData &data, std::vector<PointSet> &pointSets)
{
	const int ceiling = static_cast<int>(data.size());

	int baseNum = 0, offsetNum = 1;
	int curGrayCount = 1;

	while (offsetNum < ceiling) {
		if (data[offsetNum] == data[baseNum])
			curGrayCount++;
		else {
			pointSets.push_back(PointSet(data[baseNum], curGrayCount));
			curGrayCount = 1;
		}
		baseNum = offsetNum++;
	}
	if (curGrayCount > 1)
		pointSets.push_back(PointSet(data[baseNum], curGrayCount));
}

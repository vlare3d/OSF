#pragma once

#include "OsfConfig.h"
#include <vector>
#include <mutex>

struct IconImg 
{
	unsigned char* data;
	unsigned int len;
};

struct PointSet
{
	int gray;
	int count;
	PointSet(int g, int c) : gray(g), count(c) {}
};

struct SliceData
{
	int layerId;
	std::vector<PointSet> pointsWithSupport;
	std::vector<PointSet> pointsSupport;
};

class osfEncoder
{
public:
	using DlpData = std::vector<unsigned char>;

	void setConfig(OsfConfig config);
	void addData(const DlpData& data, const unsigned int& id);
	void addData(const DlpData &data, const DlpData &dataSupport, const unsigned int &id);
	void addIconImg(const IconImg& img1, const IconImg &img2, const IconImg &img3, const IconImg &img4);
	void saveOsfFile(const std::string& filename);

private:
	enum class HeadType { WithSupport, SupportOnly };
	void _3BOut(std::ofstream &out, int data);
	void points2Stream(std::ofstream &out, const std::vector<PointSet> &htPointSets, HeadType headType);
	void singleSet2Stream(std::ofstream &out, int gray, int count);
	void raw2Points(const DlpData &data, std::vector<PointSet> &htPointSets);

	const short _version = 0x0001;
	IconImg _img1, _img2, _img3, _img4;
	OsfConfig _config;
	std::vector<SliceData> _SliceDataVec;

};


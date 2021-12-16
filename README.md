# OSF Open Slice Format Encoder Introduction

OSF-Encoder allows programs to easily export slice image for resin printing to OSF(Open Slice Format). OSF is currently supported by [Vlare Core board and Vlare Slicer](https://www.vlare.net/).
The primary inputs are, BMP, and Slice Setting for printing  Through OSF-Encoder, you can export from your favorite slicer and print on OSF supported hardware.


> Note:Current code only support conversion under Windows. 

# Encoder

```c++
int main()
{
	OsfConfig config;
	BmpParser bmpParser;
    osfEncoder parser;
	parser.setConfig(config);
	// img:RGB565,raw; 148*80;300*140;208*116;404*240
    // This step is optional. If your print file does not have a 3D preview image,
    // you do not need to load it, which does not affect printing
	parser.addIconImg(readRawFile("./imgs/raw/img1.raw"),
					  readRawFile("./imgs/raw/img2.raw"),
					  readRawFile("./imgs/raw/img3.raw"),
					  readRawFile("./imgs/raw/img4.raw"));

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
```

The raw data in above code is BMP picture data without the header. Raw is used for preview. 


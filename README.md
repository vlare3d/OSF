## oslEncoder库介绍

oslEncoder实现了将其他光固化切片软件切出来的数据编码为osl(open source vlare). 
他的主要输入为bmp切片数据和配置文件。 通过oslEncoder，您可以灵活的配置您的切片文件，
并且让这些切片数据稳定的在[Vlare Core](https://www.vlare.net/)主板上打印运行。

> 注意:本开源库目前只支持了window的编译转换。

## 快速上手

如下为调用的全部接口:
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

上述代码中的raw数据来自于将bmp图片数据去掉头数据以后得到，raw用于在触摸屏上显示打印预览图片.

> 注意: 请将编译目录设置为build_dir, 同时要将build_dir里面的imgs解压到build_dir目录里面。 运行demo时，会用到这里面的文件。


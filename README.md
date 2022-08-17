# Introduction
OSF Encoder converts slice image data from Slicer into OSF (Open Slice Format). The core input is BMP data for each layer and print configuration files where you define exposure and other print related setting.  The output is a file format that can be printed on the [Vlare Core]() motherboard.

>Note: Currently, this code base only works in Windows environment. It is intended to serve as an example on how to export to OSF format.

# Quickstart

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
```

The raw data in the above code comes from the bmp image data after removing the header data. Raw is used to display the print preview on the touch screen menu.

>Note:
- Please set the build directory to build_dir, and at the same time extract the imgs in build_dir into the build_dir directory. The files inside will be used when running the demo.

- The code does not depend on the Qt library, you can also use MSVC to compile

# Code description

- OsfConfig.h 
A parameter structure is implemented in the OsfConfig.h header file. All parameters in the OSF format can be set by initializing the values in this structure. If you need to customize your OSF format, most of the work is to modify the values in this file structure.

- OsfEncoder.cpp/OSFEncoder.h 
This tool class implements the entire process of combining parameter configuration and image data to output in OSF format. This class provides four interface functions for the caller to use, setConfig sets the configuration of the current file, and the input is the OSFConfig structure. addData is to add image data, mainly bmp pixel data. addIconImg is to add a preview image to this print file, there are four that can be added. If it is not added, it will not affect printing.

- BmpParser.cpp/BmpParser.h 
This class is mainly to parse bmp image data, after reading the data, convert it and store it in the vector container.

- convertBMP.py
Convert images from png, jpg, bmp format to rgb565 data for generating preview image data. Need install PIL python image lib.

- convertBMP.c++
Convert images from png, jpg, bmp format to rgb565 data for generating preview image data. This function implementation needs to rely on Qt.

# OSF format data

## 1. OSF format table


The data in the OSF format is written in binary form. The fil structure is to write parameters and data of different lengths into the OSF file in the order and when we parse the OSF, we only need to read these contents in a similar order.
A completed OSF file data format, please refer to this table

| Field description                                          | Storage length                |
|:-------------------------------------------------------:|----------------------------------|
| Head length                                           | 4Bytes                         |
| Version number                                        | 2Bytes //Version：0x0001                                                                                                                                 |
| Image logarithm                                       | 1Bytes //Default Value:：2                                                                                                                               |
| Picture 1 Data Length (Preview Picture 1)             | 3Bytes                                                                                                                                                  |
| Picture 1 data (RGB565 RAW)                           | w\*h148\*80  148\*80\*2Bytes                                                                                                                                |
| Picture 2 Data Length (Preview Picture 2)             | 3Bytes                                                                                                                                                  |
| Picture 2 data (RGB565 RAW)                           | w\*h300\*140  300\*140*\2Bytes                                                                                                                              |
| Picture 3 data (RGB565 RAW)                           | 3Bytes                                                                                                                                                  |
| Picture 3 data (RGB565 RAW)                           | w\*h208\*116 208\*116\*2Bytes                                                                                                                               |
| Picture 4 data (RGB565 RAW)                           | 3Bytes                                                                                                                                                  |
| Picture 4 data (RGB565 RAW)                           | w\*h404\*240  404\*240\*2Bytes                                                                                                                              |
| Image resolution X                                    | 2Bytes                                                                                                                                                  |
| Image resolution Y                                    | 2Bytes                                                                                                                                                  |
| Pixel accuracy                                        | 2 Bytes (um, magnification 100 times: such as 100um write 10000, the same below)                                                                        |
| Mirroring                                             | 1 Bytes (0x00 not mirrored, 0x01 X-axis mirroring, 0x02 Y-axis mirroring, 0x03 XY-axis mirroring)                                                       |
| Bottom light intensity PWM                            | 1Bytes                                                                                                                                                  |
| Light intensity                                       | 1Bytes                                                                                                                                                  |
| Grayscale enable                                      | 1Bytes(Default Value:0x00)                                                                                                                              |
| Distortion enabled                                    | 1Bytes                                                                                                                                                  |
| Support delayed exposure activation                   | 1Bytes（0x01Enable 0x00Disable）                                                                                                                          |
| Total number of layers                                | 4Bytes                                                                                                                                                  |
| Number of parameter sets                              | 2Bytes(Default Value: 0x0001)                                                                                                                           |
|                                                       |                                                                                                                                                         |
| Number (the end layer number of this parameter)       | 4Bytes                                                                                                                                                  |
| Layer thickness                                       | 3Bytes（um  um write magnification one hundred times）                                                                                                    |
| Number of bottom layers                               | 1Bytes                                                                                                                                                  |
| Exposure time                                         | 3Bytes（10ms As a unit）                                                                                                                                  |
| Bottom exposure time                                  | 3Bytes（10ms As a unit）                                                                                                                                  |
| Support delay time                                    | 3Bytes（10ms As a unit）                                                                                                                                  |
| Bottom support delayed exposure                       | 3Bytes（10ms As a unit）                                                                                                                                  |
| Number of transition layers                           | 1Bytes                                                                                                                                                  |
| Transition type                                       | 1Bytes（0x00 linear transition）                                                                                                                          |
| Transition layer interval time difference             | 3Bytes（10ms As a unit）                                                                                                                                  |
| Static time before lifting                            | 3Bytes（10ms As a unit）                                                                                                                                  |
| Static time after lifting                             | 3Bytes（10ms As a unit）                                                                                                                                  |
| Static time after return                              | 3Bytes（10ms As a unit）                                                                                                                                  |
| Lifting distance of bottom layer (slow speed)         | 3Bytes（um）                                                                                                                                              |
| Lifting distance of bottom layer (total)              | 3Bytes（um）                                                                                                                                              |
| Lifting distance (slow speed)                         | 3Bytes（um）                                                                                                                                              |
| Lifting distance (total)                              | 3Bytes（um）                                                                                                                                              |
| Bottom Retraction distance (slow speed)               | 3Bytes（um）                                                                                                                                              |
| Bottom Retraction distance (total)                    | 3Bytes（um）                                                                                                                                              |
| Retraction distance (slow speed)                      | 3Bytes（um）                                                                                                                                              |
| Retraction distance (total)                           | 3Bytes（um）                                                                                                                                              |
| Curvature type                                        | 1 Bytes (0x00: S-shaped acceleration, 0x01: T-shaped acceleration, Default Value: S-shaped acceleration, currently only supports S-shaped acceleration) |
| Bottom lifting speed (start)                          | 2Bytes（mm/min）                                                                                                                                          |
| Bottom lifting speed (slow speed)                     | 2Bytes（mm/min）                                                                                                                                          |
| Lifting speed of the bottom layer (fast)              | 2Bytes（mm/min）                                                                                                                                          |
| Bottom uplift curvature                               | 1Bytes (Default Value::5)                                                                                                                               |
| Lifting speed (start)                                 | 2Bytes（mm/min）                                                                                                                                          |
| Lifting speed (slow speed)                            | 2Bytes（mm/min）                                                                                                                                          |
| Lifting speed (fast)                                  | 2Bytes（mm/min）                                                                                                                                          |
| Lifting curvature                                     | 1Bytes (Default Value::5)                                                                                                                               |
| Retraction speed of bottom layer (start)              | 2Bytes（mm/min）                                                                                                                                          |
| Bottom Retraction speed (slow speed)                  | 2Bytes（mm/min）                                                                                                                                          |
| Retraction speed of bottom layer (fast)               | 2Bytes（mm/min）                                                                                                                                          |
| Bottom backhaul curvature                             | 1Bytes (Default Value::5)                                                                                                                               |
| Retraction speed (start)                              | 2Bytes（mm/min）                                                                                                                                          |
| Retraction speed (slow speed)                         | 2Bytes（mm/min）                                                                                                                                          |
| Layer back speed (fast)                               | 2Bytes（mm/min）                                                                                                                                          |
| Retraction curvature                                  | 1Bytes (Default Value::5)                                                                                                                               |
| Base lifting speed (End)                  | 2Bytes(mm/min)                                                           |

| Base lifting Curvature  (Decelerate)                 | 1Bytes (Default Value:5)                                               |

| Lifting speed (End)                  | 2Bytes (mm/min)                                              |

| Lifting Curvature  (Decelerate)                  | 1Bytes (Default Value:5)                                              |

| Base Retract speed (End)                 | 2Bytes (mm/min)                                              |

| Base Retract Curvature  (Decelerate)                 | 1Bytes (Default Value:5)                                             |

| Retract speed (End)                  | 2Bytes (mm/min)                                             |

| Retract  Curvature  (Decelerate)                  | 1Bytes (Default Value:5)                                             |

|  Base Rest Before Lift                  | 2Bytes (unit:10ms)                                             |

| Base Rest After Lift                 | 2Bytes (unit:10ms)                                             |

|  Base Rest After Retraction                  | 2Bytes (unit:10ms)                                             |

| Number of reserved                                    | 2Bytes                                                                                                                                                 |
| Protocol type                                         | 1Bytes（Default Value: 0x00）                                                                                                                             |

>**Number of reserved** Note:For versions after vlare slicer-2.02, the parameters reserved here will be used. When exporting to osf format, all the values here can be set to 0, which has no effect on printing.

## 2. Image compression method

In light based resin printing, layer image data accounts for most of the file size. Here is an introduction to how OSF compresses image data. The image compression consists of two parts:

- Picture pixel data header
- Picture pixel data

### 2.1 Single layer data header
|Field description Storage |length/Specific value|
|--------------------------|---------------------|
|The beginning of the layer mark| OD OA begins, indicating that the model + support is included; the beginning of 0D 0B, indicating that the layer only has support data|
|Total number of pixels| 4 Bytes|
|Start line number | 2 Bytes|

### 2.2 Single-layer pixel data encoding and compression

Pixel data adopts variable length coding, and the pixel gray level is represented by 1 byte: Bit: 07-01 Pixel value (the original pixel is rounded down to the last bit) 
Bit: 00 Single and continuous flag bit. (0 is a single pixel and 1 is a continuous pixel)

|bit 07| bit 06| bit 05| bit 04| bit 03| bit 02| bit 01| bit 00|
|---|---|--|--|---|---|---|---|
|Pixel value bit| Pixel value bit|Pixel value bit| Pixel value bit| Pixel value bit| Pixel value bit| Pixel value bit| Single or continuous pixel flag bit|

If it is a continuous pixel, the number of continuous pixels is given by the following 1~4 bytes. When the data length of continuous pixels requires more than one byte for description, such as more than 256 continuous pixels, the bytes are in big-endian order arrangement. This is the only situation in the osf file format where numbers are represented by big endian. The specific description is as follows:

0b0xxxxxxx: 7-bit run length.
0b10xxxxxx: 14-bit running length, composed of 05-00 bits of this byte and the next byte, among which: 05-00 bits are high bits.
0b110xxxxx: 21-bit running length, composed of 04-00 bits of this byte and the next two bytes, among which: 04-00 bits are high bits.
0b1110xxxx: 28-bit running length, composed of 03-00 bits of this byte and the next three bytes, among which: 03-00 bits are high bits.

For example: 0xFE is a single pixel with a value of 254. 0xFF 0x2a is the operation of 42 pixels, the value is 254 (that is, the maximum gray value of the pixel is 254, if it is 0xFF, the lowest bit must be discarded to read. Each pixel value must first read its lowest Bit, and then to judge whether his next bit is a pixel value or a value representing the number of pixel values.) End: End with data from the last line.



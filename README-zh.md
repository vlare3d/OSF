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

> 注意: 

1. 请将编译目录设置为build_dir, 同时要将build_dir里面的imgs解压到build_dir目录里面。 运行demo时，会用到这里面的文件。

2. 代码不依赖Qt库，您也可以使用MSVC来进行编译

## 代码说明

- OsfConfig.h  这个头文件中实现了一个参数结构体，Osf格式的所有参数都可以通过初始化这个结构体里面的值来设置。如果你需要自定义你的Osf格式，那么绝大部分工作，都是在修改这个文件结构体里面的值。

- OsfEncoder.cpp/OsfEncoder.h 这个工具类实现了将参数配置还有图片数据结合，输出为osf格式的全部过程。这个类提供了四个接口函数供调用者使用，setConfig设置当前的文件的配置，输入为osfConfig结构体。 addData是添加图片数据，主要是bmp的像素数据。 addIconImg是为这个打印文件添加预览图片，有四张可以添加。 不添加，对打印也不会产生影响。

- BmpParser.cpp/BmpParser.h 该类主要是解析bmp图片数据，读取数据后，转化存储在vector容器里面。

## OSF 格式详细介绍


### 1. osf格式数据
OSF格式内的数据是二进制形式写入的。 我们按照一定的规则，将不同长度的参数和数据，依次写入到osf的文件中，这样，我们在解析osf的时候，也只需要按照类似的顺序，读取这些内容。

一个完成的osf文件数据格式，请参考这个表格:https://docs.google.com/spreadsheets/d/11WFr6VWvb6UGBeNqTODERWp2HCbMsIum02xYB6GEid0/edit#gid=1931565076

| 字段描述                      | 储存长度                                                           |
|:---------------------------:|-------------------------------------------------------------------|
| 头部长度                      | 4Bytes                                                            |
| 版本号                       | 2Bytes //Version：0x0001                                           |
| 图像对数                      | 1Bytes //Default Value:：2                                         |
| 图片1长度(预览图1)               | 3Bytes                                                            |
| 图片1数据（RGB565 RAW）         | w*h148*80  148*80*2Bytes                                          |
| 图片2长度(预览图2)               | 3Bytes                                                            |
| 图片2数据（RGB565 RAW）         | w*h300*140  300*140*2Bytes                                        |
| 图片3长度(预览图3)               | 3Bytes                                                            |
| 图片3数据（RGB565 RAW）         | w*h208*116 208*116*2Bytes                                         |
| 图片4长度(预览图4)               | 3Bytes                                                            |
| 图片4数据（RGB565 RAW）         | w*h404*240  404*240*2Bytes                                        |
| 图像分辨率X                    | 2Bytes                                                            |
| 图像分辨率Y                    | 2Bytes                                                            |
| 像素精度                      | 2Bytes（um 写入放大100倍：比如100um写入10000 下同）                             |
| 镜像                        | 1Bytes（0x00 未镜像    0x01  X轴镜像    0x02  Y轴镜像    0x03 XY轴镜像）        |
| 底层光强PWM                   | 1Bytes                                                            |
| 光强PWM                     | 1Bytes                                                            |
| 灰度启用                      | 1Bytes(Default Value:0x00)                                        |
| 畸变启用                      | 1Bytes                                                            |
| 支撑延时曝光启用                  | 1Bytes（0x01Enable 0x00Disable）                                    |
| 总层数                       | 4Bytes                                                            |
| 参数套数                      | 2Bytes(Default Value: 0x0001)                                     |
|                           |                                                                   |
| 编号（此参数结束层号）               | 4Bytes                                                            |
| 层厚                        | 3Bytes（um  写入放大一百倍）                                               |
| 底层数                       | 1Bytes                                                            |
| 曝光时间                      | 3Bytes（10ms As a unit）                                            |
| 底层曝光时间                    | 3Bytes（10ms As a unit）                                            |
| 支撑延时时间                    | 3Bytes（10ms As a unit）                                            |
| 底层支撑延时曝光                  | 3Bytes（10ms As a unit）                                            |
| 过渡层数                      | 1Bytes                                                            |
| 过渡类型                      | 1Bytes（0x00 线性过渡）                                                 |
| 过渡层间隔时间差                  | 3Bytes（10ms As a unit）                                            |
| 抬升前的静止时间                  | 3Bytes（10ms As a unit）                                            |
| 抬升后的静止时间                  | 3Bytes（10ms As a unit）                                            |
| 回程后的静止时间                  | 3Bytes（10ms As a unit）                                            |
| 底层抬升距离（慢速）                | 3Bytes（um）                                                        |
| 底层抬升距离（总）                 | 3Bytes（um）                                                        |
| 抬升距离（慢速）                  | 3Bytes（um）                                                        |
| 抬升距离（总）                   | 3Bytes（um）                                                        |
| 底层回程距离（慢速）                | 3Bytes（um）                                                        |
| 底层回程距离（总）                 | 3Bytes（um）                                                        |
| 回程距离（慢速）                  | 3Bytes（um）                                                        |
| 回程距离（总）                   | 3Bytes（um）                                                        |
| 曲率类型                      | 1Bytes（0x00:S型加速度  0x01:T型加速度  Default Value:S型加速度    目前只支持S型加速度） |
| 底层抬升速度（启动）                | 2Bytes（mm/min）                                                    |
| 底层抬升速度（慢速）                | 2Bytes（mm/min）                                                    |
| 底层抬升速度（快速）                | 2Bytes（mm/min）                                                    |
| 底层抬升曲率                    | 1Bytes (Default Value::5)                                         |
| 抬升速度（启动）                  | 2Bytes（mm/min）                                                    |
| 抬升速度（慢速）                  | 2Bytes（mm/min）                                                    |
| 抬升速度（快速）                  | 2Bytes（mm/min）                                                    |
| 抬升曲率                      | 1Bytes (Default Value::5)                                         |
| 底层回程速度（启动）                | 2Bytes（mm/min）                                                    |
| 底层回程速度（慢速）                | 2Bytes（mm/min）                                                    |
| 底层回程速度（快速）                | 2Bytes（mm/min）                                                    |
| 底层回程曲率                    | 1Bytes (Default Value::5)                                         |
| 回程速度（启动）                  | 2Bytes（mm/min）                                                    |
| 回程速度（慢速）                  | 2Bytes（mm/min）                                                    |
| 回层速度（快速）                  | 2Bytes（mm/min）                                                    |
| 回程曲率                      | 1Bytes (Default Value::5)                                         |
| 预留Bytes数                  | 20Bytes                                                           |
| 协议类型                      | 1Bytes（Default Value: 0x00）                                       |


### 2. 图像的压缩方式

光固化打印中，数据最多的就是图片像素数据。这里针对osf是如何压缩图像数据进行介绍。
图片的压缩包含了两部分:
- 图片头
- 图片像素数据

#### 2.1 单层数据头


| 字段描述                      | 储存长度/具体数值                                                 |
|:---------------------------:|-------------------------------------------------------------------|
| 层开始标志                 | OD OA开头，表示包含模型+支撑; 0D 0B开头，表示该层只有支撑数据         |
| 像素点总数				 | 4Bytes															|
| 开始行号					 | 2Bytes															|


#### 2.2 单层像素数据编码压缩

像素数据采用可变长度编码，像素灰度由1个字节表示:
位: 07-01  像素值(原像素舍去最后一位)
位:00    单一和连续的标志位。(0为单一像素 1为连续像素)


|bit 07| bit 06| bit 05| bit 04| bit 03| bit 02| bit 01| bit 00|
|---|---|--|--|---|---|---|---|---|
|像素值位| 像素值位|像素值位| 像素值位| 像素值位| 像素值位| 像素值位| 单一还是连续像素标志位|

如果是连续像素，连续像素的数量由一下的1~4个字节给出.当连续像素的数据长度需要一个一个以上字节进行说明时，比如超过256个连续像素，字节按照大端顺序排列。 这是osf文件格式中唯一以大端表示数字的情况。具体的描述如下:

- 0b0xxxxxxx:  7 位运行长度。
- 0b10xxxxxx: 14 位运行长度，本字节的05-00位和下一个字节构成，其中:05-00位为高位。
- 0b110xxxxx: 21 位运行长度，本字节的04-00位和下两个字节构成,其中:04-00位为高位。
- 0b1110xxxx: 28 位运行长度，本字节的03-00位和下三个字节构成,其中:03-00位为高位。

举个例子:
0xFE 是值为 254 的单个像素。
0xFF 0x2a 是 42 个像素的运行，值为 254(也就是说，像素的灰度值最大为254, 如果是0xFF,要舍去最低位来读。 每个像素值都要先读取他的最低位，然后来判断他的下一位是像素值还是表示像素值数量的值。)
结束: 从最后一行有数据结束。



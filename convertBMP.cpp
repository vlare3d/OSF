#include <fstream>

#include <QImage>
#include <QFileInfo>
#include <QFileDialog>


void ConvertImageToRGB565Raw(QString filename)
{
    QFileInfo file_info(filename);
    QString save_name = file_info.baseName()+".raw";
    std::ofstream out;
    out.open(save_name.toStdString(), std::ios::binary);

    QImage bmp_img(file_info.absoluteFilePath());
    for(int j=0; j<bmp_img.height(); j++)
    {
        for(int i=0; i<bmp_img.width(); i++)
        {
            uint16_t cc=0;
            QRgb rgb = bmp_img.pixel(i, j);
            uint8_t r = (uint8_t)qRed(rgb);
            uint8_t g = (uint8_t)qGreen(rgb);
            uint8_t b = (uint8_t)qBlue(rgb);
            cc += ((r>>3)&0x1f);
            cc += (((g>>2)&0x3f)<<5);
            cc += (((b>>3)&0x1f)<<11);
            char bytes[2];
            bytes[0] = cc & 0xFF;
            bytes[1] = (cc>>8) & 0xFF;
            out.write((char*)bytes,2);
        }
    }
    out.close();
}
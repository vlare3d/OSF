# -*- coding: utf-8 -*-
from PIL import Image
import struct, os, sys


    
def write_bin(f, pixel_list):
    for pix in pixel_list:
        r = (pix[0] >> 3) & 0x1F
        g = (pix[1] >> 2) & 0x3F
        b = (pix[2] >> 3) & 0x1F
        f.write(struct.pack('H', (r << 11) + (g << 5) + b))


if __name__ == '__main__':
    all_path = ["./preview_osf_1.bmp","./preview_osf_2.bmp","./preview_osf_3.bmp","./preview_osf_4.bmp"]
    for in_path in all_path:
        if os.path.exists(in_path) == False: error('not exists: ' + in_path)
        
        body, _ = os.path.splitext(in_path)
        out_path = body + '.raw'

        img = Image.open(in_path).convert('RGB')
        pixels = list(img.getdata())
        # print pixels
        
        with open(out_path, 'wb') as f:
            write_bin(f, pixels)
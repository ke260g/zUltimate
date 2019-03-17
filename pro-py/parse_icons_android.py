from PIL import Image, ImageDraw
import os

src_path='ic_launcher.png'
dpi_size = {
 "ldpi":(36,36),
 "mdpi":(48,48),
 "hdpi":(72,72),
 "xhdpi":(96,96),
 "xxhdpi":(144,144),
 "xxxhdpi":(192,192),
}

prefix='mipmap' # mipmap or drawable
dpis=['mdpi', 'hdpi', 'xhdpi', 'xxhdpi', 'xxxhdpi']

img = Image.open(open(src_path, 'rb'))
for dpi in dpis:
    dst_path = os.path.join(prefix + '-' + dpi, src_path)
    tmp = img.resize(dpi_size[dpi])
    tmp.save(open(dst_path, 'wb'))
img.close()
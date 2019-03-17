from PIL import Image
filename="test.jpg"
img = Image.open(filename)
if img.mode == 'RGB':
    img.putalpha(Image.new('L', img.size, 255))
img.save(filename, "PNG")
from PIL import Image, ImageDraw

w = 80  
h = 10
mImage = Image.new('RGBA',(w, h), (0,255, 255, 0))
mDraw = ImageDraw.Draw(mImage)
mDraw.ellipse((0, 0, mImage.width, mImage.height), (85, 85, 85))
# print(list(mImage.getdata()))
mImage.save(open('buttom_shadow.png', 'wb'))
mImage = Image.open(open('buttom_shadow.png', 'rb'))
mImage.show()
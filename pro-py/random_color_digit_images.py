from PIL import Image, ImageDraw
import os
# 画数字图片
'''
P:      means points
Widget: means a rectangle or a line
	P0 ==Widget1== P1
	||             ||
  Widget0        Widget2
	||             ||
	P1 ==Widget6== P2
	||             ||
  Widget5        Widget3
	||             ||
	P4 ==Widget4== P3

# [x, y]
# 一个point， 对应的坐标, r表示widget长度
digits2points = [
    [0, 0],
    [r, 0],
    [r, r],
    [r, r * 2],
    [0, r * 2],
    [0, r],
]
'''

# 一个数字, 对应的 widget
digits2widget = [
    [0, 1, 2, 3, 4, 5],     # 0
    [2, 3],                 # 1
    [1, 2, 4, 5, 6],        # 2
    [1, 2, 3, 4, 6],        # 3
    [0, 2, 3, 6],           # 4
    [0, 1, 3, 4, 6],        # 5
    [0, 1, 3, 4, 5, 6],     # 6
    [1, 2, 3],              # 7
    [0, 1, 2, 3, 4, 5, 6],  # 8
    [0, 1, 2, 3, 4, 6],     # 9
]
# 一个widget, 对应的point
widget2points = [
    [0, 5],
    [0, 1],
    [1, 2],
    [2, 3],
    [3, 4],
    [4, 5],
    [5, 2],
]

class ImageDigitMaker:
    def __init__(self, radius, bias, foreground, background):
        self.radius = radius
        self.origin = [bias, bias]
        self.width = radius + bias*2
        self.height = radius*2 + bias*2
        self.image = Image.new('RGBA', (self.width, self.height), (255, 255, 255, 0))
        self.draw = ImageDraw.Draw(self.image)
        self.foreground = foreground
        self.background = background
        self.draw.rectangle([0,0, self.width, self.height], background)
    def get_points(self):
    	r = self.radius
    	o = self.origin
    	# 一个point， 对应的坐标
    	return [
    		(0 + o[0], o[1] + 0),
		    (r + o[0], o[1] + 0),
		    (r + o[0], o[1] + r),
		    (r + o[0], o[1] + r * 2),
		    (0 + o[0], o[1] + r * 2),
		    (0 + o[0], o[1] + r),
    	]
    def draw_widget(self, fromP, toP):
        if fromP[0] == toP[0]: # x轴相等, vertical widget
            pass
        if fromP[1] == toP[1]: # y轴相等, horizontal widget
            pass
        self.draw.line([fromP, toP], self.foreground, 3)
    def draw_digit(self, digit):
        points_map = self.get_points()
        for widget in digits2widget[digit]:
            fromP =  points_map[widget2points[widget][0]]
            toP = points_map[widget2points[widget][1]]
            self.draw_widget(fromP, toP)
    def show(self):
        self.image and self.image.show()
    def dump(self, fpath):
    	self.image.save(open(fpath, 'wb'), 'PNG')

# 一个画图软件只能下面这些 字符格子的
# +-\/, a-zA-Z, 0-9

back_fore_colors = [
    ('yellow', 'red'),
    ('red', 'yellow'),
    ('cyan', 'gray'),
    ('white', 'black'),
    ]
out_dir='image_digit'
os.path.isdir(out_dir) or os.mkdir(out_dir)
for (fore, back) in back_fore_colors:
    cdir = os.path.join(out_dir, fore + '_' + back)
    os.path.isdir(cdir) or os.mkdir(cdir)
    for i in range(10):
        mIDM = ImageDigitMaker(10, 5, fore, back)
        mIDM.draw_digit(i)
        mIDM.dump(os.path.join(cdir, '%d_%s_%s.png'%(i, fore, back)))
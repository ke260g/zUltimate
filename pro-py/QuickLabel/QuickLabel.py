# -*- coding: UTF-8 -*-
from PIL import Image, ImageFont, ImageDraw # 导入模块
from functools import reduce
import os
import math

'''
QuickLabelUI
QuickLabelModel.NPC.
QuickLabelModel.Monster
把label用image替代,且image在同一个图集中会降低DC
理论上 所有 label放在一个图集中 优化效果最好
然而这样 图集 会超大, 所以稍微分类:
    场景内怪物 + NPC
    UI
而且 不在同一图集中的label不要放在同一个容器中
'''

def QuickLabel(name, labels, color, font=ImageFont.truetype('./font/simhei.ttf', size = 20, encoding="utf-8")):
    os.path.isdir(name) or os.mkdir(name)
    text = None
    if isinstance(labels, dict):
        text = lambda label: labels[label]
    elif isinstance(labels, list):
        text = lambda label: label
    else:
        Exception('labels must be dict or list')
    
    print(labels)
    for label in labels:
        label = label if isinstance(label, str) else str(label)
        mText = text(label)
        size = (math.ceil(len(mText)*font.size), math.ceil(font.size))
        mImage = Image.new('RGBA', size, (255,255, 255, 0))
        mDraw = ImageDraw.Draw(mImage)
        mDraw.text((0,0), mText, fill = color, font = font)
        mImage.save(os.path.join(name, label + '.png'))

QuickLabel('QuickLabel',
   [1913, 1914, 1915,1916,1917, 1918,1919,
	'NPC一号', 'NPC二号', 'NPC三号',
	'NPC四号', 'NPC五号', 'NPC六号',
	'NPC七号', 'NPC八号', 'NPC九号'],
	(255,0,0))
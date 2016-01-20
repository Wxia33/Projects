# -*- coding: utf-8 -*-
"""
Created on Wed Sep 16 16:59:58 2015

@author: Oracle
"""

#from PIL import Image, ImageDraw
#im = Image.new('RGBA', (2400, 1600), (30, 30, 0, 0)) 
#x = 0
#while True:    
#    draw = ImageDraw.Draw(im) 
#    draw.line((1000,10, 1000,1600), fill=200)
#    draw.line((800,10, 800,1600), fill=200)
#    draw.rectangle((1000 + x,0, 1100,1600),fill=(255,255,255))
#    draw.rectangle((1300 + x,0, 1200,1600),fill=(255,255,255))
#    x = x + 1
#    im.show()
import visual
rt = visual.shapes.rectangle(pos=(-2,3), width=5, height=3)
visual.extrusion(shape = rt)
visual.curve(pos=rt.contour(0))
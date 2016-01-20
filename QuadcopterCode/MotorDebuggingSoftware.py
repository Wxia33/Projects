#!/usr/bin/python

from Adafruit_PWM_Servo_Driver import PWM
import time
import curses

pwm = PWM(0x40, debug=True)

pwm.setPWMFreq(60)                        # Set frequency to 60 Hz

screen = curses.initscr()                 
curses.noecho()                           # turn off input echoing
curses.cbreak()                           # respond to keys immediately
screen.keypad(True)                       # map arrow keys to special values

done=False
numbmax = 600
numbmin = 200
numb1 = numbmin
numb2 = numbmin
numb3 = numbmin
numb4 = numbmin
inc = 10

try:  #press b to set bottom value
    while not done:            #once set bottom value, motors are ready
      char = screen.getch()
      if char == ord('m'):
         done=True
      else:
         if char == ord('q'):
            numb1 += inc
         if char == ord('e'):
            numb1 -= inc
         if char == ord('z'):
            numb2 += inc
         if char == ord('c'):
            numb2 -= inc
         if char == ord('a'):
            numb3 += inc
         if char == ord('d'):
            numb3 -= inc
         if char == ord('f'):
            numb4 += inc
         if char == ord('g'):
            numb4 -= inc
         if char == ord('w'):   # Throttle
            numb1 += inc
            numb2 += inc
            numb3 += inc
            numb4 += inc
         if char == ord('s'):
            numb1 -= inc
            numb2 -= inc
            numb3 -= inc
            numb4 -= inc
         if char == ord('b'):
            numb1 = numbmin
            numb2 = numbmin
            numb3 = numbmin
            numb4 = numbmin
            #numb3
            #numb4
      pwm.setPWM(0, 0, numb1)              #Quad follows X configuration
      pwm.setPWM(1, 0, numb2)              #  0  1             1  4
      pwm.setPWM(2, 0, numb3)              #   \/               \/
      pwm.setPWM(3, 0, numb4)              #   /\               /\  
                                           #  2  3             3  2 
finally:
    # shut down cleanly                 (Channel Number)   (Respective Values
    #                                                         Used in Code)
   curses.nocbreak(); screen.keypad(0); curses.echo()
   curses.endwin()
   print(numb1)
   print(numb3)

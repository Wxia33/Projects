import smbus
import math
import time
import threading
from Adafruit_PWM_Servo_Driver import PWM

pwm = PWM(0x40, debug=True)
pwm.setPWMFreq(60)

#Addresses on MPU6050 board to read data
gyro_x_addr = 0x43
gyro_y_addr = 0x45
gyro_z_addr = 0x47
accel_x_addr = 0x3B
accel_y_addr = 0x3D
accel_z_addr = 0x3F

Kp = 1
Ki = 0
Kd = .05
SumMax = 30
SumMin = -30
SumError = 0
NewXerror = 0
Newaccelx = 0
NewYerror = 0

#Sets reference point PID algorithm attempts to achieve
std_mtr_speed = 230
motorspeed0 = stdmtrspeed
motorspeed1 = stdmtrspeed
motorspeed2 = stdmtrspeed
motorspeed3 = stdmtrspeed

radians = 0

def read(addr):
    high = bus.read_byte_data(address, addr)
    low = bus.read_byte_data(address, addr+1)
    val = (high << 8) + low
    return val

def read_data(addr):
    val = read(addr)
    if (val >= 0x8000):
        return -((65535 - val) + 1)
    else:
        return val
    
#Math to determine yaw and pitch based on measurements
def dist(a,b):
    return math.sqrt((a*a) + (b*b))

def rotX(x,y,z): #Roll
    radians = math.atan2(y, dist(x,z))
    return math.degrees(radians)

def rotY(x,y,z): #Pitch
    radians = math.atan2(x, dist(y,z))
    return -math.degrees(radians)

def set_data():
    global gyrox
    global gyroy
    global gyroz
    global accelx
    global accely
    global accelz
    gyrox = read_data(gyro_x_addr) / 131
    gyroy = read_data(gyro_y_addr) / 131
    gyroz = read_data(gyro_z_addr) / 131
    accelx = read_data(accel_x_addr) / 16384.0
    accely = read_data(accel_y_addr) / 16384.0
    accelz = read_data(accel_z_addr) / 16384.0
    return

def compFilter(): #Complementary Filter, combines accelerometer and gyroscope data
    global dt
    global Xangle
    global Yangle
    dt = 0.005
    Xangle = 0.98 * (rotX(accelx,accely,accelz) + gyrox * dt) + 0.02 * accelx
    Yangle = 0.98 * (rotY(accelx,accely,accelz) + gyroy * dt) + 0.02 * accely

def stabilizer(): #PID Algorithm
    global PIDx
    global PIDy
    global NewXerror
    global NewYerror
    global SumError
    global motorspeed0
    global motorspeed1
    global motorspeed2
    global motorspeed3
    
    Xerror = - Xangle
    Yerror = - Yangle
        
    Px = Kp * Xerror
    Py = Kp * Yerror
        
    Dx = Kd * (Xerror - NewXerror)
    NewXerror = Xerror
    Dy = Kd * (Yerror - NewYerror)
    NewYerror = Yerror
    
    SumError = SumError + Xerror
    if SumError > SumMax:
        SumError = SumMax
    if SumError < SumMin:
        SumError = SumMin
    Ix = Ki * SumError

    SumError = SumError + Yerror
    if SumError > SumMax:
        SumError = SumMax
    if SumError < SumMin:
        SumError = SumMin
    Iy = Ki * SumError
        
    PIDx = Px + Ix + Dx
    PIDy = Py + Iy + Dy
        
    motorspeed1 = motorspeed1 + PIDx
    motorspeed3 = motorspeed3 - PIDx
    motorspeed0 = motorspeed0 + PIDy
    motorspeed2 = motorspeed2 - PIDy
        
    if motorspeed1 > 260:
       motorspeed1 = 260
    if motorspeed1 < 200:
       motorspeed1 = 200
    if motorspeed3 > 260:
       motorspeed3 = 260
    if motorspeed3 < 200:
       motorspeed3 = 200
    if motorspeed0 > 260:
       motorspeed0 = 260
    if motorspeed0 < 200:
       motorspeed0 = 200
    if motorspeed2 > 260:
       motorspeed2 = 260
    if motorspeed2 < 200:
       motorspeed2 = 200

def usrinput(): #User Input Function
    global std_mtr_speed  #Motorspeed the PID algorithm affects
    std_mtr_speed = raw_input("MotorSpeed: ")
    
    
def calibrate(): #Calibration Function
    x = 1
    while x < 10:
        accelx = accelx + accelx
        accely = accely + accely
        accelz = accelz + accelz
        x += 1
    xoffset = -accelx
    yoffset = -accely
    
bus = smbus.SMBus(0)
address = 0x68
bus.write_byte_data(address, 0x6b, 0)

pwm.setPWM(0, 0, 200)
pwm.setPWM(1, 0, 200)
pwm.setPWM(2, 0, 200)
pwm.setPWM(3, 0, 200)
time.sleep(6)
try:
    while True:   #Threading Functions
        t1 = threading.Thread(target = stabilizer,args = ())
        t2 = threading.Thread(target = usrinput,args = ())
        t1.start()
        t2.start()
        
except Exception, e:
    pwm.setPWM(0, 0, 200)
    pwm.setPWM(1, 0, 200)
    pwm.setPWM(2, 0, 200)
    pwm.setPWM(3, 0, 200)
    
try:
    while True: #Program Loop to stabilize Quad
        """
        Pax = Kp * accelx
        Pay = Kp * accely
        Paz = Kp * accelz
        Pgx = Kp * gyrox
        Pgy = Kp * gyroy
        Pgz = Kp * gyroz
        
        Dax = Kd * (accelx - Newaccelx)
        Day = Kd * (accely - Newaccely)
        Daz = Kd * (accelz - Newaccelz)
        Dgx = Kd * (gyrox - Newgyrox)
        Dgy = Kd * (gyroy - Newgyroy)
        Dgz = Kd * (gyroz - Newgyroz)
        
        Newaccelx = accelx
        Newaccely = accely
        Newaccelz = accelz
        Newgyrox = gyrox
        Newgyroy = gyroy
        Newgyroz = gyroz
        """

        set_data()
        
        compFilter()
        
        stabilizer()

        
        print motorspeed1 , motorspeed3, motorspeed0, motorspeed2, PIDx
        pwm.setPWM(1, 0, int(motorspeed1))
        pwm.setPWM(3, 0, int(motorspeed3))

        #print Xangle, Yangle
    
finally:
    pwm.setPWM(0, 0, 200)
    pwm.setPWM(1, 0, 200)
    pwm.setPWM(2, 0, 200)
    pwm.setPWM(3, 0, 200)




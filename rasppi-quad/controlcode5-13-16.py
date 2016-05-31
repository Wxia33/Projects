import smbus
import math
import time
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
sumError = 0
dt = 0.05

bus = smbus.SMBus(0) #initialize i2c bus
address = 0x68
bus.write_byte_data(address, 0x6b, 0)

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
    
def dist(a,b):
    return math.sqrt((a*a) + (b*b))

def rotX(x,y,z): #Roll
    radians = math.atan2(y, dist(x,z))
    return math.degrees(radians)

def rotY(x,y,z): #Pitch
    radians = math.atan2(x, dist(y,z))
    return -math.degrees(radians)

def set_data():
    gyrox = read_data(gyro_x_addr) / 131
    gyroy = read_data(gyro_y_addr) / 131
    gyroz = read_data(gyro_z_addr) / 131
    accelx = read_data(accel_x_addr) / 16384.0
    accely = read_data(accel_y_addr) / 16384.0
    accelz = read_data(accel_z_addr) / 16384.0
    return gyrox, gyroy, gyroz, accelx, accely, accelz

def compFilter(gyrox, gyroy, gyroz, accelx, accely, accelz):
    #Complementary Filter, combines accelerometer and gyroscope data
    Xangle = 0.98 * (rotX(accelx,accely,accelz) + gyrox * dt) + 0.02 * accelx
    Yangle = 0.98 * (rotY(accelx,accely,accelz) + gyroy * dt) + 0.02 * accely
    return Xangle, Yangle

def pid(targAngle, measAngle, Kp, Ki, Kd, dt): #PID Algorithm
    error = targAngle - measAngle
    P = Kp * error

    diffError = error - lastError
    derivative = diffError/dt
    D = Kd * derivative

    sumError = error + sumError
    integral = sumError * dt
    I = Ki * integral
    
    PID = P + I + D
    
    lastError = error
    
    return PID

def arm(): #function to arm ESCs
    pwm.setPWM(0, 0, 200)
    pwm.setPWM(1, 0, 200)
    pwm.setPWM(2, 0, 200)
    pwm.setPWM(3, 0, 200)
    time.sleep(6)

arm()

try:
    while True:
        gyrox, gyroy, gyroz, accelx, accely, accelz = set_data()
        Xangle, Yangle = compFilter(gyrox, gyroy, gyroz,
                                    accelx, accely, accelz)
        xpid = pid(0, Xangle, Kp, Ki, Kd, dt)
        ypid = pid(0, Yangle, Kp, Ki, Kd, dt)

        pwm.setPWM(0,0,ypid)
        pwm.setPWM(1,0,xpid)
        pwm.setPWM(2,0,ypid)
        pwm.setPWM(3,0,xpid)
    
finally:
    pwm.setPWM(0, 0, 200)
    pwm.setPWM(1, 0, 200)
    pwm.setPWM(2, 0, 200)
    pwm.setPWM(3, 0, 200)

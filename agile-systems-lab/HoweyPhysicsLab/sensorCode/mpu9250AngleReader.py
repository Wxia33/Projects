import smbus
import math
import time

#Addresses on MPU9250 board to read data
gyro_x_addr = 0x43
gyro_y_addr = 0x45
gyro_z_addr = 0x47
accel_x_addr = 0x3B
accel_y_addr = 0x3D
accel_z_addr = 0x3F

#TODO: Figure out how to read Magnetometer data
'''mag_x_addr = 0x04
mag_y_addr = 0x06
mag_z_addr = 0x08

#define AK8963_ADDRESS 0x0C   //  Address of magnetometer
bus = smbus.SMBus(1)
address = 0x0C
bus.write_byte_data(address, 0x6b, 0)

#define AK8963_XOUT_L	 0x03  // data
#define AK8963_XOUT_H	 0x04
#define AK8963_YOUT_L	 0x05
#define AK8963_YOUT_H	 0x06
#define AK8963_ZOUT_L	 0x07
#define AK8963_ZOUT_H	 0x08
'''

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

def rotX(x,y,z): #Roll
    radians = math.atan2(y, dist(x,z))
    return math.degrees(radians)

def rotY(x,y,z): #Pitch
    radians = math.atan2(x, dist(y,z))
    return -math.degrees(radians)

def compFilter(): #Complementary Filter, combines accelerometer and gyroscope data
    dt = 0.005
    Xangle = 0.98 * (rotX(accelx,accely,accelz) + gyrox * dt) + 0.02 * accelx
    Yangle = 0.98 * (rotY(accelx,accely,accelz) + gyroy * dt) + 0.02 * accely
    return Xangle, Yangle

def set_data():
    gyrox = read_data(gyro_x_addr) / 131
    gyroy = read_data(gyro_y_addr) / 131
    gyroz = read_data(gyro_z_addr) / 131
    accelx = read_data(accel_x_addr) / 16384.0
    accely = read_data(accel_y_addr) / 16384.0
    accelz = read_data(accel_z_addr) / 16384.0
    return gyrox, gyroy, gyroz, accelx, accely, accelz

bus = smbus.SMBus(1) #Addresses to read data from MPU board
address = 0x68
bus.write_byte_data(address, 0x6b, 0)

datafile = open('IMU_Data.csv','wb')
writer = csv.write(datafile)
writer.writerow(['AngleX','AngleY','accelx','accely','accelz','gyrox','gyroy','gyroz'])

while True: #Main Program Loop
    try:
        gx, gy, gz, ax, ay, az = set_data()
        xang, yang = compFilter()
        writer.writerow([xang,yang,ax,ay,az,gx,gy,gz])
    except:
        datafile.close()


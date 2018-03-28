import smbus
import math

# registers for pwr mngmnt
pwr1 = 0x6b
pwr2 = 0x6c

#raw gyro registers
raw_gyro_x = 0x43
raw_gyro_y = 0x45
raw_gyro_z = 0x47

# raw accelerometer registers
raw_acc_x = 0x3b
raw_acc_y = 0x3d
raw_acc_Z = 0x3f

# default scaling values
default_acc_scaling_factor =  16384.0
default_gyro_scaling_factor = 131

# set smbus and i2c address
bus = smbus.SMBus(1) # or bus = smbus.SMBus(0) for Revision 1 boards
address = 0x68       # This is the address value read via the i2cdetect command

def read_byte(adr):
    return bus.read_byte_data(address, adr)

def read_word(adr):
    high = bus.read_byte_data(address, adr)
    low = bus.read_byte_data(address, adr+1)
    val = (high << 8) + low
    return val

def read_word_2c(adr):
    val = read_word(adr)
    if (val >= 0x8000):
        return -((65535 - val) + 1) #two's complement
    else:
        return val

def dist(a,b):
    return math.sqrt((a*a)+(b*b))
 
def get_y_rotation(x,y,z):
    radians = math.atan2(x, dist(y,z))
    return -math.degrees(radians)

def get_x_rotation(x,y,z):
    radians = math.atan2(y, dist(x,z))
    return math.degrees(radians)

# Wake up MPU 6050 (begins in sleep mode)
bus.write_byte_data(address, pwr1, 0)

# printing
print "gyro data"
print "---------"

gyro_xout = read_word_2c(raw_gyro_x)
gyro_yout = read_word_2c(raw_gyro_y)
gyro_zout = read_word_2c(raw_gyro_z)

print "gyro_xout: ", gyro_xout, " scaled: ", (gyro_xout / default_gyro_scaling_factor)
print "gyro_yout: ", gyro_yout, " scaled: ", (gyro_yout / default_gyro_scaling_factor)
print "gyro_zout: ", gyro_zout, " scaled: ", (gyro_zout / default_gyro_scaling_factor)
print
print "accelerometer data"
print "------------------" 

accel_xout = read_word_2c(raw_acc_x)
accel_yout = read_word_2c(raw_acc_y)
accel_zout = read_word_2c(raw_acc_Z) 

accel_xout_scaled = accel_xout / default_acc_scaling_factor
accel_yout_scaled = accel_yout / default_acc_scaling_factor
accel_zout_scaled = accel_zout / default_acc_scaling_factor

print "accel_xout: ", accel_xout, " scaled: ", accel_xout_scaled
print "accel_yout: ", accel_yout, " scaled: ", accel_yout_scaled
print "accel_zout: ", accel_zout, " scaled: ", accel_zout_scaled
print "x rotation: " , get_x_rotation(accel_xout_scaled, accel_yout_scaled, accel_zout_scaled)
print "y rotation: " , get_y_rotation(accel_xout_scaled, accel_yout_scaled, accel_zout_scaled)
import math

step = 2*math.pi/60 + math.pi

x_coord = []
y_coord = []

print("\n\n\nX_COORDINATES FOR EACH MINUTE")
cur_x = 64
cur_y = 36
CENTER_X = 64
CENTER_Y = 96

arr = [0,15,30,45]
for i in range(0,60):
    x_coord.append(abs(int(math.cos(i*step))*(cur_x-CENTER_X) - int(math.sin(i*step)*(cur_y-CENTER_Y) + CENTER_X)))

print(x_coord)
print("\n\n\nEXAMPLE Y_COORDINATES")

cur_x = 64
cur_y = 36
CENTER_X = 64
CENTER_Y = 96

for i in range(0,60):
    y_coord.append(abs(int(math.sin(i*step))*(cur_x-CENTER_X) - int(math.cos(i*step)*(cur_y-CENTER_Y) + CENTER_Y)))

print(y_coord)


print("\n\n\nX coords for each hour");

cur_x = 64
cur_y = 54
CENTER_X = 64
CENTER_Y = 96

hr_x = []

hr_y = []

for i in range(0,60):
    hr_x.append(abs(int(math.cos(i*step))*(cur_x-CENTER_X) - int(math.sin(i*step)*(cur_y-CENTER_Y) + CENTER_X)))

print(hr_x)
print("\n\n\nEXAMPLE Y_COORDINATES for each hour")

cur_x = 64
cur_y = 54
CENTER_X = 64
CENTER_Y = 96

for i in range(0,60):
    hr_y.append(abs(int(math.sin(i*step))*(cur_x-CENTER_X) - int(math.cos(i*step)*(cur_y-CENTER_Y) + CENTER_Y)))

print(hr_y)

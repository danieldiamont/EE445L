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

for i in range(1,28):
    temp = x_coord[29-i]
    x_coord[29-i] = x_coord[i]
    x_coord[i] = temp

for i in range(31,59):
    temp = x_coord[59-i]
    x_coord[58-i] = x_coord[i]
    x_coord[i] = temp

i = 1
while(i < 60):
    temp = x_coord[60-i]
    x_coord[60-i] = x_coord[i]
    x_coord[i] = temp
    i = i + 2
    
print(x_coord)
print("\n\n\nEXAMPLE Y_COORDINATES")

cur_x = 64
cur_y = 36
CENTER_X = 64
CENTER_Y = 96

for i in range(0,60):
    y_coord.append(abs(int(math.sin(i*step))*(cur_x-CENTER_X) - int(math.cos(i*step)*(cur_y-CENTER_Y) + CENTER_Y)))

for i in range(1,28):
    temp = y_coord[29-i]
    y_coord[29-i] = y_coord[i]
    y_coord[i] = temp

for i in range(30,59):
    temp = y_coord[60-i]
    y_coord[60-i] = y_coord[i]
    y_coord[i] = temp

i = 1
while(i < 60):
    temp = y_coord[60-i]
    y_coord[60-i] = y_coord[i]
    y_coord[i] = temp
    i = i + 2

print(y_coord)


print("\n\n\nX coords for each hour");

cur_x = 64
cur_y = 54
CENTER_X = 64
CENTER_Y = 96

hr_x = []

hr_y = []
arr = [0,5,10,15,20,25,30,35,40,45,50,55]
for i in range(0,len(arr)):
    hr_x.append(abs(int(math.cos(arr[i]*step))*(cur_x-CENTER_X) - int(math.sin(arr[i]*step)*(cur_y-CENTER_Y) + CENTER_X)))

print(hr_x)
print("\n\n\nEXAMPLE Y_COORDINATES for each hour")

cur_x = 64
cur_y = 54
CENTER_X = 64
CENTER_Y = 96

for i in range(0,len(arr)):
    hr_y.append(abs(int(math.sin(arr[i]*step))*(cur_x-CENTER_X) - int(math.cos(arr[i]*step)*(cur_y-CENTER_Y) + CENTER_Y)))

print(hr_y)


print("\n\nx coords")
arr = []
reverse =[]
for i in range(0,15):
    arr.append(64 + i*4)
    reverse.append(64 + i*4)

reverse.reverse()
total = arr + reverse[1:len(reverse)-1]

arr1 = []
reverse1 = []
for i in range(0,15):
    arr1.append(64 - 4*i)
    reverse1.append(64 - 4*i)

reverse1.reverse()
total1 = arr1+ reverse1[1:len(reverse)-1]
print(total + total1)

print("\n\ny coords")
arr = []
reverse =[]
for i in range(0,30):
    arr.append(56 + i*4)
    reverse.append(152 - i*4)

#reverse.reverse()
total = arr + reverse[1:len(reverse)-1]

print(total)

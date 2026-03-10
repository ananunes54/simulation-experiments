import math

def metricTensor(fourVector, dimensions):
    value = 0
    for i in range(0, dimensions):
        if i == 0:
            value += fourVector[0] ** 2
        else:
            value -= fourVector[i] ** 2
    return value

# motion with constant proper acceleration
properAcceleration = float(input("proper acceleration: "))

initialPosition = int(input("initial position: "))

# considering only time and x coordinates
# initial spacial velocity = 0 
fourVelocity = [1, 0]
fourAcceleration = [0, properAcceleration]
fourPosition = [0, initialPosition]

properTime = 0
properTimeIncrement = 0.05

windowHeight = 640
windowWidth = 640

screenPosition = [windowHeight/2 - fourPosition[0], windowWidth/2 + fourPosition[1]]

for i in range(0, 10):
    theta = properAcceleration * properTime
    print(f"{screenPosition[0]} {screenPosition[1]}")
    fourAcceleration = [properAcceleration * math.sinh(theta), properAcceleration * math.cosh(theta)]
    fourVelocity = [math.cosh(theta), math.sinh(theta)]
    fourPosition = [math.sinh(theta)/properAcceleration, math.cosh(theta)/properAcceleration]
    screenPosition = [windowHeight/2 - fourPosition[0], windowWidth/2 + fourPosition[1]]
    acceleraionMetric = properAcceleration ** 2
    properTime += properTimeIncrement


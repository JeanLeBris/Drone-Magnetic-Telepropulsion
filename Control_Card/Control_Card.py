import cv2
import numpy as np
import picamera2
from math import sqrt
import time
import serial as ser

# def PutInfo(elements, Xoffset, Yoffset, stream, stream2, string, show):
#     (x, y) = (stream.shape[1]//4, stream.shape[0]//4)
#     if len(elements) > 0:
#         c=max(elements, key=cv2.contourArea)
#         ((x, y), rayon)=cv2.minEnclosingCircle(c)
#         if show and rayon>20:
#             cv2.circle(stream2, (int(x + Xoffset), int(y + Yoffset)), int(rayon), color_infos, 2)
#             cv2.circle(stream, (int(x + Xoffset), int(y + Yoffset)), 5, color_infos, 10)
#             cv2.line(stream, (int(x + Xoffset), int(y + Yoffset)), (int(x + Xoffset)+150, int(y + Yoffset)), color_infos, 2)
#             cv2.putText(stream, string, (int(x + Xoffset)+10, int(y + Yoffset) -10), cv2.FONT_HERSHEY_DUPLEX, 1, color_infos, 1, cv2.LINE_AA)
#     return (stream, stream2, (int(x + Xoffset), int(y + Yoffset)))

def GetInfo(elements, Xoffset, Yoffset, stream):
    (x, y) = (stream.shape[1]//4, stream.shape[0]//4)
    rayon = 0
    if len(elements) > 0:
        c=max(elements, key=cv2.contourArea)
        ((x, y), rayon)=cv2.minEnclosingCircle(c)
    return ((int(x + Xoffset), int(y + Yoffset)), rayon)

def ShowInfo(p, rayon, stream, stream2, string):
    if rayon>10:
        cv2.circle(stream2, p, int(rayon), color_infos, 2)
        cv2.circle(stream, p, 5, color_infos, 10)
        cv2.line(stream, p, (p[0] + 150, p[1]), color_infos, 2)
        cv2.putText(stream, string, (p[0] + 10, p[1] - 10), cv2.FONT_HERSHEY_DUPLEX, 1, color_infos, 1, cv2.LINE_AA)
    return (stream, stream2)

def DrawEnvironment(p1, p2, p3, p4, stream):
    cv2.line(stream, p1, p2, color_infos, 2)
    cv2.line(stream, p1, p3, color_infos, 2)
    cv2.line(stream, p4, p2, color_infos, 2)
    cv2.line(stream, p4, p3, color_infos, 2)
    return stream

def DrawOctagon(p1, p2, p3, p4, stream, show):
    if show:
        b1 = 0.2928932188
        b2 = 0.7071067812
        p121 = (int(p1[0] + b1 * (p2[0] - p1[0])), int(p1[1] + b1 * (p2[1] - p1[1])))
        p122 = (int(p1[0] + b2 * (p2[0] - p1[0])), int(p1[1] + b2 * (p2[1] - p1[1])))
        p131 = (int(p1[0] + b1 * (p3[0] - p1[0])), int(p1[1] + b1 * (p3[1] - p1[1])))
        p132 = (int(p1[0] + b2 * (p3[0] - p1[0])), int(p1[1] + b2 * (p3[1] - p1[1])))
        p421 = (int(p4[0] + b1 * (p2[0] - p4[0])), int(p4[1] + b1 * (p2[1] - p4[1])))
        p422 = (int(p4[0] + b2 * (p2[0] - p4[0])), int(p4[1] + b2 * (p2[1] - p4[1])))
        p431 = (int(p4[0] + b1 * (p3[0] - p4[0])), int(p4[1] + b1 * (p3[1] - p4[1])))
        p432 = (int(p4[0] + b2 * (p3[0] - p4[0])), int(p4[1] + b2 * (p3[1] - p4[1])))
        cv2.line(stream, p121, p122, color_infos_red, 2)
        cv2.line(stream, p122, p422, color_infos_red, 2)
        cv2.line(stream, p422, p421, color_infos_red, 2)
        cv2.line(stream, p421, p431, color_infos_red, 2)
        cv2.line(stream, p431, p432, color_infos_red, 2)
        cv2.line(stream, p432, p132, color_infos_red, 2)
        cv2.line(stream, p132, p131, color_infos_red, 2)
        cv2.line(stream, p131, p121, color_infos_red, 2)
    return stream

def DrawCaliber(p1, stream, show):
    if show:
        cv2.line(stream, (int(0), int(p1[1])), (int(stream.shape[1]-1), int(p1[1])), color_infos_blue, 1)
        cv2.line(stream, (int(p1[0]), int(0)), (int(p1[0]), int(stream.shape[0]-1)), color_infos_blue, 1)
    return stream

def GetCoord(p1, v_dist, h_dist, p_obj):
    x = int(((p_obj[0] - p1[0]) * 1000) / h_dist)
    y = int(((p_obj[1] - p1[1]) * 1000) / v_dist)
    if x < 0:
        x = 0
    if x > 1000:
        x = 1000
    if y < 0:
        y = 0
    if y > 1000:
        y = 1000
    return (x, y)

def TransferCoordinatesData(p):
    data = ser.Serial("/dev/ttyS0",115200,timeout=2)
    data.flush()
    data.write("{}:{}\0".format(p[0], p[1]).encode())
    string1 = data.readline().decode("utf-8")
    print(string1)
    data.close()

def PrintCoordinatesData(p):
    print("{}:{}".format(p[0], p[1]))

def TransferPowerData(auto_control, power):
    if auto_control:
        # if in auto mode
        data = ser.Serial("/dev/ttyS0",500000,timeout=2)
        # data.flush()
        data.write("1:{}{:02d}:{}{:02d}:{}{:02d}:{}{:02d}:{}{:02d}:{}{:02d}:{}{:02d}:{}{:02d}\0".format(direction[0], power[0], direction[1], power[1], direction[2], power[2], direction[3], power[3], direction[4], power[4], direction[5], power[5], direction[6], power[6], direction[7], power[7]).encode())
        # data.write("1:{:04d}:{:04d}:{:04d}:{:04d}:{:04d}:{:04d}:{:04d}:{:04d}\0".format(power).encode())
        data.close()
    else:
        # if in manual mode
        data = ser.Serial("/dev/ttyS0",500000,timeout=2)
        # data.flush()
        data.write("0\0".encode())
        data.close()

def PrintPowerData(auto_control, power):
    if auto_control:
        print("1:{:02d}:{:02d}:{:02d}:{:02d}:{:02d}:{:02d}:{:02d}:{:02d}".format(power[0], power[1], power[2], power[3], power[4], power[5], power[6], power[7]))
        # print("1:{:04d}:{:04d}:{:04d}:{:04d}:{:04d}:{:04d}:{:04d}:{:04d}".format(power))
    else:
        print("0")

def ShowMaze(stream, maze, p1, height, width):
    stream = cv2.rectangle(stream, (p1[0] + int((maze.begin[1] * width) // maze.MAZE_SIZE), p1[1] + int((maze.begin[0] * height) // maze.MAZE_SIZE)), (p1[0] + int(((maze.begin[1] + 1) * width) // maze.MAZE_SIZE), p1[1] + int(((maze.begin[0] + 1) * height) // maze.MAZE_SIZE)), (255, 0, 0), -1)
    stream = cv2.rectangle(stream, (p1[0] + int((maze.end[1] * width) // maze.MAZE_SIZE), p1[1] + int((maze.end[0] * height) // maze.MAZE_SIZE)), (p1[0] + int(((maze.end[1] + 1) * width) // maze.MAZE_SIZE), p1[1] + int(((maze.end[0] + 1) * height) // maze.MAZE_SIZE)), (0, 0, 255), -1)
    for x in range(maze.maze.shape[0]):
        for y in range(maze.maze.shape[1]):
            if maze.maze[x, y]:
                stream = cv2.rectangle(stream, (p1[0] + int((y * width) // maze.MAZE_SIZE), p1[1] + int((x * height) // maze.MAZE_SIZE)), (p1[0] + int(((y + 1) * width) // maze.MAZE_SIZE), p1[1] + int(((x + 1) * height) // maze.MAZE_SIZE)), (0, 0, 0), -1)
    return stream

def ShowMazePath(stream, maze, p1, height, width):
    for x in range(maze.maze.shape[0]):
        for y in range(maze.maze.shape[1]):
            if maze.maze_path[x, y]:
                stream = cv2.rectangle(stream, (p1[0] + int((y * width) // maze.MAZE_SIZE), p1[1] + int((x * height) // maze.MAZE_SIZE)), (p1[0] + int(((y + 1) * width) // maze.MAZE_SIZE), p1[1] + int(((x + 1) * height) // maze.MAZE_SIZE)), (255, 255, 255), -1)
    return stream

def mouseCallbackFunction(event, x, y, flags, param):
    # Maze* maze = (Maze*)ptr
    if event == cv2.EVENT_LBUTTONDOWN:
        # print("x: {}; y: {} clicked".format(x, y))
        p_pressed = GetCoord(p1, p2[1]-p1[1], p3[0]-p1[0], (x, y))
        # print("x: {}; y: {} clicked".format(p_pressed[0], p_pressed[1]))
        if p_pressed[0] > 0 and p_pressed[0] < 1000 and p_pressed[1] > 0 and p_pressed[1] < 1000:
            y = int(p_pressed[0] // (1000 / maze.MAZE_SIZE))
            x = int(p_pressed[1] // (1000 / maze.MAZE_SIZE))
            maze.maze[x, y] = not(maze.maze[x, y])
            # print(maze)
            maze.last_dragged = (p_pressed[0] // (1000 / maze.MAZE_SIZE), p_pressed[1] // (1000 / maze.MAZE_SIZE))
        maze.l_mouse_pressed = True
    elif event == cv2.EVENT_LBUTTONUP:
        maze.l_mouse_pressed = False
    elif event == cv2.EVENT_RBUTTONDOWN:
        p_pressed = GetCoord(p1, p2[1]-p1[1], p3[0]-p1[0], (x, y))
        if p_pressed[0] > 0 and p_pressed[0] < 1000 and p_pressed[1] > 0 and p_pressed[1] < 1000:
            y = int(p_pressed[0] // (1000 / maze.MAZE_SIZE))
            x = int(p_pressed[1] // (1000 / maze.MAZE_SIZE))
            if maze.start_end_state:
                maze.end = (x, y)   # end
            else:
                maze.begin = (x, y) # begin
            maze.start_end_state = not(maze.start_end_state)
    elif maze.l_mouse_pressed and event == cv2.EVENT_MOUSEMOVE:
        # print("x: {}; y: {} clicked".format(x, y))
        p_pressed = GetCoord(p1, p2[1]-p1[1], p3[0]-p1[0], (x, y))
        # print("x: {}; y: {} clicked".format(p_pressed[0], p_pressed[1]))
        if (p_pressed[0] // (1000 / maze.MAZE_SIZE), p_pressed[1] // (1000 / maze.MAZE_SIZE)) != maze.last_dragged and p_pressed[0] > 0 and p_pressed[0] < 1000 and p_pressed[1] > 0 and p_pressed[1] < 1000:
            y = int(p_pressed[0] // (1000 / maze.MAZE_SIZE))
            x = int(p_pressed[1] // (1000 / maze.MAZE_SIZE))
            maze.maze[x, y] = not(maze.maze[x, y])
            # print(maze)
            maze.last_dragged = (p_pressed[0] // (1000 / maze.MAZE_SIZE), p_pressed[1] // (1000 / maze.MAZE_SIZE))

def GetConfig(lo_drone, hi_drone, lo_env, hi_env, maze):
    f = open("Control_Card.config", "r")
    f.readline()
    buffer = f.readline().split(':')[1]
    lo_drone = np.array([int(buffer.split(',')[0]), int(buffer.split(',')[1]), int(buffer.split(',')[2])])
    buffer = f.readline().split(':')[1]
    hi_drone = np.array([int(buffer.split(',')[0]), int(buffer.split(',')[1]), int(buffer.split(',')[2])])
    f.readline()
    f.readline()
    buffer = f.readline().split(':')[1]
    lo_env = np.array([int(buffer.split(',')[0]), int(buffer.split(',')[1]), int(buffer.split(',')[2])])
    buffer = f.readline().split(':')[1]
    hi_env = np.array([int(buffer.split(',')[0]), int(buffer.split(',')[1]), int(buffer.split(',')[2])])
    f.readline()
    f.readline()
    buffer = f.readline().split(':')[1]
    maze.VALUE_MAX = int(buffer)
    buffer = f.readline().split(':')[1]
    maze.TIMEOUT = int(buffer)
    buffer = f.readline().split(':')[1]
    maze.MAZE_SIZE = int(buffer)
    f.close()
    return lo_drone, hi_drone, lo_env, hi_env, maze

class Maze:
    def __init__(self):
        self.MAZE_SIZE = 10
        self.begin = (0, 0)
        self.end = (self.MAZE_SIZE - 1, self.MAZE_SIZE - 1)
        self.maze = np.zeros((self.MAZE_SIZE, self.MAZE_SIZE), dtype=int)
        self.maze_mask = np.zeros((self.MAZE_SIZE, self.MAZE_SIZE), dtype=int)
        self.maze_path = np.zeros((self.MAZE_SIZE, self.MAZE_SIZE), dtype=int)
        self.start_end_state = False
        self.VALUE_MAX = 65000
        self.TIMEOUT = 100
        self.l_mouse_pressed = False
        self.last_dragged = (0, 0)

    def SolveMaze(self):
        self.maze_mask = np.zeros((self.MAZE_SIZE, self.MAZE_SIZE), dtype=int)
        count = 0
        file = [self.end]
        point = file[0]
        self.maze_mask[point] = self.VALUE_MAX
        while count < self.TIMEOUT and file != [] and point != self.begin:
            point = file.pop(0)
            if point[0] != 0 and self.maze[point[0] - 1, point[1]] == 0 and self.maze_mask[point[0] - 1, point[1]] == 0:
                file.append((point[0] - 1, point[1]))
                self.maze_mask[point[0] - 1, point[1]] = self.maze_mask[point] - 1
            if point[0] != self.MAZE_SIZE - 1 and self.maze[point[0] + 1, point[1]] == 0 and self.maze_mask[point[0] + 1, point[1]] == 0:
                file.append((point[0] + 1, point[1]))
                self.maze_mask[point[0] + 1, point[1]] = self.maze_mask[point] - 1
            if point[1] != 0 and self.maze[point[0], point[1] - 1] == 0 and self.maze_mask[point[0], point[1] - 1] == 0:
                file.append((point[0], point[1] - 1))
                self.maze_mask[point[0], point[1] - 1] = self.maze_mask[point] - 1
            if point[1] != self.MAZE_SIZE - 1 and self.maze[point[0], point[1] + 1] == 0 and self.maze_mask[point[0], point[1] + 1] == 0:
                file.append((point[0], point[1] + 1))
                self.maze_mask[point[0], point[1] + 1] = self.maze_mask[point] - 1
            count += 1
        # print(self.maze_mask)
        self.GetPath()
    
    def GetPath(self):
        self.maze_path = np.zeros((self.MAZE_SIZE, self.MAZE_SIZE), dtype=int)
        point = self.begin
        count = 0
        while count < self.TIMEOUT and point != self.end:
            if point[0] != 0 and self.maze_mask[point[0] - 1, point[1]] == self.maze_mask[point] + 1:
                point = (point[0] - 1, point[1])
            elif point[0] != self.MAZE_SIZE - 1 and self.maze_mask[point[0] + 1, point[1]] == self.maze_mask[point] + 1:
                point = (point[0] + 1, point[1])
            elif point[1] != 0 and self.maze_mask[point[0], point[1] - 1] == self.maze_mask[point] + 1:
                point = (point[0], point[1] - 1)
            elif point[1] != self.MAZE_SIZE - 1 and self.maze_mask[point[0], point[1] + 1] == self.maze_mask[point] + 1:
                point = (point[0], point[1] + 1)
            if point != self.end:
                self.maze_path[point] = 1
            count += 1

# # lo_drone = np.array([100, 220, 125])
# # hi_drone = np.array([125, 255, 255])
# lo_drone = np.array([40, 100, 0])
# hi_drone = np.array([100, 255, 255])
# lo_env = np.array([125, 200, 200])
# hi_env = np.array([175, 255, 255])

lo_drone = 0
hi_drone = 0
lo_env = 0
hi_env = 0

maze = Maze()

lo_drone, hi_drone, lo_env, hi_env, maze = GetConfig(lo_drone, hi_drone, lo_env, hi_env, maze)

color_infos = (0, 255, 255)
color_infos_red = (0, 0, 255)
color_infos_blue = (255, 0, 0)
WIDTH = 640
HEIGHT = 480

capture_n = 0

# pilot_mode = -1

direction = [0, 0, 0, 0, 0, 0, 0, 0]
power = [0, 0, 0, 0, 0, 0, 0, 0]

show_mask = False
show_image2 = False
show_env_mask = False
show_env_image2 = False
show_env = False
show_oct = False
show_cal = False
show_obj = False
show_maze = False
show_path = False
calibrate_env = True
auto_control = False
pressed = False
button_value = 0

camera = picamera2.Picamera2()
mode = camera.sensor_modes[1]
# camera.resolution=(WIDTH, HEIGHT)
camera.configure(camera.create_preview_configuration(main={"format": 'XRGB8888', "size": (WIDTH, HEIGHT)}, sensor = {'output_size': mode['size'], 'bit_depth': mode['bit_depth']}))
# camera.resolution = (2592, 1944)
# camera.framerate = 15
camera.start()
# cv2.namedWindow("Camera", cv2.WINDOW_NORMAL)
# cv2.resizeWindow("Camera", 1400, 800)
# cv2.namedWindow("image2", cv2.WINDOW_NORMAL)
# cv2.resizeWindow("image2", 1400, 800)
# cv2.namedWindow("Mask", cv2.WINDOW_NORMAL)
# cv2.resizeWindow("Mask", 1400, 800)
# cv2.namedWindow("env_image2", cv2.WINDOW_NORMAL)
# cv2.resizeWindow("env_image2", 1400, 800)
# cv2.namedWindow("Env_Mask", cv2.WINDOW_NORMAL)
# cv2.resizeWindow("Env_Mask", 1400, 800)
cv2.namedWindow('Camera', cv2.WINDOW_GUI_NORMAL)
cv2.setMouseCallback('Camera', mouseCallbackFunction)
while True:
    if pressed == True and button_value==255:
        pressed = False
    stream = camera.capture_array()
    # camera.capture(stream, 'bgr', use_video_port=True)
    # stream = cv2.flip(stream, 0)
    # stream = cv2.flip(stream, 1)
    stream = cv2.flip(stream, -1)
    # cv2.rotate(stream, cv2.ROTATE_90_CLOCKWISE)


    image=cv2.cvtColor(stream, cv2.COLOR_BGR2HSV)   # convert stream from bgr to hsv, store in image
    mask=cv2.inRange(image, lo_drone, hi_drone) # Verify which pixels of image are in range of lo and hi, store in mask
    env_mask=cv2.inRange(image, lo_env, hi_env) # Verify which pixels of image are in range of lo and hi, store in env_mask
    image=cv2.blur(image, (7, 7))   # Blur image

    mask=cv2.erode(mask, None, iterations=4)
    mask=cv2.dilate(mask, None, iterations=4)

    env_mask=cv2.erode(env_mask, None, iterations=4)
    env_mask=cv2.dilate(env_mask, None, iterations=4)

    image2=cv2.bitwise_and(stream, stream, mask=mask)
    elements=cv2.findContours(mask, cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]

    env_image2=cv2.bitwise_and(stream, stream, mask=env_mask)
    # print("{} : {}".format(env_mask.shape[0], env_mask.shape[1]))
    env_elements1=cv2.findContours(env_mask[:env_mask.shape[0]//2, :env_mask.shape[1]//2], cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
    env_elements2=cv2.findContours(env_mask[env_mask.shape[0]//2+1:, :env_mask.shape[1]//2], cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
    env_elements3=cv2.findContours(env_mask[:env_mask.shape[0]//2, env_mask.shape[1]//2+1:], cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]
    env_elements4=cv2.findContours(env_mask[env_mask.shape[0]//2+1:, env_mask.shape[1]//2+1:], cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2]

    (p_obj, r_obj) = GetInfo(elements, 0, 0, stream)
    if calibrate_env:
        (p1, r1) = GetInfo(env_elements1, 0, 0, stream)
        (p2, r2) = GetInfo(env_elements2, 0, env_mask.shape[0]//2+1, stream)
        (p3, r3) = GetInfo(env_elements3, env_mask.shape[1]//2+1, 0, stream)
        (p4, r4) = GetInfo(env_elements4, env_mask.shape[1]//2+1, env_mask.shape[0]//2+1, stream)
    if show_env:
        (stream, env_image2) = ShowInfo(p1, r1, stream, env_image2, "Env_1")
        (stream, env_image2) = ShowInfo(p2, r2, stream, env_image2, "Env_2")
        (stream, env_image2) = ShowInfo(p3, r3, stream, env_image2, "Env_3")
        (stream, env_image2) = ShowInfo(p4, r4, stream, env_image2, "Env_4")
        stream = DrawEnvironment(p1, p2, p3, p4, stream)
    stream = DrawOctagon(p1, p2, p3, p4, stream, show_oct)
    stream = DrawCaliber(p1, stream, show_cal)
    p = GetCoord(p1, p2[1]-p1[1], p3[0]-p1[0], p_obj)
    if show_obj:
        (stream, image2) = ShowInfo(p_obj, r_obj, stream, image2, "x={} y={}".format(p[0], p[1]))
    if show_maze:
        stream = ShowMaze(stream, maze, p1, p2[1]-p1[1], p3[0]-p1[0])
        if show_path:
            stream = ShowMazePath(stream, maze, p1, p2[1]-p1[1], p3[0]-p1[0])
    # PrintCoordinatesData(p)
    # TransferCoordinatesData(p)
    PrintPowerData(auto_control, power)
    TransferPowerData(auto_control, power)
    time.sleep(0.01)

    cv2.imshow('Camera', stream)
    if show_image2 == True:
        cv2.imshow('image2', image2)
    if show_mask == True:
        cv2.imshow('Mask', mask)
    if show_env_image2 == True:
        cv2.imshow('env_image2', env_image2)
    if show_env_mask == True:
        cv2.imshow('Env_Mask', env_mask)
    
    # cv2.imwrite("test.jpg", stream)

    # time.sleep(5)
    
    # print("{} ; {} : {} ; {} : {} ; {} : {} ; {} : {} ; {}".format(stream.shape[0], stream.shape[1], image2.shape[0], image2.shape[1], mask.shape[0], mask.shape[1], env_image2.shape[0], env_image2.shape[1], env_mask.shape[0], env_mask.shape[1]))
    button_value = cv2.waitKey(1)&0xFF
    if button_value==ord('q'):
        break
    elif button_value==ord('1') and pressed == False:
        show_mask = not(show_mask)
        if show_mask == False:
            cv2.destroyWindow('Mask')
        pressed = True
    elif button_value==ord('2') and pressed == False:
        show_image2 = not(show_image2)
        if show_image2 == False:
            cv2.destroyWindow('image2')
        pressed = True
    elif button_value==ord('4') and pressed == False:
        show_env_mask = not(show_env_mask)
        if show_env_mask == False:
            cv2.destroyWindow('Env_Mask')
        pressed = True
    elif button_value==ord('5') and pressed == False:
        show_env_image2 = not(show_env_image2)
        if show_env_image2 == False:
            cv2.destroyWindow('env_image2')
        pressed = True
    elif button_value==ord('e') and pressed == False:
        show_env = not(show_env)
        pressed = True
    elif button_value==ord('r') and pressed == False:
        show_oct = not(show_oct)
        pressed = True
    elif button_value==ord('c') and pressed == False:
        show_cal = not(show_cal)
        pressed = True
    elif button_value==ord('o') and pressed == False:
        show_obj = not(show_obj)
        pressed = True
    elif button_value==ord('f') and pressed == False:
        calibrate_env = not(calibrate_env)
        pressed = True
    elif button_value==ord('a') and pressed == False:
        auto_control = not(auto_control)
        pressed = True
    elif button_value==ord('m') and pressed == False:
        show_maze = not(show_maze)
        pressed = True
    elif button_value==ord('l') and pressed == False:
        maze.SolveMaze()
        pressed = True
    elif button_value==ord('p') and pressed == False:
        show_path = not(show_path)
        pressed = True
    elif button_value==ord('u') and pressed == False:
        lo_drone, hi_drone, lo_env, hi_env, maze = GetConfig(lo_drone, hi_drone, lo_env, hi_env, maze)
        pressed = True
    elif button_value==ord('7') and pressed == False:
        capture_n = capture_n + 1
        cv2.imwrite("img{}.jpg".format(capture_n), stream)
        pressed = True
    elif button_value==ord('8') and pressed == False:
        capture_n = capture_n + 1
        cv2.imwrite("img1m{}.jpg".format(capture_n), mask)
        pressed = True
    elif button_value==ord('9') and pressed == False:
        capture_n = capture_n + 1
        cv2.imwrite("img2m{}.jpg".format(capture_n), env_mask)
        pressed = True
    elif button_value==ord('3') and pressed == False:
        capture_n = capture_n + 1
        cv2.imwrite("img1c{}.jpg".format(capture_n), image2)
        pressed = True
    elif button_value==ord('6') and pressed == False:
        capture_n = capture_n + 1
        cv2.imwrite("img2c{}.jpg".format(capture_n), env_image2)
        pressed = True

cv2.destroyAllWindows()

import cv2
import numpy as np
import picamera2
from math import sqrt
import time
import serial as ser

def PutInfo(elements, Xoffset, Yoffset, stream, stream2, string, show):
    (x, y) = (stream.shape[1]//4, stream.shape[0]//4)
    if len(elements) > 0:
        c=max(elements, key=cv2.contourArea)
        ((x, y), rayon)=cv2.minEnclosingCircle(c)
        if show and rayon>30:
            cv2.circle(stream2, (int(x + Xoffset), int(y + Yoffset)), int(rayon), color_infos, 2)
            cv2.circle(stream, (int(x + Xoffset), int(y + Yoffset)), 5, color_infos, 10)
            cv2.line(stream, (int(x + Xoffset), int(y + Yoffset)), (int(x + Xoffset)+150, int(y + Yoffset)), color_infos, 2)
            cv2.putText(stream, string, (int(x + Xoffset)+10, int(y + Yoffset) -10), cv2.FONT_HERSHEY_DUPLEX, 1, color_infos, 1, cv2.LINE_AA)
    return (stream, stream2, (int(x + Xoffset), int(y + Yoffset)))

def DrawEnvironment(p1, p2, p3, p4, stream, show):
    if show:
        cv2.line(stream, p1, p2, color_infos, 2)
        cv2.line(stream, p1, p3, color_infos, 2)
        cv2.line(stream, p4, p2, color_infos, 2)
        cv2.line(stream, p4, p3, color_infos, 2)
        # print("(x1 , y1) = ({} , {}) ; (x2 , y2) = ({} , {}) ; (x3 , y3) = ({} , {}) ; (x4) , y4) = ({} , {})".format(x1, y1, x2, y2, x3, y3, x4, y4))
    return stream

def DrawOctagon(p1, p2, p3, p4, stream, show):
    if show:
        # c12 = ((x1 + x2) / 2, (y1 + y2) / 2)
        # c13 = ((x1 + x3) / 2, (y1 + y3) / 2)
        # c24 = ((x2 + x4) / 2, (y2 + y4) / 2)
        # c34 = ((x3 + x4) / 2, (y3 + y4) / 2)
        # r12and34 = sqrt((c34[0] - c12[0]) ** 2 + (c34[1] - c12[1]) ** 2) / 2
        # r13and24 = sqrt((c13[0] - c24[0]) ** 2 + (c13[1] - c24[1]) ** 2) / 2
        # a12and34 = r13and24 * 2 / (1 + sqrt(2))
        # a13and24 = r12and34 * 2 / (1 + sqrt(2))
        # b12 = (sqrt((x1 - x2) ** 2 + (y1 - y2) ** 2) - a12and34) / 2
        # b13 = (sqrt((x1 - x3) ** 2 + (y1 - y3) ** 2) - a13and24) / 2
        # b24 = (sqrt((x2 - x4) ** 2 + (y2 - y4) ** 2) - a13and24) / 2
        # b34 = (sqrt((x3 - x4) ** 2 + (y3 - y4) ** 2) - a12and34) / 2
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
        # print("(x1 , y1) = ({} , {}) ; (x2 , y2) = ({} , {}) ; (x3 , y3) = ({} , {}) ; (x4) , y4) = ({} , {})".format(x1, y1, x2, y2, x3, y3, x4, y4))
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

def TransferData(p):
    print("{}:{}".format(p[0], p[1]))
    data = ser.Serial("/dev/ttyS0",115200,timeout=2)
    data.flush()
    data.write("{}:{}\0".format(p[0], p[1]).encode())
    string1 = data.readline().decode("utf-8")
    print(string1)
    data.close()

lo_drone = np.array([50, 100, 0])
hi_drone = np.array([100, 255, 255])
lo_env = np.array([32, 100, 0])
hi_env = np.array([64, 255, 255])
color_infos = (0, 255, 255)
color_infos_red = (0, 0, 255)
color_infos_blue = (255, 0, 0)
WIDTH = 640
HEIGHT = 480

pilot_mode = -1

show_mask = False
show_image2 = False
show_env_mask = False
show_env_image2 = False
show_env = False
show_oct = False
show_cal = False
show_obj = False
pressed = False
button_value = 0

camera = picamera2.Picamera2()
# camera.resolution=(WIDTH, HEIGHT)
camera.configure(camera.create_preview_configuration(main={"format": 'XRGB8888', "size": (WIDTH, HEIGHT)}))
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
while True:
    if pressed == True and button_value==255:
        pressed = False
    stream = camera.capture_array()
    # camera.capture(stream, 'bgr', use_video_port=True)
    # stream = cv2.flip(stream, 0)
    # stream = cv2.flip(stream, 1)
    stream = cv2.flip(stream, -1)


    image=cv2.cvtColor(stream, cv2.COLOR_BGR2HSV)   # convert stream from bgr to hsv, store in image
    mask=cv2.inRange(image, lo_drone, hi_drone) # Verify which pixels of image are in range of lo and hi, store in mask
    env_mask=cv2.inRange(image, lo_env, hi_env) # Verify which pixels of image are in range of lo and hi, store in mask
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

    (stream, image2, p_obj) = PutInfo(elements, 0, 0, stream, image2, "Object", show_obj)
    (stream, env_image2, p1) = PutInfo(env_elements1, 0, 0, stream, env_image2, "Env_1", show_env)
    (stream, env_image2, p2) = PutInfo(env_elements2, 0, env_mask.shape[0]//2+1, stream, env_image2, "Env_2", show_env)
    (stream, env_image2, p3) = PutInfo(env_elements3, env_mask.shape[1]//2+1, 0, stream, env_image2, "Env_3", show_env)
    (stream, env_image2, p4) = PutInfo(env_elements4, env_mask.shape[1]//2+1, env_mask.shape[0]//2+1, stream, env_image2, "Env_4", show_env)
    stream = DrawEnvironment(p1, p2, p3, p4, stream, show_env)
    stream = DrawOctagon(p1, p2, p3, p4, stream, show_oct)
    stream = DrawCaliber(p1, stream, show_cal)
    p = GetCoord(p1, p2[1]-p1[1], p3[0]-p1[0], p_obj)
    # res = TransferData(p)
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
    # stream.seek(0)
    # stream.truncate()
cv2.destroyAllWindows()

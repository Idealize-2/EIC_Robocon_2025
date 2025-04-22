from ultralytics import YOLO
import torch
import cv2
import math
import numpy as np
import serial
import time

#setup PySerial
# ardu = serial.Serial(port='COM3', baudrate=115200, timeout=.1) 

#Write to ardu func
def write_to(x):
    # ardu.write(bytes(x, 'utf-8')) 
    # time.sleep(0.05) 
    # data = ardu.readline() 
    # print(data)
    pass


# load yolov8 model
model = YOLO(r"best.pt")
#Backboard model but still lag. IDK why?
#model = YOLO(r"E:\AI basketball test\runs\detect\train2\weights\best.pt")

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")
model.to(device)
model.cuda()

# load video
#video_path = 'C:/CU/EIC/color balls test video 3.mp4'
#cap = cv2.VideoCapture(1)a
print("pass")
ret = True
conf_val = 0.3
# cap
# cam_port=[]
# for i in range(10):
#     cap = cv2.VideoCapture(i)
#     ret, frame = cap.read()
#     if ret:
#         cam_port.append(i)
# print(cam_port)
# print("pass")

cap = cv2.VideoCapture(0)
cap.set(3, 1280)
cap.set(4, 720)

# read frames
theta = 30

# for EIC basketball test
basket_height = 2200
half_blackboard = 0
camera_height = 1120

# for comp adjustment 
# basket_height = 3050
# half_blackboard = 525
# camera_height = 1120

y = basket_height + half_blackboard -camera_height
L = y/math.sin(theta)
x = L*math.cos(theta)
while ret:

    ret, frame = cap.read()
    height, width, _ = frame.shape
    #print(height, width)
    cx = int(width/2)
    cy = int(height/2)
    #pixel_center = frame[cy,cx]
    #print('center',pixel_center)
    # print("pass")
    if ret:

        results = model.track(frame, conf=conf_val)
        frame_ = results[0].plot()
        boxes = results[0].boxes
        color2 = (0, 0, 255)
        color3 = (0, 255, 0)
        

        
        try:
            if len(boxes.xywh) > 0:
                center_x = int(boxes.xywh[0][0])
                center_y = int(boxes.xywh[0][1])

                if(abs(center_x - cx) < 50):
                    write_to("0")
                else:
                    if(center_x > cx):
                        write_to("1")
                    else:
                        write_to("-1")
               
                
               
                deltaYpixel = center_y-cy
                deltaX=2.02*deltaYpixel/math.tan(theta)

                length = math.sqrt((x+deltaX)**2+y**2)/1000
                cv2.line(frame_,(cx,cy),(center_x,center_y),color2, 2)
                cv2.putText(frame_, "distance" + ": " + str(length), (center_x-20, center_y - 50), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color2, 2)
        except IndexError:
            pass
        except Exception as e :
            raise e
        cv2.imshow('frame',frame_)
       
        if cv2.waitKey(25) & 0xFF == ord('q'):
           break
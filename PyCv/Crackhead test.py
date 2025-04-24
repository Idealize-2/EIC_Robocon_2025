from ultralytics import YOLO
import torch
import cv2
import math
import numpy as np
import serial
import time

#setup PySerial
ardu = serial.Serial(port='COM3', baudrate=115200, timeout=.1) 

#Write to ardu func
def write_to(x):
      ardu.write(bytes(x, 'utf-8')) 
      #time.sleep(0.1) 
      data = ardu.readline() 
      print(data)  
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

cap = cv2.VideoCapture(0, cv2.CAP_DSHOW)
cap.set(3, 1280)
cap.set(4, 720)


theta = math.radians(14) #camera angle relative to ground
alpha = math.radians(18) #half camera vertical FOV
basket_height = 2300 #hoop height relative to ground
half_blackboard = 0    #set 0 if measure from hoop
camera_height = 1160 # camera height relative to ground
vRes = 720 # vertical resolution

y = basket_height + half_blackboard -camera_height
x = 3500 # horizontal standerdized distant when basket at the center of the screen
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

        results = model.track(frame, conf=conf_val, verbose=False)
        frame_ = results[0].plot()
        boxes = results[0].boxes
        color2 = (0, 0, 255)
        color3 = (0, 255, 0)
        

        
        try:
            if len(boxes.xywh) > 0:
                center_x = int(boxes.xywh[0][0])
                center_y = int(boxes.xywh[0][1]) 

                #cal diff to center
                x_center_diff = center_x - cx

                #transmit current center diif to ardunio
                write_to(str(x_center_diff))
                #print("this: " , x_center_diff)
                box_width = float(boxes.xyxy[0][2] - boxes.xyxy[0][0])
                #print(box_width)
                Distant = (45*1400)/box_width
                Distant /= 100
                deltaYpixel = center_y - cy
              
                # deltaX=1.9*deltaYpixel/math.tan(theta)
                # length = math.sqrt((x+deltaX)**2+y**2)/1000 -0.725 
                length = math.sqrt((x+((2*x*deltaYpixel))/(math.tan(alpha)*vRes*math.tan(theta)))**2+y**2)/1000
                #print(length )
                cv2.line(frame_,(cx,cy),(center_x,center_y),color2, 2)
                cv2.putText(frame_, "distance" + ": " + str(Distant), (center_x-20, center_y - 50), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color2, 2)
        except IndexError:
            pass
        except Exception as e :
            raise e
        cv2.imshow('frame',frame_)
       
        if cv2.waitKey(25) & 0xFF == ord('q'):
           break
from ultralytics import YOLO
import cv2
import math
import numpy as np


# load yolov8 model
model = YOLO('C:/Users/scar15/Documents/Robocon/models/eicv1.pt')

# load video
#video_path = 'C:/CU/EIC/color balls test video 3.mp4'
#cap = cv2.VideoCapture(1)
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
# read frames

while ret:

    ret, frame = cap.read()
    height, width, _ = frame.shape
    
    #cx = int(width/2)
    #cy = int(height/2)
    #pixel_center = frame[cy,cx]
    #print('center',pixel_center)
    # print("pass")
    if ret:

        results = model.track(frame, conf=conf_val)
        frame_ = results[0].plot()
        boxes = results[0].boxes
        color2 = (0, 0, 255)
        color3 = (0, 255, 0)
        print(boxes.xywh)
        try:
            center_x = int(boxes.xywh[0][0])
            center_y = int(boxes.xywh[0][1])

            area = int(boxes.xywh[0][2]*boxes.xywh[0][3])

            wantedX = 325
            wantedY = 270

            d = math.sqrt((wantedX-center_x)**2 + (wantedY-center_y)**2)
            cv2.line(frame_,(wantedX,wantedY),(center_x,center_y),color2, 2)
            cv2.putText(frame_, "distance" + ": " + str(d), (center_x-20, center_y - 50), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color2, 2)
            cv2.putText(frame_, "area" + ": " + str(area), (center_x-20, center_y), cv2.FONT_HERSHEY_SIMPLEX, 0.5, color3, 2)
        except IndexError:
            print("no boxes found")
            pass
        except Exception as e :
            raise e
        cv2.imshow('frame',frame_)
        
        if cv2.waitKey(25) & 0xFF == ord('q'):
           break
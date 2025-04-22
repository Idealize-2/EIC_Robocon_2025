import cv2
from ultralytics import YOLO

import argparse

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--model', type=str)
    return parser.parse_args()

def main():
    args = get_args()
    
    model = YOLO(args.model)
    cap = cv2.VideoCapture(0)

    ret, frame = cap.read()

    if ret:
        print(frame)

    cv2.waitKey(0)
    cv2.destroyAllWindows()
    cap.release()

if __name__ == '__main__':
    main()
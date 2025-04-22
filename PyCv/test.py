import cv2
from ultralytics import YOLO

import argparse
import os

def get_args():
    parser = argparse.ArgumentParser()
    parser.add_argument('--model', type=str)
    parser.add_argument('--src', type=str)
    parser.add_argument('--save', type=str, required=True)
    return parser.parse_args()

def main():
    args = get_args()
    
    model = YOLO(args.model)
    results = model(args.src)

    count = 0
    for folder in os.listdir(args.save):
        if folder[:4] == "test": count += 1
    
    new_folder = os.path.join(args.save, f"test{count + 1}")
    os.mkdir(new_folder)

    for idx, result in enumerate(results):
        save_path = os.path.join(new_folder, f"{idx}.jpg")
        result.save(filename=save_path)

if __name__ == '__main__':
    main()
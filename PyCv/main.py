from ultralytics import YOLO
import torch

print(torch.cuda.is_available())
# import os

DATASET = "datasets/Basketball-hoop/data.yaml"

model = YOLO("yolo11n.pt")

device = torch.device("cuda" if torch.cuda.is_available() else "cpu")

results = model.train(data=DATASET, epochs=100, imgsz=640, batch=-1, device=device)
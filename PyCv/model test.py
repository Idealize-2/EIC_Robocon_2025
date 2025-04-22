import cv2
from ultralytics import YOLO

# Load your trained YOLO model
model = YOLO('best.pt')  # Replace with your trained model path

# Define the class names (You have two classes: rim and backboard)
class_names = ['backboard', 'rim']  # Ensure these match your model's class order

# Open the camera
cap = cv2.VideoCapture(0)  # 0 is the default camera, change if using another source

while True:
    # Capture frame-by-frame
    ret, frame = cap.read()

    # If frame is not read properly, continue to the next frame
    if not ret:
        print("Failed to grab frame")
        break

    # Run inference on the frame
    results = model(frame)  # This runs the model on the frame

    # Extract the results
    boxes = results[0].boxes.xyxy.numpy()  # Get bounding boxes (xyxy format)
    scores = results[0].boxes.conf.numpy()  # Get confidence scores (probabilities)
    class_ids = results[0].boxes.cls.numpy().astype(int)  # Get class ids

    # Loop through the detections and draw bounding boxes
    for box, score, class_id in zip(boxes, scores, class_ids):
        x1, y1, x2, y2 = map(int, box)  # Extract the coordinates of the bounding box

        # Only draw if the confidence is above a threshold (e.g., 0.5)
        if score > 0.1 :
            label = class_names[class_id]  # Get the label from the class ID
            # Color based on class
            color = (0, 255, 0) if label == 'rim' else (255, 0, 0)  # Green for 'rim', Blue for 'backboard'
            cv2.rectangle(frame, (x1, y1), (x2, y2), color, 2)
            cv2.putText(frame, f"{label} {score:.2f}", (x1, y1 - 10),
                        cv2.FONT_HERSHEY_SIMPLEX, 0.6, color, 2)

    # Display the resulting frame with bounding boxes
    cv2.imshow('YOLO Object Detection', frame)

    # Exit if 'q' is pressed
    if cv2.waitKey(1) & 0xFF == ord('q'):
        break

# Release the camera and close OpenCV windows
cap.release()
cv2.destroyAllWindows()

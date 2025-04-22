import pyrealsense2 as rs
import numpy as np
import cv2

# Configure pipeline
pipeline = rs.pipeline()
config = rs.config()

# Enable the RGB camera stream
width, height, fps = 640, 480, 30  # Adjust if needed
config.enable_stream(rs.stream.color, width, height, rs.format.bgr8, fps)

# Start the pipeline
pipeline.start(config)

# Define video writer (MJPG codec, adjust if needed)
fourcc = cv2.VideoWriter_fourcc(*'XVID')
out = cv2.VideoWriter('output.avi', fourcc, fps, (width, height))

try:
    while True:
        # Wait for a coherent frame
        frames = pipeline.wait_for_frames()
        color_frame = frames.get_color_frame()
        
        if not color_frame:
            continue

        # Convert image to numpy array
        color_image = np.asanyarray(color_frame.get_data())

        # Show the video feed
        cv2.imshow('RealSense D415 - RGB Feed', color_image)

        # Write frame to video file
        out.write(color_image)

        # Press 'q' to exit
        if cv2.waitKey(1) & 0xFF == ord('q'):
            break
finally:
    # Stop the pipeline and release resources
    pipeline.stop()
    out.release()
    cv2.destroyAllWindows()

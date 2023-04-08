import cv2

# Define the codec and create VideoWriter object
fourcc = cv2.VideoWriter_fourcc(*'XVID')
out = cv2.VideoWriter('output.avi', fourcc, 20.0, (640, 480))

# Open the camera
cap = cv2.VideoCapture(0)

i = 0
while(i < 1000):
    # Read a frame from the camera
    ret, frame = cap.read()
    if ret == True:
        # Flip the frame horizontally (optional)
        frame = cv2.flip(frame, 1)
        # Write the flipped frame to the output video file
        out.write(frame)
    else:
        break
    print(i)
    i = i + 1

# Release everything if job is finished
cap.release()
out.release()
cv2.destroyAllWindows()
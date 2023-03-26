import cv2
import numpy as np
'''
apiPreference    preferred Capture API backends to use. 
Can be used to enforce a specific reader implementation 
if multiple are available: 
e.g. cv2.CAP_MSMF or cv2.CAP_DSHOW.
'''
def nothing(x):
    pass
# open video0
#cap1 = cv2.VideoCapture(0)
cap2 = cv2.VideoCapture(4)
cap2.set(cv2.CAP_PROP_FOURCC,cv2.VideoWriter_fourcc('M','J','P','G'))
while True:
    # Capture frame-by-frame
    #ret1, frame1 = cap1.read()
    ret2, frame2 = cap2.read()
    #if ret1:
    #    cv2.imshow("camera1",frame1)
    if ret2:
        cv2.imshow("camera2",frame2)

    if cv2.waitKey(1) & 0xff == ord('a'):
        break
# When everything done, release the capture
#cap1.release()
cap2.release()
cv2.destroyAllWindows()

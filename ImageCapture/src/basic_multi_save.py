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
cap1 = cv2.VideoCapture("/dev/videoCam1")
cap1.set(cv2.CAP_PROP_FOURCC,cv2.VideoWriter_fourcc('M','J','P','G'))

cap2 = cv2.VideoCapture("/dev/videoCam2")
cap2.set(cv2.CAP_PROP_FOURCC,cv2.VideoWriter_fourcc('M','J','P','G'))

cap3 = cv2.VideoCapture("/dev/videoCam3")
cap3.set(cv2.CAP_PROP_FOURCC,cv2.VideoWriter_fourcc('M','J','P','G'))

i = 0
while(True):
    # Capture frame-by-frame
    ret1, frame1 = cap1.read()
    #ret2, frame2 = cap2.read()
    #ret3, frame3 = cap3.read()
    cv2.imwrite('test1.png',frame1)
    #cv2.imwrite('test2.png',frame2)
    #cv2.imwrite('test3.png',frame3)
    i = i + 1
# When everything done, release the capture
cap1.release()
cap2.release()
cap3.release()
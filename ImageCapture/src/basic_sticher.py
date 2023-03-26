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
cap1 = cv2.VideoCapture(0)
cap1.set(cv2.CAP_PROP_FOURCC,cv2.VideoWriter_fourcc('M','J','P','G'))

cap2 = cv2.VideoCapture(4)
cap2.set(cv2.CAP_PROP_FOURCC,cv2.VideoWriter_fourcc('M','J','P','G'))

cap3 = cv2.VideoCapture(8)
cap3.set(cv2.CAP_PROP_FOURCC,cv2.VideoWriter_fourcc('M','J','P','G'))
stichy = cv2.Stitcher.create()
while True:
    # Capture frame-by-frame
    ret1, frame1 = cap1.read()
    ret2, frame2 = cap2.read()
    ret3, frame3 = cap3.read()
    frame1 = cv2.resize(frame1, (0,0),fx=0.4,fy=0.4)
    frame2 = cv2.resize(frame2, (0,0),fx=0.4,fy=0.4)
    frame3 = cv2.resize(frame3, (0,0),fx=0.4,fy=0.4)

    imgs = [frame1,frame2,frame3]
    (dummy,output) = stichy.stitch(imgs)
    print(output.shape)
    if dummy != cv2.STITCHER_OK:
        cv2.imshow("Stitch",output)
    else:
        print("Stich pain")

    if cv2.waitKey(1) & 0xff == ord('a'):
        break
# When everything done, release the capture
cap1.release()
cap2.release()
cap3.release()
cv2.destroyAllWindows()

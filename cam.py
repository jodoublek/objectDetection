import cv2
import sys
import datetime
from operator import eq


def convertname():
    now=datetime.datetime.now()
    date=now.strftime("%Y%m%d_%H%M")
    return date

def folderpath():
    now=datetime.datetime.now()
    date=now.strftime("%Y%m%d_%H")
    return date

def stamptime():    
    now=datetime.datetime.now()
    date=now.strftime("%H:%M:%S.%f")
    return date


cam_id=('nvarguscamerasrc ! '
        'video/x-raw(memory:NVMM), '
        'width=640, height=480, '
        'format=NV12, framerate=30/1 ! '
        'nvvidconv ! '
        'video/x-raw, width=640, height=480, '
        'format=(string)BGRx ! '
        'videoconvert ! appsink')
def camAction():
    cam=cv2.VideoCapture(cam_id)
    width=int(cam.get(cv2.CAP_PROP_FRAME_WIDTH))
    height=int(cam.get(cv2.CAP_PROP_FRAME_HEIGHT))
    fourcc = cv2.VideoWriter_fourcc(*'DIVX')
    name=convertname()
    path="./rec/"+folderpath()+"/"+name+".avi"
    out = cv2.VideoWriter(path, fourcc, 25.0, (640, 480))
    if cam.isOpened() == False:
        print('open failed')
        exit()

#    cv2.namedWindow('CAM_Window')
#    cv2.resizeWindow('CAM_Window', 320, 240)
#    count =0
#    count1=0
    while(True):
        stamp=stamptime()
        ret, frame = cam.read()
        cv2.putText(frame, stamp,(0,100), cv2.FONT_HERSHEY_SIMPLEX, 1, (0, 2, 55,0))
        if ret:

#            print("cnt1: " + "%d" "  " % count1)
           # count1 = count1 + 1
            out.write(frame)
#            cv2.imshow('CAM_Window', frame)
#            if cv2.waitKey(0) >= 0:
#                break
#            if cv2.waitKey(0)==ord('t'):
#                while(True):
#                    if cv2.waitKey(0)==ord('s'):
#                        break
#        else:
#            break



        if eq(name,convertname())==False:


           # count=count+1

#          print("cnt: " + "%d" "  " % count)
            path="./rec/"+folderpath()+"/"+convertname()+".avi"
            out=cv2.VideoWriter(path, fourcc, 25.0, (640, 480))
            name=convertname()
        
    cam.release()
    out.release()
   # cv2.destroyWindow('CAM_Window')




camAction()

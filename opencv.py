import os
import cv2 as cv
import numpy as np

##### READ IMAGE #####
#img = cv.imread(os.getcwd() + "/pic.jpg")	#Read Image from file location, convert to a Python Object
#print(dir(img))
#cv.imshow('drums', img)	#Show/display the image on screen, large image will go off-screen
#cv.waitKey(0)	#Without this 'imshow' wont wait for any event and exit. This will make it wait for a Key to be pressed

##### DRAWING AND PAINTING ON BLANK CANVAS #####
#blank = np.zeros((500,500,3), dtype='uint8')
#blank[:] = 0,255,0	#Paint the blank canvas GREEN
#cv.imshow('Blank', blank)
#cv.waitKey(0)

##### RESIZE & RESCALE VIDEO ##### -> Reduce processing need of the processor if the video is TOO large
#Works on live Video, Video files and Image files
def rescaleFrame(frame, scale=0.75):
    height = int(frame.shape[0]*scale)	#frame.shape[0] => height
    width  = int(frame.shape[1]*scale)	#frame.shape[1] => width

    dimensions = (width, height)

    return cv.resize(frame, dimensions, interpolation = cv.INTER_AREA)

##### READ VIDEO #####
capture = cv.VideoCapture(0) #0 means webcam, 1 is another peripheral device etc. or we can give a file path
while True:
    isTrue, frame = capture.read()	#Read frames from the 'capture' object i.e. the Video

    frame_resized = rescaleFrame(frame)	#Remove this if you do NOT want to resize
    cv.imshow('Webcam', frame_resized)

    if cv.waitKey(20) & 0xFF == ord('d'):	#Press 'd' to breakout of the loop
        break

capture.release()
cv.destroyAllWindows()



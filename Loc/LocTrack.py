import cv2
from threading import Thread
import museum

visitor1 = museum.Visitor('Visitor 1')
visitor1.change_color('color1')

visitor2 = museum.Visitor('Visitor 2')
visitor2.change_color('color2')

# get the video
cap = cv2.VideoCapture(1)
if not cap.isOpened():  # for real time camera
    pass


def track_everybody():

    f = open("files/esp1", "r")
    esp1 = lastEsp1 = f.read()
    f.close()

    f = open("files/esp2", "r")
    esp2 = lastEsp2 = f.read()
    f.close()

    while cv2.waitKey(1) != 27: # ESC
        ret, img = cap.read()
        if img is None:
            break
            
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)  # Blue, Green, Red -> HSV coding
        img, x_of_1, y_of_1 = visitor1.is_getting_tracked(img, hsv, 10, 20)
        # img, x_of_2, y_of_2 = visitor2.is_getting_tracked(img, hsv, 10, 40)

        if x_of_1 < 200 and y_of_1 < 200:
            esp1 = "item3.m4a"
        else:
            esp1 = "item2.m4a"

        if esp1 != lastEsp1:

            f = open("files/esp1", "w")
            f.write(esp1)
            f.close()
            
            lastEsp1 = esp1

        cv2.imshow("Frame", img)


# faster
Thread(target=track_everybody(), args=())
cap.release()

cv2.destroyAllWindows()

import cv2
from threading import Thread
import museum

visitor1 = museum.Visitor('Swedish')
visitor1.change_color('yellow')

visitor2 = museum.Visitor('Scotch')
visitor2.change_color('red1')

# get the video
cap = cv2.VideoCapture("yellow_and_red_ball.mp4")
if not cap.isOpened():  # for real time camera
    pass


def track_everybody():
    while True:
        ret, img = cap.read()
        if img is None:
            break
        hsv = cv2.cvtColor(img, cv2.COLOR_BGR2HSV)  # Blue, Green, Red -> HSV coding
        img, x_of_1, y_of_1 = visitor1.is_getting_tracked(img, hsv, 10, 20)
        img, x_of_2, y_of_2 = visitor2.is_getting_tracked(img, hsv, 10, 40)

        cv2.imshow("Frame", img)

        if cv2.waitKey(1) == 27:
            break


# faster
Thread(target=track_everybody(), args=())
cap.release()

cv2.destroyAllWindows()

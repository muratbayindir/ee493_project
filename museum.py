import cv2
from collections import deque
import numpy as np
black = np.array([[180, 255, 30], [0, 0, 0]])
white = np.array([[180, 18, 255], [0, 0, 231]])
red1 = np.array([[180, 255, 255], [159, 50, 70]])
red2 = np.array([[9, 255, 255], [0, 50, 70]])
green = np.array([[89, 255, 255], [36, 50, 70]])
blue = np.array([[128, 255, 255], [90, 50, 70]])
yellow = np.array([[35, 255, 255], [25, 50, 70]])
purple = np.array([[158, 255, 255], [129, 50, 70]])
orange = np.array([[24, 255, 255], [10, 50, 70]])
gray = np.array([[180, 18, 230], [0, 0, 40]])

color_dict_HSV = {'black': black,
                  'white': white,
                  'red1': red1,
                  'red2': red2,
                  'green': green,
                  'blue': blue,
                  'yellow': yellow,
                  'purple': purple,
                  'orange': orange,
                  'gray': gray}


class Visitor:
    def __init__(self,
                 name=None,
                 color=None,
                 sections=None,
                 visit_map=None):
        if name is None:
            name = 'Ghost'
        if color is None:
            color = [[35, 255, 255], [25, 50, 70]]  # default color is yellow.
        if sections is None:
            sections = [True, True]
        if visit_map is None:
            visit_map = []
        self.name = name
        self.color = color
        self.sections = sections
        self.visit_map = visit_map

    def show_visit_map(self):
        pass

    def change_color(self, new_color):
        self.color = color_dict_HSV[new_color]

    def is_getting_tracked(self, frame, hsv, pixel_x, pixel_y):
        x = -1
        y = -1
        pts = deque(maxlen=50)
        krnl = np.ones((5, 5), np.uint8)  # burada yapıyor
        # masking
        mask = cv2.inRange(hsv, self.color[1], self.color[0])
        # dilation and erode
        mask = cv2.erode(mask, krnl, iterations=2)  # burada yapıyor
        mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, krnl)  # burada yapıyor
        mask = cv2.dilate(mask, krnl, iterations=1)    # burada yapıyor

        # finding contours
        count, heir = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2:]
        center = None

        if len(count) > 0:
            # Get the largest contour and its center
            c = max(count, key=cv2.contourArea)
            ((x, y), radius) = cv2.minEnclosingCircle(c)
            m = cv2.moments(c)
            center = (int(m["m10"] / m["m00"]), int(m["m01"] / m["m00"]))

            # Allow only contours that have a larger than 5 pixel radius
            if radius > 5:
                # creating circle to the contour
                text = "Location(in pixel) of " + str(self.name) + " -> x: " + str(int(x)) + " y: " + str(int(y))
                frame = cv2.circle(frame, (int(x), int(y)), int(radius), (0, 255, 255), 2)
                frame = cv2.circle(frame, center, 5, (0, 0, 255), -1)
                frame = cv2.putText(frame, text, (pixel_x, pixel_y), cv2.FONT_HERSHEY_SIMPLEX, 0.5, (34, 200, 34), 2)

        # appending centers
        pts.appendleft(center)
        for i in range(1, len(pts)):
            if pts[i - 1] is None or pts[i] is None:
                continue

            # displaying the traces
            thick = int(np.sqrt(len(pts) / float(i + 1)) * 2.5)
            frame = cv2.line(frame, pts[i - 1], pts[i], (0, 0, 225), thick)

        return frame, x, y

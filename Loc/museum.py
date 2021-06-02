import cv2
from collections import deque
import numpy as np

colorNone = np.array([[0, 0, 0], [0, 0, 0]])
# color1 = np.array([[180, 255, 30], [0, 0, 0]])
# color1 = np.array([[27, 197, 255], [21, 107, 140]])
color1 = np.array([[33, 184, 255], [26, 126, 145]])
color2 = np.array([[180, 18, 255], [0, 0, 231]])

color_dict_HSV = {'None': colorNone,
                  'color1': color1,
                  'color2': color2}


###########################################
total_x = 280  # this value must be changed
total_y = 210  # this value must be changed
###########################################

def pixels_to_cms(current_pixel, all_pixels, total_length):
    in_cms = (current_pixel/all_pixels) * total_length
    return in_cms

class Visitor:
    def __init__(self,
                 name=None,
                 color=None,
                 sections=None,
                 visit_map=None):
        if name is None:
            name = 'None'
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
        self.found = False
        self.x = 0
        self.y = 0

    def change_color(self, new_color):
        self.color = color_dict_HSV[new_color]

    def track(self, frame, hsv):
        x = -1
        y = -1
        pts = deque(maxlen=50)
        krnl = np.ones((4, 4), np.uint8)  # burada yapıyor
        # masking
        mask = cv2.inRange(hsv, self.color[1], self.color[0])
        # dilation and erode
        mask = cv2.erode(mask, krnl, iterations=1)  # burada yapıyor
        mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, krnl)  # burada yapıyor

        # orig        
        # krnl = np.ones((5, 5), np.uint8)  # burada yapıyor
        # # masking
        # mask = cv2.inRange(hsv, self.color[1], self.color[0])
        # # dilation and erode
        # mask = cv2.erode(mask, krnl, iterations=2)  # burada yapıyor
        # mask = cv2.morphologyEx(mask, cv2.MORPH_OPEN, krnl)  # burada yapıyor
        # mask = cv2.dilate(mask, krnl, iterations=1)    # burada yapıyor

        # finding contours
        count, heir = cv2.findContours(mask.copy(), cv2.RETR_EXTERNAL, cv2.CHAIN_APPROX_SIMPLE)[-2:]
        center = None
        found = False

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
                found = True


        self.found = found
        self.x = pixels_to_cms(x, frame.shape[0], total_x)
        self.y = pixels_to_cms(y, frame.shape[1], total_y)

        # appending centers
        pts.appendleft(center)
        for i in range(1, len(pts)):
            if pts[i - 1] is None or pts[i] is None:
                continue

            # displaying the traces
            thick = int(np.sqrt(len(pts) / float(i + 1)) * 2.5)
            frame = cv2.line(frame, pts[i - 1], pts[i], (0, 0, 225), thick)

        return frame
class Items:
    def __init__(self,
                 x=None,
                 y=None):
        if x is None:
            x = 0
        if y is None:
            y = 0
        self.x = x  # in cms
        self.y = y  # in cms

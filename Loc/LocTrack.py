import cv2
from threading import Thread
import museum
import math

def pixels_to_cms(current_pixel, all_pixels, total_length):
    in_cms = (current_pixel/all_pixels) * total_length
    return in_cms


def item_vs_visitors(item, visitor_x, visitor_y):
    distance = 0.00
    distance = (item.x-visitor_x)*(item.x-visitor_x)+(item.y-visitor_y)*(item.y-visitor_y)
    distance = math.sqrt(distance)
    if distance < 50:
        return True
    else:
        return False
    
  
def draw_circles_for_items(frame, item1, item2, item3):
    pass

# murat burayi duzenle!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
def choose_recordings(x, y, height, width, item1, item2, item3):
    close_to_this = 0
    if item_vs_visitors(item1, pixels_to_cms(x, height, total_x), pixels_to_cms(y, width, total_y)): # is it close to item1
        close_to_this = 1  # esp1 = "item1.m4a"
    elif item_vs_visitors(item2, pixels_to_cms(x, height, total_x), pixels_to_cms(y, width, total_y)): # is it close to item2
        close_to_this = 2  # esp1 = "item2.m4a"
    elif item_vs_visitors(item3, pixels_to_cms(x, height, total_x), pixels_to_cms(y, width, total_y)): # is it close to item3
        close_to_this = 3  # esp1 = "item3.m4a"
    else:
        close_to_this = 0  # esp1 = "item2.m4a"  # no record will be played!!!!!!!!!!! it is not close to any item
    return close_to_this


###########################################
total_x = 300  # this value must be changed
total_y = 300  # this value must be changed
###########################################

item1 = museum.Items(40, 40)  # this values must be changed
item2 = museum.Items(150, 150)  # this values must be changed
item3 = museum.Items(0, 250)  # this values must be changed


def draw_circles_for_items(frame, item1, item2, item3):
    pass
    

visitor1 = museum.Visitor('Visitor 1')
visitor1.change_color('color1')

visitor2 = museum.Visitor('Visitor 2')
visitor2.change_color('color2')

# get the video
cap = cv2.VideoCapture(1)
if not cap.isOpened():  # for real time camera
    pass


def track_everybody():
    
    v1_close_to = 0
    v2_close_to = 0
    v3_close_to = 0
    
    ret, img = cap.read()
    width, height, channels = img.shape
    
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
        
        v1_close_to = choose_recordings(x_of_1, y_of_1,
                                        height=height,
                                        width=width,
                                        item1=item1,
                                        item2=item2,
                                        item3=item3)

        v2_close_to = choose_recordings(x_of_2, y_of_2,
                                        height=height,
                                        width=width,
                                        item1=item1,
                                        item2=item2,
                                        item3=item3)
        
        frame = cv2.putText(img,
                          'Visitor 1 close to: '+str(v1_close_to),
                          (10, 60),
                          cv2.FONT_HERSHEY_SIMPLEX, 0.5, (34, 200, 34), 2)
        frame = cv2.putText(img,
                          'Visitor 2 close to: ' + str(v2_close_to),
                          (10, 80),
                          cv2.FONT_HERSHEY_SIMPLEX, 0.5, (34, 200, 34), 2)

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

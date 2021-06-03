import cv2
from threading import Thread
import museum
import math
import tkinter as tk
import threading
import time

def item_vs_visitors(item, visitor_x, visitor_y):
    distance = 0.00
    distance = (item.x-visitor_x)*(item.x-visitor_x)+(item.y-visitor_y)*(item.y-visitor_y)
    distance = math.sqrt(distance)
    if distance < 50*1.8:
        return True
    else:
        return False

# murat burayi duzenle!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
def choose_recordings(x, y, item1, item2, item3, door1, door2):
    close_to_this = 0
    if item_vs_visitors(item1, x, y): # is it close to item1
        close_to_this = 1  # esp1 = "item1.m4a"
    elif item_vs_visitors(item2, x, y): # is it close to item2
        close_to_this = 2  # esp1 = "item2.m4a"
    elif item_vs_visitors(item3, x, y): # is it close to item3
        close_to_this = 3  # esp1 = "item3.m4a"
    elif item_vs_visitors(door1, x, y):
        close_to_this = 4 # visitor at the door; play 'you are not allowed'.
    elif item_vs_visitors(door2, x, y):
        close_to_this = 5 # visitor at the door; play 'you are not allowed'.
    else:
        close_to_this = 0  # esp1 = "item2.m4a"  # no record will be played!!!!!!!!!!! it is not close to any item
    return close_to_this


###########################################
total_x = 480  # this value must be changed
total_y = 350  # this value must be changed
###########################################

item1 = museum.Items(47, 432)  # this values must be changed
item2 = museum.Items(593, 432)  # this values must be changed
item3 = museum.Items(320, 47)  # this values must be changed
door1 = museum.Items(47, 47) #tam kosede
door2 = museum.Items(593, 47) #unknown

visitor1 = museum.Visitor('Visitor 1')
visitor1.change_color('color1')

visitor2 = museum.Visitor('Visitor 2')
visitor2.change_color('color2')

window = tk.Tk()
<<<<<<< .mine
vis1Door1Allow = tk.IntVar()
=======
c = tk.Canvas(window, bg="white", width=640, height=480)
>>>>>>> .theirs
vis1Door2Allow = tk.IntVar()
vis2Door1Allow = tk.IntVar()
vis2Door2Allow = tk.IntVar()
c = tk.Canvas(window, bg="white", width=total_x, height=total_y)
lblVis1 = tk.Label(window, bg='white', width=20)
lblVis2 = tk.Label(window, bg='white', width=20)

esp1 = ""
esp2 = ""

def RefreshCanvas():

    c.delete("all")

    c.create_oval(item1.x - 4, item1.y - 4, item1.x + 4, item1.y + 4, fill="red")
    c.create_oval(item1.x - 66, item1.y - 66, item1.x + 66, item1.y + 66)
    c.create_text(item1.x-8, item1.y+12, fill="darkblue", text='Item 1')

    c.create_oval(item2.x - 4, item2.y - 4, item2.x + 4, item2.y + 4, fill="red")
    c.create_oval(item2.x - 66, item2.y - 66, item2.x + 66, item2.y + 66)
    c.create_text(item2.x-8, item2.y+12, fill="darkblue", text='Item 2')
    
    c.create_oval(item3.x - 4, item3.y - 4, item3.x + 4, item3.y + 4, fill="red")
    c.create_oval(item3.x - 66, item3.y - 66, item3.x + 66, item3.y + 66)
    c.create_text(item3.x-8, item3.y+12, fill="darkblue", text='Item 3')
    
    c.create_oval(door1.x - 4, door1.y - 4, door1.x + 4, door1.y + 4, fill="green")
    c.create_oval(door1.x - 66, door1.y - 66, door1.x + 66, door1.y + 66)
    c.create_text(door1.x+8, door1.y+12, fill="darkblue", text='Door 1')

    c.create_oval(door2.x - 4, door2.y - 4, door2.x + 4, door2.y + 4, fill="green")
    c.create_oval(door2.x - 66, door2.y - 66, door2.x + 66, door2.y + 66)
    c.create_text(door2.x-8, door2.y+12, fill="darkblue", text='Door 2')
    
    if(visitor1.found):
        c.create_oval(visitor1.x - 4, visitor1.y - 4, visitor1.x + 4, visitor1.y + 4, fill="blue")
        c.create_text(visitor1.x-8, visitor1.y+12, fill="darkblue", text='V1')
    if(visitor2.found):
        c.create_oval(visitor2.x - 4, visitor2.y - 4, visitor2.x + 4, visitor2.y + 4, fill="blue")
        c.create_text(visitor2.x-8, visitor2.y+12, fill="darkblue", text='V2')

    lblVis1.config(text=esp1)
    lblVis2.config(text=esp2)

def GuiClock():
    RefreshCanvas()
    window.after(100, GuiClock)

def GuiStart():

    chkVis1Door1Allow = tk.Checkbutton(window, text="Visitor 1 Door 1 Allow", variable=vis1Door1Allow, onvalue=1, offvalue=0)
    chkVis1Door2Allow = tk.Checkbutton(window, text="Visitor 1 Door 2 Allow", variable=vis1Door2Allow, onvalue=1, offvalue=0)
    chkVis2Door1Allow = tk.Checkbutton(window, text="Visitor 2 Door 1 Allow", variable=vis2Door1Allow, onvalue=1, offvalue=0)
    chkVis2Door2Allow = tk.Checkbutton(window, text="Visitor 2 Door 2 Allow", variable=vis2Door2Allow, onvalue=1, offvalue=0)

    lblVis1.pack()
    lblVis2.pack()
    chkVis1Door1Allow.pack()
    chkVis1Door2Allow.pack()
    chkVis2Door1Allow.pack()
    chkVis2Door2Allow.pack()
    c.pack()

    GuiClock()
    window.mainloop()

def track_everybody():
    # get the video
    cap = cv2.VideoCapture(1)
    if not cap.isOpened():  # for real time camera
        pass
    
    v1_close_to = 0
    v2_close_to = 0
    
    ret, img = cap.read()
    height, width, channels = img.shape

    global esp1
    global esp2

    global vis1Door1Allow
    global vis1Door2Allow
    global vis2Door1Allow
    global vis2Door2Allow
    
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
        img = visitor1.track(img, hsv)
        img = visitor2.track(img, hsv)
        
        v1_close_to = choose_recordings(visitor1.x, visitor1.y,
                                        item1=item1,
                                        item2=item2,
                                        item3=item3,
                                        door1=door1,
                                        door2=door2)

        v2_close_to = choose_recordings(visitor2.x, visitor2.y,
                                        item1=item1,
                                        item2=item2,
                                        item3=item3,
                                        door1=door1,
                                        door2=door2)
        
        frame = cv2.putText(img,
                          'Visitor 1 close to: '+ str(v1_close_to),
                          (10, 20),
                          cv2.FONT_HERSHEY_SIMPLEX, 0.5, (34, 200, 34), 2)
                          
        frame = cv2.putText(img,
                          'Visitor 2 close to: ' + str(v2_close_to),
                          (10, 40),
                          cv2.FONT_HERSHEY_SIMPLEX, 0.5, (34, 200, 34), 2)
        frame = cv2.putText(img,
                          'visit1alloweddoor1: '+ str(vis1Door1Allow) + 'visit1alloweddoor2: '+ str(vis1Door2Allow),
                          (10, 60),
                          cv2.FONT_HERSHEY_SIMPLEX, 0.5, (34, 200, 34), 2)

        if v1_close_to == 4: # door1
            esp1 = "door1.aac" if vis1Door1Allow.get() else "notAllowed.aac"
        elif v1_close_to == 5: # door2
            esp1 = "door2.aac" if vis1Door2Allow.get() else "notAllowed.aac"
        else:
                esp1 = "notAllowed.m4a"
        else:
            esp1 = "item" + str(v1_close_to) + ".m4a"

        if esp1 != lastEsp1:

            f = open("files/esp1", "w")
            f.write(esp1)
            f.close()
            
            lastEsp1 = esp1

        if v2_close_to == 4: # door1
            esp1 = "door1.aac" if vis2Door1Allow.get() else "notAllowed.aac"
        elif v2_close_to == 5: # door2
            esp1 = "door2.aac" if vis2Door2Allow.get() else "notAllowed.aac"
        else:
                esp2 = "notAllowed.m4a"
        else:
            esp2 = "item" + str(v2_close_to) + ".m4a"

        if esp2 != lastEsp2:

            f = open("files/esp2", "w")
            f.write(esp2)
            f.close()
            
            lastEsp2 = esp2

        cv2.imshow("Frame", img)

    cap.release()

th2 = Thread(target=track_everybody, args=())
th2.start()

GuiStart()
th2.join()

cv2.destroyAllWindows()


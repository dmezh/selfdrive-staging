#!/usr/bin/env python3
from tkinter import * 
from tkinter import messagebox
import can
import cantools
import cantools.database.can as candb

class Node():

    def __init__(self,name,root,column,color,cmdmsg):
        self.name = name
        self.root = root
        self.col = column
        self.id = column
        self.color = color
        self.cmdmsg = cmdmsg

        self.frame = Frame(root, height=300, width=500, borderwidth=3, relief='solid', bg=self.color).grid(row=0, column=self.col, padx=10, pady=10)
        self.label = Label(self.frame, text=self.name).grid(row=0, column=self.col, pady=30, sticky=N)
        self.message = StringVar()
        self.entry = Entry(self.frame, textvariable=self.message).grid(row=0, column=self.col)
        self.button = Button(self.frame, text="Send CAN Message", command=self.cansend).grid(row=0, column=self.col, pady=30, sticky=S)
        

    def cansend(self):
        send = messagebox.askyesno(title="cansend", message="Send this message to " + self.name + "?")
        if send == True:
            
            message_name = db.get_message_by_name(self.cmdmsg)
            cmd = float(self.message.get())
            print(cmd)
            data = message_name.encode({'ThrottleCmd': cmd, 'ModeCtrl': 1})
            input = can.Message(arbitration_id=message_name.frame_id, data=data, is_extended_id=False)            
            bus0.send(input)
            messagebox.showinfo(title="cansend", message="CAN message sent!")   #Not sure if we need to check if the message is garbage or not 
        else:
            messagebox.showinfo(title="cansend", message="Message not sent!")
            pass
        

def mode_switch():
    switch = messagebox.askyesno(title="mode switch", message="Switch driving mode?")
    if switch == True:
        mode_message = db.get_message_by_name('dbwNode_Mode')
        data = mode_message.encode({'Mode': mode})
        msg = can.Message(arbitration_id=mode_message.frame_id, data=data, is_extended_id=False)
        bus0.send(msg)
        messagebox.showinfo(title="mode switch", message="Mode switched!")
    else:
        messagebox.showinfo(title="mode switch", message="Mode not switched!")
        pass

bus0 = can.interface.Bus('vcan0', bustype='socketcan')
db = cantools.database.load_file('../../can/igvc_can.dbc')  #creates a can database from dbc file
mode = bool(0)

if __name__ == "__main__":

    root = Tk()
    root.title("DBW Watchdog GUI")

    node1 = Node("Accel-Node", root, 0, "red", 'dbwNode_Accel_Cmd')
    node2 = Node("Brake-Node", root, 1, "blue", 'dbwNode_Brake_Cmd')
    #node3 = Node("Steer-Node", root, 2, "green", "")

    if mode == 1:
        mode_label = Label(root, text="Current Mode: DBW", font=('Aerial', 28)).grid(row=1, column=0)
        mode = ~mode
    else:
        mode_label = Label(root, text="Current Mode: Manual", font=('Aerial', 28)).grid(row=1, column=0)
    mode_control = Button(root, text="Change Mode", command=mode_switch, font=('Aerial', 28)).grid(row=1, column=1)
        
    root.mainloop()

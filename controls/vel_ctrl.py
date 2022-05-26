#!/usr/bin/env python3
import igvcutils
import time
from cand.client import Client
from PID_beard import PIDController

from geometry_msgs.msg import Twist

class vel_ctrl:

    def __init__(self):
        self.controller = PIDController(kp=1, ki=0.05, kd=0.02, Ts=0.1)
        self.bus = Client()

    def ctrl_from_twist(self, twist_message: Twist):
        v_des = twist_message.linear.x
        enc = self.bus.get('dbwNode_Encoder_Data')
        delta = (time.time_ns() - enc[0])/1000000000
        data = enc[1]
        v_actual = self.enc_to_velocity(data, delta)
        ctrl_out = self.controller.PID(v_des, v_actual)
        pedal_percentage = self.acc_to_pedal(ctrl_out)
        print(f'v_actual: {v_actual} pedal_percentage: {pedal_percentage}')
        self.bus.send("dbwNode_SysCmd", {"DbwActive": 1, "ESTOP": 0})
        self.bus.send('dbwNode_Accel_Cmd', {'ThrottleCmd': max(pedal_percentage, 0) / 100, 'ModeCtrl': 1})

    def ctrl_vel_fixed(self):
        v_des = 2.2352
        enc = self.bus.get('dbwNode_Encoder_Data')
        delta = (time.time_ns() - enc[0])/1000000000
        data = enc[1]
        v_actual = self.enc_to_velocity(data, delta)
        ctrl_out = self.controller.PID(v_des, v_actual)
        pedal_percentage = self.acc_to_pedal(ctrl_out)
        print(f'v_actual: {v_actual} pedal_percentage: {pedal_percentage}')
        self.bus.send("dbwNode_SysCmd", {"DbwActive": 1, "ESTOP": 0})
        self.bus.send('dbwNode_Accel_Cmd', {'ThrottleCmd': max(pedal_percentage, 0) / 100, 'ModeCtrl': 1})

    def acc_to_pedal(self, acceleration):
        return 1*acceleration

    def enc_to_velocity(self, enc, time):
        enc_ticks = 4000
        wheel_circumference = 1.899156
        meters_per_tick = wheel_circumference/enc_ticks
        return (meters_per_tick*enc)/time

if __name__ == "__main__":
    c = vel_ctrl()
    while(1):
        c.ctrl_vel_fixed()
        time.sleep(0.1)

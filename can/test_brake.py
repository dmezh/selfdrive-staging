#!/usr/bin/env python3

import can
import cantools

from pprint import pprint
from time import sleep

bus = can.interface.Bus('vcan0', bustype='socketcan')

db = cantools.database.load_file('igvc_can.dbc')
pprint(db.messages)

command_message = db.get_message_by_name('dbwNode_Brake_Cmd')

for i in range(0, 60):
    i = float(i) / 100.0
    cmd = command_message.encode({'BrakeCmd': i})
    msg = can.Message(arbitration_id=command_message.frame_id, data=cmd, is_extended_id=False)
    bus.send(msg)
    sleep(0.1)

#!/bin/bash

import can
import cantools

from pprint import pprint

from time import sleep

from tqdm import trange

def main():
    img = open(".pio/build/blink/firmware.bin", "rb")
    img_raw = img.read()

    db = cantools.database.load_file("../../can/igvc_can.dbc")
    print(db.messages)

    print('')

    updater_meta = db.get_message_by_name('dbwUpdater_Meta')
    pprint(updater_meta.signals)

    print('')

    updater_data = db.get_message_by_name('dbwUpdater_Data')
    pprint(updater_data.signals)

    print('')

    bus = can.interface.Bus('can0', bustype='socketcan')

    # Trigger the update
    t_data = updater_meta.encode({'Begin': 1, 'Trigger': 0, 'FinalSize': 0})
    t_msg = can.Message(arbitration_id=updater_meta.frame_id, data=t_data, is_extended_id=False)

    bus.send(t_msg)

    response_msg = db.get_message_by_name('dbwBootloader_Data')
    pprint(response_msg.signals)

    print("Sent trigger message; waiting for node response...")
    for msg in bus:
#        if True: 
        if msg.arbitration_id == response_msg.frame_id:
            print("Got response!")
            pprint(msg)

            sleep(0.05)
            print("Sending update begin.")
            b_data = updater_meta.encode({'Begin': 1, 'Trigger': 0, 'FinalSize': 0})
            b_msg = can.Message(arbitration_id=updater_meta.frame_id, data=b_data, is_extended_id=False)
            bus.send(b_msg)

            sleep(5)
            print("Sending update data.")

            step = 5

            for i in trange(0, len(img_raw), step):
            #    sleep(0.05)
                
                raw_data = int.from_bytes(img_raw[i:i+step], 'little')
                #print(f"{img_raw[i]}, {raw_data}")
                u_data = updater_data.encode({'Position': i/step, 'Data': raw_data})
                u_msg = can.Message(arbitration_id=updater_data.frame_id, data=u_data, is_extended_id=False)

                bus.send(u_msg, timeout=None)
                sleep(0.00025)

#                print(img_raw[i:i+4].hex())

            sleep(0.05)
            d_data = updater_meta.encode({'Begin': 0, 'Trigger': 0, 'FinalSize': len(img_raw)})
            d_msg = can.Message(arbitration_id=updater_meta.frame_id, data=d_data, is_extended_id=False)
            bus.send(d_msg)

            return


if __name__ == "__main__":
    main()

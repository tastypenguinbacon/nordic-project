#!/bin/bash

address="$1"
alert_level="$2"

echo -e 'power on\n'

sleep 1

echo -e 'connect' $address '\n'

sleep 2 

echo -e 'select-attribute /org/bluez/hci0/dev_'${address:0:2}'_'${address:3:2}'_'${address:6:2}'_'${address:9:2}'_'${address:12:2}'_'${address:15:2}'/service000d/char000e\n'

sleep 2

echo -e 'write' $alert_level '\n'

sleep 2

echo -e 'disconnect' $address '\n'

sleep 1

echo -e 'quit\n'

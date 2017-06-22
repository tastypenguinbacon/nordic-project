#!/bin/bash

systemctl stop bluetooth
sleep 2
systemctl status bluetooth
sleep 2
systemctl start bluetooth

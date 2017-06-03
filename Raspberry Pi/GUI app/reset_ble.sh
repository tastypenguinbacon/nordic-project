#!/bin/bash

down="sudo hciconfig hci0 down"

eval $down

up="sudo hciconfig hci0 up"

eval $up

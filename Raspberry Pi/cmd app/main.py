# always seem to need this
import sys
import os

import signal
from subprocess import Popen, PIPE, TimeoutExpired, call
from time import monotonic as timer
import threading
 
import time
 
import json
 
import http.client
 
bKeys_1 = b'D9:D7:88:35:C4:F8'
bKeys_2 = b'E0:0E:F7:E4:6B:14'
bPer_1 = b'C1:44:EA:74:47:42'
bPer_2 = b'C5:03:B9:81:0B:D8' 
 
Keys_1 = 'D9:D7:88:35:C4:F8'
Keys_2 = 'E0:0E:F7:E4:6B:14'
Per_1 = 'C1:44:EA:74:47:42' 
Per_2 = 'C5:03:B9:81:0B:D8'  
    
ip_address = " "
    
### functions for the buttons to call
def Keys1_al_mid():
	call('./keys.sh ' + Keys_1 + ' 0x01|bluetoothctl &', shell=True)        
	
def Keys1_al_high():
	call('./keys.sh ' + Keys_1 + ' 0x02|bluetoothctl &', shell=True)            
	
def Keys2_al_mid():
	call('./keys.sh ' + Keys_2 + ' 0x01|bluetoothctl &', shell=True)        
	
def Keys2_al_high():
	call('./keys.sh ' + Keys_2 + ' 0x02|bluetoothctl &', shell=True)            
	
def Keys1_bat():
	call('./reset_ble.sh', shell=True)	
	cmd = "sudo node Keys_bat.js " + Keys_1.lower()
	output = 'None'
	with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
		try:
			output = process.communicate(timeout=5)[0]
		except TimeoutExpired:
			os.killpg(process.pid, signal.SIGINT) # send signal to the process group
			output = process.communicate()[0]
	print(output)
	output_s = output.decode("utf-8")
	s_data = output_s.split('\n')
	if len(s_data) > 1:
		send_status("Keys", "1", s_data)
	
def Keys2_bat():
	call('./reset_ble.sh', shell=True)
	cmd = "sudo node Keys_bat.js " + Keys_2.lower()
	output = 'None'
	with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
		try:
			output = process.communicate(timeout=5)[0]
		except TimeoutExpired:
			os.killpg(process.pid, signal.SIGINT) # send signal to the process group
			output = process.communicate()[0]
	print(output)	
	output_s = output.decode("utf-8")
	s_data = output_s.split('\n')
	if len(s_data) > 1:
		send_status("Keys", "2", s_data)
	
def Per1_dig_write(string_val):
	call('./reset_ble.sh', shell=True)        
	cmd = "sudo node Per_dig_write.js " + Per_1.lower() + ' ' + hex(int(string_val))
	output = 'None'
	with Popen(cmd, shell=True, preexec_fn=os.setsid) as process:
		try:
			output = process.communicate(timeout=5)[0]
		except TimeoutExpired:
			os.killpg(process.pid, signal.SIGINT) # send signal to the process group
			output = process.communicate()[0]            	

def Per1_read_all():
	call('./reset_ble.sh', shell=True)
	cmd = "sudo node Per_read_all.js " + Per_1.lower()
	output = 'None'
	with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
		try:
			output = process.communicate(timeout=5)[0]
		except TimeoutExpired:
			os.killpg(process.pid, signal.SIGINT) # send signal to the process group
			output = process.communicate()[0]
	print(output)
	output_s = output.decode("utf-8")
	values_s = output_s.split('\n')
	for s in values_s: print(s)
	if len(values_s) > 1:
		send_status("Per", "1", values_s)
	
def Per2_dig_write(string_val):
	call('./reset_ble.sh', shell=True)        
	cmd = "sudo node Per_dig_write.js " + Per_2.lower() + ' ' + hex(int(string_val))
	output = 'None'
	with Popen(cmd, shell=True, preexec_fn=os.setsid) as process:
		try:
			output = process.communicate(timeout=5)[0]
		except TimeoutExpired:
			os.killpg(process.pid, signal.SIGINT) # send signal to the process group
			output = process.communicate()[0]              
	print(output)

def Per2_read_all():
	call('./reset_ble.sh', shell=True)
	cmd = "sudo node Per_read_all.js " + Per_2.lower()
	output = 'None'
	with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
		try:
			output = process.communicate(timeout=5)[0]
		except TimeoutExpired:
			os.killpg(process.pid, signal.SIGINT) # send signal to the process group
			output = process.communicate()[0]
	print(output)
	output_s = output.decode("utf-8")
	values_s = output_s.split('\n')
	for s in values_s: print(s)
	if len(values_s) > 1:
		send_status("Per", "2", values_s)

def Per1_led_on():
	call('./reset_ble.sh', shell=True)        
	cmd = "sudo node Per_led.js " + Per_1.lower() + ' 0x01'
	output = 'None'
	with Popen(cmd, shell=True, preexec_fn=os.setsid) as process:
		try:
			output = process.communicate(timeout=5)[0]
		except TimeoutExpired:
			os.killpg(process.pid, signal.SIGINT) # send signal to the process group
			output = process.communicate()[0]

def Per1_led_off():
	call('./reset_ble.sh', shell=True)        
	cmd = "sudo node Per_led.js " + Per_1.lower() + ' 0x00'
	output = 'None'
	with Popen(cmd, shell=True, preexec_fn=os.setsid) as process:
		try:
			output = process.communicate(timeout=5)[0]
		except TimeoutExpired:
			os.killpg(process.pid, signal.SIGINT) # send signal to the process group
			output = process.communicate()[0]    

def Per2_led_on():
	call('./reset_ble.sh', shell=True)        
	cmd = "sudo node Per_led.js " + Per_2.lower() + ' 0x01'
	output = 'None'
	with Popen(cmd, shell=True, preexec_fn=os.setsid) as process:
		try:
			output = process.communicate(timeout=5)[0]
		except TimeoutExpired:
			os.killpg(process.pid, signal.SIGINT) # send signal to the process group
			output = process.communicate()[0]        

def Per2_led_off():
	call('./reset_ble.sh', shell=True)        
	cmd = "sudo node Per_led.js " + Per_2.lower() + ' 0x00'
	output = 'None'
	with Popen(cmd, shell=True, preexec_fn=os.setsid) as process:
		try:
			output = process.communicate(timeout=5)[0]
		except TimeoutExpired:
			os.killpg(process.pid, signal.SIGINT) # send signal to the process group
			output = process.communicate()[0]                    

def SearchDevices():
	call('./reset_ble.sh', shell=True)        
	call("echo search devices pressed", shell=True)
	seconds = 5
	cmd = "sudo hcitool lescan &"
	output = 'None'
	start = timer()
	with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
		try:
			output = process.communicate(timeout=3)[0]
		except TimeoutExpired:
			os.killpg(process.pid, signal.SIGINT) # send signal to the process group
			output = process.communicate()[0]
	print(output)
	close_doors = False
	if bKeys_1 in output:
		Keys1_bat()		
	else:
		close_doors = True
	if bKeys_2 in output:
		Keys2_bat()
	else:
		close_doors = True
	if bPer_1 in output:
		Per1_read_all()
		if close_doors:
			Per1_led_on()
	if bPer_2 in output:		
		Per2_read_all()
		if close_doors:
			Per2_led_on()

def send_status(device_type, device_id, data):
	conn = http.client.HTTPConnection(ip_address)
 
	payload = "{\r\n\t\"type\": \"status\",\r\n\t\"message\": {\r\n\t\t\"timestamp\": " + str(int(time.time())) + "," + \
	"\r\n\t\t\"device_type\": \"" + device_type + "\",\r\n\t\t\"device_id\": \"" + device_id + "\",\r\n\t\t\"data\": [ "
	payload += ','.join((map(lambda x: '"' + str(x) + '"', data[:-1])))
	payload += " ]\r\n\t}\r\n}"
	print (payload)
	
	headers = {
		'content-type': "application/json",
		'cache-control': "no-cache",
		'postman-token': "8a377bd7-cc4b-7909-6a13-2a0245baa7d3"
		}
	 
	conn.request("POST", "/peripherial/message/raspi", payload, headers)

	res = conn.getresponse()
	data = res.read()
	
	print(data.decode("utf-8"))

def send_heartbeat():
	conn = http.client.HTTPConnection(ip_address)
 
	payload = "{ }"

	headers = {
		'content-type': "application/json",
		'cache-control': "no-cache",
		'postman-token': "8a377bd7-cc4b-7909-6a13-2a0245baa7d3"
		}
	 
	conn.request("POST", "/peripherial/heartbeat/raspi", payload, headers)

	res = conn.getresponse()
	data = res.read()
	 
	print(data.decode("utf-8"))

def check_request():
	conn = http.client.HTTPConnection(ip_address)
 
	payload = "{\n\t\"type\": \"request\",\n\t\"message\": {\n\t\t\"timestamp\": " + str(int(time.time()) - 40) + "\n\t}\n}"
	
	print (payload)
	
	headers = {
		'content-type': "application/json",
		'cache-control': "no-cache",
		'postman-token': "8a377bd7-cc4b-7909-6a13-2a0245baa7d3"
		}
	 
	conn.request("POST", "/peripherial/message/raspi", payload, headers)

	res = conn.getresponse()
	data = res.read()
	
	print(data.decode("utf-8"))
	
	content = json.loads(data.decode("utf-8"))
	
	content = list((map(lambda x: x['code'], content)))
	
	content = list(set(content))
	
	for s in reversed(content):
		if s == 0:
			Keys1_al_mid()
			break
		elif s == 1:
			Keys1_al_high()
			break
		elif s == 2:
			Keys2_al_mid()
			break
		elif s == 3:
			Keys2_al_high()
			break
		elif s == 4:
			Per1_led_on()
		elif s == 5:
			Per1_led_off()
		elif s == 6:
			Per2_led_on()
		elif s == 7:
			Per2_led_off()
		elif (s > 9) and (s < 111):
			Per1_dig_write(s - 10)
		elif (s > 119) and (s < 221):
			Per2_dig_write(s - 120)					

# I feel better having one of these
def main():	        	    

	print(ip_address)

	counter = 0

	if True:
    # Loop until we reach 20 minutes running
		while counter != 1000:
			send_heartbeat()
			check_request()
			SearchDevices()
			# Sleep for a minute
			time.sleep(20)
			# Increment the minute total
			counter += 1
    
	input('Type something to end')
 
# python bit to figure how who started This
if __name__ == "__main__":
    ip_address = str(sys.argv[1])
    main()

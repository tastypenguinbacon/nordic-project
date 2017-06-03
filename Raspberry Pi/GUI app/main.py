# always seem to need this
import sys
import os

# This gets the Qt stuff
import PyQt5
from PyQt5.QtCore import (pyqtSignal, QAbstractListModel, QDir, QLibraryInfo,
        QModelIndex, Qt)
		
from PyQt5.QtWidgets import *

import signal
from subprocess import Popen, PIPE, TimeoutExpired, call
from time import monotonic as timer
import threading
 
# This is our window from QtCreator
import mainwindow_auto
 
bKeys_1 = b'D9:D7:88:35:C4:F8'
bKeys_2 = b'E0:0E:F7:E4:6B:14'
bPer_1 = b'C1:44:EA:74:47:42'
bPer_2 = b'C5:03:B9:81:0B:D8' 
 
Keys_1 = 'D9:D7:88:35:C4:F8'
Keys_2 = 'E0:0E:F7:E4:6B:14'
Per_1 = 'C1:44:EA:74:47:42' 
Per_2 = 'C5:03:B9:81:0B:D8'  
 
# create class for our Raspberry Pi GUI
class MainWindow(QMainWindow, mainwindow_auto.Ui_MainWindow):
	# access variables inside of the UI's file
	
	def do_every (self, interval, worker_func, iterations = 0):
		if iterations != 1:
			threading.Timer (
				interval,
				self.do_every, [interval, worker_func, 0 if iterations == 0 else iterations-1]
		).start ()

		worker_func()
	
	### functions for the buttons to call
	def pressedKey1_al_mid(self):
		call('./keys.sh ' + Keys_1 + ' 0x01|bluetoothctl &', shell=True)		
		
	def pressedKey1_al_high(self):
		call('./keys.sh ' + Keys_1 + ' 0x02|bluetoothctl &', shell=True)			
		
	def pressedKey2_al_mid(self):
		call('./keys.sh ' + Keys_2 + ' 0x01|bluetoothctl &', shell=True)		
		
	def pressedKey2_al_high(self):
		call('./keys.sh ' + Keys_2 + ' 0x02|bluetoothctl &', shell=True)			
		
	def pressedKey1_bat(self):
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
		self.lneKeys1_bat.setText(output.decode("utf-8") + " %")
		
	def pressedKey2_bat(self):
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
		self.lneKeys2_bat.setText(output.decode("utf-8") + " %")
	
	def pressedPer1_temp(self):
		call('./reset_ble.sh', shell=True)
		cmd = "sudo node Per_temp.js " + Per_1.lower()
		output = 'None'
		with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]
		print(output)
		self.lnePer1_temp.setText(output.decode("utf-8") + " °C")			
		
	def pressedPer1_dig_read(self):			
		call('./reset_ble.sh', shell=True)
		cmd = "sudo node Per_dig_read.js " + Per_1.lower()
		output = 'None'
		with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]
		print(output)
		self.lnePer1_dig.setText(output.decode("utf-8"))			
		
	def pressedPer1_dig_write(self):
		call('./reset_ble.sh', shell=True)		
		cmd = "sudo node Per_dig_write.js " + Per_1.lower() + ' ' + hex(int(self.lnePer1_dig.text()))
		output = 'None'
		with Popen(cmd, shell=True, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]			
		
		
	def pressedPer1_light(self):
		call('./reset_ble.sh', shell=True)
		cmd = "sudo node Per_light.js " + Per_1.lower()
		output = 'None'
		with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]
		print(output)
		self.lnePer1_light.setText(output.decode("utf-8") + " %")	
		
	def pressedPer1_hum(self):
		call('./reset_ble.sh', shell=True)
		cmd = "sudo node Per_hum.js " + Per_1.lower()
		output = 'None'
		with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]
		print(output)
		self.lnePer1_hum.setText(output.decode("utf-8") + " %")

	def pressedPer1_alert(self):
		call('./reset_ble.sh', shell=True)
		cmd = "sudo node Per_alert.js " + Per_1.lower()
		output = 'None'
		with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]
		print(output)
		self.lnePer1_alert.setText(output.decode("utf-8"))				
	
	def pressedPer1_but(self):
		call('./reset_ble.sh', shell=True)
		cmd = "sudo node Per_but.js " + Per_1.lower()
		output = 'None'
		with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]
		print(output)
		self.lnePer1_but.setText(output.decode("utf-8"))
	
	def pressedPer1_read_all(self):
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
		#if len(values_s) == 7:
		#	self.lnePer1_temp.setText(values_s[0] + " °C")
		#	self.lnePer1_dig.setText(values_s[1])
		#	self.lnePer1_light.setText(values_s[2] + " %")	
		#	self.lnePer1_hum.setText(values_s[3] + " %")
		#	self.lnePer1_alert.setText(values_s[4])	
		#	self.lnePer1_but.setText(values_s[5])
		#else:
		#	self.lnePer1_temp.setText(" °C")
		#	self.lnePer1_dig.setText(" ")
		#	self.lnePer1_light.setText(" %")	
		#	self.lnePer1_hum.setText(" %")
		#	self.lnePer1_alert.setText(" ")	
		#	self.lnePer1_but.setText(" ")
	
	def pressedPer2_temp(self):
		call('./reset_ble.sh', shell=True)
		cmd = "sudo node Per_temp.js " + Per_2.lower()
		output = 'None'
		with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]
		print(output)
		self.lnePer2_temp.setText(output.decode("utf-8") + " °C")			
		
	def pressedPer2_dig_read(self):			
		call('./reset_ble.sh', shell=True)
		cmd = "sudo node Per_dig_read.js " + Per_2.lower()
		output = 'None'
		with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]
		print(output)
		self.lnePer2_dig.setText(output.decode("utf-8"))			
		
	def pressedPer2_dig_write(self):
		call('./reset_ble.sh', shell=True)		
		cmd = "sudo node Per_dig_write.js " + Per_2.lower() + ' ' + hex(int(self.lnePer2_dig.text()))
		output = 'None'
		with Popen(cmd, shell=True, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]			
		
		
	def pressedPer2_light(self):
		call('./reset_ble.sh', shell=True)
		cmd = "sudo node Per_light.js " + Per_2.lower()
		output = 'None'
		with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]
		print(output)
		self.lnePer2_light.setText(output.decode("utf-8") + " %")	
		
	def pressedPer2_hum(self):
		call('./reset_ble.sh', shell=True)
		cmd = "sudo node Per_hum.js " + Per_2.lower()
		output = 'None'
		with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]
		print(output)
		self.lnePer2_hum.setText(output.decode("utf-8") + " %")

	def pressedPer2_alert(self):
		call('./reset_ble.sh', shell=True)
		cmd = "sudo node Per_alert.js " + Per_2.lower()
		output = 'None'
		with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]
		print(output)
		self.lnePer2_alert.setText(output.decode("utf-8"))				
	
	def pressedPer2_but(self):
		call('./reset_ble.sh', shell=True)
		cmd = "sudo node Per_but.js " + Per_2.lower()
		output = 'None'
		with Popen(cmd, shell=True, stdout=PIPE, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]
		print(output)
		self.lnePer2_but.setText(output.decode("utf-8"))
	
	def pressedPer2_read_all(self):
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
		#if len(values_s) == 7:		
		#	self.lnePer2_temp.setText(values_s[0] + " °C")
		#	self.lnePer2_dig.setText(values_s[1])
		#	self.lnePer2_light.setText(values_s[2] + " %")	
		#	self.lnePer2_hum.setText(values_s[3] + " %")
		#	self.lnePer2_alert.setText(values_s[4])	
		#	self.lnePer2_but.setText(values_s[5])
		#else:
		#	self.lnePer2_temp.setText(" °C")
		#	self.lnePer2_dig.setText(" ")
		#	self.lnePer2_light.setText(" %")	
		#	self.lnePer2_hum.setText(" %")
		#	self.lnePer2_alert.setText(" ")	
		#	self.lnePer2_but.setText(" ")
	
	def pressedPer1_led_on(self):
		call('./reset_ble.sh', shell=True)		
		cmd = "sudo node Per_led.js " + Per_1.lower() + ' 0x01'
		output = 'None'
		with Popen(cmd, shell=True, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]		
	
	def pressedPer1_led_off(self):
		call('./reset_ble.sh', shell=True)		
		cmd = "sudo node Per_led.js " + Per_1.lower() + ' 0x00'
		output = 'None'
		with Popen(cmd, shell=True, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]	

	def pressedPer2_led_on(self):
		call('./reset_ble.sh', shell=True)		
		cmd = "sudo node Per_led.js " + Per_2.lower() + ' 0x01'
		output = 'None'
		with Popen(cmd, shell=True, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]		
	
	def pressedPer2_led_off(self):
		call('./reset_ble.sh', shell=True)		
		cmd = "sudo node Per_led.js " + Per_2.lower() + ' 0x00'
		output = 'None'
		with Popen(cmd, shell=True, preexec_fn=os.setsid) as process:
			try:
				output = process.communicate(timeout=5)[0]
			except TimeoutExpired:
				os.killpg(process.pid, signal.SIGINT) # send signal to the process group
				output = process.communicate()[0]					
		
	def pressedSearchDevices(self):		
		call("echo search devices pressed", shell=True)
		self.grbKeys1.setEnabled(False)
		self.grbKeys2.setEnabled(False)
		self.grbPer1.setEnabled(False)
		self.grbPer2.setEnabled(False)
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
			self.grbKeys1.setEnabled(True)
		else:
			close_doors = True
		if bKeys_2 in output:
			self.grbKeys2.setEnabled(True)
		else:
			close_doors = True
		if bPer_1 in output:
			self.grbPer1.setEnabled(True)
			if self.chbclosing_doors.checkState() and close_doors:
				self.pressedPer1_led_on()
		if bPer_2 in output:
			self.grbPer2.setEnabled(True)
			if self.chbclosing_doors.checkState() and close_doors:
				self.pressedPer2_led_on()
				
	def timer_handler(self):
		self.pressedSearchDevices()
		if self.grbKeys1.isEnabled():
			self.pressedKey1_bat()
		if self.grbKeys2.isEnabled():
			self.pressedKey2_bat()
		if self.grbPer1.isEnabled():
			self.pressedPer1_read_all()
		if self.grbPer2.isEnabled():
			self.pressedPer2_read_all()
		threading.Timer(60, lambda: self.timer_handler()).start()
	
	def __init__(self):
		super(self.__class__, self).__init__()
		self.setupUi(self) # gets defined in the UI file
				
		self.grbKeys1.setEnabled(False)
		self.grbKeys2.setEnabled(False)
		self.grbPer1.setEnabled(False)
		self.grbPer2.setEnabled(False)
				
		### Hooks handler for buttons
		self.btnKeys1_mid.clicked.connect(lambda: self.pressedKey1_al_mid())
		self.btnKeys1_high.clicked.connect(lambda: self.pressedKey1_al_high())
		self.btnKeys2_mid.clicked.connect(lambda: self.pressedKey2_al_mid())
		self.btnKeys1_high.clicked.connect(lambda: self.pressedKey1_al_high())
		self.btnKeys1_bat.clicked.connect(lambda: self.pressedKey1_bat())
		self.btnKeys2_bat.clicked.connect(lambda: self.pressedKey2_bat())
		self.btnPer1_temp.clicked.connect(lambda: self.pressedPer1_temp())
		self.btnPer1_dig_read.clicked.connect(lambda: self.pressedPer1_dig_read())
		self.btnPer1_dig_write.clicked.connect(lambda: self.pressedPer1_dig_write())		
		self.btnPer1_light.clicked.connect(lambda: self.pressedPer1_light())
		self.btnPer1_hum.clicked.connect(lambda: self.pressedPer1_hum())
		self.btnPer1_alert.clicked.connect(lambda: self.pressedPer1_alert())
		self.btnPer1_but.clicked.connect(lambda: self.pressedPer1_but())
		self.btnPer1_led_on.clicked.connect(lambda: self.pressedPer1_led_on())
		self.btnPer1_led_off.clicked.connect(lambda: self.pressedPer1_led_off())
		self.btnPer1_read_all.clicked.connect(lambda: self.pressedPer1_read_all())
		self.btnPer2_temp.clicked.connect(lambda: self.pressedPer2_temp())
		self.btnPer2_dig_read.clicked.connect(lambda: self.pressedPer2_dig_read())
		self.btnPer2_dig_write.clicked.connect(lambda: self.pressedPer2_dig_write())		
		self.btnPer2_light.clicked.connect(lambda: self.pressedPer2_light())
		self.btnPer2_hum.clicked.connect(lambda: self.pressedPer2_hum())
		self.btnPer2_alert.clicked.connect(lambda: self.pressedPer2_alert())
		self.btnPer2_but.clicked.connect(lambda: self.pressedPer2_but())
		self.btnPer2_led_on.clicked.connect(lambda: self.pressedPer2_led_on())
		self.btnPer2_led_off.clicked.connect(lambda: self.pressedPer2_led_off())
		self.btnPer2_read_all.clicked.connect(lambda: self.pressedPer2_read_all())
		self.btnSearchDevices.clicked.connect(lambda: self.pressedSearchDevices())					
		threading.Timer(3, lambda: self.timer_handler()).start()
		
# I feel better having one of these
def main():
	# a new app instance
	app = QApplication(sys.argv)
	form = MainWindow()
	form.show()	
	
	# without this, the script exits immediately.
	sys.exit(app.exec_())
 
# python bit to figure how who started This
if __name__ == "__main__":
	main()

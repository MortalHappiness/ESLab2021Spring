from bluepy.btle import Peripheral, UUID
from bluepy.btle import Scanner, DefaultDelegate
class ScanDelegate(DefaultDelegate):
	def __init__(self):
		DefaultDelegate.__init__(self)
	def handleDiscovery(self, dev, isNewDev, isNewData):
		if isNewDev:
			print( "Discovered device", dev.addr)
		elif isNewData:
			print "Received new data from", dev.addr

class MyDelegate(DefaultDelegate):
	def __init__(self):
		DefaultDelegate.__init__(self)
	def handleNotification(self,cHandle,data):
		print("data receive")
		print data

#scanner = Scanner().withDelegate(ScanDelegate())
#devices = scanner.scan(10.0)
'''
n=0
for dev in devices:
	print "%d: Device %s (%s), RSSI=%d dB" % (n, dev.addr, dev.addrType, dev.rssi)
	n += 1
	for (adtype, desc, value) in dev.getScanData():
		print "  %s = %s" % (desc, value)

number = input('Enter your device number: ')
print('Device', number)
print(devices[number].addr)
'''
print "Connecting..."
dev = Peripheral('ee:11:46:b9:19:67', 'random')
dev.setDelegate(MyDelegate())

print "Connected"
for svc in dev.services:
	print str(svc)

try:
	studentID069 = dev.getCharacteristics(uuid=UUID(0xb001))[0]
	studentID052 = dev.getCharacteristics(uuid=UUID(0xb002))[0]
	if (studentID069.supportsRead() and studentID052.supportsRead()):
		print(studentID069.read())
		print(studentID052.read())


	testService = dev.getServiceByUUID(UUID(0xa000))
	for ch in testService.getCharacteristics():
		print str(ch)
	ch = dev.getCharacteristics(uuid=UUID(0xa001))[0]
	if (ch.supportsRead()):
		while True:
			print [ch.read()]

finally:
	dev.disconnect()
	print("Disconnected")

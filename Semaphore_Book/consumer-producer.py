########################################
#Productor consumidor buffer no acotado#
########################################

#Globales
mutex = Semaphore(1)
items = Semaphore(0)

#Productor

event = waitForEvent()
mutex.wait()
	buffer.add(event)
	items.signal()
mutex.signal()

#Consumidor

items.wait()
mutex.wait()
	event = buffer.get()
mutex.signal()
event.process()

########################################
#Productor consumidor con buffer finito#
########################################

#Globales

mutex = Semaphore(1)
items = Semaphore(0)
spaces = Semaphore(buffer.size())

#Productor
event = waitForEvent()

spaces.wait()
mutex.wait()
	buffer.add(event)
mutex.signal()
items.signal()

#Consumidor

items.wait()
mutex.wait()
	event = buffer.get()
mutex.signal()
spaces.signal()

event.process()




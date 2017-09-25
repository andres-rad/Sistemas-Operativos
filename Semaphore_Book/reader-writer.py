###############
#Reader-Writer#
###############

#Globales

readers = 0
mutex = Semaphore(1)
roomEmpty = Semaphore(1)

#Reader

mutex.wait()
	readers++
	if(readers == 1) roomEmpty.wait()
mutex.signal()
#Pasan cosas de lectura
mutex.wait()
	contador--
	if(contador == 0) roomEmpty.signal()
mutex.signal()

#Writer

roomEmpty.wait()
#Pasan cosas...
roomEmpty.signal()

'''
Problemas con la impltementacion:
	·Puede generar starvation en los writers si llegan readers 
	 constantemente
'''
###################################
#Reader-writer #2 (sin starvation)#
###################################

#Globales

readSwitch = Lightswitch()
roomEmpty = Semaphore(1)
turnstile = Semaphore(1)

#Reader

turnstile.wait()
	roomEmpty.wait()
	#Cosas de escritura...
turnstile.signal()
roomEmpty.signal()

#Writer
turnstile.wait()
turnstile.signal()

readSwitch.lock(roomEmpty)
	#Cosas de lectura
readSwitch.unlock(roomEmpty)

#########################################
#Reader-Writer #3 (writer con prioridad)#
#########################################

readSwitch = Lightswitch()
writeSwitch = Lightswitch()
noReaders = Semaphore(1)
noWriters = Semaphore(1)

#Reader

noReaders.wait()
	readSwitch.lock(noWriters)
noReaders.signal()
#CRIT	
readSwitch.unlock(noWriters)	

#Writer

wirteSwitch.lock(noReaders)
	noWriters.wait()
	#CRIT
	noWriters.signal()
writeSwitch.unlock(noReaders)

'''
Problema:
	·Puede generar starvation en los readers
'''
	





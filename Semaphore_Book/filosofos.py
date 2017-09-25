while True:
	think()
	get_forks()
	eat()
	put_forks()

def left(i):
	return i
	
def right(i):
	return (i + 1) % 5
	
forks = [Semaphore(1) for i in range(5)] #1 semaforo por filosofo

############
#Solucion 1#
############

def get_forks(i):
	fork[right(i)].wait()
	fork[left(i)].wait()
	
def put_forks(i):
	fork[right(i)].signal()
	fork[left(i)].signal()
	
	

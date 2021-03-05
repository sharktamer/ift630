clean:
	@echo "Suppression des executable"
	rm -f test
	rm -f philosopheSemaphore
	rm -f philosopheMoniteur
	rm -f philosopheMessage
	rm -f barbierSemaphore
	rm -f barbierMoniteur
	rm -f barbierMessage
	rm -f parcSemaphore
	rm -f parcMoniteur
	rm -f matriceMessage
test1:
	sr -o test test.sr
	./test

semaphore1:
	sr -o philosopheSemaphore philosopheSemaphore.sr
	./philosopheSemaphore


moniteur1:
	@echo "Compilation du moniteur"
	m2sr philosopheMoniteur.m
	sr -o philosopheMoniteur philosopheMoniteur.sr
	./philosopheMoniteur

semaphore2:
	sr -o barbierSemaphore barbierSemaphore.sr
	./barbierSemaphore


moniteur2:
	m2sr barbierMoniteur.m
	sr -o barbierMoniteur barbierMoniteur.sr
	./barbierMoniteur

semaphore3:
	sr -o parcSemaphore parcSemaphore.sr
	./parcSemaphore

moniteur3:
	m2sr parcMoniteur.m
	sr -o parcMoniteur parcMoniteur.sr
	./parcMoniteur

message1:
	sr -o philosopheMessage philosopheMessage.sr
	./philosopheMessage

message2:
	sr -o barbierMessage barbierMessage.sr
	./barbierMessage

message4:
	sr -o matriceMessage matriceMessage.sr
	./matriceMessage

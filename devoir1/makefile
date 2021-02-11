clean:
	@echo "Suppression des executable"
	rm test
	rm philosopheSemaphore
	rm philosopheMoniteur
	rm barbierSemaphore
	rm barbierMoniteur

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

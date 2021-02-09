clean:
	@echo "Suppression des executable"
	rm test
	rm philosopheSemaphore
	rm philosopheMoniteur

semaphore1:
	sr -o philosopheSemaphore philosopheSemaphore.sr
	./philosopheSemaphore


moniteur1:
	@echo "Compilation du moniteur"
	m2sr philosopheMoniteur.m
	sr -o philosopheMoniteur philosopheMoniteur.sr
	./philosopheMoniteur

all:
	gcc -std=c99 -o odroidu2-fan odroidu2-fan.c

clean:
	rm odroidu2-fan

usi:
	sudo cp odroidu2-fan /sbin
	sudo cp ubuntu_service/odroidu2-fan-service.conf /etc/init

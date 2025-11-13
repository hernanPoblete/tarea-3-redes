import socket
from functools import reduce


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)


print("Modo de uso: ")
print("0 (default): Dar todas las veces todos los datos")
print("1: Dar 1 vez los datos del router (Escribir todas las veces destino final y mensaje)")
print("2: Dar 1 vez el destino final y 1 vez el router (Escribir todas las veces el mensaje)")
print("3: Dar 1 vez el destino final, una vez el router y una vez el mensaje (Cada enter se envía el mismo mensaje al mismo punto)")

mode = 0
try:
	mode = int(input(": "))
except ValueError as e:
	mode = 0

first_attempt = True

dest = ""
port = 0
message = ""
router_ip = ""
router_port = 0
ttl = 0

while True:
	print("================================")

	if first_attempt or (mode in [0, 1]):
		dest = input("Destino: ")
		port = int(input("Destino (puerto): "))

	if first_attempt or (mode in [0, 1, 2]):
		message = input("Mensaje: ")

	if first_attempt or (mode in [0]):
		router_ip = input("Dirección del router (CIDR): ")
		router_port = int(input("Dirección del router (Puerto): "))


	ttl = int(input("TTL: "))

	addr_router = (router_ip, router_port)

	dest_bytes = reduce(lambda x, y: x+y, map(lambda x: x.to_bytes() ,map(int, dest.split("."))))
	dest_port_bytes = port.to_bytes(4, "little") #IMPORTANT: MANDAR MESNAJES EN LITTLE ENDIAN PARA QUE C SEA FELIZ

	msg_bytes = message.encode()
	ttl_bytes = ttl.to_bytes()

	full_msg = dest_bytes+dest_port_bytes+ttl_bytes+msg_bytes
	
	if (not first_attempt) and mode == 3:
		input()
	print("Enviando mensaje:", full_msg)
	
	sock.sendto(full_msg, addr_router)
	if (first_attempt):
		first_attempt = False
	

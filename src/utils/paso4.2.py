import socket
from functools import reduce

from random import *


nombre = input("Nombre del archivo: ")


sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

dest = "127.0.0.1"
port = 8885
message = ""
router_ip = "127.0.0.1"
router_port = 8881
ttl = 255
addr_router = (router_ip, router_port)

with open(nombre) as file:

	lines = []
	current = file.readline()

	while current != "":
		lines.append(current)
		current = file.readline()

	for line in lines:
		message = line

		dest_bytes = reduce(lambda x, y: x+y, map(lambda x: x.to_bytes() ,map(int, dest.split("."))))
		dest_port_bytes = port.to_bytes(4, "little") #IMPORTANT: MANDAR MESNAJES EN LITTLE ENDIAN PARA QUE C SEA FELIZ

		msg_bytes = message.encode()
		ttl_bytes = ttl.to_bytes()


		ID = randbytes(4)

		full_msg = dest_bytes+dest_port_bytes+ttl_bytes
		full_msg += ID+ (0).to_bytes(4) +len(msg_bytes).to_bytes(4, 'little')+b'\x00' #ID, Offset, Length, flag
		full_msg+=msg_bytes

		print("Enviando mensaje:", full_msg)
		print("\n", 
		f"IP= {dest} (", dest_bytes , ")\n",
		f"Puerto= {port}\n" , 
		f"TTL={ttl}\n", 
		f"ID= {int.from_bytes(ID, "little")} (", ID, ")\n",
		"Offset= 0\n",
		"Flag= 0\n",
		sep="")
		
		sock.sendto(full_msg, addr_router)	
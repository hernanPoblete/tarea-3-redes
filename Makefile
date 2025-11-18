


FILES = main.cpp src/Router/Router.cpp src/utils.cpp src/Router/RoutingTable/Table.cpp src/Router/Packets/Defragmentator.cpp
NAME = Router.out
FLAGS = -Wno-pointer-arith -Wwrite-strings -lstdc++ -Wreturn-local-addr -O2 -o $(NAME)

compile-cpp:
	touch ./$(NAME)
	make clean
	g++ $(FLAGS) $(FILES)

	@echo ""
compile-typescript:
	cd "Node-Logger/" && tsc
quick:
	touch $(NAME)
	rm $(NAME)
	g++ $(FLAGS) $(FILES)
	make run-compiled
clean:
	rm ./*.out
run-compiled:
	./$(NAME) 127.0.0.1 8081 "routes.txt"

compile-full:
	@echo "Compiling Router Binary..."
	make compile-cpp
	@echo "compiling logger from typescript into javascript"
	@echo "(Logger made by Hernán Poblete btw...)"
	make compile-typescript

run-full:
	make compile-full
	node ./Node-Logger/dist/index.js $(NAME)
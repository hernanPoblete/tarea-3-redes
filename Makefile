


FILES = main.cpp src/Router/Router.cpp src/utils.cpp src/Router/RoutingTable/Table.cpp
NAME = Router.out
FLAGS = -Wwrite-strings -lstdc++ -Wreturn-local-addr -O2 -o $(NAME)

compile:
	make clean
	g++ $(FLAGS) $(FILES)
	@echo "Compiled Router saved under name $(NAME)"
quick:
	touch $(NAME)
	rm $(NAME)
	g++ $(FLAGS) $(FILES)
	make run-compiled
clean:
	rm ./*.out
run-compiled:
	./$(NAME) 127.0.0.1 8081 "routes.txt"
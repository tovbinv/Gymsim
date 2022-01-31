all: clean compile link

check: bin/main
	valgrind --leak-check=full --show-reachable=yes bin/main argc argv


link:
	g++ -o bin/studio bin/main.o bin/Trainer.o bin/Customer.o bin/Workout.o bin/Action.o bin/Studio.o


compile: src/main.cpp src/Customer.cpp src/Trainer.cpp src/Workout.cpp src/Action.cpp src/Studio.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Customer.o src/Customer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Trainer.o src/Trainer.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Workout.o src/Workout.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Action.o src/Action.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Studio.o src/Studio.cpp

clean:
	rm -f bin/*

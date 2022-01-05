CXX=g++
CXXFLAGS = -O3 -w -std=c++17

battleship_game: main.o game_board.o battleship_ai.o battleship_board.o game_simulator.o ship_locations.o ship.o coordinate.o
	$(CXX) $(CXXFLAGS) -o battleship_game.exe main.o game_board.o battleship_ai.o battleship_board.o game_simulator.o ship_locations.o ship.o coordinate.o

all: battleship_game ;

clean:
	rm -f battleship_game.exe main.o game_board.o battleship_ai.o battleship_board.o game_simulator.o ship_locations.o ship.o coordinate.o
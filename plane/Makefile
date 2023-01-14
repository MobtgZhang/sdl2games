release_path=bin
main_file=main.cpp
head_file=planeGame.hpp
out_file=$(release_path)/game

SDL_FLAGS=-lSDL2 -lSDL2_image

default:
	mkdir -p $(release_path)
	g++ $(main_file) $(head_file) -w $(SDL_FLAGS) -o $(out_file)
	cp *.jpg $(release_path)
	cp *.png $(release_path)
run:
	./$(release_path)/game
clean:
	rm -rf bin


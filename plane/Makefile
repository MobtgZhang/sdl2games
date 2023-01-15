release_path=bin
src_path=src
main_file=$(src_path)/main.cpp
head_file=$(src_path)/planeGame.hpp
out_file=$(release_path)/game

SDL_FLAGS=-lSDL2 -lSDL2_image -lSDL2_ttf

default:
	mkdir -p $(release_path)
	g++ $(main_file) $(head_file) -w $(SDL_FLAGS) -o $(out_file)
	cp $(src_path)/*.jpg $(release_path)
	cp $(src_path)/*.png $(release_path)
clean:
	rm -rf bin


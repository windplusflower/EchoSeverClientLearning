all: 
	@cd ./build&&cmake ..&&make&&cp ./client/client ../Client&&cp ./server/server ../Server

.PHONY : all 
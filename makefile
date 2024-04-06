all: sync
	@cd ./client/build&&cmake ..&&make clean&&make&&cp ./client ../../target/
	@cd ./server/build&&cmake ..&&make clean&&make&&cp ./server ../../target/

sync:
	@rm -r ./client/lib/
	@rm -r ./server/lib/
	@cp -r ./lib/ ./client/
	@cp -r ./lib/ ./server/

.PHONY : sync all 
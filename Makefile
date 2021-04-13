CFLAGS=-O3
BIN=prename
SRC=prename.c
INSTALL_DIR=/usr/local/bin

$(BIN): $(SRC)
	$(CC) $(CFLAGS) -o $(BIN) $(SRC)


all:	$(BIN)


install: all
	install -s $(BIN) $(INSTALL_DIR) 

clean:
	rm -f $(BIN)

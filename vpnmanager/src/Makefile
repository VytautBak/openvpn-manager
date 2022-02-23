CC:=gcc
BIN:=vpnmanager
SRC:=$(wildcard *.c)
OBJ:=$(SRC:.c=.o)
LDFLAGS:= -lblobmsg_json -lubox -lubus -ljson-c -largp
CPPFLAGS:= 
CFLAGS:= -g


.PHONY: all clean install uninstall

all: $(BIN)

$(BIN): $(OBJ)
	$(CC) $(CPPFLAGS) $(CFLAGS) -o $@ $^ $(LDFLAGS)
clean:
	$(RM) $(BIN) $(OBJ)

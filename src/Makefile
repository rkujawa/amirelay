CC=vc
RMRF=rm -rf

CFLAGS=
LDFLAGS=-lamiga

OBJECTS=cli/cli.o common/port.o
ADDHEADERS=common/config.h

EXECUTABLE_CLI=amirelay

all: $(EXECUTABLE_CLI) 

$(EXECUTABLE_CLI) : $(OBJECTS)
	$(CC) -o $(EXECUTABLE_CLI) $(LDFLAGS) $(OBJECTS)

%.o : %.c %.h $(ADDHEADERS)
	$(CC) $(CFLAGS) -c $<

clean :
	$(RMRF) $(OBJECTS) $(EXECUTABLE_CLI) 


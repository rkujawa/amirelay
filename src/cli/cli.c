#include <stdio.h>

#include <exec/types.h>
#include <exec/execbase.h>

#include <workbench/startup.h>

#include <proto/dos.h>
#include <proto/exec.h>

#define ARGNUM	8
#define ARGNUM_RTOGGLES ARGNUM

static const STRPTR version = "\0$VER: amirelay 0.1 (22.01.2014)\0";

static LONG *argArray; 

BOOL debug = FALSE;

BOOL
arg_toggle_val(UBYTE argNo)
{
#define TOGGLE_EMPTY    -2
#define TOGGLE_FALSE    0x0
#define TOGGLE_TRUE     0xFFFFFFFF
	if ((LONG) argArray[argNo] == TOGGLE_TRUE)
		return 1;
	else if ((LONG) argArray[argNo] == TOGGLE_FALSE)
		return 0;

	return 0;
}

BOOL
arg_toggle_isempty(UBYTE argNo)
{
	if ((LONG) argArray[argNo] != TOGGLE_EMPTY)
		return 0;
	else
		return 1;
}

UBYTE
arg_toggles_to_ubyte()
{
	UBYTE rv, i;

	rv = 0; /* XXX: should read original value first */

	for (i = 0; i < ARGNUM_RTOGGLES; i++) {
		if(!arg_toggle_isempty(i))
			if(arg_toggle_val(i))
				rv |= (1 << i);
			else
				rv &= ~(1 << i);
	}

	return rv;
}

int
main(int argc, char *argv[])
{
	UBYTE dataout;

	struct RDArgs *result;
	CONST_STRPTR argTemplate = "R0/T,R1/T,R2/T,R3/T,R4/T,R5/T,R6/T,R7/T";
	UBYTE i;

	argArray = AllocVec(ARGNUM*sizeof(LONG), MEMF_ANY|MEMF_CLEAR);

	for (i = 0; i < ARGNUM_RTOGGLES; i++) {
		argArray[i] = TOGGLE_EMPTY;
	}

	result = ReadArgs(argTemplate, argArray, NULL);

	dataout = arg_toggles_to_ubyte();
	printf("gonna send %x\n", dataout); 

	parport_open();

	parport_send(dataout);

	parport_close();

	FreeArgs(result);
	FreeVec(argArray);

	return 0;
}


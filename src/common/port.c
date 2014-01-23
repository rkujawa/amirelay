#include <exec/types.h>
#include <exec/memory.h>
#include <exec/io.h>
#include <dos/dos.h>
#include <devices/parallel.h>

#include <clib/exec_protos.h>
#include <clib/alib_protos.h>

#include <stdio.h>

#include "port.h"

static struct MsgPort  *ParallelMP;
static struct IOExtPar *ParallelIO;

void
parport_close(void)
{
	CloseDevice((struct IORequest *)ParallelIO);
        DeleteExtIO((struct IORequest *)ParallelIO);
	DeletePort(ParallelMP);
}

void
parport_send(UBYTE data)
{
	ULONG waitmask, tmp;

	waitmask = SIGBREAKF_CTRL_C | SIGBREAKF_CTRL_F |
		   1L << ParallelMP->mp_SigBit;

	ParallelIO->IOPar.io_Length = 1;
	ParallelIO->IOPar.io_Data = &data; 
	ParallelIO->IOPar.io_Command = CMD_WRITE;

	printf("trying to send\n");

	SendIO((struct IORequest *) ParallelIO);

	while(1) {
		tmp = Wait(waitmask);	
		if (SIGBREAKF_CTRL_C & tmp)
			break;

		if (CheckIO((struct IORequest *) ParallelIO)) {
			WaitIO((struct IORequest *) ParallelIO);
			break;
		}
	}
}

void
parport_open(void)
{

	if (ParallelMP = CreatePort(0, 0)) {
		if (ParallelIO = (struct IOExtPar *)
		    CreateExtIO(ParallelMP,sizeof(struct IOExtPar)) ) {
			if (OpenDevice(PARALLELNAME, 0L,
			    (struct IORequest *)ParallelIO, 0) )
				printf("Error: couldn't open %s\n",PARALLELNAME);
		} else
			printf("Error: couldn't create ParallelIO\n");
	} else
		printf("Error: couldn't create message port\n");


	printf("parport open\n");
}


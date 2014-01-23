#ifndef _PORT_H_
#define _PORT_H_

#include <exec/types.h>

void parport_open();
void parport_send(UBYTE data);
void parport_close();

#endif /* _PORT_H_ */

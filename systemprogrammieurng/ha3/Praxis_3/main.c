#include <stdio.h>
#include <stdlib.h>
#include "SRTN.h"
#include "LCFS.h"
#include "SJN.h"
#include "task.h"

int main()
{
	// just a very basic example to show you, how to test your implementation (in this case: LCFS)
/*	if (init_LCFS())
	{
		arrive_LCFS(0, 1);
		arrive_LCFS(1, 5);
		arrive_LCFS(2, 3);

		printf("%i\n", running_task);	// should print: 0
		finish_LCFS();
		printf("%i\n", running_task);	// should print: 2
		finish_LCFS();
		printf("%i\n", running_task);	// should print: 1
		finish_LCFS();
		printf("%i\n", running_task);	// should print: -1 (no process left)
	}
	else
	{
		printf("LCFS is not implemented yet. Quitting.\n");
	}*/

/*	if (init_SJN())
	{
		arrive_SJN(0, 1);
		arrive_SJN(1, 5);
		arrive_SJN(2, 3);
		arrive_SJN(3, 2);

		printf("%i\n", running_task);	// should print: 0
		finish_SJN();
		printf("%i\n", running_task);	// should print: 3
		finish_SJN();
		printf("%i\n", running_task);	// should print: 2
		finish_SJN();
		printf("%i\n", running_task);	// should print: 1 (no process left)
		finish_SJN();
		printf("%i\n", running_task);	// should print: -1
	}
	else
	{
		printf("SJN is not implemented yet. Quitting.\n");
	}*/

	if (init_SRTN())
	{
		arrive_SRTN(0, 1);
		arrive_SRTN(1, 7);
		arrive_SRTN(2, 3);
		

		//printf("%i\n", running_task);	// should print: 0
		tick_SRTN();
		printf("%i\n", running_task);	// should print: 2
		tick_SRTN();
		printf("%i\n", running_task);	// should print: 2
		tick_SRTN();
		printf("%i\n", running_task);
		tick_SRTN();
		printf("%i\n", running_task);
		tick_SRTN();
		printf("%i\n", running_task);
		tick_SRTN();
		printf("%i\n", running_task);
		tick_SRTN();
		printf("%i\n", running_task);	// should print: 1 (no process left)
		arrive_SRTN(3, 2);	
		tick_SRTN();
		printf("%i\n", running_task);	// should print: -1
		tick_SRTN();
		printf("%i\n", running_task);
		tick_SRTN();
		printf("%i\n", running_task);
		tick_SRTN();
		printf("%i\n", running_task);
	}
	else
	{
		printf("SRTN is not implemented yet. Quitting.\n");
	}
	return 0;
}

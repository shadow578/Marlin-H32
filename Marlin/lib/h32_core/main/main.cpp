#include "../Arduino.h"
#include "../drivers/board/board_init.h"

int main(void)
{
	board_init();
	setup();
	while (1)
	{
		loop();
	}
}

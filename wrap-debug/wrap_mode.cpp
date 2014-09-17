#include"wrap_mode.h"
void wrap_mode::run(void *args)
{
	if(_DEBUG)
	{
		func(args);
	}
}
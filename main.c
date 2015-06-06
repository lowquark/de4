
#include <stdio.h>
#include <stdlib.h>

#include "src/vector.h"
#include "src/de4.h"

void init_prop0(de4_State * D)
{
	printf("init_prop0!\n");
}
void init_prop1(de4_State * D)
{
	printf("init_prop1!\n");
}
void init_prop2(de4_State * D)
{
	printf("init_prop2!\n");
}
void deinit_prop0(de4_State * D)
{
	printf("deinit_prop0!\n");
}
void deinit_prop1(de4_State * D)
{
	printf("deinit_prop1!\n");
}
void deinit_prop2(de4_State * D)
{
	printf("deinit_prop2!\n");
}

de4_PropertyDef prop0_def = {
	.init = init_prop0,
	.deinit = deinit_prop0,
	.size = 64,
	.name = "prop0"
};

de4_PropertyDef prop1_def = {
	.init = init_prop1,
	.deinit = deinit_prop1,
	.size = 16,
	.name = "prop1"
};

de4_PropertyDef prop2_def = {
	.init = init_prop2,
	.deinit = deinit_prop2,
	.size = 26,
	.name = "prop2"
};

void brutal(de4_State * D, void * ud)
{
	for(int i = 0 ; i < 100 ; i ++)
	{
		if(rand() % 2 == 0)
			de4_addproperty(D, "prop0");
		if(rand() % 2 == 0)
			de4_addproperty(D, "prop1");
		if(rand() % 2 == 0)
			de4_addproperty(D, "prop2");
		if(rand() % 2 == 0)
			de4_addproperty(D, "notaprop");
		/*
		if(rand() % 10 == 0)
			de4_removeproperty(D, "prop0");
		if(rand() % 10 == 0)
			de4_removeproperty(D, "prop2");
		if(rand() % 10 == 0)
			de4_removeproperty(D, "prop1");
		if(rand() % 10 == 0)
			de4_removeproperty(D, "notaprop");
			*/
	}
}

int main(int argc, char ** argv)
{
	de4_PropertyDef * core_defs[] = {
		&prop0_def,
		0
	};

	de4_State * D = de4_create(100);

	printf("de4_defcoreproperty(D, &prop0_def): 0x%08X\n", de4_defcoreproperty(D, &prop0_def));
	printf("de4_defproperty(D, &prop1_def): 0x%08X\n", de4_defproperty(D, &prop1_def));
	printf("de4_defproperty(D, &prop2_def): 0x%08X\n", de4_defproperty(D, &prop2_def));

	printf("de4_propid(D, \"prop0\"): 0x%08X\n", de4_propid(D, "prop0"));
	printf("de4_propid(D, \"prop1\"): 0x%08X\n", de4_propid(D, "prop1"));
	printf("de4_propid(D, \"prop2\"): 0x%08X\n", de4_propid(D, "prop2"));
	printf("de4_propid(D, \"notaprop\"): 0x%08X\n", de4_propid(D, "notaprop"));

	const char * props[] = {
		"prop0",
		"prop1",
		"prop2",
		0
	};

	de4_newentity(D, "joe", props);
	de4_newentity(D, "joe2", props);
	de4_newentity(D, "joe3", props);

	//de4_newentityc(D, "joejr", brutal, 0);

	de4_dump(D);

	de4_destroy(D);
	D = 0;

	return 0;
}


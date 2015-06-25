
#include <stdio.h>
#include <stdlib.h>

#include <de4/dump.h>
#include <de4/de4.h>

void init_prop0(de4_State * D)
{
	//printf("init_prop0!\n");
}
void init_prop1(de4_State * D)
{
	//printf("init_prop1!\n");
}
void init_prop2(de4_State * D)
{
	//printf("init_prop2!\n");
}
void deinit_prop0(de4_State * D)
{
	//printf("deinit_prop0!\n");
}
void deinit_prop1(de4_State * D)
{
	//printf("deinit_prop1!\n");
}
void deinit_prop2(de4_State * D)
{
	//printf("deinit_prop2!\n");
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
	.size = 64,
	.name = "prop1"
};

de4_PropertyDef prop2_def = {
	.init = init_prop2,
	.deinit = deinit_prop2,
	.size = 64,
	.name = "prop2"
};

void testsys(de4_State * D, void * prop0)
{
	float * data = prop0;

	data[1] = (data[3] + data[1]) * (data[2]) + data[3];
	data[1] = (data[2] + data[1]) * (data[3]) + data[2];
	data[1] = (data[3]*0.78 + data[2]*0.43) * (data[1]) + data[2];
	//printf("testsys: %p  eid: %u  prop: %p\n", D, de4_thisentity(D), prop0);

	for(int i = 1 ; i < 16 ; i ++)
	{
		data[i] *= data[i - 1];
	}
}

int main(int argc, char ** argv)
{
	de4_State * D = de4_create(100);

	printf("de4_defcoreproperty(D, &prop0_def): 0x%08X\n", de4_defcoreproperty(D, &prop0_def));
	printf("de4_defproperty(D, &prop1_def): 0x%08X\n", de4_defproperty(D, &prop1_def));
	printf("de4_defproperty(D, &prop2_def): 0x%08X\n", de4_defproperty(D, &prop2_def));

	printf("de4_propertyid(D, \"prop0\"): 0x%08X\n", de4_propertyid(D, "prop0"));
	printf("de4_propertyid(D, \"prop1\"): 0x%08X\n", de4_propertyid(D, "prop1"));
	printf("de4_propertyid(D, \"prop2\"): 0x%08X\n", de4_propertyid(D, "prop2"));
	printf("de4_propertyid(D, \"notaprop\"): 0x%08X\n", de4_propertyid(D, "notaprop"));

	const char * props[] = {
		"prop0",
		"prop1",
		"prop2",
		0
	};

	for(int i = 0 ; i < 100 ; i ++)
	{
		de4_newentity(D, "joe", props);
	}

	for(int i = 0 ; i < 2 ; i ++)
	{
		printf("pass %u\n", i);
		de4_pass1(D, de4_propertyid(D, argv[1]), testsys);
	}

	//de4_dump(D);

	de4_destroy(D);
	D = 0;

	return 0;
}


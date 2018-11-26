#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "helper.h"

/* run this program using the console pauser or add your own getch, system("pause") or input loop */

int i = 3;

//helper functions don't switch global variables!!
//consider programming most things in here.
int main(int argc, char *argv[]) {
	printf("%s\n", test(""));
	printf("%i\n", i);
	anotherTest();
	printf("%i\n", i);
	return 0;
}

void anotherTest(){
	i = 5;
}

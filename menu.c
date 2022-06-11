#include <stdio.h>
#include <stdlib.h>

void print_menu(void);
void check_menu_number(int a);
void result(int a);
int main(void){
	print_menu();
	return 0;
}
void print_menu(void){
	int a;
	printf("0. Dinosaur game\n");
	printf("1. Rhythm game\n");
	printf("2. Tug-of-war game\n");
	printf("3. EXIT\n");
	printf("Enter the game number you want: ");
	scanf("%d", &a);
	check_menu_number(a);
}
void check_menu_number(int a){
	if(a<0||a>3)
		print_menu();
	else
		result(a);
}
void result(int a){
	if(a==0) system( "./0_game");
	else if(a==1) system( "./1_game");
	else if(a==2) system( "./2_game");
	else if(a==3) exit(0);
}

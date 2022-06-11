#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

char t_name1[20];
char t_name2[20];
int x = 40;
int y = 11;
int win = 0;
int win_lose = 0;
int input = 0;

void menu_draw(void);
void game_fun(void);
void start_game(void);


void gotoxy(int x,int y)
{
	printf("%c[%d;%df",0x1B,y,x);
}

void clrscr()
{
	fprintf(stdout, "\033[2J\033[0;0f");
	fflush(stdout);
}

int getch()
{
	int c = 0;
	struct termios oldattr, newattr;

	tcgetattr(STDIN_FILENO, &oldattr);
	newattr = oldattr;
	newattr.c_lflag &= ~(ICANON | ECHO);
	newattr.c_cc[VMIN] = 1;
	newattr.c_cc[VTIME] = 0;
	tcsetattr(STDIN_FILENO, TCSANOW, &newattr);
	c = getchar();
	tcsetattr(STDIN_FILENO, TCSANOW, &oldattr);


	return c;
}

int kbhit(void)
{
	struct termios oldt, newt;
	int ch = 0;
	int oldf = 0;

	tcgetattr(STDIN_FILENO, &oldt);
	newt = oldt;
	newt.c_lflag &= ~(ICANON | ECHO);
	tcsetattr(STDIN_FILENO, TCSANOW, &newt);
	oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
	fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

	ch = getch();

	tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
	fcntl(STDIN_FILENO, F_SETFL, oldf);

	if(ch != EOF){
		ungetc(ch, stdin);
		return 1;
	}

	return 0;
}


void line(int x, int y)
{
	system("clear");
	int i, line[21] = { 0 };
	line[10] = 1;
	line[4] = 2;
	line[16] = 2;
	gotoxy(31, 13);
	for (i = 0; i < 78; i++)
		printf(" ");

	gotoxy(28, 8);
	printf("%s", t_name1);
	gotoxy(28, 9);
	printf("Press 'A'");
	gotoxy(86, 8);
	printf("%s", t_name2);
	gotoxy(86, 9);
	printf("Press 'L'");

	gotoxy(30, 10);
	printf("●");
	gotoxy(29, 11);
	printf("＼|/");
	gotoxy(30, 12);
	printf(" |");
	gotoxy(30, 13);
	printf("/＼");

	gotoxy(87, 10);
	printf(" ●");
	gotoxy(86, 11);
	printf("＼|/");
	gotoxy(87, 12);
	printf(" |");
	gotoxy(87, 13);
	printf("/＼");

	gotoxy(x+10, y);
	for (i = 0; i < 21; i++)
		if (line[i] == 0)
			printf("□");
		else if (line[i] == 1)
			printf("■");
		else
			printf("◆");

	gotoxy(52, 8);
	printf("reference point");
	gotoxy(60, 10);
	printf("▼");
	gotoxy(60, 12);
	printf("▲");
}
void start_game(){
	system("clear");
	gotoxy(0,0);
	printf("Enter team1's name: ");
	scanf("%s",&t_name1,20);
	gotoxy(0,2);
	printf("Enter team2's name: ");
	scanf("%s",&t_name2,20);

	system("clear");
	line(x,y);
	game_fun();
}
void game_fun() {
	while (1) {
		char click = getch();

		if(click==97||click==65) {
			x--;
			line(x, y);
			sleep(0.1);
		}
		if(click==76||click==108) {
			x++;
			line(x, y);
			sleep(0.1);
		}

		if(x==34){
			line(x, y);
			gotoxy(57, 15);
			printf("%s Victory!\n", t_name1);
			sleep(2);
			break;
		}
		if(x==46){
			line(x, y);
			gotoxy(57, 15);
			printf("%s Victory!\n", t_name2);
			sleep(2);
			break;
		}
	}
}

void main(){
	start_game();
	exit(0);
}

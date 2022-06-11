#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>
#include <stdbool.h>
#include <time.h>
#define DINO_BOTTOM_Y 12
#define TREE_BOTTOM_Y 20
#define TREE_BOTTOM_X 45

void clrscr()
{
	fprintf(stdout, "\033[2J\033[0;0f");
	fflush(stdout);
}
void GotoXY(int x, int y)
{
	printf("%c[%d;%df",0x1B,y,x);

}
void MySleep(int msec)
{
    usleep(msec * 1000);
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

int GetKeyDown()
{
	if (kbhit() != 0)
	{
		return getch();
	}
	return 0;
}

void DrawDino(int dinoY)
{
	GotoXY(0, dinoY);
	static bool legFlag = true;
	printf("        $$$$$$$ \n");
	printf("       $$ $$$$$$\n");
	printf("       $$$$$$$$$\n");
	printf("$      $$$      \n");
	printf("$$     $$$$$$$  \n");
	printf("$$$   $$$$$     \n");
	printf(" $$  $$$$$$$$$$ \n");
	printf(" $$$$$$$$$$$    \n");
	printf("  $$$$$$$$$$    \n");
	printf("    $$$$$$$$    \n");
	printf("     $$$$$$     \n");
	if (legFlag)
	{
		printf("     $    $$$    \n");
		printf("     $$          ");
		legFlag = false;
	}
	else
	{
		printf("     $$$  $     \n");
		printf("          $$    ");
		legFlag = true;
	}
}

void DrawTree(int treeX)
{
	GotoXY(treeX, TREE_BOTTOM_Y);
	printf("#  #");
	GotoXY(treeX, TREE_BOTTOM_Y + 1);
	printf("####");
	GotoXY(treeX, TREE_BOTTOM_Y + 2);
	printf(" ## ");
	GotoXY(treeX, TREE_BOTTOM_Y + 3);
	printf(" ## ");
	GotoXY(treeX, TREE_BOTTOM_Y + 4);
	printf(" ## ");
}

void DrawGameOver(const int score)
{
	system("clear");
	int x = 18;
	int y = 8;
	GotoXY(x, y);
	printf("===========================");
	GotoXY(x, y + 1);
	printf("======G A M E O V E R======");
	GotoXY(x, y + 2);	
	printf("===========================");
	GotoXY(x, y + 5);
	printf("SCORE : %d", score);

	printf("\n\n\n\n\n\n\n\n\n");
	MySleep(3000);
	exit(0);
}

bool isCollision(const int treeX, const int dinoY)
{
	GotoXY(0, 0);
	printf("treeX : %d, dinoY : %d", treeX, dinoY); 
	if (treeX <= 8 && treeX >= 4 &&
			dinoY > 8)
	{
		return true;
	}
	return false;
}

int main()
{
	clrscr();

	while (true)		
	{
		bool isJumping = false;
		bool isBottom = true;
		const int gravity = 2;

		int dinoY = DINO_BOTTOM_Y;
		int treeX = TREE_BOTTOM_X;

		int score = 0;
		clock_t start, curr;	
		start = clock();	

		while (true)	
		{
			if(isCollision(treeX, dinoY))
				break;

			if (GetKeyDown() == 32& isBottom)
			{
				isJumping = true;
				isBottom = false;
			}

			if (isJumping)
			{
				dinoY -= gravity;
			}
			else
			{
				dinoY += gravity;
			}

			if (dinoY >= DINO_BOTTOM_Y)
			{
				dinoY = DINO_BOTTOM_Y;
				isBottom = true;
			}

			treeX -= 2;
			if (treeX <= 0)
			{
				score++;	
				treeX = TREE_BOTTOM_X;
			}

			if (dinoY <= 3)
			{
				isJumping = false;
			}

			DrawDino(dinoY);		
			DrawTree(treeX);		

			MySleep(60);
			system("clear");	

			GotoXY(0, 2);	
			printf("Score : %d ", score);
		}

		DrawGameOver(score);
	}
	return 0;
}

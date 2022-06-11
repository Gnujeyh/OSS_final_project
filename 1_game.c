#include <stdio.h>
#include <time.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <termios.h>
#include <fcntl.h>

int target;

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

void randnum(){
	srand(time(NULL));
	target= rand() % 15 + 1;
}

int main() {
	double time;
	clock_t start;
	clock_t end;
	printf("It's a game to test your sense of rhythm!!! \n");
	printf("Press Space to start. \n");
	randnum();
	printf("Press the space after %d seconds\n", target);
	if(getch()==32){
		start = clock();
		printf("...ing\n");
	}
	while(1){
		//printf(".");
		if(kbhit()){
			if(getch()==32) break;
		}
	}
	end = clock();
	time = (end - start) / CLOCKS_PER_SEC; 
	double error = time-target; 
	printf("%f seconds have passed! error: %f \n",time,error);

	return 0;
}


#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include </usr/include/SDL/SDL.h>
#include </usr/include/SDL/SDL_image.h>
//we have 4 actions (move left , move right , move up , move down )
#define MAX_states 4  //we have 4 possible states  0 : station, 1: road , 3 : fire , 2 : obstacle and indexes as defined below
#define MAX_actions  4 //we have 4 possible actions 0 : for right ,  1 : left ,2:up , 3:down
#define STATION 0 // we assume that we will start from the station  
//#define road 1
//#define fire 3
//#define obstacle 2
#define RIGHT 0
#define LEFT 1
#define UP 2 
#define DOWN 3
#define ALPHA 0.9 // is the learning rate 
#define GAMMA 0.1 // is the discount rate 
#define MAX 20 // X et Y peuvent aller jusqu'à 15 
typedef struct point {
	int x ; 
	int y ;
}point;

typedef enum bo{
	false,
	true
}boolean;   // True and false instead of 1 and 0.

typedef enum whatsin{
	road,
	obstacle,
	robot,
	fire
}whatsin;  

typedef struct caracs{
	point p ;
	whatsin whatsin ; 
	int deg ; // degree of the fire if there is one. 0 if nothing. 

}caracs;




float Q[MAX_states][MAX_actions] ; // our Q-matrix
int Reward[MAX_states][MAX_actions] ; 
caracs MAP[MAX][MAX] ;


// the epsilon greedy strat
void loadTheGarden(SDL_Surface * jardin ,point robot_location);
int possible_move(point location);
void move_down(point * location);
void move_up(point * location);
void move_right(point * location);
void move_left(point * location);
void move(point robot_location , caracs MAP[MAX][MAX], SDL_Surface * jardin);
void Q_matrix();
float max_Qstate(int state );
void setQ2zeros();
int R_value(int state , int action);
void Load_MAP();
int max_action(int state ) ;

int main(int argc,char *argv[]){


	//on cree une surface d'experience on a ici jardin 

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface * jardin=NULL ;
	jardin=SDL_SetVideoMode(800,900,32,SDL_HWSURFACE);



    // on rempli notre jardin par des obstacles et des feu
	Load_MAP();
	SDL_Rect robot_position ; 
	point robot_location={0,1};
	robot_position.x=0;
	robot_position.y=1;
	int continuer=1;

	

	int enough_water ; // entier qui presente difference entre la taille du feu et le reservoir du robot 
	point station_location;
	station_location.x = 0 ;
	station_location.y = 0 ;
	int distance=0 ;
	SDL_Surface * text1;
	SDL_Surface * text2 ;
	text1=IMG_Load("interface/text1.png");
	text2=IMG_Load("interface/text2.png");
	SDL_Rect text_position ;
	text_position.x=0;
	text_position.y=800;
	
	while(continuer){
		SDL_Event event;
		SDL_WaitEvent(&event);
		switch(event.type){
        case SDL_QUIT:
            continuer = 0;
            break;
        case SDL_KEYDOWN:
            switch (event.key.keysym.sym)
            {
                case SDLK_ESCAPE: //Appui sur la touche Echap pour arrêter le programme 
                    continuer = 0;
                    break;
            }
            break;}
        move(robot_location,MAP,jardin) ;
        loadTheGarden(jardin,robot_location);
        SDL_Flip(jardin);
          

	}
	SDL_FreeSurface(jardin);
	SDL_Quit();
	return 1;}

void setQ2zeros(){ 
	int i , j ;
	for(i=0;i<MAX_states;++i){
		for(j=0;j<MAX_actions;++j){
			Q[i][j]=0;}}}
int R_value(int state , int action ){ // j'ai choisit -100 seulement pour obstacle apres je vais designer le feu comme punishement 

	int X ;
	
	switch (state ){
		case road :
			switch (action){
				case RIGHT :
					X = -1;
					break ;
				case LEFT :
					X = -1;
					break ;
				case UP :
					X = -1;
					break;
				case DOWN:
					X = -1;
					break ;

			}
		case obstacle :
			switch (action){
				case RIGHT :
					X = -100 ;
					break ;
				case LEFT:
					X = -100;
					break ;
				case UP :
					X = -100;
					break ;
				case DOWN :
					X = -100;
					break ;

			}
		case fire :

			switch (action){
				case RIGHT :
					X = 100 ;
					break ;
				case LEFT :
					X = 100;
					break ;
				case UP   :
					X = 100;
					break ;
				case DOWN :
					X = 100;
					break ;

			}
			


	}
	return X ;

}

float max_Qstate(int state ){
	int i ;
	float max = Q[state][0];
	for (i=1;i<MAX_actions;++i){
		if (Q[state][i]>max){
			max=Q[state][i];
		}


	}
	return max ;

}


void Q_matrix(){
	int i ;
	float N ;
	int B ;
	setQ2zeros();
	point current ;
	
	for (i=0;i<400;i++){//1000 cycles of training
    int state = rand()%4;

	
	do{
		int action = rand()%4 ;
		
		B = R_value(state , action) ;
		N = max_Qstate(state );
		Q[state][action]=Q[state][action]+ALPHA*(B+GAMMA*N-Q[state][action]);
		state = state + 1 ;
	}while (state<MAX_states) ;}

}




void move(point robot_location , caracs MAP[MAX][MAX], SDL_Surface * jardin){
	int start ;
	
	Q_matrix();
	int action = 3 ;
	int V ;
	point previous_point ;
	point pt ;
	int next_state ;

	start = MAP[rand()%20][rand()%20].whatsin ;

	do{

	
	 
	float max=Q[start][action];


	printf("the action now is %d \n" , action);
	switch(action){

		case RIGHT :
			pt.x = robot_location.x+1 ;
			pt.y = robot_location.y ;
			next_state = MAP[pt.x][pt.y].whatsin;
			if (MAP[robot_location.x+1][robot_location.y].whatsin!=obstacle && possible_move(pt)==1){
			previous_point.x=robot_location.x;
			previous_point.y=robot_location.y;
			move_right(&robot_location);
			MAP[previous_point.x][previous_point.y].whatsin=road;
			MAP[previous_point.x][previous_point.y].deg=-1;


			R_value(next_state,  action)	;		
			loadTheGarden(jardin,robot_location);
			sleep(1);

			break;}


		case LEFT :
			pt.x = robot_location.x-1 ;
			pt.y = robot_location.y ;
			next_state = MAP[pt.x][pt.y].whatsin;
			if(MAP[robot_location.x-1][robot_location.y].whatsin!=obstacle && possible_move(pt) == 1){
			previous_point.x=robot_location.x;
			previous_point.y=robot_location.y;		
			move_left(&robot_location);
			MAP[previous_point.x][previous_point.y].whatsin=road;
			MAP[previous_point.x][previous_point.y].deg=-1;

			R_value(next_state,  action)	;	
			loadTheGarden(jardin,robot_location);
			sleep(1);

			break;}

		case DOWN :
			pt.x = robot_location.x ;
			pt.y = robot_location.y+1 ;
			next_state = MAP[pt.x][pt.y].whatsin;
			if(MAP[robot_location.x][robot_location.y+1].whatsin!=obstacle && possible_move(pt) == 1){
			previous_point.x=robot_location.x;
			previous_point.y=robot_location.y;
			move_down(&robot_location);
			MAP[previous_point.x][previous_point.y].whatsin=road;
			MAP[previous_point.x][previous_point.y].deg=-1;
			R_value(next_state,  action)	;	
			loadTheGarden(jardin,robot_location);
			sleep(1);
			break;}

		case UP :
			pt.x = robot_location.x ;
			pt.y = robot_location.y-1 ;
			next_state = MAP[pt.x][pt.y].whatsin;
			if(MAP[robot_location.x][robot_location.y-1].whatsin!=obstacle && possible_move(pt) == 1){
			previous_point.x=robot_location.x;
			previous_point.y=robot_location.y; 
			move_up(&robot_location);
			MAP[previous_point.x][previous_point.y].whatsin=road;
			MAP[previous_point.x][previous_point.y].deg=-1;

			R_value(next_state,  action)	;	
			loadTheGarden(jardin,robot_location);


			sleep(1);
			break;}


	}
	
	start = MAP[rand()%20][rand()%20].whatsin ;
	V = possible_move(robot_location) ;

	action = max_action(start) ;

	SDL_Flip(jardin);
	

	}	while(start!=fire || !V ) ;

	
}
int max_action(int state ) {
	float max = Q[0][0] ; 
	int action = rand()%4 ;

	for (int i=0 ; i < MAX_actions ; ++i ){
		if (Q[state][i]>max){
			max=Q[state][i];
			action = i ;
			
		}
	}
	return action ;

}







void move_right(point * location){
	(*location).x++;
	MAP[(*location).x][(*location).y].whatsin=robot ;
	
}
void move_left(point * location){
	(*location).x--;
	MAP[(*location).x][(*location).y].whatsin=robot ;
	
}
void move_up(point * location){
	(*location).y--;
	MAP[(*location).x][(*location).y].whatsin=robot ;
}
void move_down(point * location){
	(*location).y++;
	MAP[(*location).x][(*location).y].whatsin=robot ;
}


int possible_move(point location){
	if (location.x<0 || location.y<0 || location.x>MAX || location.y>MAX ){
		return 0 ;
	}else {
		return 1 ;
	}
}
void loadTheGarden(SDL_Surface * jardin ,point robot_location){
	int i , j ;
	SDL_Rect backgrnd_position ;
	backgrnd_position.x=0;
	backgrnd_position.y=0;

	SDL_Surface * jardin_herbes;
	jardin_herbes=IMG_Load("interface/image.bmp");
	SDL_BlitSurface(jardin_herbes,NULL,jardin,&backgrnd_position);	

	
    SDL_Rect fire_position={robot_location.y*35,robot_location.x*35};
	SDL_Surface * station=IMG_Load("interface/water_plz.bmp");
	SDL_Surface * robot_0_img=IMG_Load("interface/robot.bmp");
	
	SDL_Surface * obstacle_img=IMG_Load("interface/obstacle.bmp");
	SDL_Surface * fire_picture=IMG_Load("interface/fire.bmp");
	SDL_Surface * num1=IMG_Load("interface/1.bmp");
	SDL_Surface * num2=IMG_Load("interface/2.bmp");
	SDL_Surface * num3=IMG_Load("interface/3.bmp");
	//SDL_Surface * select=IMG_Load("interface/select.bmp");
	//SDL_BlitSurface(select,NULL,jardin,&fire_position);
	SDL_Rect position = {0,0};// pour parcourir notre surface 
		for (i = 0; i < MAX; ++i)
		{
			for (j = 0; j < MAX; ++j)
			{	
			
				position.x = 35*j;
				position.y = 35*i;
				if (i == 0 && j ==0) {
					SDL_BlitSurface(station,NULL,jardin,&position); // l'emplacement de la station ou on charge le robot par l'eau 
				}								 
				else if (MAP[i][j].whatsin== obstacle){
					SDL_BlitSurface(obstacle_img,NULL,jardin,&position);
				}
				else if(MAP[i][j].whatsin == fire){
					SDL_BlitSurface(fire_picture,NULL,jardin,&position);
					switch(MAP[i][j].deg){
						
						case 0 :
							SDL_BlitSurface(num1,NULL,jardin,&position);
							break;
						case 1:
							SDL_BlitSurface(num1,NULL,jardin,&position);
							break;
						case 2:
							SDL_BlitSurface(num2,NULL,jardin,&position);
							break;
						case 3:
							SDL_BlitSurface(num3,NULL,jardin,&position);
							break;

					}
				}				
				else if(MAP[i][j].whatsin == robot){
					SDL_BlitSurface(robot_0_img,NULL,jardin,&position);


					
		}}}}

		void Load_MAP()
{
  int i,j,x; 
	
	for (i = 0; i < MAX; ++i)
	{
		for (j = 0; j < MAX; ++j)
		{
			MAP[i][j].whatsin = road;
			MAP[i][j].deg = -1;
		}
	}
	
	MAP[6][6].whatsin		= obstacle;
	MAP[6][5].whatsin		= obstacle;
	MAP[7][5].whatsin		= obstacle;
	MAP[10][7].whatsin		= obstacle;
	MAP[11][7].whatsin		= obstacle;
	MAP[10][8].whatsin		= obstacle;
	MAP[11][8].whatsin		= obstacle;
	MAP[7][10].whatsin		= obstacle;
	MAP[8][10].whatsin		= obstacle;
	MAP[8][9].whatsin		= obstacle;
	MAP[14][11].whatsin		= obstacle;
	MAP[15][11].whatsin		= obstacle;
	MAP[15][9].whatsin		= obstacle;
	MAP[15][10].whatsin		= obstacle;
	MAP[10][13].whatsin		= obstacle;
	MAP[11][13].whatsin		= obstacle;
	MAP[10][14].whatsin		= obstacle;
	MAP[15][16].whatsin		= obstacle;
	MAP[16][16].whatsin		= obstacle;
	MAP[17][16].whatsin		= obstacle;
	MAP[18][16].whatsin		= obstacle;
	MAP[15][17].whatsin		= obstacle;
	MAP[16][17].whatsin		= obstacle;
	MAP[17][17].whatsin		= obstacle;
	MAP[18][17].whatsin		= obstacle;
	MAP[5][17].whatsin		= obstacle;
	MAP[6][17].whatsin		= obstacle;
	MAP[7][17].whatsin		= obstacle;
	MAP[8][17].whatsin		= obstacle;
	

	MAP[8][17].whatsin 		= fire;
	MAP[8][17].deg 			= 1;

	MAP[7][6].whatsin 		= fire;
	MAP[7][6].deg 			= 1;
	MAP[19][19].whatsin 		= fire;
	MAP[19][19].deg 			= 3;
	MAP[19][9].whatsin 		= fire;
	MAP[19][9].deg 			= 2;

}
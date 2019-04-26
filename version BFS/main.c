#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include </usr/include/SDL/SDL.h>
#include </usr/include/SDL/SDL_image.h>
#include "main.h"


int main(int argc,char *argv[]){


	//on cree une surface d'experience on a ici jardin 

	SDL_Init(SDL_INIT_VIDEO);
	SDL_Surface * jardin=NULL ;
	jardin=SDL_SetVideoMode(800,900,32,SDL_HWSURFACE);



    // on rempli notre jardin par des obstacles et des feu
	Load_MAP();
	point fire_position=drone();//on trouve un feu dans notre jardin
	SDL_Rect robot_position ; 
	point robot_location={0,1};
	robot_position.x=0;
	robot_position.y=1;
	int continuer=1;
	

	int enough_water ; // entier qui presente difference entre la taille du feu et le reservoir du robot 
	node T[MAX][MAX] ; // 
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
        while(fire_position.x != -1){
        		enough_water=MAP[robot_location.x][robot_location.y].deg-MAP[fire_position.x][fire_position.y].deg ;

			if(enough_water >= 0){

				SDL_BlitSurface(text1,NULL,jardin,&text_position);
				SDL_Flip(jardin);
				BFS(MAP,T,robot_location,fire_position);
				distance=T[fire_position.x][fire_position.y].dist ;
				point * path=GetPath(T,fire_position);
				send_robot(distance,fire_position,jardin,path);
				robot_location.x=path[distance-1].x;
				robot_location.y=path[distance-1].y;
				MAP[robot_location.x][robot_location.y].deg -= MAP[fire_position.x][fire_position.y].deg;
				sleep(3);
				make_it_way(fire_position);
				fire_position=drone();}
			else{
				SDL_BlitSurface(text2,NULL,jardin,&text_position);
				SDL_Flip(jardin);
				BFS(MAP,T,robot_location,station_location);
				distance=T[station_location.x][station_location.y].dist ;
				point * path=GetPath(T,station_location);
				send_robot(distance,station_location , jardin ,path);
				robot_location.x=path[distance-1].x;
				robot_location.y=path[distance-1].y;
				sleep(4);
				MAP[robot_location.x][robot_location.y].deg=3;}
        			}
	SDL_FreeSurface(jardin);
	SDL_Quit();
	return 1;}}

/*----------------------------------*/
	boolean isinrange(int row, int col) {  // Pour vérifier si le point entré est valable/existant dans notre MAP.
	if (row <MAX && row >=0 && col<MAX && col>=0)
		return true ;
	else 
		return false ; 

}

Queue *initialise() {   // Pour initialiser notre liste.
	Queue * queue  = (Queue*)malloc(sizeof(Queue)) ; 
	queue->first = NULL ; 
	return queue ;
}

void enqueuenode(Queue * queue, node no) { // Pour enfiler un noeud à la fin de la liste.
	chain *nv = (chain*)malloc(sizeof(chain)) ; 

	if (queue == NULL || nv == NULL) {
		exit(EXIT_FAILURE);
	}

	
	nv->n = no ; nv->next = NULL ; 


	if(queue->first != NULL) {

		chain * current = queue->first;
	    while (current->next != NULL) {
        current = current->next;
    	}

    /* now we can add a new variable */
    	current->next = (chain*)malloc(sizeof(chain));
    	current->next = nv ; 
    }
    else 
    	queue->first = nv ;
}


boolean vide(Queue *queue) { // On vérifie si notre liste est vide ou pas.
	if (queue == NULL)
		return true ; 
	else
		return false ;

}

void dequeuenode(Queue *queue) { // On dépile le 1er élement de notre liste
	if (queue == NULL)
		exit(EXIT_FAILURE);
	if (queue->first !=NULL) {
		chain *p = NULL ; 
		p = queue->first ;
		queue->first = queue->first->next ; 
		free(p) ; 
		
	}
}

node front(Queue * queue){ // On prends le 1er noeud de la liste
	node r = queue->first->n ; 
	return r ; 
}


point* GetPath(node T[MAX][MAX], point dest)
{
	int i=0 ; 
	int N=0 ;
	N = T[dest.x][dest.y].dist+1;   // N représente la distance de la destination +1. Distance = Nombres de pts parcourus.
	point *P=(point*)malloc(N*sizeof(point)); //Tableau de points de taille N. 
	point pt = dest; 
	
	for( i=0; i<N; i++){
		P[N-1-i] = pt;  // On remplit le tableau par ordre décroissant. Le pt dest sera à la fin et ainsi de suite.
		pt = T[pt.x][pt.y].p;
	}
	return P;
}

void BFS(caracs MAP[MAX][MAX], node T[MAX][MAX], point src, point dest) { //Breath first scearch
	int i,j ; 
	int row,col;
    int rowNum[] = {-1, 0, 0, 1};
    int colNum[] = {0, -1, 1, 0}; //Ces 2 tableau sont pour parcourir les noeuds adjacents. 

    Queue * q = initialise() ; // On initialise la liste chainé pour notre BFS.

    boolean visited[MAX][MAX] ; // On initialise le tableau visited.

    node source = {src, 0} ; //la distance de la source à la source est 0.

    for (i = 0; i < MAX; i++)
	{
		for (j = 0; j < MAX; j++)
		{
			visited[i][j] = false; //On a pas encore visité aucun pt alors, on met tt les points à false.
		}
	}

	visited[src.x][src.y] = true; // On marque la source comme visité.

	enqueuenode(q, source); // Et on l'enfile à notre liste.

	while (!vide(q))
    {
        node curr = front(q);
        point pt = curr.p;
 
        // Si on a atteint le pt destination, alors on est terminé.
        if (pt.x == dest.x && pt.y == dest.y)
        
            return ;
 
        // Sinon, défiler le noeud en 1er de la file et empiler ses noeuds adjacents.
        dequeuenode(q);
 
        for (i = 0; i < 4; i++) // i prends 4 valeurs car chaque noeud peut avoir 4 noeuds adjacents au maximum. (Gauche, droit, bas, haut) 
        {
            row = pt.x + rowNum[i]; 
            col = pt.y + colNum[i];
			             
            // if adjacent cell is valid, has path and
            // not visited yet, enqueue it.
            if (isinrange(row, col) && (MAP[row][col].whatsin!= obstacle ) && !visited[row][col])
            {
            	T[row][col].p = pt;
            	T[row][col].dist = curr.dist + 1;
                // mark cell as visited and enqueue it
                visited[row][col] = true;
                node Adjcell = { {row, col}, curr.dist + 1 };
                enqueuenode(q,Adjcell);
            }
        }
    }



}


// ceci represente le manager qui est composé de drone et les fonction du deplacement de notre robot 
/*--------------------------*/

// premierement drone qui va trouver l'emplacement du feu 

point  drone (){
	int i , j,a,b ; 
	point  fire_location={-1,-1} ;
	for (i=0 ; i<MAX;i++){
		for(j=0;j<MAX;j++){
			if(MAP[i][j].whatsin==fire){
				fire_location.x=i;
				fire_location.y=j;
				printf("on a trouver un danger de feu de degrée : %d \n",MAP[i][j].deg);
				sleep(1);
				return fire_location;
				
			}
		}
	}
		printf("pas de danger maintenant \n ");
		fire_location.x=-1;
		fire_location.y=-1;
		return fire_location;}

void make_it_way(point p ){
	MAP[p.x][p.y].whatsin=road;
	MAP[p.x][p.y].deg=-1;

}


// fonction qui mise a jour l'environnement "qui est le jardin" a chaque fois on veut changer l'emplacement du robot 


void loadTheGarden(SDL_Surface * jardin ,point fire_location ){
	int i , j ;
	SDL_Rect backgrnd_position ;
	backgrnd_position.x=0;
	backgrnd_position.y=0;

	SDL_Surface * jardin_herbes;
	jardin_herbes=IMG_Load("interface/image.bmp");
	SDL_BlitSurface(jardin_herbes,NULL,jardin,&backgrnd_position);	

	
    SDL_Rect fire_position={fire_location.y*35,fire_location.x*35};
	SDL_Surface * station=IMG_Load("interface/water_plz.bmp");
	SDL_Surface * robot_0_img=IMG_Load("interface/robot.bmp");
	
	SDL_Surface * obstacle_img=IMG_Load("interface/obstacle.bmp");
	SDL_Surface * fire_picture=IMG_Load("interface/fire.bmp");
	SDL_Surface * num1=IMG_Load("interface/1.bmp");
	SDL_Surface * num2=IMG_Load("interface/2.bmp");
	SDL_Surface * num3=IMG_Load("interface/3.bmp");
	SDL_Surface * select=IMG_Load("interface/select.bmp");
	SDL_BlitSurface(select,NULL,jardin,&fire_position);
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
					switch(MAP[i][j].deg){
						

						case 0:
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
		}	}	}
		
	    


}



// fontion qui consiste a envoyer le robot vers un emplacement donnée


void send_robot(int dist, point fire_location , SDL_Surface * jardin , point * path){
	int i ;
    for (i=0;i<dist-1;i++){
    	MAP[path[i+1].x][path[i+1].y].whatsin=robot;
    	MAP[path[i+1].x][path[i+1].y].deg=MAP[path[i].x][path[i].y].deg;
    	make_it_way(path[i]);
    	loadTheGarden(jardin,fire_location);
    	SDL_Flip(jardin);
    	sleep(1);

    }
}
// et finallement fonction qui mettre en place les obstacles et les feu  

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






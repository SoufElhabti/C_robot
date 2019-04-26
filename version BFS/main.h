
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
}whatsin; // pour un point donné on peut trouver soit (road : 0 = chemin valable pour robot),(obstacle : 1 = robot ne peut pas le depasser ),(robo : 2),(fire : 3 = c'est le feu )

typedef struct caracs{
	point p ;
	whatsin whatsin ; 
	int deg ; // pour le robot c'est le reservoire "0:empty,1:valable pour un feu de taille 1,2:valable pour un feu de taille <=2 ,3: valable pour feu de taille <=3" .. pour le feu c'et la taille  

}caracs;

typedef struct node{
	point p ; 
	int dist ; //distance du pt de la source
	
}node; // chaque noeud a des coordonnées et la distance entre son emplacement et le source (c'est a dire depart du robot)
typedef struct chain{
	node n ; 
	struct chain* next ; 
	
}chain;// une chaine ou on va poser les points et leur distance qui vont etre parcourue 

typedef struct {
	chain * first ; 
}Queue; 

caracs MAP[MAX][MAX] ; //X can go from 0 to 14. Same for Y. 
//prototype des fonction utilisées dans le main.c

boolean isinrange(int row, int col) ;
Queue *initialise();
void enqueuenode(Queue * queue, node no);
boolean vide(Queue *queue);		
void dequeuenode(Queue *queue);
node front(Queue * queue);						
point* GetPath(node T[MAX][MAX], point dest);							
void BFS(caracs MAP[MAX][MAX], node T[MAX][MAX], point src, point dest) ;
void make_it_way(point p );
void loadTheGuarden(SDL_Surface * jardin ,point fire_location );
void make_it_way(point p );
void send_robot(int dist, point fire_location , SDL_Surface * jardin , point * path);
void Load_MAP();
void shut_down_fire (point fire_position , point robot_location , SDL_Surface * jardin);
point  drone ();

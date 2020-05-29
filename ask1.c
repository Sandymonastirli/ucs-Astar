//kotopoulos stergios AM 1867,monastirli chrysanthi AM 1716

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define N 2   

struct state{
    char board[2*N + 2]; //maures, aspres, keno , sin 2 kai gia to backslash miden
    int g;          //to kostos tis arxis ws edw
    int h;
    int flag;  //an o kombos anikei sto metwpo anazitisis i sto kleisto sinolo
    struct state *next;  //mia katastasi mporei na me paei se alli katastasi, den kserw poses thelw lista
    struct state *parent; //prepei na kserw apo pou irtha
};

void UCS(char *buf);
void astar(char *buf);
int finalState(struct state *s);
struct state *findNext(struct state *ma);
int search(struct state *ma, struct state *s);
struct state *stateNode(char *board, struct state *parent, int k); //sinartisi pou mou dimiourgei neo kombo
void printPath(struct state *s);
int findEmpty(char *board);
struct state *insert(struct state *ma, struct state *newNode);
struct state *insertAstar(struct state *ma, struct state *newNode);

int h(struct state *s);

int main(){    
    int i;
    char buf[2*N+2], temp[2*N+2];
    int correctInput = 0;
    int counter1, counter2;
    
    do{
        printf("Give the ... (e.g. AM-MA):");
        fgets(buf, 2*N+2, stdin);
        fgets(temp, 2*N+2, stdin); //epeidi afinei ena akiro backslash ni kanw akiro fgets
        
        correctInput = 1;
        
        counter1 = 0;
        counter2 = 0;
        
        for(i = 0; i < 2*N+1; i++){
            if(buf[i] != 'A' && buf[i] != 'M' && buf[i] != '-'){  //pote tha einai correct
               correctInput = 0;
               break;
            }
            else if(buf[i] == 'A'){
                counter1++;
            }
            else if(buf[i] == 'M'){
                counter2++;
            }
        }
        if(counter1 != N || counter2 != N){ //pote allote exw problima, prepei na dw an oi dio counters einai idioi. an den einai idioi exw thema
            correctInput = 0;
        }
        
    }while(correctInput == 0);
    printf("%s\n", buf);
    strcpy(temp, buf);
    UCS(buf);
    astar(temp);
}


void UCS(char *board){

    struct state *ma;   //paw ftiaxnw to metwpo anazitisis,arxika einai keno
    struct state *nextNode, *newNode, *temp; 
    int pos, i, k;  //k einai o arithmos twn thesewn pou exw endiamesa
    int count = 0;

    printf("UCS\n");
    ma = stateNode(board, NULL, 0);
    nextNode = ma;
    
    while(nextNode != NULL){
        if(search(ma, nextNode) == 0){     //de to brika
            count++;
            if(finalState(nextNode) == 0){   //prepei na arxisw na balw ta paidia, na mpw sto kleisto sinolo k paw stin epomeni katastasi
                pos = findEmpty((*nextNode).board);
                
                for(i = 0; i < 2*N + 1; i++){
                    k = abs(i - pos);
                    if(i != pos && k <= N){
                        strcpy(board, (*nextNode).board);
                        board[pos] = board[i];
                        board[i] = '-';
                        newNode = stateNode(board, nextNode, k);
                        insert(ma, newNode);
                    }
                }
                
                
                (*nextNode).flag = 1;
                nextNode = findNext(ma);
            }
            else{
                printPath(nextNode);
                printf("Epektaseis = %d\n", count);
                return;
            }
        }
        else{
            (*nextNode).flag = 1;
            nextNode = findNext(ma);
        }
    }
    
}

//o algorithmos einai o idios me ucs, exoume mono to h(n) epipleon
//stin state pros8etw to h
void astar(char *board){
    struct state *ma;
    struct state *nextNode, *newNode, *temp;
    int pos, i, k;
    int count = 0;
    
    printf("A*\n");

    ma = stateNode(board, NULL, 0);
    nextNode = ma;
    
    while(nextNode != NULL){
        if(search(ma, nextNode) == 0){
            if(finalState(nextNode) == 0){
                count++;
                pos = findEmpty((*nextNode).board);
                
                for(i = 0; i < 2*N + 1; i++){
                    k = abs(i - pos);
                    if(i != pos && k <= N){
                        strcpy(board, (*nextNode).board);
                        board[pos] = board[i];
                        board[i] = '-';
                        newNode = stateNode(board, nextNode, k);
                        (*newNode).h = h(newNode);
                        insertAstar(ma, newNode);
                    }
                }
                
                
                (*nextNode).flag = 1;
                nextNode = findNext(ma);
            }
            else{
                printPath(nextNode);
                printf("Epektaseis = %d\n", count);
                return;
            }
        }
        else{
            (*nextNode).flag = 1;
            nextNode = findNext(ma);
        }
    }
    
}

//otan theloume mia ipoektimisi:koitame posa koutakia exoume lathos stin euretiki
//poses mpales einai se lathos thesi mou lene poses kiniseis toulaxiston thelw
//px an exe 2 theseis lathos tote toulaxiston 2 kiniseis
//tha elegxw poses aspres einai aristera apo tis maures gia na tis ferw sta deksia
int h(struct state *s){
    int count = 0;
    int i, pos, j;
    
    pos = findEmpty((*s).board);  //to keno pou einai apo tin s
    
    for(i = 0; i <= N; i++){  //diatrexoume olo to mikos
        if((*s).board[i] == 'A'){
            count += abs(i - (N+1));
        }
    }
    
	//ola ta aspra deksia apo ta maura kai o teleutaio paula
	//stin ousia briskoume teliki katastasi
    if(count == 0 && (*s).board[2*N] != 'A')
        count = 1;
    if((*s).board[2*N] != 'A' && count > 1)
        count--;
    printf("%s-->%d\n", (*s).board, count);
    return count;
}

//prepei na valw ti bala k sti swsti thesi to metwpo anazitisis
//bazw to newNode sto metwpo anazitisis
//o kombos prepei na mpei etsi wste i lista na einai taksinomimeni
//prepei na brw ton akirbws pisw mou
struct state *insert(struct state *ma, struct state *newNode){
    struct state *temp = ma, *prev = NULL;  //sti geniki periptwsi to prev deixnei se enan kombo
    

    while(temp != NULL){
        if((*temp).g > (*newNode).g){  //eimaste akribws edw pou prepei na mpoume, dld pisw apo to temp
            (*newNode).next = temp;
            if(prev != NULL)       
                (*prev).next = newNode;
            else     //alliws o newNode mpainei sto metwpo anazitisis
                ma = newNode;
            return ma;
        }
        prev = temp;
        temp = (*temp).next;
    }
    (*prev).next = newNode;
    return ma;
}

struct state *insertAstar(struct state *ma, struct state *newNode){
    struct state *temp = ma, *prev = NULL;

    temp = ma;
    while(temp != NULL){
        if((*temp).g + (*temp).h > (*newNode).g + (*newNode).h ){
            (*newNode).next = temp;
            if(prev != NULL)   
                (*prev).next = newNode;
            else
                ma = newNode;
            return ma;
        }
        prev = temp;
        temp = (*temp).next;
    }
    (*prev).next = newNode;
    return ma;
}

int findEmpty(char *board){  //prepei na brw pou einai i paula
    int i;
    
    for(i = 0; i < strlen(board); i++){
        if(board[i] == '-')
            return i;   //brikame tin thesi tis kenis
    }
    return -1;
}

//bazoume tous kombous se fthinousa seira me basi to kostos monopatiou dld g(n)
void printPath(struct state *s){
    struct state *temp = s, *temp2, *temp3;
    
	//oso den einai null thelw na antistrepsw deiktes, parents kai 
	//na deixnoun ston epomeno kombo
	//bazoume temp3 se parent gia na kseorume poios einai
    temp2 = NULL;
    while(temp != NULL){
        temp3 = (*temp).parent;
        (*temp).parent = temp2;
        temp2 = temp;
        temp = temp3;
    }
    
    temp = temp2;
    while(temp != NULL){
        printf("%s %d\n", (*temp).board, (*temp).g);
        temp = (*temp).parent;
    }

}
int finalState(struct state *s){
    int i, j;
	
    //elegxe an einai teliki katastasti
	//an to teleutaio einai to aspro 2*N+1 san arithmisi ara elegxe to 2*N
	//an den einai teliki katastasi return 0
	//skanaroume apo tin pio aristeri mayri kai 
	//an exw aspri exw problima
    if((*s).board[2*N] != 'A') 
        return 0;
    
	
    for(i = 2*N - 1; i >= 0; i--){
        if((*s).board[i] == 'M'){
            j = i;
            break;
        }
    }
    
	//an brei aspro:
    for(i = j - 1; i >= 0; i--){
        if((*s).board[i] == 'A'){
            return 0; //problima
        }
    }
    
    return 1;  //alliws mia xara
}
struct state *findNext(struct state *ma){
    struct state *temp = ma;
    
    while(temp != NULL){
        if((*temp).flag == 0)
            return temp;
        temp = (*temp).next;
    }
    return NULL;
}
int search(struct state *ma, struct state *s){   //psaxnw sto metwpo anazitisis
    struct state *temp = ma;
    
    while(temp != NULL){
		//thelw na dw an uparxei idi :ayto pou leei mesa sto board  me to *s.board. 
		//an ayto kanei miden to brika kai prepei o temp na anikei sto kleisto sinolo ara flag=1
		//an de to brika return 0
        if((*temp).flag == 1 && strcmp((*temp).board, (*s).board) == 0) 
            return 1;
        temp = (*temp).next;  // o temp ginetai o,ti deixnei o next tou
    }
    return 0;
}
struct state *stateNode(char *board, struct state *parent, int k){
    struct state *temp;
    
    temp = (struct state *)malloc(sizeof(struct state));  //kanw malloc giati tha paw na dimiourgisw neo kombo
    if(temp == NULL){ 
        printf("Error allocating memory\n");
        exit(0);
    }
	
	//twra arxikopoiw ta pedia tis domis:
    (*temp).board[0] = '\0';   
    strcpy((*temp).board, board);
    (*temp).next = NULL;
    (*temp).parent = parent;
    if(parent == NULL)
        (*temp).g = 0;    //otan eimai stin riza
    else                    //alliws gia ta ipoloipa
        (*temp).g = (*parent).g + k;
    
    (*temp).flag = 0;  //giati eimai sto metwpo anazitisis
    
    return temp;
}

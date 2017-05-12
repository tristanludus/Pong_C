#include <stdio.h>
#include <stdlib.h>
#include <SDL2/SDL.h>
#include <time.h>
#include <math.h>
#include <SDL2/SDL_ttf.h>


#define SCREEN_WIDTH 800
#define SCREEN_HEIGHT 600
#define SPEED 5
#define rayon 18

typedef struct game{


     SDL_Window *g_pWindow;
     SDL_Renderer *g_pRenderer;
     SDL_Texture *g_ptexture;
     SDL_Surface *g_psurface;


}game;

typedef struct gameState{

    int g_bRunning;


}gameState;

typedef struct font{

    TTF_Font *g_font;

}font;

typedef struct coordonnees{

    double x;
    double y;

}coordonnees;

//prototypes
int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame, font *mFont);
void destroy(game *myGame);
void handleEvents(gameState *state,coordonnees *dep,coordonnees *dep2,coordonnees *z,coordonnees *s,int *raquette1,int *raquette2,coordonnees *balle,coordonnees *MouvementBalle);
void delay(unsigned int frameLimit);

void renderTexture(game *myGame,coordonnees *dep,coordonnees *dep2,coordonnees *z,coordonnees *s);
void CarreNoirFondBlanc(game *myGame);
void writeSDL(game *myGame,font mFont);
void MoveRaquette(int raquette1,int raquette2,coordonnees *dep,coordonnees *dep2,coordonnees *z,coordonnees *s);
void DessineBalle (game *myGame,coordonnees *dep,coordonnees *dep2,coordonnees *balle);


int main(int argc, char *argv[])
{


     game myGame;
     gameState state;
     font mFont;

     unsigned int frameLimit = SDL_GetTicks() + 16;
     coordonnees dep;
     coordonnees dep2;
     coordonnees balle;
     coordonnees z;
     coordonnees s;
     coordonnees MouvementBallon;
     MouvementBallon.x=1;
     MouvementBallon.y=1;

     int raquette1,raquette2;

     int choix=0;

     balle.x=SCREEN_WIDTH/2;
     balle.y=SCREEN_HEIGHT/2;


     state.g_bRunning=1;




        //Create texture for drawing in texture or load picture
        myGame.g_ptexture=SDL_CreateTexture(myGame.g_pRenderer, SDL_PIXELFORMAT_RGBA8888, SDL_TEXTUREACCESS_TARGET,SCREEN_WIDTH,SCREEN_HEIGHT);




        while(state.g_bRunning){



                if(choix==0){
                    printf("1: Lancer le Pong\n");

                    scanf("%i",&choix);


                }
                switch (choix){


                        case 1:init("Chapter 1 setting up SDL",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,SCREEN_WIDTH,SCREEN_HEIGHT,SDL_WINDOW_SHOWN,&myGame,&mFont);
                                while(state.g_bRunning){

                                    handleEvents(&state,&dep,&dep2,&z,&s,&raquette1,&raquette2,&balle,&MouvementBallon);
                                    MoveRaquette(raquette1,raquette2,&dep,&dep2,&z,&s);
                                    renderTexture(&myGame,&dep,&dep2,&z,&s);
                                    CarreNoirFondBlanc(&myGame);
                                    writeSDL(&myGame,mFont);
                                    DessineBalle(&myGame,&dep,&dep2,&balle);

                                }

                                 break;
                        default:break;


                }

                 system ("cls");


             // Gestion des 60 fps (1000ms/60 = 16.6 -> 16
            delay(frameLimit);
            frameLimit = SDL_GetTicks() + 16;



        }

        destroy(&myGame);

        TTF_Quit();
        SDL_Quit();


    return 0;
}


int init(char *title, int xpos,int ypos,int width, int height,int flags,game *myGame, font *mFont){


    myGame->g_pWindow=NULL;
    myGame->g_pRenderer=NULL;
    myGame->g_psurface=NULL;
    myGame->g_ptexture=NULL;


    //initialize SDL

    if(SDL_Init(SDL_INIT_EVERYTHING)>=0)
    {
            //if succeeded create our window
            myGame->g_pWindow=SDL_CreateWindow(title,xpos,ypos,width,height,flags);
            //if succeeded create window, create our render
            if(myGame->g_pWindow!=NULL){
                myGame->g_pRenderer=SDL_CreateRenderer(myGame->g_pWindow,-1,SDL_RENDERER_PRESENTVSYNC);


            }


    }else{


        return 0;
    }

    if(TTF_Init() == -1)
    {
        fprintf(stderr, "Erreur d'initialisation de TTF_Init : %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    mFont->g_font=TTF_OpenFont("./assets/fonts/adlery/LemonMilk.otf",65);

    if(!mFont->g_font) {
        printf("TTF_OpenFont: %s\n", TTF_GetError());
        exit(EXIT_FAILURE);
    }

    return 1;



}

void CarreNoirFondBlanc(game *myGame){

           //Définition du rectangle a dessiner
        SDL_Rect rectangle;


        SDL_SetRenderDrawColor(myGame->g_pRenderer, 0, 0, 0, SDL_ALPHA_OPAQUE);
        SDL_RenderClear(myGame->g_pRenderer);

        SDL_SetRenderDrawColor(myGame->g_pRenderer, 255, 255, 255, SDL_ALPHA_OPAQUE);

        //Triangle
        SDL_RenderDrawLine(myGame->g_pRenderer, 400, 0, 400, 700);

}

void DessineBalle (game *myGame,coordonnees *dep,coordonnees *dep2,coordonnees *balle){

    int i;
        int j;
        SDL_Point bille;

        for (j=rayon;j!=0;j--){
            for(i=1;i<=360;i++){

            bille.x=j*cos(i);
            bille.y=j*sin(i);
            SDL_SetRenderDrawColor(myGame->g_pRenderer,0,0,255,1);
            SDL_RenderDrawPoint(myGame->g_pRenderer,bille.x+balle->x,bille.y+balle->y);

            }
        }

}

void renderTexture(game *myGame,coordonnees *dep,coordonnees *dep2,coordonnees *z,coordonnees *s) {

       //Définition du rectangle 1 a dessiner
        SDL_Rect rectangle;
        rectangle.x=0;//debut x
        rectangle.y=z->y;//debut y
        rectangle.w=10; //Largeur
        rectangle.h=100; //Hauteur

        if (rectangle.y<=0){
            rectangle.y=0;
        }

        if (rectangle.y>=SCREEN_HEIGHT-100){
            rectangle.y=500;
        }


        //Définition du rectangle 2 a dessiner
        SDL_Rect rectangle2;
        rectangle2.x=SCREEN_WIDTH-10;//debut x
        rectangle2.y=dep->y;//debut y
        rectangle2.w=10; //Largeur
        rectangle2.h=100; //Hauteur

        if (rectangle2.y<=0){
            rectangle2.y=0;
        }

        if (rectangle2.y>=SCREEN_HEIGHT-100){
            rectangle2.y=500;
        }


        //SDL_RenderDrawLine(myGame->g_pRenderer, 320, 200, 300, 240);



        //Draw in texture
        SDL_SetRenderDrawColor(myGame->g_pRenderer,255,0,0,255);
        SDL_SetRenderTarget(myGame->g_pRenderer, myGame->g_ptexture); //on modifie la texture

        SDL_RenderFillRect(myGame->g_pRenderer, &rectangle);
        SDL_RenderFillRect(myGame->g_pRenderer, &rectangle2);

        SDL_SetRenderTarget(myGame->g_pRenderer, NULL);// Dorénavent, on modifie à nouveau le renderer

        SDL_Rect position;//position de la texture dans screen
        position.x = 0;
        position.y = 0;
        SDL_QueryTexture(myGame->g_ptexture, NULL, NULL, &position.w, &position.h);
        SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,NULL,&position);


         SDL_RenderPresent(myGame->g_pRenderer);

         SDL_SetRenderDrawColor(myGame->g_pRenderer,0,0,0,255);
         SDL_DestroyTexture(myGame->g_ptexture);
         SDL_RenderClear(myGame->g_pRenderer);




}

void MoveRaquette(int raquette1, int raquette2, coordonnees *dep, coordonnees *dep2, coordonnees *z, coordonnees *s){

    if (raquette1==1 && dep->y>0){
        dep->y-=SPEED;
    }else if (raquette1==2 && dep->y<SCREEN_HEIGHT-100){
        dep->y+=SPEED;
    }

    if (raquette2==1 && z->y>0){
        z->y-=SPEED;
    }else if (raquette2==2 && z->y<SCREEN_HEIGHT-100){
        z->y+=SPEED;
    }

}

void destroy(game *myGame){

      //Destroy render
     if(myGame->g_pRenderer!=NULL)
        SDL_DestroyRenderer(myGame->g_pRenderer);

      //Destroy texture
     if(myGame->g_ptexture!=NULL)
        SDL_DestroyTexture(myGame->g_ptexture);

    //Destroy surface
     if(myGame->g_psurface!=NULL)
         SDL_FreeSurface(myGame->g_psurface);

    //Destroy window
    if(myGame->g_pWindow!=NULL)
        SDL_DestroyWindow(myGame->g_pWindow);

}


void handleEvents(gameState *state,coordonnees *dep,coordonnees *dep2,coordonnees *z,coordonnees *s, int *raquette1, int *raquette2, coordonnees *balle, coordonnees *MouvementBallon){

    balle->x-= MouvementBallon->x;
    balle->y-=MouvementBallon->y;

    SDL_Event event;

    if(SDL_PollEvent(&event)){
        switch(event.type){
        case SDL_QUIT:
              state->g_bRunning=0;break;
        case SDL_KEYDOWN:
                            if (event.key.keysym.sym == SDLK_UP){
                                *raquette1 = 1;
                            }

                            if (event.key.keysym.sym == SDLK_DOWN){
                                *raquette1 = 2;
                            }
                            if (event.key.keysym.sym == SDLK_z){
                                *raquette2 = 1;
                            }

                            if (event.key.keysym.sym == SDLK_s){
                                *raquette2 = 2;
                            };break;

        case SDL_KEYUP:

                            if (event.key.keysym.sym == SDLK_UP){
                                *raquette1 = 0;
                            }

                            if (event.key.keysym.sym == SDLK_DOWN){
                                *raquette1 = 0;
                            }
                            if (event.key.keysym.sym == SDLK_z){
                                *raquette2 = 0;
                            }

                            if (event.key.keysym.sym == SDLK_s){
                                *raquette2 = 0;
                            };break;

        default:break;

        }
    }

}

void writeSDL(game *myGame,font mFont) {

        SDL_Color fontColor={0,255,0};

        myGame->g_psurface=TTF_RenderText_Blended(mFont.g_font, "0 - 0", fontColor);//Charge la police

        if(myGame->g_psurface){


                //Définition du rectangle dest pour blitter la chaine
                SDL_Rect rectangle;
                rectangle.x=SCREEN_WIDTH/2.45;//debut x
                rectangle.y=25;//debut y
                rectangle.w=150; //Largeur
                rectangle.h=100; //Hauteur


                 myGame->g_ptexture = SDL_CreateTextureFromSurface(myGame->g_pRenderer,myGame->g_psurface); // Préparation de la texture pour la chaine
                 SDL_FreeSurface(myGame->g_psurface); // Libération de la ressource occupée par le sprite

                 if(myGame->g_ptexture){

                        SDL_RenderCopy(myGame->g_pRenderer,myGame->g_ptexture,NULL,&rectangle); // Copie du sprite grâce au SDL_Renderer
                        //SDL_RenderPresent(myGame->g_pRenderer); // Affichage
                 }
                 else{
                        fprintf(stdout,"Echec de creation de la texture (%s)\n",SDL_GetError());
                }



        }else{
            fprintf(stdout,"Echec de creation surface pour chaine (%s)\n",SDL_GetError());
        }

}

void delay(unsigned int frameLimit)
{
    // Gestion des 60 fps (images/seconde)
    unsigned int ticks = SDL_GetTicks();

    if (frameLimit < ticks)
    {
        return;
    }

    if (frameLimit > ticks + 16)
    {
        SDL_Delay(16);
    }

    else
    {
        SDL_Delay(frameLimit - ticks);
    }
}

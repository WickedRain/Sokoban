#include <SDL.h>
#include <stdbool.h>
#include <stdio.h>
#include <SDL_image.h>
#include <SDL_ttf.h>

/*Screen attributes*/
const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 512;
const int SCREEN_BPP = 32;

/*FPS Cap*/
const int FPS = 144;
bool running;

/*Score*/
int nummoves = 0;

/*The surfaces that will be used*/
SDL_Surface *grass = NULL;
SDL_Surface *wall = NULL;
SDL_Surface *screen = NULL;
SDL_Surface *icon = NULL;
SDL_Surface *sprites = NULL;
SDL_Surface *menupic = NULL;
SDL_Surface *htppic = NULL;
SDL_Surface *text = NULL;
SDL_Surface *text2 = NULL;
SDL_Surface *title = NULL;
SDL_Surface *htp = NULL;
SDL_Surface *htp1 = NULL;
SDL_Surface *htp2 = NULL;
SDL_Surface *htp3 = NULL;
SDL_Surface *htp4 = NULL;

/*The fonts that are going to be used*/
TTF_Font *font = NULL;
TTF_Font *fonttitle = NULL;
TTF_Font *fonthtp = NULL;

/*The color of the font*/
SDL_Color textColor = { 255, 255, 255 };

/*This function is called in the beginning to initialize all subsystems*/
bool init()
{
    /*Initialize all SDL subsystems*/
    if( SDL_Init( SDL_INIT_EVERYTHING ) == -1 )
    {
        return false;
    }

    /*Set up the screen*/
    screen = SDL_SetVideoMode( SCREEN_WIDTH, SCREEN_HEIGHT, SCREEN_BPP, SDL_SWSURFACE );

    /*If there was an error in setting up the screen*/
    if( screen == NULL )
    {
        return false;
    }

    TTF_Init();

    /*Sets Window caption and Icon*/
    icon = IMG_Load("Icon.png");
    SDL_WM_SetCaption("Sokoban", "Sokoban_Icon");
    SDL_WM_SetIcon(icon, NULL);

    /*If everything initialized fine*/
    return true;
}

void apply_surface( int x, int y, SDL_Surface* source, SDL_Surface* destination )
{
    /*Make a temporary rectangle to hold the offsets*/
    SDL_Rect offset;

    /*Give the offsets to the rectangle*/
    offset.x = x;
    offset.y = y;
    /*Blit the surface*/
    SDL_BlitSurface( source, NULL, destination, &offset );
}

SDL_Surface *load_image( char* filename )
{
    /*Temporary storage for the image that's loaded*/
    SDL_Surface* loadedImage = NULL;

    /*The optimized image that will be used*/
    SDL_Surface* optimizedImage = NULL;
    /*Load the image*/
    loadedImage = IMG_Load( filename );
    /*If nothing went wrong in loading the image*/
    if( loadedImage != NULL )
    {
        /*Create an optimized image*/
        optimizedImage = SDL_DisplayFormat( loadedImage );

        /*Free the old image*/
        SDL_FreeSurface( loadedImage );
    }
    /*Return the optimized image*/
    return optimizedImage;
}

/*This function clips the character from a spritesheet (sprites.bmp)*/
void setrects( SDL_Rect* clip)
{
    clip[0].x = 84; /*Facing back*/
    clip[0].y = 64;
    clip[0].w = 36;
    clip[0].h = 57;

    clip[1].x = 45; /*Facing left*/
    clip[1].y = 65;
    clip[1].w = 36;
    clip[1].h = 57;

    clip[2].x = 85; /*Facing forward*/
    clip[2].y = 126;
    clip[2].w = 36;
    clip[2].h = 57;

    clip[3].x = 47; /*Facing right*/
    clip[3].y = 126;
    clip[3].w = 36;
    clip[3].h = 57;

    clip[4].x = 0; /*Box*/
    clip[4].y = 0;
    clip[4].w = 64;
    clip[4].h = 64;

    clip[5].x = 2; /*Pressure pad*/
    clip[5].y = 66;
    clip[5].w = 30;
    clip[5].h = 30;
}

void menu()
{
    running = true;
    menupic = load_image("splashscreen.bmp");

    /*Load text*/
    font = TTF_OpenFont("KGCrossingALine.ttf", 30);
    fonttitle = TTF_OpenFont("KGCrossingALine.ttf", 100);
    title = TTF_RenderUTF8_Solid( fonttitle, "SOKOBAN", textColor );
    text = TTF_RenderUTF8_Solid( font, "Press SPACE to play!", textColor );
    text2 = TTF_RenderUTF8_Solid(font, "Press the H key to see how to play!", textColor);
    apply_surface(NULL, NULL, menupic, screen);
    apply_surface( 220, 350, text, screen );
    apply_surface( 160, 460, text2, screen );
    apply_surface( 190, 150, title, screen );
    SDL_Flip(screen);
    while(running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_SPACE:
                    sokoban();
                    break;
                case SDLK_ESCAPE:
                    cleanup();
                    break;
                case SDLK_h:
                    howtoplay();
                    break;
                }
            }
        }
    }
}

void howtoplay()
{
    menupic = IMG_Load("howtoplay.bmp");

    /*Load text*/
    fonthtp = TTF_OpenFont("KGCrossingALine.ttf", 30);
    htp = TTF_RenderUTF8_Solid( font, "How to play:", textColor );
    htp1 = TTF_RenderUTF8_Solid( font, "Push the boxes onto the pressure pads,", textColor );
    htp2 = TTF_RenderUTF8_Solid( font, "Be careful not to push boxes into walls,", textColor );
    htp3 = TTF_RenderUTF8_Solid( font, "As you won't be able to get them off walls anymore", textColor );
    htp4 = TTF_RenderUTF8_Solid( font, "This game is under development, please don't report any bugs :P", textColor );
    apply_surface(NULL, NULL, menupic, screen);
    apply_surface( 220, 50, htp, screen );
    apply_surface( 20, 150, htp1, screen );
    apply_surface( 20, 250, htp2, screen );
    apply_surface( 20, 350, htp3, screen );
    apply_surface( 20, 450, htp4, screen );
    SDL_Flip(screen);
    while(running)
    {
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_ESCAPE:
                    menu();
                    break;
                }
            }
        }
    }

}
/*This function is used for detecting collisions*/
bool collision(SDL_Rect* collider, SDL_Rect* collidewith)
{
    if((*collider).y >= (*collidewith).y + (*collidewith).h)
        return 0;
    if((*collider).x >= (*collidewith).x + (*collidewith).w)
        return 0;
    if((*collider).y + (*collider).h <= (*collidewith).y)
        return 0;
    if((*collider).x + (*collider).w <= (*collidewith).x)
        return 0;
    return 1;
}

void sokoban()
{
    running = true;
    grass = load_image("tile.bmp");
    wall = load_image("wall.bmp");
    Uint32 start;
    bool dir[4] = {0, 0, 0, 0};
    bool cont[4] = {0, 0, 0, 0};
    /*Rectangles*/
    SDL_Rect sprite, box, pad, tile, temp, walls;
    sprite.x = 64;
    sprite.y = 64;
    sprite.w = 36;
    sprite.h = 57;
    box.x = 160;
    box.y = 256;
    box.w = 64;
    box.h = 64;
    int frame = 0;
    sprites = SDL_DisplayFormat(load_image("sprites.bmp"));
    SDL_Rect playeranim[6];
    setrects(playeranim);
    SDL_SetColorKey(sprites, SDL_SRCCOLORKEY, SDL_MapRGB(screen -> format, 0xFF, 0x46, 0xFA));
    /*Game loop*/
    while(running)
    {
        start = SDL_GetTicks();
        SDL_Event event;
        while (SDL_PollEvent(&event))
        {
            switch(event.type)
            {
            case SDL_QUIT:
                running = false;
                break;
            /*Switch statements for directional movement (WASD)*/
            case SDL_KEYDOWN:
                switch (event.key.keysym.sym)
                {
                case SDLK_w: /*Up*/
                    if (cont[0] == 0 && cont[1] == 0 && cont[2] == 0 && cont[3] == 0)
                    {
                        dir[0] = 1;
                        cont[0] = 1;
                        dir[1] = 0;
                        dir[2] = 0;
                        dir[3] = 0;
                    }
                    break;
                case SDLK_a: /*Left*/
                    if (cont[0] == 0 && cont[1] == 0 && cont[2] == 0 && cont[3] == 0)
                    {
                        dir[1] = 1;
                        dir[0] = 0;
                        dir[2] = 0;
                        dir[3] = 0;
                    }
                    break;
                case SDLK_s: /*Right*/
                    if (cont[0] == 0 && cont[1] == 0 && cont[2] == 0 && cont[3] == 0)
                    {

                        dir[2] = 1;
                        dir[0] = 0;
                        dir[1] = 0;
                        dir[3] = 0;
                    }
                    break;
                case SDLK_d: /*Down*/
                    if (cont[0] == 0 && cont[1] == 0 && cont[2] == 0 && cont[3] == 0)
                    {
                        dir[3] = 1;
                        dir[0] = 0;
                        dir[1] = 0;
                        dir[2] = 0;
                    }
                    break;
                case SDLK_ESCAPE:
                    cleanup();
                    break;
                }
                break;
            case SDL_KEYUP:
                switch (event.key.keysym.sym)
                {
                case SDLK_w:
                    dir[0] = 0;
                    break;
                case SDLK_a:
                    dir[1] = 0;
                    break;
                case SDLK_s:
                    dir[2] = 0;
                    break;
                case SDLK_d:
                    dir[3] = 0;
                    break;
                }
                break;
            }
        }
        /*Logic && Render*/
        if(dir[0] || cont[0])
        {
            temp = sprite;
            temp.y --;
            if(!collision(&temp, &box))
            {
                cont[0]=1;
                sprite.y --;
                if(sprite.y % 64 == 0)
                {
                    cont[0] = 0;
                    nummoves++;
                }
            }
            else {
                cont[0] = 0;
                box.y -= 64;
            }
            frame = 0;
        }
        if(dir[1] || cont[1])
        {
            temp = sprite;
            temp.x --;
            if(!collision(&temp, &box))
            {
                cont[1] = 1;
                sprite.x --;
                if(sprite.x % 64 == 0)
                {
                    cont[1] = 0;
                    nummoves++;
                }
            }
            else {
                cont[1] = 0;
                box.x -= 64;
            }
            frame = 1;
        }
        if(dir[2] || cont[2])
        {
            temp = sprite;
            temp.y ++;
            if(!collision(&temp, &box))
            {
                cont[2] = 1;
                sprite.y ++;
                if(sprite.y % 64 == 0)
                {
                    cont[2] = 0;
                    nummoves++;
                }
            }
            else {
                cont[2] = 0;
                box.y += 64;
            }
            frame = 2;
        }
        if( dir[3] || cont[3])
        {
            temp = sprite;
            temp.x ++;
            if(!collision(&temp, &box))
            {
                cont[3] = 1;
                sprite.x ++;
                if(sprite.x % 64 == 0)
                {
                    cont[3] = 0;
                    nummoves++;
                }
            }
            else {
                cont[3] = 0;
                box.x += 64;
            }
            frame = 3;
        }
        /*Collision for screen*/
        if (sprite.x < 64 || sprite.y < 64 || sprite.x > 544 || sprite.y > 448) {
            dir[0] = 0;
            dir[1] = 0;
            dir[2] = 0;
            dir[3] = 0;
            cont[0] = 0;
            cont[1] = 0;
            cont[2] = 0;
            cont[3] = 0;
        }
        /* Draws the grass tilemap */
        for (int x = 0; x < SCREEN_WIDTH / 64; x++)
        {
            for (int y = 0; y < SCREEN_HEIGHT / 64; y++)
            {
                tile.x = x * 64;
                tile.y = y * 64;
                SDL_BlitSurface(grass, NULL, screen, &tile);
            }
        }
        /*Draws walls*/
        for (int x = 0; x < SCREEN_WIDTH / 64; x++)
        {
            walls.x = x * 64;
            SDL_BlitSurface(wall, NULL, screen, &walls);
        }
        for (int y = 0; y < SCREEN_HEIGHT / 64; y++)
        {
            walls.y = y * 64;
            SDL_BlitSurface(wall, NULL, screen, &walls);
        }
        SDL_BlitSurface(sprites, &playeranim[4], screen, &box);
        SDL_BlitSurface(sprites, &playeranim[frame], screen, &sprite);
        SDL_Flip(screen);

        /*If program is running fast, delay, if its running slow, delay less*/
        if( 1000 / FPS > SDL_GetTicks() - start)
            SDL_Delay(1000 / FPS - (SDL_GetTicks() - start));
    }
}

void cleanup()
{
    SDL_FreeSurface( sprites );
    SDL_FreeSurface( grass );
    SDL_FreeSurface( icon );
    SDL_FreeSurface( menupic );
    SDL_FreeSurface( htppic );
    SDL_FreeSurface( text );
    SDL_FreeSurface( text2 );
    SDL_FreeSurface( title );
    SDL_FreeSurface( htp );
    SDL_FreeSurface( htp1 );
    SDL_FreeSurface( htp2 );
    SDL_FreeSurface( htp3 );
    SDL_FreeSurface( htp4 );
    TTF_CloseFont( font );
    TTF_CloseFont( fonttitle );
    TTF_CloseFont( fonthtp );
    running = false;
    TTF_Quit();
    SDL_Quit();
}

int main(int argc, char* argv[])
{
    init();
    menu();
    return 0;
}

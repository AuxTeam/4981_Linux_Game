#include <stdio.h>
#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
#include "GameStateMenu.h"
#include "LTimer.h"
#include "LTexture.h"
#include "Window.h"

bool GameStateMenu::load() {

	bool success = true;

	//Open the font
	this->frameFont = TTF_OpenFont( "assets/fonts/kenpixelsquare.ttf", 28 );
	if ( this->frameFont == NULL ) {
		printf( "Failed to load font! SDL_ttf Error: %s\n", TTF_GetError() );
		success = false;
	}

	return success;
}

void GameStateMenu::loop() {
	//The frames per second timer
	LTimer fpsTimer;

	//The frames per second cap timer
	LTimer capTimer;

	//Keeps track of time between steps
	LTimer stepTimer;

	//Start counting frames per second
	unsigned long countedFrames = 0;
	float avgFPS = 0;
	fpsTimer.start();

	// State Loop
	while (this->play) {
		//Start cap timer
		capTimer.start();

		//Calculate and correct fps
		avgFPS = countedFrames / ( fpsTimer.getTicks() / 1000.f );

		//Set FPS text to be rendered
		this->frameTimeText.str( "" );
		this->frameTimeText << std::fixed << std::setprecision(0) << "FPS: " << avgFPS;

		// Process frame
		this->handle();	// Handle user input
		this->update(stepTimer.getTicks() / 1000.f); // Update state values
		stepTimer.start(); //Restart step timer
		this->sync();	// Sync game to server
		this->render();	// Render game state to window

		++countedFrames;

		//If frame finished early
		int frameTicks = capTimer.getTicks();
		if ( frameTicks < SCREEN_TICK_PER_FRAME ) {
			//Wait remaining time
			SDL_Delay( SCREEN_TICK_PER_FRAME - frameTicks );
		}

	}
}

void GameStateMenu::sync() {

}

void GameStateMenu::handle() {
	//Handle events on queue
	while ( SDL_PollEvent( &this->event )) {
		this->game->window->handleEvent(this->event);
   		switch( this->event.type ) {
      	case SDL_KEYDOWN:
        	switch( this->event.key.keysym.sym ) {
			case SDLK_ESCAPE:
				play = false;
				break;
			default:
                break;
			}
        	break;
      	case SDL_KEYUP:
       		switch( this->event.key.keysym.sym ) {
			default:
               	break;
			}
        	break;
		case SDL_QUIT:
			play = false;
			break;
      	default:
        	break;
    	}
	}
}

void GameStateMenu::update(const float& delta) {
	
	// TEMP: Skip to GameStateMatch
	// Remove this when working on the main menu
	this->game->stateID = 2;
	play = false;
	
}

void GameStateMenu::render() {
	//Only draw when not minimized
	if ( !this->game->window->isMinimized() ) {

		//Clear screen
		SDL_SetRenderDrawColor( this->game->renderer, 0xFF, 0xFF, 0xFF, 0xFF );
		SDL_RenderClear( this->game->renderer );
	
		SDL_Color textColor = { 0, 0, 0, 255 };

		//Render text
		if ( !this->frameFPSTextTexture.loadFromRenderedText( this->frameTimeText.str().c_str(),
											  textColor, this->game->renderer, this->frameFont ) ) {
			printf( "Unable to render FPS texture!\n" );
		}
		
		this->frameFPSTextTexture.render(this->game->renderer,
								( this->game->window->getWidth() - this->frameFPSTextTexture.getWidth() ), 0);
		
		//Update screen
		SDL_RenderPresent( this->game->renderer );
	}
}

GameStateMenu::~GameStateMenu() {
	
	// Free texture and font
	this->frameFPSTextTexture.free();
	TTF_CloseFont(this->frameFont);
	this->frameFont = NULL;

}
 
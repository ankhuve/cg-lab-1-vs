// Introduction lab that covers:
// * C++
// * SDL
// * 2D graphics
// * Plotting pixels
// * Video memory
// * Color representation
// * Linear interpolation
// * glm::vec3 and std::vector

#include "SDL.h"
#include <iostream>
#include <glm/glm.hpp>
#include <vector>
#include "SDLauxiliary.h"

using namespace std;
using glm::vec3;

// --------------------------------------------------------
// GLOBAL VARIABLES

const int SCREEN_WIDTH = 640;
const int SCREEN_HEIGHT = 480;
const int FOCAL_LENGTH = SCREEN_HEIGHT / 2;
SDL_Surface* screen;
vector<vec3> stars(1000);

// --------------------------------------------------------
// FUNCTION DECLARATIONS

void Draw();
void DrawSpace();

// --------------------------------------------------------
// FUNCTION DEFINITIONS

int main( int argc, char* argv[] )
{
	screen = InitializeSDL( SCREEN_WIDTH, SCREEN_HEIGHT );

	for (int i = 0; i < stars.size(); ++i) {
		vec3 currentStar = stars[i];

		for (int k = 0; k < 2; ++k) {
			float r = float(rand()) / float(RAND_MAX);
			float posOrNeg = float(rand()) / float(RAND_MAX);

			if (posOrNeg <= 0.5) {
				r *= (-1);
			}
			currentStar[k] = r;
		}

		float z = float(rand()) / float(RAND_MAX);
		currentStar[2] = z;
		stars[i] = currentStar;
	}

	while( NoQuitMessageSDL() )
	{
		DrawSpace();
	}
	SDL_SaveBMP( screen, "screenshot.bmp" );
	return 0;
}

void Interpolate(vec3 a, vec3 b, vector<vec3>& result)
{
	int numSteps;
	int vectorLength = result.size();
	if (vectorLength < 2) {
		numSteps = 1;
	}
	else {
		numSteps = vectorLength - 1;
	}

	vec3 stepIncrement = (b - a);
	stepIncrement /= numSteps;


	for (int i = 0; i < vectorLength; ++i) {
		vec3 currentStep = stepIncrement;
		currentStep *= i;

		result[i] = a + currentStep;
	}
}

void Draw()
{
	vector<vec3> result(SCREEN_WIDTH);

	vec3 topLeft(1, 0, 0); // red
	vec3 topRight(0, 0, 1); // blue
	vec3 bottomLeft(0, 1, 0); // green
	vec3 bottomRight(1, 1, 0); // yellow

	vector<vec3> leftSide(SCREEN_HEIGHT);
	vector<vec3> rightSide(SCREEN_HEIGHT);

	Interpolate(topLeft, bottomLeft, leftSide);
	Interpolate(topRight, bottomRight, rightSide);
	
	for( int y=0; y<SCREEN_HEIGHT; ++y )
	{
		Interpolate(leftSide[y], rightSide[y], result);

		for( int x=0; x<SCREEN_WIDTH; ++x )
		{
			PutPixelSDL( screen, x, y, result[x] );
		}
	}

	if( SDL_MUSTLOCK(screen) )
		SDL_UnlockSurface(screen);

	SDL_UpdateRect( screen, 0, 0, 0, 0 );
}

void DrawSpace()
{



	SDL_FillRect(screen, 0, 0);
	if (SDL_MUSTLOCK(screen))
		SDL_LockSurface(screen);
	for (size_t s = 0; s<stars.size(); ++s)
	{
		// project and draw each star
	}
	if (SDL_MUSTLOCK(screen))
		SDL_UnlockSurface(screen);
	SDL_UpdateRect(screen, 0, 0, 0, 0);
}
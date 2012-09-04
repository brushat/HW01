/**
* @file HW01.cpp
* 
* @author Adam Brush
* @date 2012-08-28
* 
* @note This file is (c) 2012.  It is licensed under the
* CC gv 3.0 license (http://creativecommons.org/licenses/by/3.0/),
* which means you are free to use, share, and remix is as long as you
* give attribution.  Commercial uses are allowed.
* 
* This project was heavily influenced by Dr. Brinkman's solution for the HW01 project:
* https://github.com/brinkmwj/HW01
* 
* This project fulfills the following requirements for homework 1:
* 
*
*/

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"
#include "cinder/ImageIo.h"
#include "Resources.h"

using namespace ci;
using namespace ci::app;
using namespace std;

class HW01App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();
	void prepSettings(Settings* settings);

private:
	static const int kWinWidth = 800;
	static const int kWinHeight = 600;
	static const int kSurfaceSize=1024;
	Surface* mySurface_;
	uint8_t* myPixels;
	void drawCircle(uint8_t* pixels, int center_x, int center_y, int radius, Color8u color);
	void drawRectangle(uint8_t* pixels,int start_x, int start_y, int rect_x, int rect_y, Color8u fill);
	void gradient(uint8_t* pixels, Color8u fill);
};

void HW01App::prepSettings(Settings* settings) {
	(*settings).setWindowSize(kWinWidth,kWinHeight);
	(*settings).setResizable(false);
}


void HW01App::drawCircle (uint8_t* pixels, int center_x, int center_y, int radius, Color8u fill){
	for(int y = center_y - radius; y <= center_y + radius; y++)
	{
		for(int x = center_x - radius; x <= center_x + radius; x++)
		{
			if(y < 0 || x < 0 || x >= kWinWidth || y >= kWinHeight) continue;
			int dist = (int)sqrt((double)((x-center_x)*(x-center_x) + (y-center_y)*(y-center_y)));
			if(dist <= radius)
			{
					int offset = 3*(x + y*kSurfaceSize);
					pixels[offset] = fill.r;
					pixels[offset+1] = fill.g;
					pixels[offset+2] = fill.b;
			}
		}
	}
}

void HW01App::drawRectangle (uint8_t* pixels, int start_x, int start_y, int width, int height, Color8u fill)
{

	// start at the top left of where the rectangle should be in the array and change the pixels from there
	for (int y = start_y; y <= (start_y + height); y++)
	{
		for (int x = start_x; x <= (start_x + width); x++)
		{
			int offset = 3*(x + y*kSurfaceSize);
			pixels[offset] = fill.r;
			pixels[offset+1] = fill.g;
			pixels[offset+2] = fill.b;
		}
	}
}

void HW01App::setup()
{
	mySurface_ = new Surface(kSurfaceSize,kSurfaceSize,false);
	myPixels = (*mySurface_).getData();
	
	
	
}

void HW01App::mouseDown( MouseEvent event )
{
}

void HW01App::update()
{
	
}

void HW01App::draw()
{
	drawRectangle(myPixels, 0, 0, 200, 300, Color8u(1,200,200));
	drawCircle(myPixels, 100, 100, 50, Color8u(200,200,1));
	gl::draw(*mySurface_);
}

CINDER_APP_BASIC( HW01App, RendererGl )

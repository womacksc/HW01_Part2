/** 
* @file HW01App.cpp 
* CSE 274 - Fall 2012  
* 
* @author Shiloh Womack 
* @due date Sept 5, 2012 
* 
* @note You are free to use, share, and remix this, privately or commercially, as long as you 
* give attribution.
*  
*/

#include "cinder/app/AppBasic.h"
#include "cinder/gl/gl.h"
#include "cinder/gl/Texture.h"

using namespace ci;
using namespace ci::app;
using namespace std;

const int TEXTURE_WIDTH=1024;
const int TEXTURE_HEIGHT=1024;

class HW01App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void update();
	void draw();

  private:	
	Surface* mySurface_;
		
	RECT rectSource1;
	RECT rectSource2;

	static const int kAppWidth = 800;	
	static const int kAppHeight = 600;

	void drawRectangle(uint8_t* pixel, int x, int y, int width, int height, Color8u shapeColor);
	void drawCircle(uint8_t* pixel, int centerX, int centerY, int radius, Color8u shapeColor);
	void copyRectangle(uint8_t* pixels, int xSrc, int ySrc, int widthSrc, int heightSrc, int xDest, int yDest);
	void gradient(uint8_t* pixels, Color8u color1, Color8u color2);
	void HW01App::televisionSnow(uint8_t* pixels);

		
};

/** 
*  Draws a rectangle and fills it with color 
**/
void HW01App::drawRectangle(uint8_t* pixel, int x, int y, int width, int height, Color8u shapeColor){	
	int startx = x;	
	int endx = x + width;	
	int starty = y;	
	int endy = y + height;	
	for(int y = starty; y < endy; y++){		
		for(int x = startx; x < endx; x++){			
			pixel[3*(x + y*TEXTURE_WIDTH)] = shapeColor.r;			
			pixel[3*(x + y*TEXTURE_WIDTH)+1] = shapeColor.g;			
			pixel[3*(x + y*TEXTURE_WIDTH)+2] = shapeColor.b;		
		}	
	}
}

/** 
*  Draws a circle and fills it with color 
**/
void HW01App::drawCircle(uint8_t* pixels, int centerX, int centerY , int radius, Color8u shapeColor){	
	for(int y=0; y<kAppHeight; y++){		
		for(int x=0; x<kAppWidth; x++){			
			double doubleY = y+0.0;			
			double doubleX = x+0.0;			
			if(((doubleX-centerX)*(doubleX-centerX) + (doubleY-centerY)*(doubleY-centerY) <= radius*radius)){				
				pixels[3*(x + y*TEXTURE_WIDTH)] = shapeColor.r;				
				pixels[3*(x + y*TEXTURE_WIDTH)+1] = shapeColor.g;				
				pixels[3*(x + y*TEXTURE_WIDTH)+2] = shapeColor.b;			
			}
		}
	}
}

/** 
*  Copies a rectangular section of the image to another section
*
*  INCOMPLETE
**/
void HW01App::copyRectangle(uint8_t* pixel, int xSrc, int ySrc, int widthSrc, int heightSrc, int xDest, int yDest){	
	int startx = xSrc;	
	int endx = xSrc + widthSrc;	
	int starty = ySrc;	
	int endy = ySrc + heightSrc;	
	for(int y = starty; y < endy; y++){		
		for(int x = startx; x < endx; x++){			
			//Do something	
		}	
	}

}

/** 
*  Gradually changes the background color from one color to another
*  
*  https://github.com/kosciaaj/HW01Part2/blob/master/src/HW01Part2App.cpp 
**/
void HW01App::gradient(uint8_t* pixels, Color8u color1, Color8u color2){	
	for(int i = 0; i <= kAppHeight; i++)	{		
		int ratio = i/kAppHeight;		
		for(int j = 0; j <= kAppWidth; j++)		{			
			int offset = 3*(i + j*kAppWidth);						
			pixels[offset] = (int) (color1.r * (1 + ratio) + color2.r * ratio);			
			pixels[offset+100] = (int) (color1.g * (1 + ratio) + color2.g * ratio);			
			pixels[offset+200] = (int) (color1.b * (1 + ratio) + color2.b * ratio);		
		}
	}
}

/** 
*  Distorts the background, possibly undoing gradient, and makes it look like
*  technicolor television snow
**/
void HW01App::televisionSnow(uint8_t* pixels){	
	for(int y = 1; y <= TEXTURE_HEIGHT-1; y++)	{		
		for(int x = 1; x <= TEXTURE_HEIGHT-1; x++)		{			
			int offset = 3*(x + y*TEXTURE_HEIGHT);			
			uint8_t r_total = 0;			
			uint8_t g_total = 0;			
			uint8_t b_total = 0;			
			for(int y2 = -1; y2 <= 1; y2++)			{				
				for(int x2 = -1; x2 <= 1; x2++)				{					
					int offset2 = 3*(x + x2 + (y + y2)*TEXTURE_HEIGHT);					
					r_total += pixels[offset2];					
					g_total += pixels[offset2 + 1];					
					b_total += pixels[offset2 + 2];				
				}			
			}			
			pixels[offset] = r_total/2;			
			pixels[offset + 1] = g_total/2;			
			pixels[offset + 2] = b_total/2;	
		}	
	}
}
void HW01App::setup()
{
	mySurface_ = new Surface(TEXTURE_WIDTH,TEXTURE_HEIGHT,false);
	
	rectSource1.left = 10;
	rectSource1.top = 10;
	rectSource1.bottom = 100;
	rectSource1.right = 100;

	rectSource2.left = 100;
	rectSource2.top = 100;
	rectSource2.bottom = 200;
	rectSource2.right = 200;

}

void HW01App::mouseDown( MouseEvent event )
{
}

void HW01App::update()
{

	//Get our array of pixel information	
	uint8_t* dataArray = (*mySurface_).getData();
	
	Color8u rectColor = Color8u(0, 140, 57);	
	Color8u circleColor = Color8u(50, 100, 80);
	Color8u gradientColorStart = Color8u(20, 20, 20);
	Color8u gradientColorEnd = Color8u(40, 0, 0);
	gradient(dataArray, gradientColorStart, gradientColorEnd);
	televisionSnow(dataArray);
	drawCircle(dataArray, 50, 50, 25, circleColor);	
	drawRectangle(dataArray, 200, 300, 200, 100, rectColor);
	copyRectangle(dataArray, 10, 10, 50, 50, 10, 200);
	
	
}

void HW01App::draw()
{
	gl::draw( *mySurface_ ); 
}

CINDER_APP_BASIC( HW01App, RendererGl )

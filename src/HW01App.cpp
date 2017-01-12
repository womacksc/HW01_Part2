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

class HW01App : public AppBasic {
  public:
	void setup();
	void mouseDown( MouseEvent event );	
	void prepareSettings(Settings* settings);
	void update();
	void draw();

  private:	
	Surface* mySurface_;

	//Just for readability use one standard for constants and put them all in the same place
	static const int APP_WIDTH = 800;	
	static const int APP_HEIGHT = 600;
	//Having both width and height is unnessay because surface must be square
	static const int TEXTURE_SIZE=1024;

	/** 
	*  Draws a rectangle and fills it with color 
	**/
	void drawRectangle(uint8_t* pixel, int x, int y, int width, int height, Color8u shapeColor);

	/** 
	*  Draws a circle and fills it with color 
	**/
	void drawCircle(uint8_t* pixel, int centerX, int centerY, int radius, Color8u shapeColor);

	/** 
	*  Copies a rectangular section of the image to another section
	**/
	void copyRectangle(uint8_t* pixels, int xSrc, int ySrc, int widthSrc, int heightSrc, int xDest, int yDest);

	/** 
	*  Gradually changes the background color from one color to another
	*  
	*  https://github.com/kosciaaj/HW01Part2/blob/master/src/HW01Part2App.cpp 
	**/
	void gradient(uint8_t* pixels, Color8u color1, Color8u color2);

	/** 
	*  Distorts the background, possibly undoing gradient, and makes it look like
	*  technicolor television snow
	**/
	void HW01App::televisionSnow(uint8_t* pixels);

};

//Keeps the user from changing window size to a size greater than the surface.
void HW01App::prepareSettings(Settings* settings){
	(*settings).setWindowSize(APP_WIDTH,APP_HEIGHT);
	(*settings).setResizable(false);
}

//I prefer the setup function to be at the top so the methods go in the general order of exicution.
//However thats just a personal openion.
void HW01App::setup()
{
	mySurface_ = new Surface(TEXTURE_SIZE,TEXTURE_SIZE,false);
}


void HW01App::drawRectangle(uint8_t* pixel, int x, int y, int width, int height, Color8u shapeColor){	
	int startx = x;	
	int endx = x + width;	
	int starty = y;	
	int endy = y + height;	
	for(int y = starty; y < endy; y++){		
		for(int x = startx; x < endx; x++){			
			pixel[3*(x + y*TEXTURE_SIZE)] = shapeColor.r;			
			pixel[3*(x + y*TEXTURE_SIZE)+1] = shapeColor.g;			
			pixel[3*(x + y*TEXTURE_SIZE)+2] = shapeColor.b;		
		}	
	}
}


void HW01App::drawCircle(uint8_t* pixels, int centerX, int centerY , int radius, Color8u shapeColor){	
	for(int y=0; y<APP_HEIGHT; y++){		
		for(int x=0; x<APP_WIDTH; x++){			
			double doubleY = y+0.0;			
			double doubleX = x+0.0;			
			//I like how you got around doing the square root function call 
			if(((doubleX-centerX)*(doubleX-centerX) + (doubleY-centerY)*(doubleY-centerY) <= radius*radius)){				
				pixels[3*(x + y*TEXTURE_SIZE)] = shapeColor.r;				
				pixels[3*(x + y*TEXTURE_SIZE)+1] = shapeColor.g;				
				pixels[3*(x + y*TEXTURE_SIZE)+2] = shapeColor.b;			
			}
		}
	}
}


void HW01App::copyRectangle(uint8_t* pixel, int xSrc, int ySrc, int widthSrc, int heightSrc, int xDest, int yDest){	
	int startx = xSrc;	
	int endx = xSrc + widthSrc;	
	int starty = ySrc;	
	int endy = ySrc + heightSrc;
	int srcOffset, destOffset;
	for(int y = 0; y < heightSrc; y++){		
		for(int x = 0; x < widthSrc; x++){	
			srcOffset = 3*((xSrc+x) + (ySrc+y)*TEXTURE_SIZE);
			destOffset = 3*((xDest+x) + (yDest+y)*TEXTURE_SIZE);
			pixel[destOffset] = pixel[srcOffset];
			pixel[destOffset+1] = pixel[srcOffset+1];
			pixel[destOffset+2] = pixel[srcOffset+2];
		}	
	}

}


void HW01App::gradient(uint8_t* pixels, Color8u color1, Color8u color2){	
	for(int i = 0; i <= APP_HEIGHT; i++)	{		
		int ratio = i/APP_HEIGHT;		
		for(int j = 0; j <= APP_WIDTH; j++)		{			
			int offset = 3*(i + j*APP_WIDTH);						
			pixels[offset] = (int) (color1.r * (1 + ratio) + color2.r * ratio);			
			pixels[offset+100] = (int) (color1.g * (1 + ratio) + color2.g * ratio);			
			pixels[offset+200] = (int) (color1.b * (1 + ratio) + color2.b * ratio);		
		}
	}
}


void HW01App::televisionSnow(uint8_t* pixels){	
	for(int y = 1; y <= TEXTURE_SIZE-1; y++)	{		
		for(int x = 1; x <= TEXTURE_SIZE-1; x++)		{			
			int offset = 3*(x + y*TEXTURE_SIZE);			
			uint8_t r_total = 0;			
			uint8_t g_total = 0;			
			uint8_t b_total = 0;			
			for(int y2 = -1; y2 <= 1; y2++)			{				
				for(int x2 = -1; x2 <= 1; x2++)				{					
					int offset2 = 3*(x + x2 + (y + y2)*TEXTURE_SIZE);					
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

void HW01App::update()
{

	//Get our array of pixel information	
	uint8_t* dataArray = (*mySurface_).getData();
	
	Color8u rectColor = Color8u(0, 140, 57);	
	Color8u circleColor = Color8u(50, 100, 80);
	Color8u gradientColorStart = Color8u(20, 20, 20);
	Color8u gradientColorEnd = Color8u(40, 0, 0);
	televisionSnow(dataArray);
	//Gradient doesn't add anything when used in combination with the televisionSnow method.
	//The order of the two functions does not change this. 
	gradient(dataArray, gradientColorStart, gradientColorEnd);
	drawCircle(dataArray, 50, 50, 25, circleColor);	
	drawRectangle(dataArray, 200, 300, 200, 100, rectColor);
	copyRectangle(dataArray, 10, 10, 50, 200, 500, 100);
}

void HW01App::draw()
{
	gl::draw( *mySurface_ ); 
}

CINDER_APP_BASIC( HW01App, RendererGl )

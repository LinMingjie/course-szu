// The sample model.  You should build a file
// very similar to this for when you make your model.
#include "modelerview.h"
#include "modelerapp.h"
#include "modelerdraw.h"
#include <FL/gl.h>
#include <gl/glu.h>
#include "bitmap.h"
#include "modelerglobals.h"

// To make a SampleModel, we inherit off of ModelerView
class SampleModel : public ModelerView 
{
public:
    SampleModel(int x, int y, int w, int h, char *label) 
        : ModelerView(x,y,w,h,label) { }

    virtual void draw();
};

// We need to make a creator function, mostly because of
// nasty API stuff that we'd rather stay away from.
ModelerView* createSampleModel(int x, int y, int w, int h, char *label)
{ 
    return new SampleModel(x,y,w,h,label); 
}

// We are going to override (is that the right word?) the draw()
// method of ModelerView to draw out SampleModel

static GLfloat lightDiffuse0[]  = { 1,1,1,1 };
static GLfloat lightDiffuse1[]  = { 1, 1, 1, 1 };
static GLfloat lightPosition0[] = { 4, 2, -4, 0 };
static GLfloat lightPosition1[] = { -2, 1, 5, 0 };
int level = 6;
void SampleModel::draw()
{
	level = VAL(LEVELOFDETAIL);
	lightPosition0[0] = (float)VAL(LIGHT0XPOS), lightPosition0[1] = (float)VAL(LIGHT0YPOS), lightPosition0[2] = (float)VAL(LIGHT0ZPOS);
	lightPosition1[0] = (float)VAL(LIGHT1XPOS), lightPosition1[1] = (float)VAL(LIGHT1YPOS), lightPosition1[2] = (float)VAL(LIGHT1ZPOS);


    glLightfv( GL_LIGHT0, GL_POSITION, lightPosition0 );
    glLightfv( GL_LIGHT0, GL_DIFFUSE, lightDiffuse0 );
    glLightfv( GL_LIGHT1, GL_POSITION, lightPosition1 );
    glLightfv( GL_LIGHT1, GL_DIFFUSE, lightDiffuse1 );
    // This call takes care of a lot of the nasty projection 
    // matrix stuff.  Unless you want to fudge directly with the 
	// projection matrix, don't bother with this ...
    ModelerView::draw();

	// draw the floor
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(0.0, 0.0, 0.0);
	glPushMatrix();
	glTranslated(-5,-3,-5);
	drawBox(10,0.01f,10);
	glPopMatrix();

	// draw the sample model
	setAmbientColor(.1f,.1f,.1f);
	setDiffuseColor(COLOR_GREEN);
	glPushMatrix();
	glTranslated(VAL(XPOS), VAL(YPOS), VAL(ZPOS));
	
	// Çû¸É
	{
		//setQuality(LOW);
		//setDrawMode(WIREFRAME);

		glRotated(VAL(ROTATE), 0.0, 1.0, 0.0);
		glRotated(-90, 1.0, 0.0, 0.0);
		glTranslated(0.0, 0.0, VAL(HEIGHT));

		if(level >= 2)
		{
			//Ñü´ø
			setDiffuseColor(COLOR_LEATHER);
			drawEllipticCylinder(0.2, 1.05, 1.05,1,0.7);
			glPushMatrix();
			glTranslated(-0.15, -0.85, -1);
			glRotated(-6, 1.0, 0.0, 0.0);
			drawBox(0.16, 0.04, 1.10);
			glTranslated(0.18, 0.0, 0.0);
			drawBox(0.16, 0.04, 1.10);
			glTranslated(0.18, 0.0, 0.0);
			drawBox(0.16, 0.04, 1.10);
			glTranslated(-0.54, 0.0, 0.0);
			drawBox(0.16, 0.04, 1.10);
			glTranslated(-0.18, 0.0, 0.0);
			drawBox(0.16, 0.04, 1.10);
			glPopMatrix();
		}

		unsigned char* texture_data = NULL; // it stores an array of pixels value
		int texture_width = 0; // the width of the texture
		int texture_height = 0; // the height of the texture
		// Read the bitmap
		texture_data = readBMP("arm2.bmp", texture_width, texture_height);

		// Generate The Texture
		glTexImage2D(GL_TEXTURE_2D, 0, 3, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);

		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
		glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
		

		glEnable(GL_TEXTURE_2D);
		glTranslated(0.0, 0.0, 0.2);
		setDiffuseColor(COLOR_ARM);
		drawEllipticCylinder(1, 1.0, 1.0,1,0.7);		

		//Frame All
		if(VAL(FRAMEALL) > 0)
		{
			ModelerApplication::Instance()->SetControlValue(XPOS, 0);
			ModelerApplication::Instance()->SetControlValue(YPOS, 0);
			ModelerApplication::Instance()->SetControlValue(ZPOS, 0);
			ModelerApplication::Instance()->SetControlValue(LIGHT0XPOS, 4);
			ModelerApplication::Instance()->SetControlValue(LIGHT0YPOS, 2);
			ModelerApplication::Instance()->SetControlValue(LIGHT0ZPOS, -4);
			ModelerApplication::Instance()->SetControlValue(LIGHT1XPOS, -2);
			ModelerApplication::Instance()->SetControlValue(LIGHT1YPOS, 1);
			ModelerApplication::Instance()->SetControlValue(LIGHT1ZPOS, 5);
			ModelerApplication::Instance()->SetControlValue(FRAMEALL, 0);
		}
		
		//Ñü
		if(level >= 1)
		{
			
			//ÐØ
			glPushMatrix();
			glTranslated(0.0, 0.0, 1.0);
			drawEllipticCylinder(0.3, 1.0, 1.1,1,0.7);
			glTranslated(0.0, 0.0, 0.3);
			drawEllipticCylinder(0.6, 1.1, 1.1,1,0.7);
			glTranslated(0.0, 0.0, 0.6);
			drawEllipticCylinder(0.3, 1.1, 0.5,1,0.7);
			glDisable(GL_TEXTURE_2D);
			if(level >= 2)
			{
				setDiffuseColor(COLOR_SKIN);
				//Í·²¿
				glPushMatrix();
				setDiffuseColor(COLOR_RED);
				drawCylinder(0.4, 0.4, 0.4);	//Áì
				setDiffuseColor(COLOR_SKIN);
				glRotated(VAL(TURNHEAD), 0.0, 0.0, 1.0);	//¿ØÖÆÍ·µÄÐý×ª
				drawCylinder(0.6, 0.2, 0.2);	//²±
				glTranslated(0.0, 0.0, 1.2);
				glRotated(-90, 1.0, 0.0, 0.0);
				drawHead();
				glPopMatrix();
			}
			if(level >= 2)
			{
				setDiffuseColor(COLOR_SKIN);
				//ÓÒ±Û
				glPushMatrix();
				glTranslated(-0.75, 0.0, -0.3);
				glRotated(VAL(RIGHTARMRT), 1.0, 0.0, 0.0);
				glRotated(VAL(RIGHTARMUD), 0.0, 1.0, 0.0);
				glRotated(-90, 0.0, 1.0, 0.0);
				drawCylinder(1.3, 0.25, 0.25);

				glTexImage2D(GL_TEXTURE_2D, 0, 3, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
				glEnable(GL_TEXTURE_2D);
				setDiffuseColor(COLOR_ARM);
				drawCylinder(0.8, 0.3, 0.3);
				glDisable(GL_TEXTURE_2D);
				setDiffuseColor(COLOR_RED);
				drawCylinder(1.0, 0.28, 0.28);
				setDiffuseColor(COLOR_SKIN);
				if(level >= 3)
				{
					glPushMatrix();
					glTranslated(0.0, 0.0, 1.3);
					drawSphere(0.25);
					{
						//ÓÒÇ°±Û
						glPushMatrix();
						glRotated(VAL(RIGHTLOWARMUD), 0.0, 1.0, 0.0);
						drawCylinder(1.2, 0.25, 0.2);
						if(level >= 4)
						{
							glPushMatrix();
							glTranslated(0.0, 0.0, 1.2);
							glRotated(-VAL(TURNRIGHTWRIST), 0.0, 0.0, 1.0);
							glRotated(-VAL(ANGLERIGHTWRIST), 1.0, 0.0, 0.0);
							glRotated(-VAL(WAVERIGHTWRIST), 0.0, 1.0, 0.0);
							glRotated(180, 0.0, 0.0, 1.0);
							drawSphere(0.2);	//ÓÒÊÖÍó
							
							glTranslated(0.0, 0.0, 0.2);
							drawHand(0);	//ÓÒÊÖ
							
							glRotated(90, 0.0, 0.0, 1.0);
							glTranslated(-0.15, 0.4, 0.10);
							
							if(VAL(WEAPONSWITCH) >= 1 && VAL(WEAPONSWITCH) < 3)
								drawSword();
							glPopMatrix();
						}
						glPopMatrix();
					}
					glPopMatrix();
				}
				glPopMatrix();

				//×ó±Û
				glPushMatrix();
				glTranslated(0.75, 0.0, -0.3);
				glRotated(VAL(LEFTARMRT), 1.0, 0.0, 0.0);
				glRotated(-VAL(LEFTARMUD), 0.0, 1.0, 0.0);
				glRotated(90, 0.0, 1.0, 0.0);
				drawCylinder(1.3, 0.25, 0.25);

				glTexImage2D(GL_TEXTURE_2D, 0, 3, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
				glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering
				glEnable(GL_TEXTURE_2D);
				setDiffuseColor(COLOR_ARM);
				drawCylinder(0.8, 0.3, 0.3);
				glDisable(GL_TEXTURE_2D);
				setDiffuseColor(COLOR_RED);
				drawCylinder(1.0, 0.28, 0.28);
				setDiffuseColor(COLOR_SKIN);
				if(level >= 3)
				{
					glPushMatrix();
					glTranslated(0.0, 0.0, 1.3);
					drawSphere(0.25);
					{
						//×óÇ°±Û
						glPushMatrix();
						glRotated(-VAL(LEFTLOWARMUD), 0.0, 1.0, 0.0);
						drawCylinder(1.2, 0.25, 0.2);
						if(level >= 4)
						{
							glPushMatrix();
							glTranslated(0.0, 0.0, 1.2);
							glRotated(VAL(TURNLEFTWRIST), 0.0, 0.0, 1.0);
							glRotated(VAL(ANGLELEFTWRIST), 1.0, 0.0, 0.0);
							glRotated(-VAL(WAVELEFTWRIST), 0.0, 1.0, 0.0);
							drawSphere(0.2);	//×óÊÖÍó
							glTranslated(0.0, 0.0, 0.2);
							
							drawHand(1);	//×óÊÖ
							glPopMatrix();
						}
						glPopMatrix();
					}
					glPopMatrix();
				}
				glPopMatrix();
			}
			glPopMatrix();//¶ÔÓ¦ÐØ
		}
		if(level >= 1)
		{
			//ÍÎ
			
			glTexImage2D(GL_TEXTURE_2D, 0, 3, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_data);
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
			glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering

			glEnable(GL_TEXTURE_2D);
			glTranslated(0.0, 0.0, -1.2);
			setDiffuseColor(COLOR_ARM);
			drawEllipticCylinder(1.2, 1.2, 1.0, 1.0, 0.7);
			glDisable(GL_TEXTURE_2D);
			if(level >= 2)
			{
				//È¹
				glPushMatrix();
				setDiffuseColor(COLOR_RED);
				glTranslated(0.0, 0.0, -0.2);
				drawEllipticCylinder(0.5, 1.15, 1.0, 1.0, 0.7);
				glPopMatrix();
				setDiffuseColor(COLOR_SKIN);
				//ÓÒ´óÍÈ
				glPushMatrix();
				glRotated(180, 0.0, 1.0, 0.0);
				glTranslated(0.5, 0.0, -0.5);
				glRotated(VAL(RIGHTLEGFB), 1.0, 0.0, 0.0);
				glRotated(VAL(RIGHTLEGIO), 0.0, 1.0, 0.0);
				drawCylinder(1.0, 0.45, 0.35);
				if(level >= 3)
				{
					//ÓÒÐ¡ÍÈ
					glPushMatrix();
					glTranslated(0.0, 0.0, 1.0);
					glRotated(VAL(RIGHTLOWLEGFB), 1.0, 0.0, 0.0);
					drawSphere(0.35);				//Ï¥¸Ç
					drawCylinder(1.2, 0.35, 0.3);	//Ð¡ÍÈ
					glTranslated(0.0, 0.0, 1.2);
					drawSphere(0.3);				//½Åõ×
					if(level >= 4)
					{
						//ÓÒ½Å
						drawCylinder(0.3, 0.32, 0.35);
						glTranslated(0.0, -0.2, -0.1);
						glScaled(0.6, 1.0, 1.0);
						drawCylinder(0.4, 0.0, 0.60);
					}
					glPopMatrix();
				}
				glPopMatrix();

				//×ó´óÍÈ
				glPushMatrix();
				glRotated(180, 0.0, 1.0, 0.0);
				glTranslated(-0.5, 0.0, -0.5);
				glRotated(VAL(LEFTLEGFB), 1.0, 0.0, 0.0);
				glRotated(VAL(LEFTLEGIO), 0.0, 1.0, 0.0);
				drawCylinder(1.0, 0.45, 0.35);
				if(level >= 3)
				{
					//×óÐ¡ÍÈ
					glPushMatrix();
					glTranslated(0.0, 0.0, 1.0);
					glRotated(VAL(LEFTLOWLEGFB), 1.0, 0.0, 0.0);
					drawSphere(0.35);				//Ï¥¸Ç
					drawCylinder(1.2, 0.35, 0.3);	//Ð¡ÍÈ
					glTranslated(0.0, 0.0, 1.2);
					drawSphere(0.3);				//½Åõ×
					if(level >= 4)
					{
						//×ó½Å
						drawCylinder(0.3, 0.32, 0.35);
						glTranslated(0.0, -0.2, -0.1);
						glScaled(0.6, 1.0, 1.0);
						drawCylinder(0.4, 0.0, 0.60);
					}
					glPopMatrix();
				}
				glPopMatrix();//¶ÔÓ¦ÍÎ
			}
		}
		glPopMatrix();	//¶ÔÓ¦¸¹
	delete [] texture_data;

	}
	glPopMatrix();

}

int main()
{
	// Initialize the controls
	// Constructor is ModelerControl(name, minimumvalue, maximumvalue, 
	// stepsize, defaultvalue)
    ModelerControl controls[NUMCONTROLS];
	controls[LEVELOFDETAIL] = ModelerControl("Level of Detail", 0, 4, 1, 4);
	controls[LIGHT0XPOS] = ModelerControl("Light0 X Position", -5, 5, 0.1, 4);
	controls[LIGHT0YPOS] = ModelerControl("Light0 Y Position", -5, 5, 0.1, 2);
	controls[LIGHT0ZPOS] = ModelerControl("Light0 Z Position", -5, 5, 0.1, -4);
	controls[LIGHT1XPOS] = ModelerControl("Light1 X Position", -5, 5, 0.1, -2);
	controls[LIGHT1YPOS] = ModelerControl("Light1 Y Position", -5, 5, 0.1, 1);
	controls[LIGHT1ZPOS] = ModelerControl("Light1 Z Position", -5, 5, 0.1, 5);

    controls[XPOS] = ModelerControl("X Position", -5, 5, 0.1, 0);
    controls[YPOS] = ModelerControl("Y Position", 0, 5, 0.1, 0);
    controls[ZPOS] = ModelerControl("Z Position", -5, 5, 0.1, 0);
	controls[FRAMEALL] = ModelerControl("Frame All?", 0, 1, 1, 0);
	controls[WEAPONSWITCH] = ModelerControl("Weapon Switch", 0, 3, 1, 2);


    controls[HEIGHT] = ModelerControl("Height", 1, 2.5, 0.1, 1);
	controls[ROTATE] = ModelerControl("Rotate", -135, 135, 1, 0);
	controls[TURNHEAD] = ModelerControl("Turn Head", -50, 50, 1, 0);

	controls[EDGE_LEFTLEG] = ModelerControl("--------Left Leg--------", 0, 1, 1, 0);
	controls[LEFTLEGIO] = ModelerControl("Left Leg In/out", 0, 35, 1, 0);
	controls[LEFTLEGFB] = ModelerControl("Left Leg Forward/Back", -40, 40, 1, 0);
	controls[LEFTLOWLEGFB] = ModelerControl("Left Lower Leg Forward/Back", -60, 0, 1, 0);

	controls[EDGE_RIGHTLEG] = ModelerControl("--------Right Leg--------", 0, 1, 1, 0);
	controls[RIGHTLEGIO] = ModelerControl("Right Leg In/out", 0, 35, 1, 0);
	controls[RIGHTLEGFB] = ModelerControl("Right Leg Forward/Back", -40, 40, 1, 0);
	controls[RIGHTLOWLEGFB] = ModelerControl("Right Lower Leg Forward/Back", -60, 0, 1, 0);

	controls[EDGE_LEFTARM] = ModelerControl("--------Left Arm--------", 0, 1, 1, 0);
	controls[LEFTARMRT] = ModelerControl("Left Arm Rotate", -180, 180, 1, 0);
	controls[LEFTARMUD] = ModelerControl("Left Arm Up/Down", 20, -80, 1, -27);
	controls[LEFTLOWARMUD] = ModelerControl("Left Lower Arm Up/Down", 0, -90, 1, -58);

	controls[EDGE_RIGHTARM] = ModelerControl("--------Right Arm--------", 0, 1, 1, 0);
	controls[RIGHTARMRT] = ModelerControl("Right Arm Rotate", -180, 180, 1, 0);
	controls[RIGHTARMUD] = ModelerControl("Right Arm Up/Down", 20, -80, 1, -42);
	controls[RIGHTLOWARMUD] = ModelerControl("Right Lower Arm Up/Down", 0, -90, 1, -43);

	controls[EDGE_LEFTHAND] = ModelerControl("--------Left Hand--------", 0, 1, 1, 0);
	controls[TURNLEFTWRIST] = ModelerControl("Turn Left Wrist", -90, 90, 1, 90);
	controls[ANGLELEFTWRIST] = ModelerControl("Angle Left Wrist", -90, 90, 1, 0);
	controls[WAVELEFTWRIST] = ModelerControl("Wave Left Wrist", -90, 90, 1, 0);
	controls[CURLLEFTTHUMB] = ModelerControl("Curl Left Thumb", 0, 120, 1, 60);
	controls[IOLEFTTHUMB] = ModelerControl("In/Out Left Thumb", 0, 30, 1, 30);
	controls[CURLLEFTFINGERS] = ModelerControl("Curl Left Fingers", 0, 120, 1, 75);
	
	controls[EDGE_RIGHTHAND] = ModelerControl("--------Right Hand--------", 0, 1, 1, 0);
	controls[TURNRIGHTWRIST] = ModelerControl("Turn Right Wrist", -90, 90, 1, -80);	//0 , 65
	controls[ANGLERIGHTWRIST] = ModelerControl("Angle Right Wrist", -90, 90, 1, 0); //-20, 45
	controls[WAVERIGHTWRIST] = ModelerControl("Wave Right Wrist", -90, 90, 1, -30);	//0, 50
	controls[CURLRIGHTTHUMB] = ModelerControl("Curl Right Thumb", 0, 120, 1, 60);
	controls[IORIGHTTHUMB] = ModelerControl("In/Out Right Thumb", 0, 30, 1, 30);
	controls[CURLRIGHTFINGERS] = ModelerControl("Curl Right Fingers", 0, 120, 1, 75);

	/*
	controls[DEBUG_EDGE] = ModelerControl("--------DEBUG_EDGE--------", 0, 1, 1, 0);
	controls[DEBUG_XPOS] = ModelerControl("DEBUG_XPOS", -5, 5, 0.05, 0.75);
	controls[DEBUG_YPOS] = ModelerControl("DEBUG_YPOS", -5, 5, 0.05, 0);
	controls[DEBUG_ZPOS] = ModelerControl("DEBUG_ZPOS", -5, 5, 0.05, 0);
	controls[DEBUG_YPOS] = ModelerControl("DEBUG_YPOS", -5, 5, 0.05, 0);
	controls[DEBUG_ZPOS] = ModelerControl("DEBUG_ZPOS", -5, 5, 0.05, 0);
	controls[DEBUG_XPOS] = ModelerControl("DEBUG_XPOS", -5, 5, 0.05, 0);
	controls[DEBUG_YPOS] = ModelerControl("DEBUG_YPOS", -5, 5, 0.05, 0);
	controls[DEBUG_ZPOS] = ModelerControl("DEBUG_ZPOS", -5, 5, 0.05, 0);
	controls[DEBUG_XSCALE] = ModelerControl("DEBUG_X_Scale", 0, 3.0, 0.05, 1.0);
	controls[DEBUG_YSCALE] = ModelerControl("DEBUG_Y_Scale", 0, 3.0, 0.05, 1);
	controls[DEBUG_ZSCALE] = ModelerControl("DEBUG_Z_Scale", 0, 3.0, 0.05, 1);
	controls[DEBUG_R1SIZE] = ModelerControl("DEBUG_R1_Size", 0, 10.0, 0.05, 1.0);
	controls[DEBUG_R2SIZE] = ModelerControl("DEBUG_R2_Size", 0, 10.0, 0.05, 1.0);
	controls[DEBUG_HSIZE] = ModelerControl("DEBUG_H_Size", 0, 10.0, 0.05, 1.0);
	controls[DEBUG_XROTATE] = ModelerControl("DEBUG_Rotate on X", -360, 360, 1, 0);
	controls[DEBUG_YROTATE] = ModelerControl("DEBUG_Rotate on Y", -360, 360, 1, 0);
	controls[DEBUG_ZROTATE] = ModelerControl("DEBUG_Rotate on Z", -360, 360, 1, 0);
*/
    ModelerApplication::Instance()->Init(&createSampleModel, controls, NUMCONTROLS);
    return ModelerApplication::Instance()->Run();
}

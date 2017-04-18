#include "modelerdraw.h"
#include "modelerapp.h"
#include <FL/gl.h>
#include <GL/glu.h>
#include <cstdio>
#include <math.h>

// ********************************************************
// Support functions from previous version of modeler
// ********************************************************
void _dump_current_modelview( void )
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    if (mds->m_rayFile == NULL)
    {
        fprintf(stderr, "No .ray file opened for writing, bailing out.\n");
        exit(-1);
    }
    
    GLdouble mv[16];
    glGetDoublev( GL_MODELVIEW_MATRIX, mv );
    fprintf( mds->m_rayFile, 
        "transform(\n    (%f,%f,%f,%f),\n    (%f,%f,%f,%f),\n     (%f,%f,%f,%f),\n    (%f,%f,%f,%f),\n",
        mv[0], mv[4], mv[8], mv[12],
        mv[1], mv[5], mv[9], mv[13],
        mv[2], mv[6], mv[10], mv[14],
        mv[3], mv[7], mv[11], mv[15] );
}

void _dump_current_material( void )
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    if (mds->m_rayFile == NULL)
    {
        fprintf(stderr, "No .ray file opened for writing, bailing out.\n");
        exit(-1);
    }
    
    fprintf( mds->m_rayFile, 
        "material={\n    diffuse=(%f,%f,%f);\n    ambient=(%f,%f,%f);\n}\n",
        mds->m_diffuseColor[0], mds->m_diffuseColor[1], mds->m_diffuseColor[2], 
        mds->m_diffuseColor[0], mds->m_diffuseColor[1], mds->m_diffuseColor[2]);
}

// ****************************************************************************

// Initially assign singleton instance to NULL
ModelerDrawState* ModelerDrawState::m_instance = NULL;

ModelerDrawState::ModelerDrawState() : m_drawMode(NORMAL), m_quality(MEDIUM)
{
    float grey[]  = {.5f, .5f, .5f, 1};
    float white[] = {1,1,1,1};
    float black[] = {0,0,0,1};
    
    memcpy(m_ambientColor, black, 4 * sizeof(float));
    memcpy(m_diffuseColor, grey, 4 * sizeof(float));
    memcpy(m_specularColor, white, 4 * sizeof(float));
    
    m_shininess = 0.5;
    
    m_rayFile = NULL;
}

// CLASS ModelerDrawState METHODS
ModelerDrawState* ModelerDrawState::Instance()
{
    // Return the singleton if it exists, otherwise, create it
    return (m_instance) ? (m_instance) : m_instance = new ModelerDrawState();
}

// ****************************************************************************
// Modeler functions for your use
// ****************************************************************************
// Set the current material properties

void setAmbientColor(float r, float g, float b)
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    mds->m_ambientColor[0] = (GLfloat)r;
    mds->m_ambientColor[1] = (GLfloat)g;
    mds->m_ambientColor[2] = (GLfloat)b;
    mds->m_ambientColor[3] = (GLfloat)1.0;
    
    if (mds->m_drawMode == NORMAL)
        glMaterialfv( GL_FRONT_AND_BACK, GL_AMBIENT, mds->m_ambientColor);
}

void setDiffuseColor(float r, float g, float b)
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    mds->m_diffuseColor[0] = (GLfloat)r;
    mds->m_diffuseColor[1] = (GLfloat)g;
    mds->m_diffuseColor[2] = (GLfloat)b;
    mds->m_diffuseColor[3] = (GLfloat)1.0;
    
    if (mds->m_drawMode == NORMAL)
        glMaterialfv( GL_FRONT_AND_BACK, GL_DIFFUSE, mds->m_diffuseColor);
    else
        glColor3f(r,g,b);
}

void setSpecularColor(float r, float g, float b)
{	
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    mds->m_specularColor[0] = (GLfloat)r;
    mds->m_specularColor[1] = (GLfloat)g;
    mds->m_specularColor[2] = (GLfloat)b;
    mds->m_specularColor[3] = (GLfloat)1.0;
    
    if (mds->m_drawMode == NORMAL)
        glMaterialfv( GL_FRONT_AND_BACK, GL_SPECULAR, mds->m_specularColor);
}

void setShininess(float s)
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    mds->m_shininess = (GLfloat)s;
    
    if (mds->m_drawMode == NORMAL)
        glMaterialf( GL_FRONT, GL_SHININESS, mds->m_shininess);
}

void setDrawMode(DrawModeSetting_t drawMode)
{
    ModelerDrawState::Instance()->m_drawMode = drawMode;
}

void setQuality(QualitySetting_t quality)
{
    ModelerDrawState::Instance()->m_quality = quality;
}

bool openRayFile(const char rayFileName[])
{
    ModelerDrawState *mds = ModelerDrawState::Instance();

	fprintf(stderr, "Ray file format output is buggy (ehsu)\n");
    
    if (!rayFileName)
        return false;
    
    if (mds->m_rayFile) 
        closeRayFile();
    
    mds->m_rayFile = fopen(rayFileName, "w");
    
    if (mds->m_rayFile != NULL) 
    {
        fprintf( mds->m_rayFile, "SBT-raytracer 1.0\n\n" );
        fprintf( mds->m_rayFile, "camera { fov=30; position=(0,0.8,5); direction=(0,-0.8,-5); }\n\n" );
        fprintf( mds->m_rayFile, 
            "directional_light { direction=(-1,-2,-1); color=(0.7,0.7,0.7); }\n\n" );
        return true;
    }
    else
        return false;
}

void _setupOpenGl()
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
	switch (mds->m_drawMode)
	{
	case NORMAL:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_SMOOTH);
		break;
	case FLATSHADE:
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
		glShadeModel(GL_FLAT);
		break;
	case WIREFRAME:
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
		glShadeModel(GL_FLAT);
	default:
		break;
	}

}

void closeRayFile()
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    
    if (mds->m_rayFile) 
        fclose(mds->m_rayFile);
    
    mds->m_rayFile = NULL;
}

void drawTriangle( double x1, double y1, double z1, double x2,double y2, double z2, double x3, double y3, double z3 )
{
    ModelerDrawState *mds = ModelerDrawState::Instance();

	_setupOpenGl();

    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile, 
            "polymesh { points=((%f,%f,%f),(%f,%f,%f),(%f,%f,%f)); faces=((0,1,2));\n", x1, y1, z1, x2, y2, z2, x3, y3, z3 );
        _dump_current_material();
        fprintf(mds->m_rayFile, "})\n" );
    }
    else
    {
        double a, b, c, d, e, f;
        
        /* the normal to the triangle is the cross product of two of its edges. */
        a = x2-x1;
        b = y2-y1;
        c = z2-z1;
        
        d = x3-x1;
        e = y3-y1;
        f = z3-z1;
        
        glBegin( GL_TRIANGLES );
        glNormal3d( b*f - c*e, c*d - a*f, a*e - b*d );
        glVertex3d( x1, y1, z1 );
        glVertex3d( x2, y2, z2 );
        glVertex3d( x3, y3, z3 );
        glEnd();
    }
}


void drawSphere(double r)
{
    ModelerDrawState *mds = ModelerDrawState::Instance();

	_setupOpenGl();
    
    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile, "scale(%f,%f,%f,sphere {\n", r, r, r );
        _dump_current_material();
        fprintf(mds->m_rayFile, "}))\n" );
    }
    else
    {
        int divisions; 
        GLUquadricObj* gluq;
        
        switch(mds->m_quality)
        {
        case HIGH: 
            divisions = 32; break;
        case MEDIUM: 
            divisions = 20; break;
        case LOW:
            divisions = 12; break;
        case POOR:
            divisions = 8; break;
        }
        
        gluq = gluNewQuadric();
        gluQuadricDrawStyle( gluq, GLU_FILL );
        gluQuadricTexture( gluq, GL_TRUE );
        gluSphere(gluq, r, divisions, divisions);
        gluDeleteQuadric( gluq );
    }
}


void drawBox( double x, double y, double z )
{
    ModelerDrawState *mds = ModelerDrawState::Instance();

	_setupOpenGl();
    
    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile,  
            "scale(%f,%f,%f,translate(0.5,0.5,0.5,box {\n", x, y, z );
        _dump_current_material();
        fprintf(mds->m_rayFile,  "})))\n" );
    }
    else
    {
        /* remember which matrix mode OpenGL was in. */
        int savemode;
        glGetIntegerv( GL_MATRIX_MODE, &savemode );
        
        /* switch to the model matrix and scale by x,y,z. */
        glMatrixMode( GL_MODELVIEW );
        glPushMatrix();
        glScaled( x, y, z );
        
        glBegin( GL_QUADS );
        
        glNormal3d( 0.0, 0.0, -1.0 );
        glVertex3d( 0.0, 0.0, 0.0 ); glVertex3d( 0.0, 1.0, 0.0 );
        glVertex3d( 1.0, 1.0, 0.0 ); glVertex3d( 1.0, 0.0, 0.0 );
        
        glNormal3d( 0.0, -1.0, 0.0 );
        glVertex3d( 0.0, 0.0, 0.0 ); glVertex3d( 1.0, 0.0, 0.0 );
        glVertex3d( 1.0, 0.0, 1.0 ); glVertex3d( 0.0, 0.0, 1.0 );
        
        glNormal3d( -1.0, 0.0, 0.0 );
        glVertex3d( 0.0, 0.0, 0.0 ); glVertex3d( 0.0, 0.0, 1.0 );
        glVertex3d( 0.0, 1.0, 1.0 ); glVertex3d( 0.0, 1.0, 0.0 );
        
        glNormal3d( 0.0, 0.0, 1.0 );
        glVertex3d( 0.0, 0.0, 1.0 ); glVertex3d( 1.0, 0.0, 1.0 );
        glVertex3d( 1.0, 1.0, 1.0 ); glVertex3d( 0.0, 1.0, 1.0 );
        
        glNormal3d( 0.0, 1.0, 0.0 );
        glVertex3d( 0.0, 1.0, 0.0 ); glVertex3d( 0.0, 1.0, 1.0 );
        glVertex3d( 1.0, 1.0, 1.0 ); glVertex3d( 1.0, 1.0, 0.0 );
        
        glNormal3d( 1.0, 0.0, 0.0 );
        glVertex3d( 1.0, 0.0, 0.0 ); glVertex3d( 1.0, 1.0, 0.0 );
        glVertex3d( 1.0, 1.0, 1.0 ); glVertex3d( 1.0, 0.0, 1.0 );
        
        glEnd();
        
        /* restore the model matrix stack, and switch back to the matrix
        mode we were in. */
        glPopMatrix();
        glMatrixMode( savemode );
    }
}

void drawTextureBox( double x, double y, double z )
{
    // NOT IMPLEMENTED, SORRY (ehsu)
}

void drawCylinder( double h, double r1, double r2 )
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    int divisions;

	_setupOpenGl();
    
    switch(mds->m_quality)
    {
    case HIGH: 
        divisions = 32; break;
    case MEDIUM: 
        divisions = 20; break;
    case LOW:
        divisions = 12; break;
    case POOR:
        divisions = 8; break;
    }
    
    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile, 
            "cone { height=%f; bottom_radius=%f; top_radius=%f;\n", h, r1, r2 );
        _dump_current_material();
        fprintf(mds->m_rayFile, "})\n" );
    }
    else
    {
        GLUquadricObj* gluq;

        /* GLU will again do the work.  draw the sides of the cylinder. */
        gluq = gluNewQuadric();
        gluQuadricDrawStyle( gluq, GLU_FILL );
        gluQuadricTexture( gluq, GL_TRUE );
        gluCylinder( gluq, r1, r2, h, divisions, divisions);
        gluDeleteQuadric( gluq );

        if ( r1 > 0.0 )
        {
        /* if the r1 end does not come to a point, draw a flat disk to
            cover it up. */
            
            gluq = gluNewQuadric();
            gluQuadricDrawStyle( gluq, GLU_FILL );
            gluQuadricTexture( gluq, GL_TRUE );
            gluQuadricOrientation( gluq, GLU_INSIDE );
            gluDisk( gluq, 0.0, r1, divisions, divisions);
            gluDeleteQuadric( gluq );
        }
        
        if ( r2 > 0.0 )
        {
        /* if the r2 end does not come to a point, draw a flat disk to
            cover it up. */
            
            /* save the current matrix mode. */	
            int savemode;
            glGetIntegerv( GL_MATRIX_MODE, &savemode );
            
            /* translate the origin to the other end of the cylinder. */
            glMatrixMode( GL_MODELVIEW );
            glPushMatrix();
            glTranslated( 0.0, 0.0, h );
            
            /* draw a disk centered at the new origin. */
            gluq = gluNewQuadric();
            gluQuadricDrawStyle( gluq, GLU_FILL );
            gluQuadricTexture( gluq, GL_TRUE );
            gluQuadricOrientation( gluq, GLU_OUTSIDE );
            gluDisk( gluq, 0.0, r2, divisions, divisions);
            gluDeleteQuadric( gluq );
            
            /* restore the matrix stack and mode. */
            glPopMatrix();
            glMatrixMode( savemode );
        }
    }
    
}
// 椭圆柱体 a/b 长轴短轴的比例
void drawEllipticCylinder( double h, double r1, double r2, double a, double b )
{
    ModelerDrawState *mds = ModelerDrawState::Instance();
    int divisions;

	_setupOpenGl();
    
    switch(mds->m_quality)
    {
    case HIGH: 
        divisions = 32; break;
    case MEDIUM: 
        divisions = 20; break;
    case LOW:
        divisions = 12; break;
    case POOR:
        divisions = 8; break;
    }
    
    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile, 
            "cone { height=%f; bottom_radius=%f; top_radius=%f;\n", h, r1, r2 );
        _dump_current_material();
        fprintf(mds->m_rayFile, "})\n" );
    }
    else
    {
		glPushMatrix();

        GLUquadricObj* gluq;
        
        /* GLU will again do the work.  draw the sides of the cylinder. */
        gluq = gluNewQuadric();
        gluQuadricDrawStyle( gluq, GLU_FILL );
        gluQuadricTexture( gluq, GL_TRUE );
	
		glScaled( 1, b/a, 1 );
        gluCylinder( gluq, r1, r2, h, divisions, divisions);

        gluDeleteQuadric( gluq );
        
        if ( r1 > 0.0 )
        {
        /* if the r1 end does not come to a point, draw a flat disk to
            cover it up. */
            
            gluq = gluNewQuadric();
            gluQuadricDrawStyle( gluq, GLU_FILL );
            gluQuadricTexture( gluq, GL_TRUE );
            gluQuadricOrientation( gluq, GLU_INSIDE );
            gluDisk( gluq, 0.0, r1, divisions, divisions);
            gluDeleteQuadric( gluq );
        }
        
        if ( r2 > 0.0 )
        {
        /* if the r2 end does not come to a point, draw a flat disk to
            cover it up. */
            
            /* save the current matrix mode. */	
            int savemode;
            glGetIntegerv( GL_MATRIX_MODE, &savemode );
            
            /* translate the origin to the other end of the cylinder. */
            glMatrixMode( GL_MODELVIEW );
            glPushMatrix();
            glTranslated( 0.0, 0.0, h );
            
            /* draw a disk centered at the new origin. */
            gluq = gluNewQuadric();
            gluQuadricDrawStyle( gluq, GLU_FILL );
            gluQuadricTexture( gluq, GL_TRUE );
            gluQuadricOrientation( gluq, GLU_OUTSIDE );
            gluDisk( gluq, 0.0, r2, divisions, divisions);
            gluDeleteQuadric( gluq );
            
            /* restore the matrix stack and mode. */
            glPopMatrix();
            glMatrixMode( savemode );
        }
		glPopMatrix();
    }
    
}

// 椭球体 a/b 长轴短轴的比例
void drawEllipsoid(double r, double a, double b)
{
	ModelerDrawState *mds = ModelerDrawState::Instance();

	_setupOpenGl();
    
    if (mds->m_rayFile)
    {
        _dump_current_modelview();
        fprintf(mds->m_rayFile, "scale(%f,%f,%f,sphere {\n", r, r, r );
        _dump_current_material();
        fprintf(mds->m_rayFile, "}))\n" );
    }
    else
    {
        int divisions; 
        GLUquadricObj* gluq;
        
        switch(mds->m_quality)
        {
        case HIGH: 
            divisions = 32; break;
        case MEDIUM: 
            divisions = 20; break;
        case LOW:
            divisions = 12; break;
        case POOR:
            divisions = 8; break;
        }
        
		glPushMatrix();
		glScaled( 1, b/a, 1 );
        gluq = gluNewQuadric();
        gluQuadricDrawStyle( gluq, GLU_FILL );
        gluQuadricTexture( gluq, GL_TRUE );
        gluSphere(gluq, r, divisions, divisions);
        gluDeleteQuadric( gluq );
		glPopMatrix();
    }
}







// 手
void drawHand(int isLeftHand)
{
	int type;
	if(isLeftHand)
	{
		type = -1;
	}
	else type = 1;
	glPushMatrix();
	glScaled(0.85, 1.0, 1.0);
	drawEllipsoid(0.3, 1.0, 0.6);	//手掌
	{
		if(type == -1 && VAL(WEAPONSWITCH) >= 2)
		{
			//左手持盾
			glPushMatrix();
			{
				glRotated(-175, 1.0, 0.0, 0.0);
				glTranslated(0.3, 2.3, -2.4);
				glRotated(-127, 0.0, 0.0, 1.0);
				drawShield(4.5, 2.5, 145);
				setDiffuseColor(COLOR_ARM);
				{
					glPushMatrix();
					glTranslated(-0.55, -0.25, -0.5);
					glRotated(-91, 1.0, 0.0, 0.0);
					glRotated(-34, 0.0, 1.0, 0.0);
					glRotated(91, 0.0, 0.0, 1.0);
					glTranslated(-4.1, -3.05, -0.85);
					
					drawBox(2.35, 0.05, 1.4);
					{
						glPushMatrix();
						glTranslated(1.175, 0.1, 0.7);
						GLdouble eqn[4]={0.0, -1.0, 0.0, 0.0};
						glClipPlane(GL_CLIP_PLANE0,eqn);
						glEnable(GL_CLIP_PLANE0);
						drawSphere(0.5);
						glDisable(GL_CLIP_PLANE0);
						{
							glPushMatrix();
							glTranslated(0.0, 0.3, -1.0);
							setDiffuseColor(COLOR_WOOD);
							drawCylinder(1.65, 0.1, 0.1);
							glPopMatrix();
						}
						glPopMatrix();
					}
				}
				glPopMatrix();
			}
			setDiffuseColor(COLOR_SKIN);
			glPopMatrix();
		}
		//拇指
		glPushMatrix();
		if(type == 1)
			glRotated(70 + VAL(IORIGHTTHUMB), 0.0, 1.0, 0.0);
		else
			glRotated(70 + VAL(IOLEFTTHUMB), 0.0, -1.0, 0.0);
		glTranslated(0.0, 0.0, 0.3);
		drawSphere(0.07);
		if(type == 1)
			glRotated(VAL(CURLRIGHTTHUMB), 1.0, -0.5, 0.0);	//Curl Thumb
		else
			glRotated(VAL(CURLLEFTTHUMB), 1.0, -0.5, 0.0);
		drawCylinder(0.13, 0.07, 0.06);	//拇指根
		glTranslated(0.0, 0.0, 0.13);
		drawSphere(0.06);
		if(type == 1)
			glRotated(VAL(CURLRIGHTTHUMB), 1.0, -0.5, 0.0);	//Curl Thumb
		else
			glRotated(VAL(CURLLEFTTHUMB), 1.0, -0.5, 0.0);
		drawCylinder(0.14, 0.06, 0.04);	//拇指头
		glTranslated(0.0, 0.0, 0.14);
		drawSphere(0.04);				//拇指尖
		glPopMatrix();

		//食指
		glPushMatrix();
		//glRotated(50, 0.0, 1.0, 0.0);
		glTranslated(type*0.19, 0.0, 0.20);
		drawSphere(0.07);
		if(type == 1)
			glRotated(VAL(CURLRIGHTFINGERS), 1.0, 0.0, 0.0);	//Curl Fingers
		else
			glRotated(VAL(CURLLEFTFINGERS), 1.0, 0.0, 0.0);
		drawCylinder(0.17, 0.07, 0.06);	//食指根
		glTranslated(0.0, 0.0, 0.17);
		drawSphere(0.06);
		if(type == 1)
			glRotated(VAL(CURLRIGHTFINGERS), 1.0, 0.0, 0.0);	//Curl Fingers
		else
			glRotated(VAL(CURLLEFTFINGERS), 1.0, 0.0, 0.0);
		drawCylinder(0.15, 0.06, 0.04); //食指头
		glTranslated(0.0, 0.0, 0.15);
		drawSphere(0.04);				//食指尖
		glPopMatrix();

		//中指
		glPushMatrix();
		//glRotated(0, 0.0, 1.0, 0.0);
		glTranslated(type*0.05, 0.0, 0.25);
		drawSphere(0.07);
		if(type == 1)
			glRotated(VAL(CURLRIGHTFINGERS), 1.0, 0.0, 0.0);	//Curl Fingers
		else
			glRotated(VAL(CURLLEFTFINGERS), 1.0, 0.0, 0.0);
		drawCylinder(0.18, 0.07, 0.06);	//中指根
		glTranslated(0.0, 0.0, 0.18);
		drawSphere(0.06);
		if(type == 1)
			glRotated(VAL(CURLRIGHTFINGERS), 1.0, 0.0, 0.0);	//Curl Fingers
		else
			glRotated(VAL(CURLLEFTFINGERS), 1.0, 0.0, 0.0);
		drawCylinder(0.15, 0.06, 0.04); //中指头
		glTranslated(0.0, 0.0, 0.15);
		drawSphere(0.04);				//中指尖
		glPopMatrix();

		//无名指
		glPushMatrix();
		//glRotated(-15, 0.0, 1.0, 0.0);
		glTranslated(type*(-0.09), 0.0, 0.22);
		drawSphere(0.07);
		if(type == 1)
			glRotated(VAL(CURLRIGHTFINGERS), 1.0, 0.0, 0.0);	//Curl Fingers
		else
			glRotated(VAL(CURLLEFTFINGERS), 1.0, 0.0, 0.0);
		drawCylinder(0.18, 0.07, 0.06);	//无名指根
		glTranslated(0.0, 0.0, 0.18);
		drawSphere(0.06);
		if(type == 1)
			glRotated(VAL(CURLRIGHTFINGERS), 1.0, 0.0, 0.0);	//Curl Fingers
		else
			glRotated(VAL(CURLLEFTFINGERS), 1.0, 0.0, 0.0);
		drawCylinder(0.15, 0.06, 0.04); //无名指头
		glTranslated(0.0, 0.0, 0.15);
		drawSphere(0.04);				//无名指尖
		glPopMatrix();

		//小指
		glPushMatrix();
		glRotated(type*(-5), 0.0, 1.0, 0.0);
		glTranslated(type*(-0.20), 0.0, 0.20);
		drawSphere(0.07);
		if(type == 1)
			glRotated(VAL(CURLRIGHTFINGERS), 1.0, 0.0, 0.0);	//Curl Fingers
		else
			glRotated(VAL(CURLLEFTFINGERS), 1.0, 0.0, 0.0);
		drawCylinder(0.17, 0.07, 0.06);	//小指根
		glTranslated(0.0, 0.0, 0.17);
		drawSphere(0.06);
		if(type == 1)
			glRotated(VAL(CURLRIGHTFINGERS), 1.0, 0.0, 0.0);	//Curl Fingers
		else
			glRotated(VAL(CURLLEFTFINGERS), 1.0, 0.0, 0.0);
		drawCylinder(0.14, 0.06, 0.04); //小指头
		glTranslated(0.0, 0.0, 0.14);
		drawSphere(0.04);				//小指尖
		glPopMatrix();

	}
	glPopMatrix();
}







// 盾牌
void drawShield(double h, double r, double angle)
{
	ModelerDrawState *mds = ModelerDrawState::Instance();
    int divisions;

	_setupOpenGl();
    
    switch(mds->m_quality)
    {
    case HIGH: 
        divisions = 32; break;
    case MEDIUM: 
        divisions = 20; break;
    case LOW:
        divisions = 12; break;
    case POOR:
        divisions = 8; break;
    }
    
    if (mds->m_rayFile)
    {
		/*
        _dump_current_modelview();
        fprintf(mds->m_rayFile, 
            "cone { height=%f; bottom_radius=%f; top_radius=%f;\n", h, r1, r2 );
        _dump_current_material();
        fprintf(mds->m_rayFile, "})\n" ); */
    }
    else
    {	
		/* remember which matrix mode OpenGL was in. */
        int savemode;
        glGetIntegerv( GL_MATRIX_MODE, &savemode );
        
        /* switch to the model matrix and scale by x,y,z. */
        glMatrixMode( GL_MODELVIEW );
		
        glPushMatrix();

		//glRotated(VAL(DEBUG_XROTATE), 1.0, 0.0, 0.0);
		//glRotated(VAL(DEBUG_YROTATE), 0.0, 1.0, 0.0);
		
        glBegin( GL_QUAD_STRIP );

        int i; double x, y, lastX, lastY, theta, a, b, c, d, e, f;
		setDiffuseColor(COLOR_RED);
		
		divisions *= 2;
		for(i = 0; i < divisions; i += 2)								//正面
		{
			theta = (angle / 360.0 * M_PI) * (i * 1.0 / divisions);
			x = r * cos(theta); y = r * sin(theta);
			if(i)
			{
				/* calculate the normal */
				a = 0;b = 0;c = h;d = lastX - x;e = lastY - y;f = 0;
				glNormal3d( b*f - c*e, c*d - a*f, a*e - b*d );
			}
			lastX = x, lastY = y;
			glVertex3d(x, y, h * (i % 2)); glVertex3d(x, y, h * ((i + 1) % 2));
		}
		r -= 0.1;
		for(i = divisions - 2; i >= 0; i -= 2)							//反面
		{
			theta = (angle / 360.0 * M_PI) * (i * 1.0 / divisions);
			x = r * cos(theta); y = r * sin(theta);
			if(i)
			{
				/* calculate the normal */
				a = 0;
				b = 0;
				c = h;
	        
				d = lastX - x;
				e = lastY - y;
				f = 0;
	        
				glNormal3d( b*f - c*e, c*d - a*f, a*e - b*d );
			}
			lastX = x, lastY = y;
			glVertex3d(x, y, h * (i % 2)); glVertex3d(x, y, h * ((i + 1) % 2)); 
			if(i == divisions - 2)
				setDiffuseColor(COLOR_WOOD);
		}
		setDiffuseColor(COLOR_RED);
		r += 0.1;
		for(i = 0; i < 1; i ++)											//给侧边封口
		{
			theta = (angle / 360.0 * M_PI) * (i * 1.0 / divisions);
			x = r * cos(theta); y = r * sin(theta);
			a = 0;				/* calculate the normal */
			b = 0;
			c = h;
			d = lastX - x;
			e = lastY - y;
			f = 0;
			glNormal3d( b*f - c*e, c*d - a*f, a*e - b*d );
			glVertex3d(x, y, h * (i % 2)); glVertex3d(x, y, h * ((i + 1) % 2));
		}
        glEnd();
		
		//绘制顶边和底边
		setDiffuseColor(COLOR_RED);
		divisions /= 2;
		double x2, y2, lastX2, lastY2;
		for(i = 0; i < divisions; i ++)								//正面
		{
			theta = (angle / 360.0 * M_PI) * (i * 1.0 / divisions);
			x = r * cos(theta); y = r * sin(theta);
			x2 = (r-0.1) * cos(theta); y2 = (r-0.1) * sin(theta);
			if(i)
			{
				drawTriangle(lastX2, lastY2, 0, x2, y2, 0, x, y, 0);
				drawTriangle(lastX2, lastY2, 0, lastX, lastY, 0, x, y, 0);
				drawTriangle(lastX2, lastY2, h, lastX, lastY, h, x, y, h);
				drawTriangle(lastX2, lastY2, h, x2, y2, h, x, y, h);
			}
			lastX = x, lastY = y, lastX2 = x2, lastY2 = y2;
		}
		setDiffuseColor(COLOR_SKIN);
		glPopMatrix();
	}
}


void drawSword()
{
	ModelerDrawState *mds = ModelerDrawState::Instance();
    int divisions;

	_setupOpenGl();
    
    switch(mds->m_quality)
    {
    case HIGH: 
        divisions = 32; break;
    case MEDIUM: 
        divisions = 20; break;
    case LOW:
        divisions = 12; break;
    case POOR:
        divisions = 8; break;
    }
    
    if (mds->m_rayFile)
    {
		/*
        _dump_current_modelview();
        fprintf(mds->m_rayFile, 
            "cone { height=%f; bottom_radius=%f; top_radius=%f;\n", h, r1, r2 );
        _dump_current_material();
        fprintf(mds->m_rayFile, "})\n" ); */
    }
    else
    {	
		/* remember which matrix mode OpenGL was in. */
        int savemode;
        glGetIntegerv( GL_MATRIX_MODE, &savemode );
        
        /* switch to the model matrix and scale by x,y,z. */
        glMatrixMode( GL_MODELVIEW );
		
		
        glPushMatrix();
		setDiffuseColor(COLOR_WOOD);
		//剑柄
		{	//剑首
			glPushMatrix();
			glScaled(0.7, 1., 1.0);
			drawSphere(0.2);
			glPopMatrix();
		}
		{	//剑把
			glPushMatrix();
			glRotated(90, 1.0, 0.0, 0.0);
			drawCylinder(0.9, 0.1, 0.1);
			glPopMatrix();
		}
		{	//剑格
			glPushMatrix();
			glScaled(0.6, 1.0, 1.0);
			glTranslated(0.0, -0.9, 0.0);
			glRotated(90, 1.0, 0.0, 0.0);
			drawCylinder(0.3, 0.2, 0.3);
			glPopMatrix();
		}
		{//剑身
			
			glPushMatrix();
			setDiffuseColor(COLOR_WEAPON);
			glTranslated(0.0, -3.8, 0.0);
			glRotated(-90, 1.0, 0.0, 0.0);
			//剑刃
			drawTriangle(0.0, 0.0, 0.0,  0.1, 0.0, 0.7,  0.0, 0.3, 0.7);
			drawTriangle(0.0, 0.0, 0.0,  0.0, 0.3, 0.7,  -0.1, 0.0, 0.7);
			drawTriangle(0.0, 0.0, 0.0,  -0.1, 0.0, 0.7,  0.0, -0.3, 0.7);
			drawTriangle(0.0, 0.0, 0.0,  0.0, -0.3, 0.7,  0.1, 0.0, 0.7);
			//剑锋
			drawTriangle(0.1, 0.0, 0.7,  0.08, 0.0, 2.6,  0.0, 0.3, 0.7);
			drawTriangle(0.0, 0.2, 2.6,  0.0, 0.3, 0.7,  0.08, 0.0, 2.6);
			drawTriangle(0.0, 0.2, 2.6,  0.0, 0.3, 0.7,  -0.08, 0.0, 2.6);
			drawTriangle(-0.1, 0.0, 0.7,  -0.08, 0.0, 2.6,  0.0, 0.3, 0.7);
			drawTriangle(-0.1, 0.0, 0.7,  -0.08, 0.0, 2.6,  0.0, -0.3, 0.7);
			drawTriangle(0.0, -0.2, 2.6,  0.0, -0.3, 0.7,  -0.08, 0.0, 2.6);
			drawTriangle(0.0, -0.2, 2.6,  0.0, -0.3, 0.7,  0.08, 0.0, 2.6);
			drawTriangle(0.1, 0.0, 0.7,  0.08, 0.0, 2.6,  0.0, -0.3, 0.7);

			glPopMatrix();
		}
		setDiffuseColor(COLOR_SKIN);
		glPopMatrix();
	}
}
// 头盔
void drawHelmet()
{
	ModelerDrawState *mds = ModelerDrawState::Instance();
    int divisions;

	_setupOpenGl();
    
    switch(mds->m_quality)
    {
    case HIGH: 
        divisions = 32; break;
    case MEDIUM: 
        divisions = 20; break;
    case LOW:
        divisions = 12; break;
    case POOR:
        divisions = 8; break;
    }

	glPushMatrix();
	glTranslated(0.0, 0.0, -0.5);
    setDiffuseColor(COLOR_ARM);
 
	GLdouble eqn[4]={0.0, 1.0, 0.0, 0.45};
	glClipPlane(GL_CLIP_PLANE0,eqn);
	glEnable(GL_CLIP_PLANE0);

    GLUquadricObj* gluq;

    /* GLU will again do the work.  draw the sides of the cylinder. */
    gluq = gluNewQuadric();
    gluQuadricDrawStyle( gluq, GLU_FILL );
    gluQuadricTexture( gluq, GL_TRUE );
    gluCylinder( gluq, 0.6, 0.6, 0.8, divisions, divisions);
    gluDeleteQuadric( gluq );

	glDisable(GL_CLIP_PLANE0);
	glTranslated(0.0, 0.0, 0.6);

	GLdouble eqn1[4]={0.0, 0.0, 1.0, 0.0};
    glClipPlane(GL_CLIP_PLANE0,eqn);
	glEnable(GL_CLIP_PLANE0);
	drawSphere(0.6);
	glDisable(GL_CLIP_PLANE0);

    setDiffuseColor(COLOR_SKIN);
    glPopMatrix();

}
// 头
void drawHead()
{
	
/*
	glPushMatrix();
	glTranslated(-0.5, -0.35, -0.5);
	GLdouble eqn[4]={1.0, 0.0, 0.0, 1.0};
	glClipPlane(GL_CLIP_PLANE0,eqn);
	glEnable(GL_CLIP_PLANE0);
	drawBox(VAL(DEBUG_R1SIZE), VAL(DEBUG_R2SIZE), VAL(DEBUG_HSIZE));
	glDisable(GL_CLIP_PLANE0);
	glPopMatrix();
*/
	unsigned char* texture_head = NULL; // it stores an array of pixels value
	int texture_width = 0; // the width of the texture
	int texture_height = 0; // the height of the texture
	// Read the bitmap
	texture_head = readBMP("facef2.bmp", texture_width, texture_height);

	// Generate The Texture
	glTexImage2D(GL_TEXTURE_2D, 0, 3, texture_width, texture_height, 0, GL_RGB, GL_UNSIGNED_BYTE, texture_head);

	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MIN_FILTER,GL_LINEAR); // Linear Filtering
	glTexParameteri(GL_TEXTURE_2D,GL_TEXTURE_MAG_FILTER,GL_LINEAR); // Linear Filtering

	glEnable(GL_TEXTURE_2D);
	glScaled(0.8, 1.0, 0.8);
	glRotated(90, 1.0, 0.0, 0.0);
	glTranslated(0.0, 0.0, -0.2);
	drawSphere(0.6);
	glDisable(GL_TEXTURE_2D);

	drawHelmet();

	delete [] texture_head;

}
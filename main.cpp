#include <windows.h>
#include <iostream.h>
#include <gl/glut.h>
#include <stdio.h>

#define Up_Daown_Scale 1
#define WHI 600.0f
#define WWI 800.0f 

void PlaySnd(LPSTR lpName); 
void SelectCam(int);
void CamRotationAngle();
void CamZoom();
void SetupRC();

int  CurentCam;
bool blDrawBackGr=false;
bool blDrawHelpSplash=false;
bool blCourtWantGround=false;
bool blim_in=false;
bool blFogy=false;
bool blBilBoardLoad=false;
bool blBilBoardShow=false;
bool blNeedRain=false;

unsigned int GameSpeed = 10;

GLfloat Player1 =		 175.0f;
GLfloat Player2 =		 175.0f;
GLfloat	ballX =			 200.0f;
GLfloat ballY =			 200.0f;

GLfloat ix=0.0;
GLfloat iy=0.0;
GLfloat iz=0.0;
GLfloat RotateFactor=2.0f;
GLfloat Zoom_Scale=5.0;
GLfloat ZoomFact=1.0;
GLubyte DayNight=0;
GLubyte BillAipha=50;
GLdouble Up_Daown_Factor=0;

GLfloat step = 4.0f;
GLfloat balSpX = -4.0f;
GLfloat balSpY = -4.0f;

GLfloat windowWidth;
GLfloat windowHeight;

unsigned char *nbmBilBoard;
unsigned char *nbmGrass;
unsigned char *nbmBits;
int imageWidth, imageHeight;

unsigned char *nLoadTexture(LPCTSTR fileName){

		BITMAP BM;
		
		// Load the bitmap.
        HBITMAP hBmp = (HBITMAP) LoadImage (NULL,(LPCTSTR) fileName, 
								IMAGE_BITMAP, 0, 0, 
								LR_LOADFROMFILE | LR_CREATEDIBSECTION);

        if (hBmp == NULL)
        {
          printf("Error: unable to load bitmap file\n");
          return FALSE;
        }

        GetObject(hBmp, sizeof (BM), &BM);

        if (BM.bmBitsPixel != 24)
        {
          printf("Error: bitmap must be 24 bit\n");
          return FALSE;
        }

		imageHeight=BM.bmHeight;
		imageWidth=BM.bmWidth;

        return (unsigned char*)BM.bmBits;
}

int TextureMapper (char *name, int id)
{
    unsigned char *imageData = NULL;
    imageData =nLoadTexture(name);
	
	glBindTexture (GL_TEXTURE_2D, id);
    glPixelStorei (GL_UNPACK_ALIGNMENT, 1);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
    /* glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST); */
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    /* glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST); */
    glTexParameteri (GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexEnvf (GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
    glTexImage2D (GL_TEXTURE_2D, 0, GL_RGB, imageWidth, imageHeight, 0, GL_BGR_EXT, GL_UNSIGNED_BYTE, imageData);
	
    return 1;
}


void DrawPanels(GLfloat SpaceToCourt,GLfloat Height,GLfloat zPlace) {

		glEnable (GL_TEXTURE_2D); 
		glBindTexture (GL_TEXTURE_2D, 13); //Bind to our texture
		glColor3ub(255,255,255); 
			
	glBegin(GL_QUADS);
		glTexCoord2f (0.0f,0.0f);  //Lower Left	
		glVertex3f(0,windowHeight-SpaceToCourt,zPlace);
		glTexCoord2f (0.0f, 1.0f); //Upper Left
		glVertex3f(0,windowHeight-SpaceToCourt,Height+zPlace);
		glTexCoord2f (1.0f, 1.0f); //Upper Right
		glVertex3f(windowWidth,windowHeight-SpaceToCourt,Height+zPlace);
		glTexCoord2f (1.0f, 0.0f); //Lower Right	
		glVertex3f(windowWidth,windowHeight-SpaceToCourt,zPlace);
		
		glTexCoord2f (0.0f,0.0f);  //Lower Left	
		glVertex3f(windowWidth-SpaceToCourt,windowHeight,zPlace);
		glTexCoord2f (0.0f, 1.0f); //Upper Left
		glVertex3f(windowWidth-SpaceToCourt,windowHeight,Height+zPlace);
		glTexCoord2f (1.0f, 1.0f); //Upper Right
		glVertex3f(windowWidth-SpaceToCourt,0,Height+zPlace);
		glTexCoord2f (1.0f, 0.0f); //Lower Right	
		glVertex3f(windowWidth-SpaceToCourt,0,zPlace);

		glTexCoord2f (0.0f,0.0f);  //Lower Left	
		glVertex3f(SpaceToCourt,0,zPlace);
		glTexCoord2f (0.0f, 1.0f); //Upper Left
		glVertex3f(SpaceToCourt,0,Height+zPlace);
		glTexCoord2f (1.0f, 1.0f); //Upper Right
		glVertex3f(SpaceToCourt,windowHeight,Height+zPlace);
		glTexCoord2f (1.0f, 0.0f); //Lower Right	
		glVertex3f(SpaceToCourt,windowHeight,zPlace);
		
		glTexCoord2f (0.0f,0.0f);  //Lower Left
		glVertex3f(0,SpaceToCourt,zPlace);
		glTexCoord2f (0.0f, 1.0f); //Upper Left
		glVertex3f(0,SpaceToCourt,Height+zPlace);
		glTexCoord2f (1.0f, 1.0f); //Upper Right
		glVertex3f(windowWidth+SpaceToCourt,SpaceToCourt,Height+zPlace);
		glTexCoord2f (1.0f, 0.0f); //Lower Right
		glVertex3f(windowWidth+SpaceToCourt,SpaceToCourt,zPlace);

	glEnd();

		glDisable (GL_TEXTURE_2D); 

}

void Foger(bool Fog_UnFog){

	if (Fog_UnFog){
		GLfloat color[4]={0.90,0.90,0.90,0.50};
		glEnable(GL_FOG);
		glFogf(GL_FOG_START, 0.00);
		glFogf(GL_FOG_END, 1600);
		glFogi(GL_FOG_MODE,GL_LINEAR );
		glFogfv(GL_FOG_COLOR, color);
	}
	else
		glDisable(GL_FOG);


}

void CamRotationAngle(GLfloat xAx,GLfloat yAx,GLfloat zAx)
{
	glRotatef(zAx,0.0,0.0,1.0);
	glRotatef(yAx,0.0,1.0,0.0);
	glRotatef(xAx,1.0,0.0,0.0);
}

void Sky(){
		glEnable (GL_TEXTURE_2D); 
		glBindTexture (GL_TEXTURE_2D, 11); //Bind to our texture
		glColor3ub(255,255,255); 
		int SkyZPlace=500;
	glBegin(GL_QUADS);
		glTexCoord2f (0.0f,0.0f);  //Lower Left		
		glVertex3i(-windowWidth,-windowHeight,SkyZPlace); 
		glTexCoord2f (0.0f, 1.0f); //Upper Left
		glVertex3i(-windowWidth, windowHeight*2,SkyZPlace); 
		glTexCoord2f (1.0f, 1.0f); //Upper Right
		glVertex3i(windowWidth*2, windowHeight*2,SkyZPlace); 
		glTexCoord2f (1.0f, 0.0f); //Lower Right	
		glVertex3i(windowWidth*2, -windowHeight,SkyZPlace);
	glEnd();

	    glBindTexture (GL_TEXTURE_2D, 10); //Bind to our texture
		glColor3ub(255,255,255); 

	glBegin(GL_QUADS);
		glTexCoord2f (0.0f,0.0f);  //Lower Left		
		glVertex3i(-windowWidth,-windowHeight,0); 
		glTexCoord2f (0.0f, 1.0f); //Upper Left
		glVertex3i(-windowWidth, -windowHeight,SkyZPlace); 
		glTexCoord2f (1.0f, 1.0f); //Upper Right
		glVertex3i(windowWidth*2, -windowHeight,SkyZPlace); 
		glTexCoord2f (1.0f, 0.0f); //Lower Right	
		glVertex3i(windowWidth*2, -windowHeight,0);

		glTexCoord2f (0.0f,0.0f);  //Lower Left		
		glVertex3i(-windowWidth,-windowHeight,0); 
		glTexCoord2f (0.0f, 1.0f); //Upper Left
		glVertex3i(-windowWidth, -windowHeight,SkyZPlace); 
		glTexCoord2f (1.0f, 1.0f); //Upper Right
		glVertex3i(-windowWidth, windowHeight*2,SkyZPlace); 
		glTexCoord2f (1.0f, 0.0f); //Lower Right	
		glVertex3i(-windowWidth, windowHeight*2,0);

		glTexCoord2f (0.0f,0.0f);  //Lower Left		
		glVertex3i(-windowWidth,windowHeight*2,0); 
		glTexCoord2f (0.0f, 1.0f); //Upper Left
		glVertex3i(-windowWidth, windowHeight*2,SkyZPlace); 
		glTexCoord2f (1.0f, 1.0f); //Upper Right
		glVertex3i(windowWidth*2, windowHeight*2,SkyZPlace); 
		glTexCoord2f (1.0f, 0.0f); //Lower Right	
		glVertex3i(windowWidth*2, windowHeight*2,0);
	
		glTexCoord2f (0.0f,0.0f);  //Lower Left		
		glVertex3i(windowWidth*2,windowHeight*2,0); 
		glTexCoord2f (0.0f, 1.0f); //Upper Left
		glVertex3i(windowWidth*2, windowHeight*2,SkyZPlace); 
		glTexCoord2f (1.0f, 1.0f); //Upper Right
		glVertex3i(windowWidth*2, -windowHeight,SkyZPlace); 
		glTexCoord2f (1.0f, 0.0f); //Lower Right	
		glVertex3i(windowWidth*2, -windowHeight,0);
	glEnd();


	glDisable (GL_TEXTURE_2D); 
}

void SelectCam(int iCam)
{

		if (blDrawBackGr || blDrawHelpSplash)
   			glDrawPixels(WWI,WHI,GL_BGR_EXT,GL_UNSIGNED_BYTE,nbmBits);

	switch(iCam)
	{
	case 0: //Left1
		gluLookAt(windowWidth/2,ZoomFact,150,windowWidth/2,windowWidth/2,150,0.0,0.0,1.0);
		break;
	case 1: //Left2
		gluLookAt(windowWidth/2,ZoomFact,150,ballX,ballY,0,0.0,0.0,1.0);
		break;
	case 2: //Top1
		gluLookAt(ballX,ballY,200-ZoomFact,ballX,ballY,0.0,0.0,1.0,0.0);
		break;
	case 3: //Corner
		gluLookAt(ZoomFact,ZoomFact,100,WWI,WHI,100,0.0,0.0,1.0);
		break;
	case 4: //Top2
		gluLookAt(windowWidth/2,windowHeight/2,500-ZoomFact,(windowWidth/2)-1,windowHeight/2,0,0.0,1.0,0.0);
		break;
	case 5: //Op1
		gluLookAt(-100,Player1,400.0-ZoomFact,180,Player1,0.0,1.0,0.0,0.0);
		break;
	case 6: //Free Cam1
		gluLookAt(ZoomFact-100,ZoomFact-100,100,WWI,WHI,100,0.0,0.0,1.0);
		break;
	case 7: //Left3
		gluLookAt(windowWidth/2,ZoomFact,ballY,ballX,ballY+10,0,0.0,1.0,0.0);
		break;
	case 8: //Op2
		gluLookAt(-100,Player1,400.0-ZoomFact,180,Player1,0.0,0.0,1.0,0.5);
		break;
	case 9: //Free Cam2
		gluLookAt(ballX-130,ballY,50,ballX,ballY,50,0.0,0.0,1.0);
		break;
	}

}

void PlaySnd(LPSTR lpName) 
{ 
    PlaySound(lpName, NULL,SND_ASYNC);
}
 
void TheRain(bool I_Love_Rain)
{
	if (I_Love_Rain)
	{	
		int x=0,y=0,z=0,RainTail=5;
		glColor4ub(255,255,255,60);
		for (int i=0; i<700;i++)
		{
			x=rand()/25;
			y=rand()/50;
			z=rand()/100;
			glBegin(GL_QUADS);
			glVertex3d(x, y,z);
			glVertex3d(x, y+1,z);
			glVertex3d(x+RainTail,y+1,z+RainTail);
			glVertex3d(x+RainTail, y,z+RainTail);
			glEnd();
		}
	}
}

void RenderScene()
{
	
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		glPushMatrix();
		CamRotationAngle(ix,iy,iz);
		
		SelectCam(CurentCam);

	if (!blDrawHelpSplash){	
		Sky();
		//World Ground Area
		//glColor3ub(200,55,0);
		glColor3ub(108,42,40);
		glRectf(-windowWidth,-windowHeight, windowWidth*2, windowHeight*2);
		
		if(blCourtWantGround==true) {
			glEnable (GL_TEXTURE_2D); 
			glBindTexture (GL_TEXTURE_2D, 12); //Bind to our texture
			glColor4ub(255,255,255,255); 
			
			glBegin(GL_QUADS); 
				glTexCoord2f (0.0f,0.0f);  //Lower Left
				glVertex2f(0.0f,0.0f); 
				glTexCoord2f (1.0f, 0.0f); //Lower Right
				glVertex2f(0.0f, windowHeight);
				glTexCoord2f (1.0f, 1.0f); //Upper Right
				glVertex2f(windowWidth, windowHeight); 
				glTexCoord2f (0.0f, 1.0f); //Upper Left
				glVertex2f(windowWidth, 0.0f);
			glEnd(); 
			
			glDisable (GL_TEXTURE_2D); 
		}
		else{
			//Court grasses
			glColor3b(0,55,0);
			glRectf(0, 0, windowWidth, windowHeight);
		}

		//Court Lines
		glColor3d(255,255,255);
		glRectf(0, 0, windowWidth,10);  //_
		glRectf(0,windowHeight-10, windowWidth,windowHeight); //-
		glRectf(0,0,10,windowHeight); //| 0
		glRectf(windowWidth,0,windowWidth-10,windowHeight); //0 |
		glRectf(windowWidth/2-5,10,windowWidth/2+5,windowHeight-10); //0 | 0
		
		
		// Player1 paddle
		glColor3ub(100, 100, 255);
		glRectf(10, Player1, 20, Player1 + 50);
		
		// Player2 paddle
		glColor3ub(255,200,0);
		glRectf(windowWidth - 20, Player2, windowWidth - 10, Player2 + 50);
		
		// Ball
		glColor3ub(255, 255, 255);
		glRectf(ballX, ballY, ballX + 10, ballY + 10);
		
		// Simple Night / Day
		glBegin(GL_QUADS); 
			glColor4ub(0,0,0,DayNight); 
			glVertex2f(0.0f,0.0f); 
			glVertex2f(0.0f, windowHeight); 
			glVertex2f(windowWidth, windowHeight); 
			glVertex2f(windowWidth, 0.0f);
			// Or front page
			/*glVertex3f(0.0f,ZoomFact,0.0f); 
			glVertex3f(0.0f,ZoomFact,windowHeight); 
			glVertex3f(windowWidth,ZoomFact, windowHeight); 
			glVertex3f(windowWidth,ZoomFact,0.0f);*/
		glEnd(); 
		
		DrawPanels(-50,30,0);

			if (blBilBoardShow){
				long Lbit=0;
				//Drawing the bill board. "it is just for learning"
				for (int i=50;i<50+240;i++)
				{
					glBegin(GL_QUADS);
					for(int j=windowWidth/2-160;j<int(windowWidth/2)+160;j++)
					{
						glColor4ub(nbmBilBoard[Lbit+2],nbmBilBoard[Lbit+1],nbmBilBoard[Lbit],BillAipha);
						glVertex3i(j,300,i);
						glVertex3i(j,300,i+1);
						glVertex3i(j+1,300,i+1);
						glVertex3i(j+1,300,i);
						Lbit+=3;
					}
					glEnd();
				}
			
			}
			
			TheRain(blNeedRain);
	}	
		glPopMatrix();
	
	glutSwapBuffers();
	
}


void TimerFunction(int value)
{
	if(GetAsyncKeyState(VK_SUBTRACT)) // Subtract key lower game speed
		GameSpeed=10;
		
	if(GetAsyncKeyState(VK_ADD)) // Plus key lower game speed
			GameSpeed=1;
	
	if(GetAsyncKeyState (VK_F1)) // Day ON, Night OFF
	{
		glClearColor(0.7f, 0.7f, 0.7f, 1.0f);
		DayNight=0;
	}
	
	if(GetAsyncKeyState (VK_F2)) // Day OFF, Night ON
	{
		glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
		DayNight=100;
	}

	if(GetAsyncKeyState(VK_F3)) // Camera Left1 case 0 of void SelectCam(int iCam)
	{
		ZoomFact=1.0; //Reset zoom factor (Min 1, Max 50)
		CurentCam=0;  //Select Camera value  "void SelectCam(int iCam)"
		ix=0.0;		  //Resets X axis to default position
		iy=0.0;		  //Reset Y axis
		iz=0.0;		  //Reset Z axis
	}

	if(GetAsyncKeyState(VK_F4))	// Camera case 1 of void ...
	{
		ZoomFact=1.0;
		CurentCam=1;
		ix=0.0;
		iy=0.0;
		iz=0.0;
	}

	if(GetAsyncKeyState(VK_F5))	// Camera case 2 of ...
	{
		ZoomFact=1.0;
		CurentCam=2;
		ix=0.0;
		iy=0.0;
		iz=0.0;
	}

	if(GetAsyncKeyState(VK_F6))	// Camera case 3 of ...
	{
		ZoomFact=1.0;
		CurentCam=3;
		ix=0.0;
		iy=0.0;
		iz=0.0;
	}

	if(GetAsyncKeyState(VK_F7))	// Camera case 4 ...
	{
		ZoomFact=1.0;
		CurentCam=4;
		ix=0.0;
		iy=0.0;
		iz=0.0;
	}

	if(GetAsyncKeyState(VK_F8))	// Camera case 5 ...
	{
		ZoomFact=1.0;
		CurentCam=5;
		ix=0.0;
		iy=0.0;
		iz=0.0;
	}
	if(GetAsyncKeyState(VK_F9))	// Camera case 6 ...
	{
		ZoomFact=1.0;
		CurentCam=6;
		ix=0.0;
		iy=0.0;
		iz=0.0;
	}
	
	if(GetAsyncKeyState(VK_F10))// Camera case 7 ...
	{
		ZoomFact=1.0;
		CurentCam=7;
		ix=0.0;
		iy=0.0;
		iz=0.0;
	}		
	if(GetAsyncKeyState(VK_F11))// Camera case 8 ...
	{
		ZoomFact=1.0;
		CurentCam=8;
		ix=0.0;
		iy=0.0;
		iz=0.0;
	}			

	if(GetAsyncKeyState(VK_F12))// Camera case 9 ...
	{
		ZoomFact=1.0;
		CurentCam=9;
		ix=0.0;
		iy=0.0;
		iz=0.0;
	}			

	if(GetAsyncKeyState(0x42)) // Drawing help. B key ON, Shift+B OFF 
	{
		if (!GetAsyncKeyState(VK_SHIFT) && !blDrawHelpSplash)
		{
			nbmBits=nLoadTexture(".\\back.bmp");
			blDrawBackGr=true;
		}
		else
			blDrawBackGr=false;
	}


	if(GetAsyncKeyState(0x48)) // H key shows help splash. H key ON, Shift+H OFF
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			{
				nbmBits=nLoadTexture(".\\help.bmp");
				blDrawHelpSplash=true;
			}
		 else
			{
				nbmBits=nLoadTexture(".\\back.bmp");	
				blDrawHelpSplash=false;

			}
	}
	
	if(GetAsyncKeyState(0x50)) // P key shows Billboard. P key ON, Shift+P OFF
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			{
				nbmBilBoard=nLoadTexture(".\\billBoard.bmp");
				blBilBoardShow=true;
			}
		 else
			{
				blBilBoardShow=false;
			}
	}
	
	if(GetAsyncKeyState(0x47)) // G key shows play court and side panels texture. G key ON, Shift+G OFF
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			{
				TextureMapper ("panels.bmp", 13);
				TextureMapper ("floor1.bmp", 12);
				blCourtWantGround=true;
			}
		 else
			{
				blCourtWantGround=false;
			}
	}

	if(GetAsyncKeyState(0x4F))  // O key Billboard Alpha, O key ON, Shift+O OFF
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			{
					
				if (BillAipha<255)
					BillAipha+=1;
			}
		 else
			{
					
				if (BillAipha>1)
					BillAipha-=1;
			}
	}

	if(GetAsyncKeyState(0x46)) // F key fog.  F key ON, Shift+F OFF
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			{
			Foger(true);	
			}
		 else
			{
			 Foger(false);
			}
	}

	if(GetAsyncKeyState(0x54)) // T key Rain.  T key ON, Shift+F OFF
	{
		if (!GetAsyncKeyState(VK_SHIFT))
			{
			blNeedRain=true;	
			}
		 else
			{
			 blNeedRain=false;
			}
	}

	if(GetAsyncKeyState(0x52)) // R key resets all positions 
	{
		ZoomFact=1.0;
		ix=0.0;
		iy=0.0;
		iz=0.0;
	}
	
	if(GetAsyncKeyState(VK_ESCAPE))
	{
		exit(0);
	}

	if(GetAsyncKeyState(VK_LEFT))  //increase Z rotation
	{
			iz+=RotateFactor;
		if (iz>360)
			iz=0;
	}
	
	if(GetAsyncKeyState(VK_RIGHT)) //decrease Z rotation
	{
			iz-=RotateFactor;
		if (iz<0)
			iz=360;
	}
	if(GetAsyncKeyState(VK_UP))    //increase X rotation
	{
		ix+=RotateFactor;
		if (ix>360)
			ix=0;
	}
	
	if(GetAsyncKeyState(VK_DOWN))  //decrease X rotation
	{
			ix-=RotateFactor;
		if (ix<0)
			ix=360;
	}
	

	if(GetAsyncKeyState(VK_HOME))  //increase Y rotation
	{
			iy-=RotateFactor;
		if (iy<0)
			iy=360;
	}
	
	if(GetAsyncKeyState(VK_END))  //decrease Y rotation
	{
			iy+=RotateFactor;
		if (iy>360)
			iy=0;
	}
	
	if(GetAsyncKeyState(VK_RBUTTON)) // Mouse right click zoom -
	{
			ZoomFact-=Zoom_Scale;
		if(ZoomFact==0)
			ZoomFact=1;
	}
	if(GetAsyncKeyState(VK_LBUTTON)) // Mouse left click zoom +
	{
			ZoomFact+=Zoom_Scale;
		if(ZoomFact==0)
			ZoomFact=1;
	}
	
	if(GetAsyncKeyState(VK_PRIOR)) // increase zoom scale (Min 5,Max 50)
	{
			Zoom_Scale+=5.0;
		if (Zoom_Scale>50)
			Zoom_Scale=50.0;
	}

	if(GetAsyncKeyState(VK_NEXT)) // decrease zoom scale
	{		
			Zoom_Scale-=5.0;
		if (Zoom_Scale<5)
			Zoom_Scale=5.0;
	}

	// Ball physics
	if(ballX >= windowWidth-20 && ballX < windowWidth - 10)
	{
		if(ballY >= Player2 && ballY <= Player2 + 50)
		{
			balSpX = -balSpX;
			PlaySnd(".\\shot1.wav");
		}
	}


	if(ballX <= 20 && ballX > 10)
	{
		if(ballY >= Player1 && ballY <= Player1 + 50)
			balSpX = -balSpX;
			PlaySnd(".\\shot1.wav");
	}


	if(ballY > windowHeight || ballY - 5 < 0)
		balSpY = -balSpY;
	
	if(ballY < Player2 + 25)
	{
		Player2 -= step;
	}
	else
	{
		Player2 += step;
	}

	
	if(ballY < Player1 + 25)
	{
		Player1 -= step;
	}
	else
	{
		Player1 += step;
	}

	ballX += balSpX;
	ballY += balSpY;

	glutPostRedisplay();
		
		glutTimerFunc(GameSpeed, TimerFunction, 1);
}


void SetupRC()
{
	glClearColor(0.5f, 0.5f, 0.5f, 1.0f);
//	glEnable(GL_DEPTH_TEST);
//	glEnable(GL_CULL_FACE);
//	glFrontFace(GL_CCW);	

	//glEnable(GL_POLYGON_SMOOTH);
	TextureMapper(".\\sky.bmp",11);
	TextureMapper(".\\wall.bmp",10);
}



void ChangeSize(GLsizei w, GLsizei h)
{
	if(h == 0)
		h = 1;
	
	glViewport(0, 0, w, h);

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	
	if (w<=h)
	{
		windowHeight = WHI*h/w;
		windowWidth = WWI;
	}
	else
	{
		windowHeight = WHI;
		windowWidth = WWI*w/h;
	}

    gluPerspective(45.0,(windowWidth/windowHeight),0.0,-1000);
	
	glMatrixMode(GL_MODELVIEW);
	
	glEnable (GL_BLEND);
/*	#define GL_ZERO                           0
#define GL_ONE                            1
#define GL_SRC_COLOR                      0x0300
#define GL_ONE_MINUS_SRC_COLOR            0x0301
#define GL_SRC_ALPHA                      0x0302
#define GL_ONE_MINUS_SRC_ALPHA            0x0303
#define GL_DST_ALPHA                      0x0304
#define GL_ONE_MINUS_DST_ALPHA            0x0305*/


//    glBlendFunc (GL_SRC_ALPHA, GL_ONE);

   glBlendFunc (GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	
 
 }

void FullScreen(){
    glutGameModeString( "800x600:32@60" );
	glutEnterGameMode();
}

int main()
{
	CurentCam=0;
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	if (MessageBox(NULL, "Do you wish Fullscreen!", "Screen Size", MB_OKCANCEL)==IDOK)
		FullScreen();
	else{
	glutInitWindowSize(WWI, WHI);
	glutCreateWindow("Tennis");
	}
	glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
	glutDisplayFunc(RenderScene);
	glutReshapeFunc(ChangeSize);
	glutTimerFunc(1, TimerFunction, 1);

	SetupRC();

	glutMainLoop();

	return 0;
}
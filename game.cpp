#include <GL/glut.h>
#include<iostream>
#include<string.h>
#include <cmath>
#include <ctime>
#include <unistd.h>
#include "glm.cpp"
#include "imageloader.h"
using namespace std;
#define PI 3.141592653589
#define DEG2RAD(deg) (deg * PI / 180)
GLfloat xhand1Rotate=0.0f,xhand2Rotate=45.0f,headz=0.5;
GLfloat xRotated=25.0f, yRotated=-45.0f, zRotated = 0.0f;
GLfloat xhrotate = 30.0f, zhrotate = -30.0f;
int face = 0;
int xhand1flag = 0,xhand2flag=1;
int windowWidth;
int windowHeight;
int w,h;
GLMmodel* pmodel1 = NULL;
GLuint displayArea;
GLuint _textureId1;
GLuint _textureId2;
GLuint _textureId3;
GLuint _textureId4;
struct Point{
	float x;
	float y;
	float z;
};
struct Face{
	Point a;
	Point b;
	Point c;
	Point d;
};
GLfloat mov = 0.1;
GLfloat movcnt = 1;
int Life = 3;
GLfloat Health = 0.8;
float r=1,g=1,bl=0;
int starttimer = 0;
int endtimer = 0;
int col =0;
int colflag = 0;
int jump = 0;
int jflag=0;
int jbegflag =0;
int jumpdirflag = 0;
int tflag = 0;
int fflag = 0;
int oflag = 0;
int hflag = 0;
int bflag = 0;
int lflag = 1;
int rflag = 0;
int vflag = 0;
int vtilenum;
int level=1;
//obnum=0;
double mouse_x,mouse_y,mouse_z;
double pmouse_x,pmouse_y,pmouse_z;
void mouseHandler(int,int);
void drawcoin(float x,float y,float z);
void drawplus(float x,float y,float z);
void drawf(void);
void catchcoin();
float coinangle = 0.0f;
double modelview[16];
double projection[16];
int viewport[4];
GLfloat jumplimit = 1.0f;
void helview();
Point hel_at;
Point hel_look;
//Lasers 
GLfloat laser1 = 0.0f;
int laser1flag = 1;
GLfloat laser2 = 10.0f;
int laser2flag = 0;
void lasercheck();
int robocoin=0;

void drawtext(char *message,float x,float y,float z)
{
	glRasterPos3f(x,y,z);
	while(*message)
	{
		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24,*message);
		*message++;
	}
}

char* itoa(int i,  char b[]){
	char const digit[] = "0123456789";
	char* p = b;
	if(i<0){
		*p++ = '-';
		i = -1;
	}
	int shifter = i;
	do{ //Move to where representation ends
		++p;
		shifter = shifter/10;
	}while(shifter);
	*p = '\0';
	do{ //Move back, inserting digits as u go
		*--p = digit[i%10];
		i = i/10;
	}while(i);
	return b;
}




GLfloat density = 0.2; //set the density to 0.3 which is

GLfloat fogColor[4] = {0.5, 0.5, 0.5, 1.0}; //set the forcolor to grey

int fogflag=0;
int fogtile;
int randyflag[30];
int Groundtile[25];
void groundset(){
	Groundtile[0] = 0;
	for(int i=1;i<25;i++)
	{
		Groundtile[i] = rand()%98 + 1;
	}
	fogtile = Groundtile[1];
}
int groundcheck(int x){
	for(int i=0;i<25;i++)
		if(Groundtile[i] == x)
			return 1;
	return 0;
}

int Gaptile[10];
void gapset(){
	int size=0;
	while(1)
	{
		Gaptile[size] = rand()%98 + 1;
		if(groundcheck(Gaptile[size])==0)
			size++;
		if(size == 10)
			break;
	}


}
int gapcheck(int x){
	for(int i=0;i<10;i++)
		if(Gaptile[i] == x)
			return 1;
	return 0;
}
int Randtile[30];
void randset(){
	int size=0;
	while(1)
	{
		Randtile[size] = rand()%98 + 1;
		if(groundcheck(Randtile[size])==0 && !gapcheck(Randtile[size]))
		{
			size++;
			if(size == 30)
				break;
		}
	}

}
int randcheck(int x){
	for(int i=0;i<30;i++)
		if(Randtile[i] == x)
			return 1;
	return 0;
}
int Uprtile[5];
void uprset(){
	int size=0;
	while(1)
	{
		Uprtile[size] = rand()%98 + 1;
		if(groundcheck(Uprtile[size])==0 && !gapcheck(Uprtile[size]) && !randcheck(Uprtile[size]))
			size++;
		if(size == 4)
			break;
	}
	Uprtile[4]=99;


}
int uprcheck(int x){
	for(int i=0;i<5;i++)
		if(Uprtile[i] == x)
			return 1;
	return 0;
}

int Upstile[5];
void upsset(){
	int size=0;
	while(1)
	{
		Upstile[size] = rand()%98 + 1;
		if(groundcheck(Upstile[size])==0 && !gapcheck(Upstile[size]) && !randcheck(Upstile[size]) && !uprcheck(Upstile[size]))
			size++;
		if(size == 5)
			break;
	}


}
int upscheck(int x){
	for(int i=0;i<5;i++)
		if(Upstile[i] == x)
			return 1;
	return 0;
}
int Uptile[25];
void upset(){
	int size=0;
	for(int i=1;i<100;i ++)
	{

		if(groundcheck(i)==0 && !gapcheck(i) && !randcheck(i) && !uprcheck(i) && !upscheck(i))
		{
			Uptile[size++]=i;
		}
	}
}
int upcount = 25;			
int upcheck(int x){
	for(int i=0;i<upcount;i++)
		if(Uptile[i] == x)
			return 1;
	return 0;
}

int Blocktile[5];
void blockset(){
	int size = 0;
	while(1){
		Blocktile[size] = rand()%98 +1;
		if(!randcheck(Blocktile[size]) && !gapcheck(Blocktile[size]))
			size++;
		if(size == 5)
			break;
	}
}
int blockcheck(int x){
	for(int i=0;i<5;i++)
		if(Blocktile[i] == x)
			return 1;
	return 0;
}

int Blasttile[3];
int blastflag[3]= {0};
int Blastlife[]= {1,1,1};
void blastset(){
	int size = 0;
	while(1){
		Blasttile[size] = rand()%98 +1;
		if(!randcheck(Blasttile[size]) && !gapcheck(Blasttile[size]) && !groundcheck(Blasttile[size]) && !blockcheck(Blasttile[size]))
			size++;
		if(size == 3)
			break;
	}

}
int blastcheck(int x){
	for(int i=0;i<3;i++)
		if(Blasttile[i] == x)
			return i;
	return -1;
}

int Teleftile[3];
int Telettile[3];
void teleset(){
	int size = 0;
	while(1){
		int t = rand()%20;
		//		if(!gapcheck(Teleftile[size]) && !blockcheck(Teleftile[size]) && !blastcheck(Teleftile[size]))
		Teleftile[size] = Uptile[t];
		size++;
		if(size == 3)
			break;
	}
	size = 0;
	while(1){
		int t = rand()%20;
		//	if(!gapcheck(Telettile[size]) && !blockcheck(Telettile[size]) && !blastcheck(Telettile[size]))
		Telettile[size] = Groundtile[t];
		size++;
		if(size == 3)
			break;
	}

}
int telecheck(int x){
	for(int i=0;i<3;i++)
		if(Teleftile[i] == x)
			return i;
	return -1;
}
int Springtile[2];
void springset(){
	int size = 0;
	while(1){
		int t = rand()%20 ;
		Springtile[size] = Uptile[t];
		size++;
		if(size == 2)
			break;
	}
}
int springcheck(int x){
	for(int i=0;i<2;i++)
		if(Springtile[i] == x)
			return 1;
	return 0;
}

int Healthtile[3];
int healthflag[3];
void healthset(){
	int size=0;
	healthflag[0]=1;
	healthflag[1]=1;
	healthflag[2]=1;

	while(1){
		int t = rand()%22 + 1;
		Healthtile[size] = Groundtile[t];
		size++;
		if(size==3)
			break;
	}
}


void drawpyramid(Point a){
	glColor3f(1,0,0);
	glBegin(GL_TRIANGLES);
	glNormal3f(1,1,0);
	glVertex3f(a.x,a.y,a.z);
	glVertex3f(a.x+ 0.5,a.y+0.75,a.z+0.5);
	glVertex3f(a.x,a.y,a.z+1);
	//
	glNormal3f(0,1,-1);
	glVertex3f(a.x,a.y,a.z+1);
	glVertex3f(a.x+ 0.5,a.y+0.75,a.z+0.5);
	glVertex3f(a.x+1,a.y,a.z+1);
	//
	glNormal3f(-1,1,0);
	glVertex3f(a.x+1,a.y,a.z+1);
	glVertex3f(a.x+ 0.5,a.y+0.75,a.z+0.5);
	glVertex3f(a.x+1,a.y,a.z);
	//
	glNormal3f(0,1,1);
	glVertex3f(a.x+1,a.y,a.z);
	glVertex3f(a.x+ 0.5,a.y+0.75,a.z+0.5);
	glVertex3f(a.x,a.y,a.z);
	glEnd();
	glFlush();
}

void drawhealth(){
	glColor3f(1,0,0);
	if(Health > 0.3)
		glColor3f(0,1,0);
	glBegin(GL_QUADS);
	glVertex3f(-0.3f,0.3f,0.0f);
	glVertex3f(-0.3f,0.32f,0.0f);
	glVertex3f(-0.3f + Health,0.32f,0.0f);
	glVertex3f(-0.3f + Health,0.3f,0.0f);
	glEnd();
	glFlush();
}


GLvoid glmDraw1(GLMmodel* model, GLuint mode,char *drawonly,Point pos)
{
	static GLuint i;
	static GLMgroup* group;
	static GLMtriangle* triangle;
	static GLMmaterial* material;
	GLuint IDTextura;

	assert(model);
	assert(model->vertices);

	/* do a bit of warning */
	if (mode & GLM_FLAT && !model->facetnorms) {
		printf("glmDraw() warning: flat render mode requested "
				"with no facet normals defined.\n");
		mode &= ~GLM_FLAT;
	}
	if (mode & GLM_SMOOTH && !model->normals) {
		printf("glmDraw() warning: smooth render mode requested "
				"with no normals defined.\n");
		mode &= ~GLM_SMOOTH;
	}
	if (mode & GLM_TEXTURE && !model->texcoords) {
		printf("glmDraw() warning: texture render mode requested "
				"with no texture coordinates defined.\n");
		mode &= ~GLM_TEXTURE;
	}
	if (mode & GLM_FLAT && mode & GLM_SMOOTH) {
		printf("glmDraw() warning: flat render mode requested "
				"and smooth render mode requested (using smooth).\n");
		mode &= ~GLM_FLAT;
	}
	if (mode & GLM_COLOR && !model->materials) {
		printf("glmDraw() warning: color render mode requested "
				"with no materials defined.\n");
		mode &= ~GLM_COLOR;
	}
	if (mode & GLM_MATERIAL && !model->materials) {
		printf("glmDraw() warning: material render mode requested "
				"with no materials defined.\n");
		mode &= ~GLM_MATERIAL;
	}
	if (mode & GLM_COLOR && mode & GLM_MATERIAL) {
		printf("glmDraw() warning: color and material render mode requested "
				"using only material mode.\n");
		mode &= ~GLM_COLOR;
	}
	if (mode & GLM_COLOR)
		glEnable(GL_COLOR_MATERIAL);
	else if (mode & GLM_MATERIAL)
		glDisable(GL_COLOR_MATERIAL);
	if (mode & GLM_TEXTURE) {
		glEnable(GL_TEXTURE_2D);
		glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	}
	/* perhaps this loop should be unrolled into material, color, flat,
	   smooth, etc. loops?  since most cpu's have good branch prediction
	   schemes (and these branches will always go one way), probably
	   wouldn't gain too much?  */

	IDTextura = -1;
	group = model->groups;
	while (group) 
	{
		glPushMatrix();
		/*		if(face == 90 || face == -270)
				glTranslatef(pos.x-0.2,pos.y+0.4,pos.z);
				if(face == -90 || face == 270)
				glTranslatef(pos.x+0.2,pos.y+0.4,pos.z);
				if(face == 0 || face == 360)
				glTranslatef(pos.x + 0.2,pos.y+0.4,pos.z);
				if(face == 180 || face == -180)
				glTranslatef(pos.x,pos.y+0.4,pos.z);*/
		glTranslatef(pos.x,pos.y+0.4,pos.z);
		glScalef(0.4,0.4,0.4);
		glRotatef(face,0,1,0);
		if(strcmp(group->name, " lefthand") == 0)
		{
			glTranslatef(-0.2f,0.55f,-0.13f);
			glRotatef(xhand1Rotate,1,0,0);
			glTranslatef(0.2f,-0.55f,0.13f);
			if(jump == 1)
			{
				xhand1Rotate = 0;
			}
		}
		if(strcmp(group->name, " righthand") == 0)
		{
			glTranslatef(-0.2f,0.55f,-0.13f);
			glRotatef(xhand2Rotate,1,0,0);
			glTranslatef(0.2f,-0.55f,0.13f);
			if(jump == 1)
			{
				xhand2Rotate = 0;
			}
			//			cout <<group->name<<"\n";
		}
		if(strcmp(group->name, " leftleg") == 0)
		{
			glRotatef(-xhand1Rotate,1,0,0);
			if(jump == 1)
			{
				xhand1Rotate = 0;
			}
			//			cout <<group->name<<"\n";
		}
		if(strcmp(group->name, " rightleg") == 0)
		{
			glRotatef(-xhand2Rotate,1,0,0);
			if(jump == 1)
			{
				xhand2Rotate = 0;
			}
			//			cout <<group->name<<"\n";
		}


		if (drawonly)
			if (strcmp(group->name,drawonly))
			{
				group=group->next;
				continue;
			}		

		material = &model->materials[group->material];
		if (material)
			IDTextura = material->IDTextura;
		else IDTextura=-1;

		if (mode & GLM_MATERIAL) 
		{            
			glMaterialfv(GL_FRONT_AND_BACK, GL_AMBIENT, material->ambient);
			glMaterialfv(GL_FRONT_AND_BACK, GL_DIFFUSE, material->diffuse);
			glMaterialfv(GL_FRONT_AND_BACK, GL_SPECULAR, material->specular);
			glMaterialf(GL_FRONT_AND_BACK, GL_SHININESS, material->shininess);
		}

		if (mode & GLM_TEXTURE) 
		{				
			if(IDTextura == -1)
				glBindTexture(GL_TEXTURE_2D, 0);
			else
				glBindTexture(GL_TEXTURE_2D, model->textures[IDTextura].id);		
		}

		if (mode & GLM_COLOR) {
			glColor3fv(material->diffuse);
		}

		glBegin(GL_TRIANGLES);
		for (i = 0; i < group->numtriangles; i++) {
			triangle = &T(group->triangles[i]);
#ifdef DebugVisibleSurfaces
			if (!triangle->visible) continue;
#endif
			if (mode & GLM_FLAT)
				glNormal3fv(&model->facetnorms[3 * triangle->findex]);

			if (mode & GLM_SMOOTH)
				glNormal3fv(&model->normals[3 * triangle->nindices[0]]);
			if (mode & GLM_TEXTURE)
				glTexCoord2fv(&model->texcoords[2 * triangle->tindices[0]]);
			glVertex3fv(&model->vertices[3 * triangle->vindices[0]]);

			if (mode & GLM_SMOOTH)
				glNormal3fv(&model->normals[3 * triangle->nindices[1]]);
			if (mode & GLM_TEXTURE)
			{
				//if (IDTextura==-1) printf("Warning: GLM_TEXTURE este on dar nu este setata nici o textura in material!");
				glTexCoord2fv(&model->texcoords[2 * triangle->tindices[1]]);
			}
			glVertex3fv(&model->vertices[3 * triangle->vindices[1]]);

			if (mode & GLM_SMOOTH)
				glNormal3fv(&model->normals[3 * triangle->nindices[2]]);
			if (mode & GLM_TEXTURE)
				glTexCoord2fv(&model->texcoords[2 * triangle->tindices[2]]);
			glVertex3fv(&model->vertices[3 * triangle->vindices[2]]);

		}
		glEnd();
		glPopMatrix();
		group = group->next;
	}
}




float xrotate =0.0f,yrotate = 180.0f,zrotate= 0.0f;
float jumptemp = 0.0f;
class Tile{
	public:
		Point a,b,c,d,a1,b1,c1,d1;
		Point obpos;
		int coinflag;
		void draw(){
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _textureId1);

			//Bottom
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			//	glColor3f(1.0f, 0.2f, 0.2f);

			glBegin(GL_QUADS);
			glNormal3f(0.0f, -1.0f, 0.0f);
			glVertex3f( a.x, a.y,a.z);  
			glVertex3f( b.x, b.y,b.z);
			glVertex3f( c.x, c.y,c.z); 
			glVertex3f( d.x, d.y,d.z);   
			//   
			glNormal3f(0.0f, 1.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f( a1.x, a1.y,a1.z);
			glTexCoord2f(1.0f, 0.0f);  
			glVertex3f( b1.x, b1.y,b1.z);
			glTexCoord2f(1.0f, 1.0f);    
			glVertex3f( c1.x, c1.y,c1.z);
			glTexCoord2f(0.0f, 1.0f);    
			glVertex3f( d1.x, d1.y,d1.z); 
			//Back
			glEnd();
			glDisable(GL_TEXTURE_2D);
			glEnable(GL_TEXTURE_2D);
			glBindTexture(GL_TEXTURE_2D, _textureId3);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
			glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
			glBegin(GL_QUADS);
			glNormal3f(0.0f, 0.0f, -1.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f( a.x, a.y,a.z);  
			glTexCoord2f(1.0f, 0.0f);  
			glVertex3f( b.x, b.y,b.z);
			glTexCoord2f(1.0f, 1.0f);    
			glVertex3f( b1.x, b1.y,b1.z);    
			glTexCoord2f(0.0f, 1.0f);    
			glVertex3f( a1.x, a1.y,a1.z);  
			//Left
			glNormal3f(-1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f( a.x, a.y,a.z);  
			glTexCoord2f(1.0f, 0.0f);  
			glVertex3f( d.x, d.y,d.z);   
			glTexCoord2f(1.0f, 1.0f);    
			glVertex3f( d1.x, d1.y,d1.z); 
			glTexCoord2f(0.0f, 1.0f);    
			glVertex3f( a1.x, a1.y,a1.z); 
			//Front
			glNormal3f(0.0f, 0.0f, 1.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f( c.x, c.y,c.z); 
			glTexCoord2f(1.0f, 0.0f);  
			glVertex3f( d.x, d.y,d.z);   
			glTexCoord2f(1.0f, 1.0f);    
			glVertex3f( d1.x, d1.y,d1.z); 
			glTexCoord2f(0.0f, 1.0f);    
			glVertex3f( c1.x, c1.y,c1.z);    
			//Right
			glNormal3f(1.0f, 0.0f, 0.0f);
			glTexCoord2f(0.0f, 0.0f);
			glVertex3f( b.x, b.y,b.z);
			glTexCoord2f(1.0f, 0.0f);  
			glVertex3f( c.x, c.y,c.z); 
			glTexCoord2f(1.0f, 1.0f);    
			glVertex3f( c1.x, c1.y,c1.z);    
			glTexCoord2f(0.0f, 1.0f);    
			glVertex3f( b1.x, b1.y,b1.z);    

			glEnd();  
			glDisable(GL_TEXTURE_2D);
			glColor3f(r/2,g/2,bl/2);   
			glBegin(GL_LINES);
			glVertex3f(a1.x,a1.y,a1.z);
			glVertex3f(b1.x,b1.y,b1.z);
			glVertex3f(b1.x,b1.y,b1.z);
			glVertex3f(c1.x,c1.y,c1.z);
			glVertex3f(c1.x,c1.y,c1.z);
			glVertex3f(d1.x,d1.y,d1.z);
			glVertex3f(d1.x,d1.y,d1.z);
			glVertex3f(a1.x,a1.y,a1.z);
			glVertex3f(a.x,a.y,a.z);
			glVertex3f(a1.x,a1.y,a1.z);
			glVertex3f(b.x,b.y,b.z);
			glVertex3f(b1.x,b1.y,b1.z);
			glVertex3f(c.x,c.y,c.z);
			glVertex3f(c1.x,c1.y,c1.z);
			glVertex3f(d.x,d.y,d.z);
			glVertex3f(d1.x,d1.y,d1.z);
			glEnd();  
			glFlush();

		}


		void set(){
			obpos.x = (a1.x + b1.x)/2;
			obpos.y = 0.1f;
			obpos.z = (b1.z + c1.z)/2;
		}





};
Tile Set[100];

class Object{
	public:
		Point pos;
		Point relpos;
		int tilenum;
		void draw(){
			if(randcheck(tilenum)==1)
				pos.y = Set[tilenum].a1.y;
			// Load the model only if it hasn't been loaded before
			// If it's been loaded then pmodel1 should be a pointer to the model geometry data...otherwise it's null
			if (!pmodel1) 
			{
				// this is the call that actualy reads the OBJ and creates the model object
				pmodel1 = glmReadOBJ("android.obj");	
				if (!pmodel1) exit(0);
				// This will rescale the object to fit into the unity matrix
				// Depending on your project you might want to keep the original size and positions you had in 3DS Max or GMAX so you may have to comment this.
				glmUnitize(pmodel1);
				// These 2 functions calculate triangle and vertex normals from the geometry data.
				// To be honest I had some problem with very complex models that didn't look to good because of how vertex normals were calculated
				// So if you can export these directly from you modeling tool do it and comment these line
				// 3DS Max can calculate these for you and GLM is perfectly capable of loading them
				glmFacetNormals(pmodel1);        
				glmVertexNormals(pmodel1, 90.0);
			}
			// This is the call that will actualy draw the model
			// Don't forget to tell it if you want textures or not :))
			glmDraw1(pmodel1, GLM_SMOOTH| GLM_TEXTURE,0,pos);
			//glDisable(GL_LIGHTING);	
		}
};
// Here are the fonts: 


void drawVoid(){
	glPushMatrix();
	glColor3f(1.0f,1.0f,1.0f);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	//glOrtho(0,1,0,1,-1,1);
	glPushMatrix();
	glTranslatef(0,0,-25);
	glBegin(GL_QUADS);
	glTexCoord2f(0.0f,0.0f);
	glVertex2f(0,0);
	glTexCoord2f(1.0f,0.0f);
	glVertex2f(1.0,0);
	glTexCoord2f(1.0f,1.0f);
	glVertex2f(1,1);
	glTexCoord2f(0.0f,1.0f);
	glVertex2f(0,1);
	glEnd();
	glPopMatrix();
	glPopMatrix();
}



Object ob1;
float timer= 1;
int endflag = 0;
int timercheck =0;
void handleKeypress1(unsigned char key, int x, int y);
void handleKeypress2(int key, int x, int y) ;
void handleKeypress3(int key,int x,int y);



GLuint loadTexture(Image* image) {
	GLuint textureId;
	glGenTextures(1, &textureId); //Make room for our texture
	glBindTexture(GL_TEXTURE_2D, textureId); //Tell OpenGL which texture to edit
	//Map the image to the texture
	glTexImage2D(GL_TEXTURE_2D,                //Always GL_TEXTURE_2D
			0,                            //0 for now
			GL_RGB,                       //Format OpenGL uses for image
			image->width, image->height,  //Width and height
			0,                            //The border of the image
			GL_RGB, //GL_RGB, because pixels are stored in RGB format
			GL_UNSIGNED_BYTE, //GL_UNSIGNED_BYTE, because pixels are stored
			//as unsigned numbers
			image->pixels);               //The actual pixel data
	return textureId; //Returns the id of the texture
}

void drawEnd()
{
	if(endtimer < 50)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId4);
		glColor3f(1.0f,1.0f,1.0f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f( -0.6f, 0.3f,0.0f);
		glTexCoord2f(1.0f, 1.0f);  
		glVertex3f( 0.6f, 0.3f,0.0f);
		glTexCoord2f(1.0f, 0.0f);    
		glVertex3f( 0.6f, -0.32f,0.0f);
		glTexCoord2f(0.0f, 0.0f);    
		glVertex3f( -0.6f, -0.32f,0.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
		endtimer++;
	}
}


void init(void)
{
	//glClearColor(0,0,0,0);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_COLOR_MATERIAL);
	glEnable(GL_LIGHTING); //Enable lighting
	glEnable(GL_LIGHT0); //Enable light #0
	glEnable(GL_LIGHT1); //Enable light #1
	glEnable(GL_NORMALIZE); //Automatically normalize normals
	//glShadeModel(GL_SMOOTH); //Enable smooth shading
	Image* image1 = loadBMP("vtr.bmp");
	_textureId1 = loadTexture(image1);
	Image* image2 = loadBMP("bg.bmp");
	_textureId2 = loadTexture(image2);
	Image* image3 = loadBMP("side.bmp");
	_textureId3 = loadTexture(image3);
	Image* image4 = loadBMP("start.bmp");
	_textureId4 = loadTexture(image4);
	delete image1;
	delete image2;
	delete image3;
	delete image4;

}

void colordecide(){

	if((col+colflag)%2 == 0)
		r=1,g=1,bl=1;
	if((col+colflag)%2 == 1)
		r=0,g=0.47,bl=0.47;//Blue
	col++;
	if(col == 10)
	{
		col =0;
		colflag = (colflag +1)%2;
	}

}


void initialize(){
	srand ( time(NULL) );
	groundset();
	randset();
	for(int i=0;i<30;i++)
		randyflag[i] = (i%2);
	gapset();
	uprset();
	upsset();
	upset();
	blockset();
	blastset();
	blastflag[3] = 0;
	blastflag[1] = 0;
	blastflag[2] = 0;
	teleset();
	healthset();
	springset();
	vtilenum = ob1.tilenum;
	jumplimit = 1.0f;
	mov = 0.1;
	int i=0,j,k;
	float x=0,y=0,z=-1.0f;
	for(j=0;j<10;j++)
	{
		x = -1.0f;
		z = z + 1.0f;
		for(k=0;k<10;k++)
		{
			x = x + 1.0f;
			Set[i].a.x = x;
			Set[i].a.y = y;
			Set[i].a.z = z;
			Set[i].b.x = x + 1;
			Set[i].b.y = y;
			Set[i].b.z = z;
			Set[i].c.x = x + 1;
			Set[i].c.y = y;
			Set[i].c.z = z + 1;
			Set[i].d.x = x;
			Set[i].d.y = y;
			Set[i].d.z = z + 1;
			Set[i].a1.x = x;
			Set[i].a1.y = y + 0.1;
			Set[i].a1.z = z;
			Set[i].b1.x = x + 1;
			Set[i].b1.y = y + 0.1;
			Set[i].b1.z = z;
			Set[i].c1.x = x + 1;
			Set[i].c1.y = y + 0.1;
			Set[i].c1.z = z + 1;
			Set[i].d1.x = x;
			Set[i].d1.y = y + 0.1;
			Set[i].d1.z = z + 1;
			Set[i].coinflag=0;
			Set[i++].set();
		}
	}
	for(int i =0;i<30;i++)
	{
		if(i%2 == 0)
			y = 1.9f;
		else
			y = 0.1f;
		Set[Randtile[i]].a1.y = y;
		Set[Randtile[i]].b1.y = y;
		Set[Randtile[i]].c1.y = y;
		Set[Randtile[i]].d1.y = y;
	}
	for(int up =0;up<10;up++)
	{
		Set[Uprtile[up]].a1.y = 1.3f;
		Set[Uprtile[up]].b1.y = 1.3f;
		Set[Uprtile[up]].c1.y = 1.3f;
		Set[Uprtile[up]].d1.y = 1.3f;
		Set[Uprtile[up]].coinflag=1;
	}
	for(int up =0;up<10;up++)
	{
		Set[Upstile[up]].a1.y = 1.9f;
		Set[Upstile[up]].b1.y = 1.9f;
		Set[Upstile[up]].c1.y = 1.9f;
		Set[Upstile[up]].d1.y = 1.9f;
		Set[Upstile[up]].coinflag=1;
	}


	for(int up =0;up<upcount;up++)
	{
		Set[Uptile[up]].a1.y = 0.7f;
		Set[Uptile[up]].b1.y = 0.7f;
		Set[Uptile[up]].c1.y = 0.7f;
		Set[Uptile[up]].d1.y = 0.7f;
		Set[Uptile[up]].coinflag=1;
	}
	// tower
	ob1.tilenum = 0;
	ob1.pos = Set[0].obpos;
	ob1.relpos = ob1.pos;
	drawpyramid(Set[0].a1);
	hel_at.x =ob1.pos.x;
	hel_at.y =ob1.pos.y ;
	hel_at.z =ob1.pos.z;
	hel_look.x = ob1.pos.x ;
	hel_look.y = 0.0f;
	hel_look.z = ob1.pos.z;


	//	ob1.pos.x = Set[0].obpos.x;
	//	ob1.pos.y = Set[0].obpos.y;
	//	ob1.pos.z = Set[0].obpos.z;
}

Tile Floor[4];


void DrawScene()
{
	if(ob1.tilenum == fogtile)
	{
		fogflag = 0;
	}
	if(fogflag == 1)
	{
		glEnable (GL_FOG); //enable the fog

		glFogi (GL_FOG_MODE, GL_EXP2); //set the fog mode to GL_EXP2

		glFogfv (GL_FOG_COLOR, fogColor); //set the fog color toour color chosen above

		glFogf (GL_FOG_DENSITY, density); //set the density to thevalue above

		glHint (GL_FOG_HINT, GL_NICEST); // set the fog to look the	nicest, may slow down on older cards
	}
	if(fogflag == 0)
		glDisable(GL_FOG);
	glMatrixMode(GL_MODELVIEW);
	// clear the drawing buffer.
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	
	if(endflag == 1)
		drawEnd();
		
	glPushMatrix();
	//	glTranslatef(0,0,-30);
	glTranslatef(0.0,-1.0f,-15.5f);
	glRotatef(yrotate,0,1,0);
	glRotatef(zrotate,0,0,1);
	glRotatef(xrotate-10,1,0,0);
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, _textureId2);
	glColor3f(1.0f,1.0f,1.0f);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glBegin(GL_QUADS);
	glNormal3f(0.0f, 0.0f, 1.0f);
	glTexCoord2f(0.0f, 0.0f);
	glVertex3f( -15.0f, -5.0f,0.0f);
	glTexCoord2f(1.0f, 0.0f);  
	glVertex3f( 15.0f, -5.0f,0.0f);
	glTexCoord2f(1.0f, 1.0f);    
	glVertex3f( 15.0f, 10.0f,0.0f);
	glTexCoord2f(0.0f, 1.0f);    
	glVertex3f( -15.0f, 10.0f,0.0f);
	glEnd();
	glDisable(GL_TEXTURE_2D);

	glPopMatrix();
	glTranslatef(0,0,-1);
	if(starttimer < 50)
	{
		glEnable(GL_TEXTURE_2D);
		glBindTexture(GL_TEXTURE_2D, _textureId4);
		glColor3f(1.0f,1.0f,1.0f);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
		glBegin(GL_QUADS);
		glNormal3f(0.0f, 0.0f, 1.0f);
		glTexCoord2f(0.0f, 1.0f);
		glVertex3f( -0.6f, 0.3f,0.0f);
		glTexCoord2f(1.0f, 1.0f);  
		glVertex3f( 0.6f, 0.3f,0.0f);
		glTexCoord2f(1.0f, 0.0f);    
		glVertex3f( 0.6f, -0.32f,0.0f);
		glTexCoord2f(0.0f, 0.0f);    
		glVertex3f( -0.6f, -0.32f,0.0f);
		glEnd();
		glDisable(GL_TEXTURE_2D);
	}
	if(Life > 1)
		glColor3f(0,1,0);
	else	   
		glColor3f(1,0,0);
	glBegin(GL_QUADS);
	float lx=0.03;
	for(int i=0;i<Life;i++)
	{
		glVertex3f(-0.6f+lx,0.3f,0.0f);
		glVertex3f(-0.58f+lx,0.3f,0.0f);
		glVertex3f(-0.58f+lx,0.32f,0.0f);
		glVertex3f(-0.6f+lx,0.32f,0.0f);
		lx+=0.03;
	}
	glEnd();
	glColor3f(0,0,1);
	lx = 0.03;
	for(int i=0;i<level;i++)
	{
		glVertex3f(-0.6f+lx,0.25f,0.0f);
		glVertex3f(-0.58f+lx,0.25f,0.0f);
		glVertex3f(-0.58f+lx,0.27f,0.0f);
		glVertex3f(-0.6f+lx,0.27f,0.0f);
		lx+=0.03;
	}
	glEnd();
	
	glColor3f(0,1,0);
	char lifetext[20];
	strcpy(lifetext,"Life ");
	char temp[20];
	strcat(lifetext,itoa(Life,temp));
//	drawtext(lifetext,-0.6f,0.3f,0.0f);
//	drawtext("Health",-0.4f,0.3f,0.0f);
//	drawcoin(2.2f,1.3f,-3.0f);
	glColor3f(0,1,0);
//	drawtext(itoa(robocoin,temp),0.6f,0.31f,0.0f);
	strcpy(lifetext,"Level ");
	strcat(lifetext,itoa(level,temp));
//	drawtext(lifetext,-0.6f,0.25f,0.0f);
	

	drawhealth();
	glPushMatrix();
	if(oflag == 1)
	{
		gluLookAt(ob1.pos.x , ob1.pos.y +13,ob1.pos. z , ob1.pos.x + 0.7*sin(DEG2RAD(face)),ob1.pos.y +0.2 ,ob1.pos.z + 0.7*cos(DEG2RAD(face)),  0,1,0);
		glRotatef(0,0,1,0);
		glRotatef(0,0,0,1);
		glRotatef(0,1,0,0);
	}

	else if(fflag == 1)
	{
		gluLookAt(ob1.pos.x + 0.8*sin(DEG2RAD(face)) , ob1.pos.y +0.3,ob1.pos. z + 0.8*cos(DEG2RAD(face)), ob1.pos.x + 1.5*sin(DEG2RAD(face)) ,ob1.pos.y +0.2 ,ob1.pos.z + 1.5*cos(DEG2RAD(face)),      0,1,0);
		glRotatef(0,0,1,0);
		glRotatef(0,0,0,1);
		glRotatef(0,1,0,0);
	}
	else if(tflag == 1)
	{
		// 	 gluLookAt(ob1.pos.x - 3*0  , ob1.pos.y + 3 ,ob1.pos. z - 3  ,	 ob1.pos.x + 0.5*0 ,ob1.pos.y + 1,ob1.pos.z + 0.5,      0,1,0);
		gluLookAt(ob1.pos.x - 1*sin(DEG2RAD(face))  , ob1.pos.y + 2 ,ob1.pos. z - 1*cos(DEG2RAD(face))  ,	 ob1.pos.x + 0.0*sin(DEG2RAD(face)) ,ob1.pos.y + 1,ob1.pos.z + 0.5*cos(DEG2RAD(face)),      0,1,0);
		glRotatef(0,0,1,0);
		glRotatef(0,0,0,1);
		glRotatef(0,1,0,0);
	}
	else if(hflag == 1)
	{
		/*	 glTranslatef(4.0,-1.0f,-5.5f);
			 glRotatef(yrotate,0,1,0);
			 glRotatef(zrotate,0,0,1);
			 glRotatef(xrotate-10,1,0,0);*/
		/*	hel_at.x =ob1.pos.x;
			hel_at.y =ob1.pos.y + 5.0;
			hel_at.z =ob1.pos.z;
			hel_look.x = ob1.pos.x ;
			hel_look.y = 0.0f;
			hel_look.z = ob1.pos.z;*/
		gluLookAt(hel_at.x  , hel_at.y + 5.0f ,hel_at.z  ,hel_look.x + 0.7*cos(DEG2RAD(xhrotate)) ,hel_look.y,hel_look.z + 0.7*sin(DEG2RAD(xhrotate)),      0,1,0);

	}
	else if(bflag == 1)
	{
		gluLookAt(0.0f , 10.0f ,0.0f  ,3.5f,0.0f,3.5f,      0,1,0);
	}
	else if(vflag == 1)
	{
		gluLookAt(Set[vtilenum].a1.x , Set[vtilenum].a1.y ,Set[vtilenum].a1.z  ,ob1.pos.x,ob1.pos.y,ob1.pos.z,  0,1,0);
	}

	else
	{
		glTranslatef(4.0,-1.0f,-5.5f);
		glRotatef(yrotate,0,1,0);
		glRotatef(zrotate,0,0,1);
		glRotatef(xrotate-10,1,0,0);
	}
	//Add ambient light
	GLfloat ambientColor[] = {0.4f, 0.4f, 0.4f, 1.0f}; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	//Add positioned light
	GLfloat lightColor02[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos02[] = {0.0f, 0.0,0.0f ,1.0f};
	//   GLfloat lightPos0[] = {ob1.pos.x, ob1.pos.y + 1.0, ob1.pos.z, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor02);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos02);
	GLfloat lightColor0[] = {0.5f, 0.5f, 0.5f, 1.0f}; //Color (0.5, 0.5, 0.5)
	GLfloat lightPos0[] = {0.0f, 10.0,2.0f ,1.0f};
	//   GLfloat lightPos0[] = {ob1.pos.x, ob1.pos.y + 1.0, ob1.pos.z, 0.0f};
	glLightfv(GL_LIGHT0, GL_DIFFUSE, lightColor0);
	glLightfv(GL_LIGHT0, GL_POSITION, lightPos0);
	//


	//Add directed light
	GLfloat lightColor1[] = {0.2f, 0.2f, 0.2f, 1.0f}; //Color (0.5, 0.2, 0.2)
	//Coming from the direction (-1, 0.5, 0.5)
	GLfloat lightPos1[] = {0.5f, 0.5,0.5f ,0.0f};
	glLightfv(GL_LIGHT1, GL_DIFFUSE, lightColor1);
	glLightfv(GL_LIGHT1, GL_POSITION, lightPos1);
	if(rflag == 1 && lflag ==1)
	{
		glEnable(GL_LIGHTING); //Enable lighting
		GLfloat light_position[]= {ob1.pos.x ,ob1.pos.y+0.7f,ob1.pos.z,1.0f};
		GLfloat spotDir[] = {0.0f,-1.0f,0.0f};   //
		GLfloat color[] = {0.5f,0.5f,0.5f,1.0f};   //
		glLightfv(GL_LIGHT0, GL_DIFFUSE, color);

		glLightfv(GL_LIGHT0,GL_POSITION,light_position);

		glLightf(GL_LIGHT0,GL_SPOT_CUTOFF,45.0);
		glLightf(GL_LIGHT0,GL_SPOT_EXPONENT,1.0);
		glLightfv(GL_LIGHT0,GL_SPOT_DIRECTION,spotDir);   //
		glEnable(GL_COLOR_MATERIAL);
	}

	if(ob1.tilenum == 89 || ob1.tilenum == 98)
	{
		jumplimit = 2.1;
	}



	int i;
	if(level == 2)
	{

		glColor3f(1,0,0);
		glLineWidth(3.0f);
		glBegin(GL_QUADS);
		glNormal3f(0,1,0);
		glVertex3f(10.0f,1.0f,0.0f);
		glVertex3f(10.0f,1.0f,0.1f);
		glVertex3f(laser1+0.1f,1.0f,10.f);
		glVertex3f(laser1+0.0f,1.0f,10.0f);
		//
		glNormal3f(0,1,0);
		glVertex3f(0.0f,0.5f,5.1f);
		glVertex3f(0.0f,0.5f,5.0f);
		glVertex3f(10.0f,0.5f,laser2 + 0.0f);
		glVertex3f(10.0f,0.5f,laser2 + 0.1f);
		glEnd();
	}
		//char *text = "h";
		//char *p;
		//for (p = text; *p; p++)
	//        	 glutStrokeCharacter(GLUT_STROKE_ROMAN,'a');

	for(i=99;i>=0;i--)
	{
		colordecide();
		if(gapcheck(i) == 0)
		{
			//if(blockcheck(i) == 0 && randcheck(i)==0 && groundcheck(i) == 0)
			if(Set[i].coinflag == 1)
				drawcoin(Set[i].a1.x+0.5,Set[i].a1.y+1,Set[i].a1.z + 0.5);

			glColor3f(r,g,bl);   
			if(blastcheck(i)!= -1)
			{
				if((blastflag[blastcheck(i)] == 1))
					glColor3f(1,0,0); 
				if((blastcheck(i) == 0))
					glColor3f(1,0,0); 
			}

			if(telecheck(i) != -1)
				glColor3f(0.95,0.71,0.2);
			if(springcheck(i) == 1)
				glColor3f(1,1,0);
			if( i == 99 || i ==0)
				glColor3f(0,0,1);  

			if(blastcheck(i) != -1)
			{
				if((Blastlife[blastcheck(i)] == 1))
					Set[i].draw();
			}
			else
			{
				Set[i].draw();
				//drawcoin();
			}
		}
	}
	for(int i=0;i<5;i++)
	{
		drawpyramid(Set[Blocktile[i]].a1);
	}
	if(springcheck(ob1.tilenum)==1)
	{
		jump = 1;
		jumplimit = 1.4;
		//	    jflag=0;
		jbegflag=0;
		jumptemp = Set[ob1.tilenum].a1.y;
	}
	if(movcnt > 1 && jumpdirflag == 0)
	{
		movcnt -= 1;
		if( face == 90 || face == -270)
		{
			ob1.pos.x += mov;
			ob1.relpos.x += mov;
			if(ob1.relpos.x > 1.0f)
			{
				if(Set[ob1.tilenum+1].a1.y == ob1.pos.y)
				{
					if(blockcheck(ob1.tilenum +1))
					{
						ob1.pos.x -=0.3;
						ob1.relpos.x -= 0.3;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.x = 0;
						ob1.tilenum++;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;

					}
				}
				else if(Set[ob1.tilenum+1].a1.y < ob1.pos.y)
				{
					if((Set[ob1.tilenum+1].a1.y-ob1.pos.y)<=-0.7f)
						Health -=0.2;
					if(blockcheck(ob1.tilenum +1))
					{
						ob1.pos.x -=0.3;
						ob1.relpos.x -= 0.3;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.x = 0;
						ob1.tilenum++;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
						ob1.pos.y = Set[ob1.tilenum].a1.y;
					}
				}
				else
				{
					ob1.pos.x -=mov;
					ob1.relpos.x -=mov;
				}

			}
		}
		if(face == -90 || face == 270)
		{
			ob1.pos.x -= mov;
			ob1.relpos.x -= mov;
			if(ob1.relpos.x < 0.0f)
			{
				if(Set[ob1.tilenum-1].a1.y == ob1.pos.y)
				{
					if((Set[ob1.tilenum-1].a1.y-ob1.pos.y)<=-0.7f)
						Health -=0.2;
					if(blockcheck(ob1.tilenum -1))
					{
						ob1.pos.x +=0.3;
						ob1.relpos.x += 0.3;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.x = 1;
						ob1.tilenum--;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
					}
				}
				else if(Set[ob1.tilenum-1].a1.y < ob1.pos.y)
				{
					if(blockcheck(ob1.tilenum -1))
					{
						ob1.pos.x +=0.3;
						ob1.relpos.x += 0.3;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.x = 1;
						ob1.tilenum--;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
						ob1.pos.y = Set[ob1.tilenum].a1.y;
					}
				}
				else
				{
					ob1.pos.x +=mov;
					ob1.relpos.x +=mov;
				}

			}
		}
		if(face == 0 || face == 360)
		{
			ob1.pos.z += mov;
			ob1.relpos.z += mov;
			if(ob1.relpos.z > 1.0f)
			{
				if(Set[ob1.tilenum+10].a1.y == ob1.pos.y)
				{
					if((Set[ob1.tilenum+10].a1.y-ob1.pos.y)<=-0.7f)
						Health -=0.2;
					if(blockcheck(ob1.tilenum +10))
					{
						ob1.pos.z -=0.3;
						ob1.relpos.z -= 0.3;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.z = 0;
						ob1.tilenum +=10;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
					}
				}
				else if(Set[ob1.tilenum+10].a1.y < ob1.pos.y)
				{
					if(blockcheck(ob1.tilenum +10))
					{
						ob1.pos.z -=0.3;
						ob1.relpos.z -= 0.3;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.z = 0;
						ob1.tilenum +=10;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
						ob1.pos.y = Set[ob1.tilenum].a1.y;
					}
				}
				else
				{
					ob1.pos.z -=mov;
					ob1.relpos.z -=mov;
				}
			}
		}
		if(face == 180 || face == -180)
		{
			ob1.pos.z -= mov;
			ob1.relpos.z -= mov;
			if(ob1.relpos.z < 0.0f)
			{
				if(Set[ob1.tilenum-10].a1.y == ob1.pos.y)
				{
					if((Set[ob1.tilenum-10].a1.y-ob1.pos.y)<=-0.7f)
						Health -=0.2;
					if(blockcheck(ob1.tilenum -10))
					{
						ob1.pos.z +=0.3;
						ob1.relpos.z += 0.3;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.z = 1.0f;
						ob1.tilenum -= 10;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
					}
				}
				else if(Set[ob1.tilenum-10].a1.y < ob1.pos.y)
				{
					if(blockcheck(ob1.tilenum -10))
					{
						ob1.pos.z +=0.3;
						ob1.relpos.z += 0.3;
						Health -=0.2;
					}
					else{
						ob1.relpos.z = 1.0f;
						ob1.tilenum -= 10;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
						ob1.pos.y = Set[ob1.tilenum].a1.y;
					}
				}
				else
				{
					ob1.pos.z +=mov;
					ob1.relpos.z +=mov;
				}
			}
		}
	}


	if(jump ==1)
	{
		if(jumpdirflag == 1)
		{
			ob1.pos.x += 0.033;
			ob1.relpos.x += 0.033;
			if(ob1.relpos.x > 1.0f)
			{
				if((Set[ob1.tilenum+1].a1.y-Set[ob1.tilenum].a1.y)<=0.7f)
				{
					if((Set[ob1.tilenum+1].a1.y-Set[ob1.tilenum].a1.y)<=-0.7f)
						Health -=0.2;
					if(blockcheck(ob1.tilenum +1))
					{
						ob1.pos.x -=1;
						ob1.relpos.x -= 1;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.x = 0;
						ob1.tilenum++;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
					}
				}
				else if(blockcheck(ob1.tilenum +1))
				{
					ob1.pos.x -=1;
					ob1.relpos.x -= 1;
					Health -=0.2;
				}

				else
				{
					ob1.pos.x -=0.033;
					ob1.relpos.x -= 0.033;
				}

			}
		}
		if(jumpdirflag == 2)
		{
			ob1.pos.x -= 0.033;
			ob1.relpos.x -= 0.033;
			if(ob1.relpos.x < 0.0f)
			{
				if((Set[ob1.tilenum-1].a1.y-Set[ob1.tilenum].a1.y)<=0.7f)
				{
					if((Set[ob1.tilenum-1].a1.y-Set[ob1.tilenum].a1.y)<=-0.7f)
						Health -=0.2;
					if(blockcheck(ob1.tilenum -1))
					{
						ob1.pos.x +=1;
						ob1.relpos.x += 1;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.x = 1;
						ob1.tilenum--;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
					}
				}
				else if(blockcheck(ob1.tilenum -1))
				{
					ob1.pos.x +=1;
					ob1.relpos.x += 1;
					Health -=0.2;
				}
				else
				{
					ob1.pos.x +=0.033;
					ob1.relpos.x +=0.033;
				}
			}
		}
		if(jumpdirflag == 3)
		{
			ob1.pos.z += 0.033;
			ob1.relpos.z += 0.033;
			if(ob1.relpos.z > 1.0f)
			{
				if((Set[ob1.tilenum+10].a1.y-Set[ob1.tilenum].a1.y)<=0.7f)
				{
					if((Set[ob1.tilenum+10].a1.y-Set[ob1.tilenum].a1.y)<=-0.7f)
						Health -=0.2;
					if(blockcheck(ob1.tilenum +10))
					{
						ob1.pos.z -=1;
						ob1.relpos.z -= 1;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.z = 0;
						ob1.tilenum +=10;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
					}
				}
				else if(blockcheck(ob1.tilenum +10))
				{
					ob1.pos.z -=1;
					ob1.relpos.z -= 1;
					Health -=0.2;
				}
				else
				{
					ob1.pos.z -= 0.033;
					ob1.relpos.z -= 0.033;
				}
			}
		}
		if(jumpdirflag == 4)
		{
			ob1.pos.z -= 0.033;
			ob1.relpos.z -= 0.033;
			if(ob1.relpos.z < 0.0f)
			{
				if((Set[ob1.tilenum-10].a1.y-Set[ob1.tilenum].a1.y)<=0.7f)
				{
					if((Set[ob1.tilenum-10].a1.y-Set[ob1.tilenum].a1.y)<=-0.7f)
						Health -=0.2;
					if(blockcheck(ob1.tilenum -10))
					{
						ob1.pos.z +=1;
						ob1.relpos.z += 1;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.z = 1.0f;
						ob1.tilenum -= 10;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
					}
				}
				else if(blockcheck(ob1.tilenum -10))
				{
					ob1.pos.z +=1;
					ob1.relpos.z += 1;
					Health -=0.2;
				}
				else
				{
					ob1.pos.z += 0.033;
					ob1.relpos.z += 0.033;
				}
			}
		}
		if(jflag==0)
		{

			ob1.pos.y += 0.1;
			ob1.relpos.y += 0.1;
		}
		else{
			ob1.pos.y -= 0.1;
			ob1.relpos.y -= 0.1;
			if(ob1.pos.y == Set[ob1.tilenum].a1.y)
			{
				jump =0;
			}



		}
		if((ob1.pos.y - jumptemp) >= jumplimit)
		{
			jflag =1;
		}

		if(  ob1.pos.y < Set[ob1.tilenum].a1.y)
		{
			ob1.pos.y = Set[ob1.tilenum].a1.y;

			jump=0;
			jflag = 0;
		}
		else
			jbegflag=1;
	}

	glPushMatrix();
	glColor3f(0.64f,0.77f,0.22f);
	GLfloat ambientColor1[] = {0.2f, 0.2f, 0.2f, 0.0f}; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor1);
	ob1.draw();
	GLfloat ambientColor2[] = {0.4f, 0.4f, 0.4f, 0.0f}; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor2);
	glPopMatrix();
	catchcoin();
	if(level == 2)
		lasercheck();

	for(i=0;i<3;i++)
	{
		if(healthflag[i] == 1)
			drawplus(Set[Healthtile[i]].a1.x+0.5f,Set[Healthtile[i]].a1.y+0.5f,Set[Healthtile[i]].a1.z+0.5);
	}
	if(fogflag == 1)
		drawf();	
	for(int up=0;up<upcount;up++)
	{
		if(ob1.tilenum > Uptile[up])
		{
			Set[Uptile[up]].draw();
		}
	}
	glPopMatrix();
	
	glutSwapBuffers();
}


void update(int val)
{
	starttimer++;
	coinangle += 5.0f;
	if(laser1flag ==1)
	{
		laser1 += 0.1f;
		if(laser1 >= 10.0f)
			laser1flag = 0;
	}
	else
	{
		laser1 -= 0.1f;
		if(laser1 <= -0.5f)
			laser1flag = 1;
	}
/*	if(laser2flag ==1)
	{
		laser2 += 0.1f;
		if(laser2 >= 10.0f)
			laser2flag = 0;
	}
	else
	{
		laser2 -= 0.1f;
		if(laser2 <= -0.5f)
			laser2flag = 1;
	}*/
	if(ob1.tilenum == 99)
	{
		if(level == 2)
		{
			cout<<"CONGRATULATIONS\n";
			printf("Score: %d\n",robocoin);
			exit(1);
		}
		else if(level == 1)
		{
			level ++;
			fogflag=1;
			initialize();
		}
			
	}
	// To Move limbs of robo
	if(jumpdirflag != 0)
	{
		if(xhand1flag==0)
		{
			xhand1Rotate -= 15;
			//	    xhand1flag =1;
		}
		else
		{
			xhand1Rotate += 15;
			//	    xhand1flag = 0;
		}
		if(xhand1Rotate <= -45.0f)
			xhand1flag=1;
		if(xhand1Rotate >= 45.0f)
			xhand1flag=0;
		//     xRotated += 0.02;
		if(xhand2flag==0)
		{
			xhand2Rotate -= 15;
			//	    xhand2flag = 1;
		}
		else
		{
			xhand2Rotate += 15;
			//	    xhand2flag = 0;
		}
		if(xhand2Rotate <= -45.0f)
			xhand2flag=1;
		if(xhand2Rotate >= 45.0f)
			xhand2flag=0;
	}
	headz = 0.5f;
	//For Random up down tiles
	if(timercheck == 10)
	{
		for(int i=0;i<30;i++)
		{
			if(randyflag[i] == 1)
			{
				Set[Randtile[i]].a1.y +=0.3f;
				Set[Randtile[i]].b1.y +=0.3f;
				Set[Randtile[i]].c1.y +=0.3f;
				Set[Randtile[i]].d1.y +=0.3f;
			}
			if(randyflag[i] == 0)
			{
				Set[Randtile[i]].a1.y -=0.3f;
				Set[Randtile[i]].b1.y -=0.3f;
				Set[Randtile[i]].c1.y -=0.3f;
				Set[Randtile[i]].d1.y -=0.3f;
			}
			if(Set[Randtile[i]].a1.y <=0.0f)
				randyflag[i]=1;
			if(Set[Randtile[i]].a1.y >=1.9f)
				randyflag[i]=0;
		}
		timercheck = 0;
	}
	else
		timercheck++;
	//For Health bar and Lives
	if(Health <= 0.0f)
	{
		Life--;
		robocoin=0;
		Health = 0.8f;
		if(Life ==0)
		{

			cout<<"GAME OVER!!! :( \n";
			cout<<"Score: "<<robocoin<<"\n";
			exit(0);
		}
		else
		{
			initialize();
		}
	}

	if(ob1.pos.z <0.0f || ob1.pos.z > 10.0f || ob1.pos.x < 0.0f || ob1.pos.x > 10.0f || gapcheck(ob1.tilenum))
	{
		Life--;
		robocoin = 0;
		Health = 0.8f;
		if(Life == 0)
		{
			endflag = 1;
			if(endtimer > 50)
			{
				cout<<"GAME OVER!!! :( \n";
				cout<<"Score: "<<robocoin<<"\n";
				exit(0);
			}
		}
		else
		{
			initialize();
		}
	}
	if(blastcheck(ob1.tilenum) !=-1)
		if(Blastlife[blastcheck(ob1.tilenum)]==0)
		{
			Life--;
			robocoin = 0;
			Health = 0.8f;
			if(Life ==0)
			{
				cout<<"GAME OVER!!! :( \n";
				cout<<"Score: "<<robocoin<<"\n";
				exit(0);
			}
			else
			{
				initialize();
			}
		}

	// For falling tiles;
	for(int i=0;i<3;i++)
	{
		if(blastflag[i]==1 && Blastlife[i]==1)
		{
			Set[Blasttile[i]].a1.y -=0.005f;
			Set[Blasttile[i]].b1.y -=0.005f;
			Set[Blasttile[i]].c1.y -=0.005f;
			Set[Blasttile[i]].d1.y -=0.005f;
		}
		if( Set[Blasttile[i]].a1.y <= 0.0f)
			Blastlife[i]=0;

	}




	glutTimerFunc(50, update, 0);
}


void reshape(int w, int h)
{
	glViewport(0, 0, w, h);
	glMatrixMode(GL_PROJECTION);  
	glLoadIdentity();
	//Angle of view:40 degrees
	//Near clipping plane distance: 0.5
	//Far clipping plane distance: 20.0

	gluPerspective(40.0,(GLdouble)w/(GLdouble)h,0.5,30.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

int main(int argc, char** argv){

	glutInit(&argc, argv);
	//we initizlilze the glut. functions
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	w = glutGet(GLUT_SCREEN_WIDTH);
	h = glutGet(GLUT_SCREEN_HEIGHT);
	windowWidth = w * 2 / 3;
	windowHeight = h * 2 / 3;

	glutInitWindowSize(windowWidth, windowHeight);
	glutInitWindowPosition((w - windowWidth) / 2, (h - windowHeight) / 2);

	initialize();
	glutCreateWindow("Game");
	init();
	glutDisplayFunc(DrawScene);
	glutReshapeFunc(reshape);
	//Set the function for the animation.
	glutKeyboardFunc(handleKeypress1);
	glutSpecialFunc(handleKeypress2);
	glutSpecialUpFunc(handleKeypress3);
	glutPassiveMotionFunc(mouseHandler);
	glutIdleFunc(DrawScene);
	glutTimerFunc(timer, update, 0);
	glutMainLoop();
	return 0;
}

void handleKeypress1(unsigned char key, int x, int y) 
{
	if (key == 27) 
		exit(0);     // escape key is pressed
	else if(key == 'j')
	{
		jump=1;
		jflag=0;
		jbegflag=0;
		jumptemp = Set[ob1.tilenum].a1.y;
	}
	else if(key == 't')
	{
		tflag = (tflag+1)%2;
	}
	else if(key == 'f')
	{
		fflag = (fflag+1)%2;
	}
	else if(key == 'o')
	{
		oflag = (tflag+1)%2;
	}
	else if(key == 'x')
	{
		xrotate -=5.0f;
		if(xrotate > 360.0f)
			xrotate=0.0f;
	}
	else if(key == 'y')
	{
		yrotate +=5.0f;
		if(yrotate > 540.0f)
			yrotate=180.0f;
	}
	else if(key == 'z')
	{
		zrotate -=5.0f;
		if(zrotate > 360.0f)
			zrotate=0.0f;
	}
	else if(key == 'p')
	{
		if(telecheck(ob1.tilenum) != -1)
		{
			ob1.tilenum = Telettile[telecheck(ob1.tilenum)];
			ob1.pos.x = Set[ob1.tilenum].a1.x+ 0.35;
			ob1.pos.y = Set[ob1.tilenum].a1.y;
			ob1.pos.z = Set[ob1.tilenum].a1.z+ 0.35;
		}
	}
	else if(key == 'h')
	{
		hflag= (hflag +1)%2;
	}
	else if(key == 'w')
	{
		if(hflag ==1)
		{
			hel_at.z += 0.1;
			hel_look.z += 0.1;
		}
		else if( vflag == 1)
		{
			if(vtilenum <90)
				vtilenum+=10;
		}
	}
	else if(key == 'c')
	{
		if(hflag ==1)
		{
			hel_at.z -= 0.1;
			hel_look.z -= 0.1;
		}
		else if( vflag == 1)
		{
			if(vtilenum >=10)
				vtilenum-=10;
		}
	}
	else if(key == 'a')
	{
		if(hflag ==1)
		{
			hel_at.x += 0.1;
			hel_look.x += 0.1;
		}
		else if( vflag == 1)
		{
			if(vtilenum%10 != 9 )
				vtilenum++;
		}
	}
	else if(key == 's')
	{
		if(hflag ==1)
		{
			hel_at.x -= 0.1;
			hel_look.x -= 0.1;
		}
		else if( vflag == 1)
		{
			if(vtilenum%10 !=0)
				vtilenum--;
		}
	}
	else if(key == 'q')
	{
		hel_at.y -= 0.1;
	}
	else if(key == 'e')
	{
		hel_at.y += 0.1;
	}
	else if(key == 'b')
	{
		bflag = (bflag +1)%2;
	}
	else if(key == 'l')
	{
		lflag = (lflag +1)%2;
		if(lflag == 1)
			glEnable(GL_LIGHTING); //Enable lighting
		else
			glDisable(GL_LIGHTING); //Enable lighting

	}
	else if(key == 'r')
	{
		rflag = (rflag +1)%2;
	}
	else if(key == 'v')
	{
		vflag = (vflag +1)%2;
	}






}




void handleKeypress2(int key, int x, int y) 
{
	//	int bodymovflag = 0;
	if (key == GLUT_KEY_LEFT)
	{
		//		if(ob1.tilenum>89)
		//		{}
		//		else
		//		{
		//	ob1.pos = Set[ob1.tilenum + 10].obpos;
		//	ob1.tilenum = ob1.tilenum + 10;
		//		bodymovflag=1;
		face +=90;
		while(face < -180)
			face += 360;
		while(face > 360)
			face -= 360;
		if(face == 360)
			face = 0;
		if(face == -180)
			face = 180;



	}

	if (key == GLUT_KEY_RIGHT)
	{
		face -= 90;
		while(face < -180)
			face += 360;
		while(face > 360)
			face -= 360;
		if(face == 360)
			face = 0;
		if(face == -180)
			face = 180;
	}

	if (key == GLUT_KEY_UP)
	{
		if( face == 90 || face == -270)
		{
			ob1.pos.x += mov;
			ob1.relpos.x += mov;
			if(ob1.relpos.x > 1.0f)
			{
				if(Set[ob1.tilenum+1].a1.y == ob1.pos.y)
				{
					if(blockcheck(ob1.tilenum +1))
					{
						ob1.pos.x -=0.3;
						ob1.relpos.x -= 0.3;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.x = 0;
						ob1.tilenum++;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;

					}
				}
				else if(Set[ob1.tilenum+1].a1.y < ob1.pos.y)
				{
					if((Set[ob1.tilenum+1].a1.y-ob1.pos.y)<=-0.7f)
						Health -=0.2;
					if(blockcheck(ob1.tilenum +1))
					{
						ob1.pos.x -=0.3;
						ob1.relpos.x -= 0.3;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.x = 0;
						ob1.tilenum++;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
						ob1.pos.y = Set[ob1.tilenum].a1.y;
					}
				}
				else
				{
					ob1.pos.x -=mov;
					ob1.relpos.x -=mov;
				//	ob1.pos.x -= 0.5;
				//	ob1.relpos.x -=0.5;
				}

			}
			//		ob1.pos.y = Set[ob1.tilenum].a1.y;
			jumpdirflag =1;

		}
		if(face == -90 || face == 270)
		{
			ob1.pos.x -= mov;
			ob1.relpos.x -= mov;
			if(ob1.relpos.x < 0.0f)
			{
				if(Set[ob1.tilenum-1].a1.y == ob1.pos.y)
				{
					if((Set[ob1.tilenum-1].a1.y-ob1.pos.y)<=-0.7f)
						Health -=0.2;
					if(blockcheck(ob1.tilenum -1))
					{
						ob1.pos.x +=0.3;
						ob1.relpos.x += 0.3;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.x = 1;
						ob1.tilenum--;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
					}
				}
				else if(Set[ob1.tilenum-1].a1.y < ob1.pos.y)
				{
					if(blockcheck(ob1.tilenum -1))
					{
						ob1.pos.x +=0.3;
						ob1.relpos.x += 0.3;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.x = 1;
						ob1.tilenum--;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
						ob1.pos.y = Set[ob1.tilenum].a1.y;
					}
				}
				else
				{
					ob1.pos.x +=mov;
					ob1.relpos.x +=mov;
					//ob1.pos.x += 0.5;
					//ob1.relpos.x +=0.5;
				}

			}
			jumpdirflag =2;
		}
		if(face == 0 || face == 360)
		{
			ob1.pos.z += mov;
			ob1.relpos.z += mov;
			if(ob1.relpos.z > 1.0f)
			{
				if(Set[ob1.tilenum+10].a1.y == ob1.pos.y)
				{
					if((Set[ob1.tilenum+10].a1.y-ob1.pos.y)<=-0.7f)
						Health -=0.2;
					if(blockcheck(ob1.tilenum +10))
					{
						ob1.pos.z -=0.3;
						ob1.relpos.z -= 0.3;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.z = 0;
						ob1.tilenum +=10;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
					}
				}
				else if(Set[ob1.tilenum+10].a1.y < ob1.pos.y)
				{
					if(blockcheck(ob1.tilenum +10))
					{
						ob1.pos.z -=0.3;
						ob1.relpos.z -= 0.3;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.z = 0;
						ob1.tilenum +=10;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
						ob1.pos.y = Set[ob1.tilenum].a1.y;
					}
				}
				else
				{
					ob1.pos.z -=mov;
					ob1.relpos.z -=mov;
				//	ob1.pos.z -= 0.5;
				//	ob1.relpos.z -=0.5;
				}
			}
			jumpdirflag =3;
		}
		if(face == 180 || face == -180)
		{
			ob1.pos.z -= mov;
			ob1.relpos.z -= mov;
			if(ob1.relpos.z < 0.0f)
			{
				if(Set[ob1.tilenum-10].a1.y == ob1.pos.y)
				{
					if((Set[ob1.tilenum-10].a1.y-ob1.pos.y)<=-0.7f)
						Health -=0.2;
					if(blockcheck(ob1.tilenum -10))
					{
						ob1.pos.z +=0.3;
						ob1.relpos.z += 0.3;
						Health -=0.2;
					}
					else
					{
						ob1.relpos.z = 1;
						ob1.tilenum -= 10;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
					}
				}
				else if(Set[ob1.tilenum-10].a1.y < ob1.pos.y)
				{
					if(blockcheck(ob1.tilenum -10))
					{
						ob1.pos.z +=0.3;
						ob1.relpos.z += 0.3;
						Health -=0.2;
					}
					else{
						ob1.relpos.z = 1.0f;
						ob1.tilenum -= 10;
						if(blastcheck(ob1.tilenum) != -1)
							blastflag[blastcheck(ob1.tilenum)]=1;
						ob1.pos.y = Set[ob1.tilenum].a1.y;
					}
				}
				else
				{
					ob1.pos.z +=mov;
					ob1.relpos.z +=mov;
				//	ob1.pos.z += 0.5;
				//	ob1.relpos.z +=0.5;
				}
			}
			jumpdirflag =4;
		}
		movcnt +=1;

	}

	if (key == GLUT_KEY_DOWN)
	{
		face -= 180;
		while(face < -180)
			face += 360;
		while(face > 360)
			face -= 360;
		if(face == 360)
			face = 0;
		if(face == -180)
			face = 180;

	}


}

void handleKeypress3(int key,int x,int y)
{
	if(key == GLUT_KEY_UP || key == GLUT_KEY_DOWN || key == GLUT_KEY_LEFT || key == GLUT_KEY_RIGHT)
	{
		//		movcnt = 1;
		jumpdirflag=0;
	}
}
void mouseHandler(int width_world, int height_world)
{
	glPushMatrix();
	glEnable(GL_DEPTH);
	glEnable(GL_DEPTH_COMPONENT);
	glGetDoublev(GL_MODELVIEW_MATRIX,modelview);
	glGetDoublev(GL_PROJECTION_MATRIX,projection);
	glGetIntegerv(GL_VIEWPORT,viewport);

	float x,y,z;
	x = (float)width_world;
	y = (float)viewport[3] - (float)height_world;
	glReadPixels((int)x,(int)y,1,1,GL_DEPTH_COMPONENT,GL_FLOAT,&z);
	gluUnProject(x,y,z,modelview,projection,viewport,&mouse_x,&mouse_y,&mouse_z);
	if(hflag == 1)
		helview();

	glPopMatrix();
}
void helview()
{
	if(pmouse_x < mouse_x)
		xhrotate -=3;
	else
		xhrotate +=3;
	pmouse_x = mouse_x;
	pmouse_y = mouse_y;
	pmouse_z = mouse_z;

}

void drawcoin(float x,float y,float z){
	glPushMatrix();
	glTranslatef(x,y,z);
	glScalef(0.1,0.1,0.1);
	glRotatef(coinangle,0,1,0);
	GLfloat ambientColor[] = {1.2f, 1.2f, 1.2f, 0.0f}; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor);
	glColor3f(25,215,0);
	glBegin(GL_QUADS);
	glNormal3f(0,1,0);
	glVertex3f( 0.0f, 1.0f,0.0f);
	glVertex3f( 1.0f, 0.0f,0.0f);
	glVertex3f( 0.0f, -1.0f,0.0f);
	glVertex3f( -1.0f, 0.0f,0.0f);
/*	glNormal3f(1,1,0);
	glVertex3f( 0.0f, 1.5f,0.0f);
	glVertex3f( 1.0f, 0.0f,1.0f);
	glVertex3f( 1.0f, 0.0f,-1.0f);
	glNormal3f(0,1,-1);
	glVertex3f( 0.0f, 1.5f,0.0f);
	glVertex3f( 1.0f, 0.0f,-1.0f);
	glVertex3f( -1.0f, 0.0f,-1.0f);
	glNormal3f(-1,1,0);
	glVertex3f( 0.0f, 1.5f,0.0f);
	glVertex3f( -1.0f, 0.0f,-1.0f);
	glVertex3f( -1.0f, 0.0f,1.0f);
	glNormal3f(0,1,1);
	glVertex3f( 0.0f, 1.5f,0.0f);
	glVertex3f( -1.0f, 0.0f,1.0f);
	glVertex3f( 1.0f, 0.0f,1.0f);*/
	
	glEnd();
	GLfloat ambientColor1[] = {0.4f, 0.4f, 0.4f, 0.0f}; //Color (0.2, 0.2, 0.2)
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, ambientColor1);
	glPopMatrix();
}

void catchcoin()
{
	if(Set[ob1.tilenum].coinflag == 1)
	{
		Point coinp;
		coinp.x = Set[ob1.tilenum].a1.x+0.5;
		coinp.y = Set[ob1.tilenum].a1.y+1.0;
		coinp.z = Set[ob1.tilenum].a1.z+0.5;

		if((coinp.x<(ob1.pos.x+0.3)) && (coinp.x >(ob1.pos.x-0.3)))
			if((coinp.z<(ob1.pos.z+0.3)) && (coinp.z >(ob1.pos.z-0.3)))
				if((coinp.y<(ob1.pos.y+0.7)) && (coinp.y >(ob1.pos.y-0.2)))
				{
					Set[ob1.tilenum].coinflag=0;
					robocoin++;
				}
	}
	int i;
	for(i=0;i<3;i++)
		if(ob1.tilenum == Healthtile[i])
			break;
	if(i!=3)
	{
		if(healthflag[i]==1)
		{
			healthflag[i] =0;
			Health =0.8;
		}
	}
}



void drawplus(GLfloat x,GLfloat y,GLfloat z)
{
	glColor3f(1,0,0);
	glPushMatrix();
	glTranslatef(x,y,z);
	glRotatef(coinangle,0,1,0);
	glScalef(0.1,0.1,0.1);
	glBegin(GL_QUADS);
	glVertex3f( -0.25f, 1.0f,0.0f);
	glVertex3f( 0.25f, 1.0f,0.0f);
	glVertex3f( 0.25f, -1.0f,0.0f);
	glVertex3f( -0.25f, -1.0f,0.0f);
	glVertex3f( -1.0f, 0.25f,0.0f);
	glVertex3f( -1.0f, -0.25f,0.0f);
	glVertex3f( 1.0f, -0.25f,0.0f);
	glVertex3f( 1.0f, 0.25f,0.0f);
	glEnd();
	glPopMatrix();
}

void drawf(void){
	glColor3f(0,1,1);
	glPushMatrix();
	glTranslatef(Set[fogtile].a1.x+0.5,Set[fogtile].a1.y+0.5,Set[fogtile].a1.z + 0.5);
	glRotatef(coinangle,0,1,0);
	glScalef(0.1,0.1,0.1);
	glBegin(GL_QUADS);
	glVertex3f( -1.0f, 0.25f,0.0f);
	glVertex3f( -1.0f, -0.25f,0.0f);
	glVertex3f( 1.0f, -0.25f,0.0f);
	glVertex3f( 1.0f, 0.25f,0.0f);
	glEnd();
	glPopMatrix();
}

void lasercheck()
{
	float ax = ob1.pos.x + 0.2;
	float az = ob1.pos.z + 0.2;
	float bx = ob1.pos.x - 0.2;
	float bz = ob1.pos.z - 0.2;
	
	float cx = 10.0f;
	float cz = 0.0f;
	float dx = laser1;
	float dz = 10.0f;

	float t1 = (cx-dx)*(az-dz) - (cz-dz)*(ax-dx);
	float t2 = (cx-dx)*(bz-dz) - (cz-dz)*(bx-dx);

	if((t1*t2) <= 0)
	{
		if((ob1.pos.y + 0.5) > 1.0f && ob1.pos.y < 1.0f)
			Health -= 0.5f;	
		else
		{
			if(Health < 0.8f)
				Health += 0.2f;
		}
	}
	cx = 0.0f;
	cz = 5.0f;
	dx = 10.0f;
	dx = laser2;

	t1 = (cx-dx)*(az-dz) - (cz-dz)*(ax-dx);
	t2 = (cx-dx)*(bz-dz) - (cz-dz)*(bx-dx);
	if((t1*t2) <= 0)
	{
		cout<<"yes\n";
		if((ob1.pos.y + 0.5) > 0.5f && ob1.pos.y < 0.5f)
			Health -= 0.5f;	
		else
		{
			if(Health < 0.8f)
				Health += 0.2f;
		}
	}


}

#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>
#include <stdlib.h>
#include <time.h>

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

class Source {
public:
	int q;
	double x, y, z;
	double speed;
	double cR, cG, cB;
	double size;
};

class Point {
public:
	double x, y, z;
	Point(double x1, double y1, double z1)
	{
		x = x1; y = y1; z = z1;
	}
};

class Particle {
public:
	double x, y, z;
	double r, g, b;
	double v;
	int t;
	double sX, sY, sZ;
	double osX, osY, osZ;
	double angle;
	double size;
};

Source s;
vector<Particle> particles;

//*********************************************************************************
//	Pokazivac na glavni prozor i pocetna velicina.
//*********************************************************************************

GLuint window;
GLuint sub_width = 800, sub_height = 800;

//*********************************************************************************
//	Function Prototypes.
//*********************************************************************************

void myDisplay();
void myIdle();
void myReshape(int width, int height);
void myKeyboard(unsigned char theKey, int mouseX, int mouseY);

void drawParticles();
void drawParticle(Particle c);
GLuint LoadTexture(const char* filename);

GLuint tex;
Point ociste(0.0, 5.0, 0.0);
int tempQ = -1;
double pi = 3.1415926535;

int main(int argc, char** argv)
{
	s.q = 21;
	s.x = -20.0; s.y = 30; s.z = -75.0;
	s.speed = 0.2;
	s.cB = 1.0; s.cG = 1.0; s.cR = 1.0;
	s.size = 0.4;
	srand(time(NULL));


	// Nakon ucitavanja svih potrebnih podataka
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(sub_width, sub_height);
	glutInitWindowPosition(250, 25);
	glutInit(&argc, argv);

	window = glutCreateWindow("Vjezba");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutKeyboardFunc(myKeyboard);
	glutIdleFunc(myIdle);
	tex = LoadTexture("snow.bmp");

	glBlendFunc(GL_SRC_ALPHA, GL_ONE);
	glEnable(GL_BLEND);


	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, tex);

	glutMainLoop();
	return 0;
}

void myReshape(int width, int height)
{
	sub_width = width;                      	//promjena sirine prozora
	sub_height = height;						//promjena visine prozora

	glViewport(0, 0, sub_width, sub_height);	//  otvor u prozoru

	//-------------------------
	glMatrixMode(GL_PROJECTION);                // Select The Projection Matrix
	glLoadIdentity();                           // Reset The Projection Matrix

	// Calculate The Aspect Ratio Of The Window
	gluPerspective(45.0f, (GLfloat)width / (GLfloat)height, 0.1f, 150.0f);

	glMatrixMode(GL_MODELVIEW);                 // Select The Modelview Matrix
	glLoadIdentity();                           // Reset The Modelview Matrix
	//-------------------------

	glLoadIdentity();							//	jedinicna matrica
	glClearColor(0.0f, 0.0f, 0.0f, 0.0f);		//	boja pozadine
	glClear(GL_COLOR_BUFFER_BIT);				//	brisanje pozadine
	glPointSize(1.0);							//	postavi velicinu tocke
	glColor3f(0.0f, 0.0f, 0.0f);				//	postavi boju
}

int t = 0;

void myDisplay()
{

	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();

	glTranslatef(ociste.x, ociste.y, ociste.z);

	drawParticles();

	glutSwapBuffers();
}

int currentTime = 0; int previousTime = 0;

void myIdle()
{
	int interval = 50;
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;
	if (timeInterval > interval) {

		if (s.q > 0) {
			int n = (int) (((double) rand() / RAND_MAX) * s.q);

			// create particles
			for (int j = 0; j < n; j++) {
				double x, y, z;
				x = ((double)rand() / RAND_MAX) - 0.5;
				y = -((double)rand() / RAND_MAX);
				z = ((double)rand() / RAND_MAX) - 0.5;

				Particle c;
				c.x = s.x;
				c.z = s.z;
				c.y = s.y;

				c.r = s.cR;
				c.g = s.cG;
				c.b = s.cB;

				c.v = s.speed;

				c.sX = x;
				c.sY = y;
				c.sZ = z;

				c.t = 500 + (((double) rand() / RAND_MAX) * 251 - 125);
				c.size = s.size;
				particles.push_back(c);
			}
		}
		printf("%d\n", particles.size());

		// move, rotate and destroy particles
		for (int j = particles.size() - 1; j >= 0; j--) {

			Point e(0.0, 0.0, 0.0);
			e.x = particles.at(j).x - ociste.x;
			e.y = particles.at(j).y - ociste.y;
			e.z = particles.at(j).z - ociste.z;

			Point s(0.0, 0.0, 1.0);
			Point os(0.0, 0.0, 0.0);
			os.x = s.y * e.z - e.y * s.z;
			os.y = e.x * s.z - s.x * e.z;
			os.z = s.x * e.y - s.y * e.x;

			double normS = pow(pow((double)s.x, 2.0) + pow((double)s.y, 2.0) + pow((double)s.z, 2.0), 0.5);
			double normE = pow(pow((double)e.x, 2.0) + pow((double)e.y, 2.0) + pow((double)e.z, 2.0), 0.5);
			double se = s.x * e.x + s.y * e.y + s.z * e.z;
			double angle = acos(se / (normS * normE));
			particles.at(j).angle = angle / (2 * pi) * 360;
			particles.at(j).osX = os.x;
			particles.at(j).osY = os.y;
			particles.at(j).osZ = os.z;

			particles.at(j).y += particles.at(j).v * particles.at(j).sY;
			particles.at(j).z += particles.at(j).v * (particles.at(j).sZ + ((double)rand() / RAND_MAX));
			particles.at(j).x += particles.at(j).v * (particles.at(j).sX + particles.at(j).y / 15);

			particles.at(j).t--;

			if (particles.at(j).r > 0) particles.at(j).r -= 0.002;
			if (particles.at(j).g < 1.0) particles.at(j).g += 0.002;

			if (particles.at(j).t <= 0) {
				particles.erase(particles.begin() + j);
			}
		}

		myDisplay();
		previousTime = currentTime;
	}
}

void drawParticles()
{
	for (int j = 0; j < particles.size(); j++) {
		drawParticle(particles.at(j));
	}
}

void drawParticle(Particle c)
{

	glColor3f(c.r, c.g, c.b);
	glTranslatef(c.x, c.y, c.z);
	glRotatef(c.angle, c.osX, c.osY, c.osZ);
	glBegin(GL_QUADS);

	glTexCoord2d(0.0, 0.0); 
	glVertex3f(-c.size, -c.size, 0.0);

	glTexCoord2d(1.0, 0.0);
	glVertex3f(-c.size, +c.size, 0.0);

	glTexCoord2d(1.0, 1.0); 
	glVertex3f(+c.size, +c.size, 0.0);

	glTexCoord2d(0.0, 1.0); 
	glVertex3f(+c.size, -c.size, 0.0);

	glEnd();
	glRotatef(-c.angle, c.osX, c.osY, c.osZ);
	glTranslatef(-c.x, -c.y, -c.z);
}

void myKeyboard(unsigned char theKey, int mouseX, int mouseY)
{
	// change source position
	if (theKey == 'a') s.x -= 0.5;
	if (theKey == 'd') s.x += 0.5;
	if (theKey == 's') s.y -= 0.5;
	if (theKey == 'w') s.y += 0.5;
	if (theKey == 'q') s.z -= 0.5;
	if (theKey == 'e') s.z += 0.5;

	// change max created particles in each moment
	if (theKey == 'o' && s.q > 0) s.q--;
	if (theKey == 'p' && s.q < 49) s.q++;

	// change color of particles
	if (theKey == 't' && s.cR > 0.0) s.cR -= 0.02;
	if (theKey == 'z' && s.cR < 1.0) s.cR += 0.02;
	if (theKey == 'g' && s.cG > 0.0) s.cG -= 0.02;
	if (theKey == 'h' && s.cG < 1.0) s.cG += 0.02;
	if (theKey == 'b' && s.cB > 0.0) s.cB -= 0.02;
	if (theKey == 'n' && s.cB < 1.0) s.cB += 0.02;

	// change size of particles
	if (theKey == '+' && s.size < 4.0) s.size += 0.01;
	if (theKey == '-' && s.size > 0.02) s.size -= 0.01;

	// change position of ociste
	if (theKey == '2') ociste.y += 0.1;
	if (theKey == '8') ociste.y -= 0.1;
	if (theKey == '6') ociste.x -= 0.1;
	if (theKey == '4') ociste.x += 0.1;

	// shut down or start up creation of new particles
	if (theKey == 'u') {
		if (tempQ == -1) {
			tempQ = s.q;
			s.q = 0;
		}
		else {
			s.q = tempQ;
			tempQ = -1;
		}
	}
}

// load a 256x256 RGB .bmp file as a texture
GLuint LoadTexture(const char* filename)
{
	GLuint texture;
	int width, height;
	BYTE* data;
	FILE* file;
	// open texture data
	file = fopen(filename, "rb");
	if (file == NULL) {
		return 0;
	}

	// get memory
	width = 256;
	height = 256;
	data = (BYTE*)malloc(width * height * 3);

	// read texture
	fread(data, width * height * 3, 1, file);
	fclose(file);

	// prepare texture name
	glGenTextures(1, &texture);

	// select the texture
	glBindTexture(GL_TEXTURE_2D, texture);

	// select texture environment parameters, specifies how texture values are interpreted when a fragment is textured
	glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);

	// interpolation for small and large texture
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR_MIPMAP_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	// build texture mipmaps
	gluBuild2DMipmaps(GL_TEXTURE_2D, 3, width, height,
		GL_RGB, GL_UNSIGNED_BYTE, data);

	// free memory
	free(data);

	return texture;
}
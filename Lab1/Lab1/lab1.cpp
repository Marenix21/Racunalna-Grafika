#ifdef _WIN32
#include <windows.h>
#endif

#include <GL/glut.h>
#include <stdio.h>
#include <math.h>
#include <windows.h>

#include <string>
#include <fstream>
#include <vector>
#include <iostream>

using namespace std;

int brF = 0, brV = 0, brB = 0;
int brS = 0;

class Vrh {
public:
	float x, y, z;
	Vrh(float x1, float y1, float z1)
	{
		x = x1; y = y1; z = z1;
	}
};
class Poligon {
public:
	Vrh* v1, * v2, * v3;
	Poligon(Vrh* vX, Vrh* vY, Vrh* vZ)
	{
		v1 = vX; v2 = vY; v3 = vZ;
	}
};


Poligon* poligoni;
Vrh* vrhovi, * vrhB, * bSpline, * tangente;
Vrh srediste(0.0, 0.0, 0.0), os(0.0, 0.0, 0.0);
Vrh s(1.0, 0.0, 0.0), e(0.0, 0.0, 0.0);
float pi = 3.14159265;
double tockeUSegmentu = 100;
int dio = 14;

//*********************************************************************************
//	Pokazivac na glavni prozor i pocetna velicina.
//*********************************************************************************

GLuint window;
GLuint width = 800, height = 800;

//*********************************************************************************
//	Function Prototypes.
//*********************************************************************************

void myDisplay();
void myIdle();
void myReshape(int w, int h);


//*********************************************************************************
//	Glavni program.
//*********************************************************************************


int main(int argc, char** argv)
{
	vector<string> text_file;

	ifstream ifs("Ncc.obj");
	string temp;

	while (getline(ifs, temp))
		text_file.push_back(temp);						// Uzmi sve linije iz datoteke

	// Prebroji broj vrhova i poligona
	for (int i = 0; i < text_file.size(); i++) {
		if (text_file.at(i).size() != 0) {
			if (text_file.at(i).at(0) == 'f') brF++;
			else if (text_file.at(i).at(0) == 'v') brV++;
		}
	}

	// Alociraj memoriju za vrhove i poligone
	vrhovi = (Vrh*)malloc(brV * sizeof(Vrh));
	poligoni = (Poligon*)malloc(brF * sizeof(Poligon));


	int indexVrhovi = 0, indexPoligoni = 0;

	for (int i = 0; i < text_file.size(); i++) {
		if (text_file.at(i).size() != 0) {
			char* text = new char[text_file.at(i).size() + 1];
			std::copy(text_file.at(i).begin(), text_file.at(i).end(), text);
			if (text_file.at(i).at(0) == 'v') {
				float d1, d2, d3;
				sscanf_s(text, "v %f %f %f", &d1, &d2, &d3);

				d1 *= 5; 
				d2 *= 5; 
				d3 *= 5;

				srediste.x += d1; srediste.y += d2; srediste.z += d3;
				Vrh v(d1, d2, d3);
				vrhovi[indexVrhovi] = v;
				indexVrhovi++;
			} else if (text_file.at(i).at(0) == 'f') {
				int d1, d2, d3;
				sscanf_s(text, "f %d %d %d", &d1, &d2, &d3);
				Poligon p(&vrhovi[d1 - 1], &vrhovi[d2 - 1], &vrhovi[d3 - 1]);
				poligoni[indexPoligoni] = p;
				indexPoligoni++;
			}
		}
	}
	srediste.x /= indexVrhovi; srediste.y /= indexVrhovi; srediste.z /= indexVrhovi;

	text_file.clear();
	ifstream ifs2("bSpline.txt");

	while (getline(ifs2, temp))
		text_file.push_back(temp);

	brB = text_file.size();
	vrhB = (Vrh*)malloc(brB * sizeof(Vrh));
	brS = brB - 3;								// n-3 segmenta


	// stvori vrh za svaki redak u datoteci
	for (int i = 0; i < brB; i++) {
		float d1, d2, d3;
		char* text = new char[text_file.at(i).size() + 1];
		std::copy(text_file.at(i).begin(), text_file.at(i).end(), text);
		sscanf_s(text, "%f %f %f", &d1, &d2, &d3);
		Vrh v(d1, d2, d3);
		vrhB[i] = v;
	}

	// stvori segment uzimajuci po 4 vrha
	for (int i = 0; i < brS; i++) {
		Vrh v1 = vrhB[i];
		Vrh v2 = vrhB[i + 1];
		Vrh v3 = vrhB[i + 2];
		Vrh v4 = vrhB[i + 3];
	}


	bSpline = (Vrh*)malloc((int)tockeUSegmentu * brS * sizeof(Vrh));
	tangente = (Vrh*)malloc(brS * 2 * (int)tockeUSegmentu * sizeof(Vrh));
	int brojSvihTan = 0;
	// Za svaki segment
	for (int i = 0; i < brS; i++) {
		Vrh v0 = vrhB[i];
		Vrh v1 = vrhB[i + 1];
		Vrh v2 = vrhB[i + 2];
		Vrh v3 = vrhB[i + 3];

		// Faktor t raste od 0 do 1 (k je pomocna varijabla)
		for (int k = 0; k < tockeUSegmentu; k++) {
			double t = k / tockeUSegmentu;
			float t11 = (-pow(t, 3.0) + 3 * pow(t, 2.0) - 3 * t + 1) / 6.0;
			float t12 = (3 * pow(t, 3.0) - 6 * pow(t, 2.0) + 4) / 6.0;
			float t13 = (-3 * pow(t, 3.0) + 3 * pow(t, 2.0) + 3 * t + 1) / 6.0;
			float t14 = pow(t, 3.0) / 6.0;

			// derivacija
			float t21 = 0.5 * (-pow(t, 2.0) + 2 * t - 1);
			float t22 = 0.5 * (3 * pow(t, 2.0) - 4 * t);
			float t23 = 0.5 * (-3 * pow(t, 2.0) + 2 * t + 1);
			float t24 = 0.5 * (pow(t, 2.0));

			bSpline[(int)tockeUSegmentu * i + k].x = t11 * v0.x + t12 * v1.x + t13 * v2.x + t14 * v3.x;
			bSpline[(int)tockeUSegmentu * i + k].y = t11 * v0.y + t12 * v1.y + t13 * v2.y + t14 * v3.y;
			bSpline[(int)tockeUSegmentu * i + k].z = t11 * v0.z + t12 * v1.z + t13 * v2.z + t14 * v3.z;

			tangente[brojSvihTan].x = bSpline[(int)tockeUSegmentu * i + k].x;
			tangente[brojSvihTan].y = bSpline[(int)tockeUSegmentu * i + k].y;
			tangente[brojSvihTan].z = bSpline[(int)tockeUSegmentu * i + k].z;
			brojSvihTan++;

			float vx = t21 * v0.x + t22 * v1.x + t23 * v2.x + t24 * v3.x;
			float vy = t21 * v0.y + t22 * v1.y + t23 * v2.y + t24 * v3.y;
			float vz = t21 * v0.z + t22 * v1.z + t23 * v2.z + t24 * v3.z;

			tangente[brojSvihTan].x = tangente[brojSvihTan - 1].x + vx / 2;
			tangente[brojSvihTan].y = tangente[brojSvihTan - 1].y + vy / 2;
			tangente[brojSvihTan].z = tangente[brojSvihTan - 1].z + vz / 2;
			brojSvihTan++;
		}

	}

	printf("Napravljeno je %d tocaka tangenti\n", brojSvihTan);

	// Nakon ucitavanja svih potrebnih podataka
	glutInitDisplayMode(GLUT_SINGLE | GLUT_RGB);
	glutInitWindowSize(width, height);
	glutInitWindowPosition(250, 25);
	glutInit(&argc, argv);

	window = glutCreateWindow("Animacija objekta");
	glutReshapeFunc(myReshape);
	glutDisplayFunc(myDisplay);
	glutIdleFunc(myIdle);

	glutMainLoop();
	return 0;
}

void myReshape(int w, int h)
{
	width = w, height = h;                      // promjena sirine i visine prozora
	glViewport(0, 0, width, height);			//  otvor u prozoru

	//-------------------------
	glMatrixMode(GL_PROJECTION);                // matrica projekcije
	glLoadIdentity();                           // jedinicna matrica

	// kut pogleda, x/y, prednja i straznja ravnina odsjecanja
	gluPerspective(45.0f, (GLfloat)w / (GLfloat)h, 0.1f, 100.0f);

	glMatrixMode(GL_MODELVIEW);                 // matrica pogleda
	glLoadIdentity();                           // jedinicna matrica
	//-------------------------
	glLoadIdentity();							//	jedinicna matrica
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);		//	boja pozadine
	glClear(GL_COLOR_BUFFER_BIT);				//	brisanje pozadine
	glPointSize(1.0);							//	postavi velicinu tocke
	glColor3f(0.0f, 0.0f, 0.0f);				//	postavi boju
}

int t = 0;

void myDisplay()
{
	glLoadIdentity();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glTranslatef(-5.0, -30.0, -75.0);
	glRotatef(-45.0, 1.0, 0.0, 0.0);
	glBegin(GL_LINE_STRIP);
	for (int i = 0; i < tockeUSegmentu * brS; i++) {
		glVertex3f(bSpline[i].x, bSpline[i].y, bSpline[i].z);
	}
	glEnd();
	glBegin(GL_LINES);
	for (int i = 0; i < brS * 2 * tockeUSegmentu; i += dio) {
		glVertex3f(tangente[i].x, tangente[i].y, tangente[i].z);
		glVertex3f(tangente[i + 1].x, tangente[i + 1].y, tangente[i + 1].z);
	}
	glEnd();

	glTranslatef(bSpline[t].x, bSpline[t].y, bSpline[t].z);

	e.x = tangente[2 * t + 1].x - tangente[2 * t].x;
	e.y = tangente[2 * t + 1].y - tangente[2 * t].y;
	e.z = tangente[2 * t + 1].z - tangente[2 * t].z;

	os.x = s.y * e.z - e.y * s.z;
	os.y = e.x * s.z - s.x * e.z;
	os.z = s.x * e.y - s.y * e.x;

	double apsS = sqrt(pow((double)s.x, 2.0) + pow((double)s.y, 2.0) + pow((double)s.z, 2.0));
	double apsE = sqrt(pow((double)e.x, 2.0) + pow((double)e.y, 2.0) + pow((double)e.z, 2.0));
	double se = s.x * e.x + s.y * e.y + s.z * e.z;
	double kut = acos(se / (apsS * apsE));
	kut = kut / (2 * pi) * 360;
	glRotatef(kut, os.x, os.y, os.z);

	glTranslatef(-srediste.x, -srediste.y, -srediste.z);

	glColor3f(0.25, 0.25, 0.25);
	glBegin(GL_LINES);
	for (int i = 0; i < brF; i++) {
		Vrh v1 = *poligoni[i].v1;
		Vrh v2 = *poligoni[i].v2;
		Vrh v3 = *poligoni[i].v3;

		glVertex3f(v1.x, v1.y, v1.z);
		glVertex3f(v2.x, v2.y, v2.z);

		glVertex3f(v2.x, v2.y, v2.z);
		glVertex3f(v3.x, v3.y, v3.z);

		glVertex3f(v3.x, v3.y, v3.z);
		glVertex3f(v1.x, v1.y, v1.z);
	}

	glColor3f(1.0, 0.0, 0.0);
	glVertex3f(srediste.x, srediste.y, srediste.z);
	glVertex3f(srediste.x + 2.5, srediste.y, srediste.z);

	glColor3f(0.0, 1.0, 0.0);
	glVertex3f(srediste.x, srediste.y, srediste.z);
	glVertex3f(srediste.x, srediste.y + 2.5, srediste.z);

	glColor3f(0.0, 0.0, 1.0);
	glVertex3f(srediste.x, srediste.y, srediste.z);
	glVertex3f(srediste.x, srediste.y, srediste.z + 2.5);

	glColor3f(0.0, 0.0, 0.0);
	glEnd();

	t++;
	if (t == tockeUSegmentu * brS) t = 0;

	glFlush();
}

int currentTime = 0; int previousTime = 0;

void myIdle()
{
	currentTime = glutGet(GLUT_ELAPSED_TIME);
	int timeInterval = currentTime - previousTime;
	if (timeInterval > 15) {
		myDisplay();
		previousTime = currentTime;
	}
}
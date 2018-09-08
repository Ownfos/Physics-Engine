#pragma comment(lib,"GL/glut32.lib")
#include "GL/glut.h"

#include "Clock.h"
#include "Collision.h"
#include "Spring.h"
#include <vector>

#define LEFT 0
#define MIDDLE 1
#define RIGHT 2
#define PRESS 0
#define RELEASE 1

//setting
double fps = 500;
double dt = 1000 / fps;
double fovy = 60;
double camera = 30;
double cameraAngle = 0;
double width = 700;
double height = 500;
double aspect;
double viewHeight;
double viewWidth;
double mouseX = 0;
double mouseY = 0;
int frames = 0;
int fpsReal = 0;
int maxSimulation = 10;

bool showInformation = true;
bool showCollision = false;
bool showSpring = true;
bool isJoint = false;
bool gravity = true;
bool pause = false;
bool uniformSize = true;
bool dragForce = true;
bool toCenter = false;

//simulation variables
std::vector<Object*> objects;
std::vector<Spring*> springs;
std::vector<Collision> contacts;

//additional variables
std::vector<Vector> vertices;//custom polygon's vertices
Vector dragVector;
Vector p1, p2;//relative position of currently creating spring's endpoints
Object *o1, *o2;//objects connected to currently creating spring
Object *drag;//selected dragging object
Object *o;

enum MODE {
	DRAG,
	POLY,
	CIRC,
	BOX,
	SPRING,
	FREE,
	DESTROY
};
std::vector<const char*> modeString = {
	"drag and solidify",
	"polygon",
	"circle",
	"box",
	"spring",
	"delete spring",
	"destroy"
};
MODE mode;

//simulation functions
void setFps(double v) {
	fps = v;
	dt = 1000 / fps;
}

void init() {
	previousTime = GetCurrentTime();
	previousFpsCheck = GetCurrentTime();
	///*
	Object* o4 = new Object();
	o4->shape = new Polygon({
		Vector(-10,1),
		Vector(-10,-1),
		Vector(10,-1),
		Vector(10, 1)
	});
	o4->position = Vector(0, -5);
	o4->setInertia(0);
	o4->setMass(0);

	Object* o5 = new Object();
	o5->shape = new Polygon({
		Vector(1,4),
		Vector(-1,4),
		Vector(-1,-4),
		Vector(1,-4)
	});
	o5->position = Vector(-8, 0);
	o5->setInertia(0);
	o5->setMass(0);

	Object* o6 = new Object();
	o6->shape = new Polygon({
		Vector(1,4),
		Vector(-1,4),
		Vector(-1,-4),
		Vector(1,-4)
	});
	o6->position = Vector(8, 0);
	o6->setInertia(0);
	o6->setMass(0);

	Object* o7 = new Object();
	o7->shape = new Polygon({
		Vector(-10,1),
		Vector(-10,-1),
		Vector(10,-1),
		Vector(10, 1)
	});
	o7->position = Vector(0, 5);
	o7->setInertia(0);
	o7->setMass(0);

	Object* o8 = new Object();
	o8->shape = new Polygon({
		Vector(-20,1),
		Vector(-20,-1),
		Vector(20,-1),
		Vector(20, 1)
	});
	o8->position = Vector(0, -15);
	o8->setInertia(0);
	o8->setMass(0);

	objects.push_back(o4);//down
	objects.push_back(o5);//left
	objects.push_back(o6);//right
	objects.push_back(o7);//up
	objects.push_back(o8);//down big
	//*/
}

void deleteSpring(Object* o) {
	for (int i = 0; i < springs.size(); i++) {
		if (springs[i]->o1 == o || springs[i]->o2 == o) {
			delete springs[i];
			springs.erase(springs.begin() + i--);
		}
	}
}

void deleteObject(int index) {
	Object* o = objects[index];
	deleteSpring(o);
	objects.erase(objects.begin() + index);
	delete o;
}

void simulate() {
	for (int i = 0; i < springs.size(); i++) {
		springs[i]->solve(dt);
	}
	contacts.clear();
	int size = objects.size();
	for (int i = 0; i < size; i++) {
		for (int j = i + 1; j < size; j++) {
			Collision m(objects[i], objects[j]);
			m.solve();
			if (m.contacts.size() > 0)
				contacts.emplace_back(m);
		}
	}
	for (int i = 0; i < contacts.size(); i++) {
		contacts[i].initializeCollision();
		contacts[i].applyImpulse(dt);
		contacts[i].positionalCorrection();
	}
	for (int i = 0; i < size; i++) {
		if (gravity)
			objects[i]->linearForce -= Vector(0, 60)*objects[i]->mass;
		objects[i]->update(dt);
	}
	//dragging
	if (drag != nullptr) {
		dragVector = Vector(mouseX, mouseY) - (drag->position + drag->dragPosition.Rotated(drag->angle));
		if (dragForce) {
			drag->applyImpulse(dragVector * 50 * drag->mass * dt / 1000, drag->dragPosition.Rotated(drag->angle), dt);
		}
		else {
			drag->position += dragVector;
			drag->linearVelocity = Vector();
			drag->angularVelocity = 0;
		}
	}
}

//draw text
template<typename... Args>
void drawText(double x, double y, const char* format, Args...args) {
	std::string s;
	char buf[256];
	int len = sprintf_s(buf, format, args...);
	for (int i = 0; i < len; i++)
		s += buf[i];



	glRotated(-cameraAngle, 0, 0, 1);




	glRasterPos2d(x, y);
	for (int i = 0; i < s.length(); i++)
		glutBitmapCharacter(GLUT_BITMAP_HELVETICA_12, s[i]);



	glRotated(cameraAngle, 0, 0, 1);
}

//glut functions
void reshape(int w, int h) {
	if (h == 0)
		h = 1;
	width = w;
	height = h;
	aspect = (double)w / h;
	viewHeight = camera*tan(radian(fovy / 2)) * 2;
	viewWidth = viewHeight*aspect;

	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0, 0, w, h);
	gluPerspective(fovy, aspect, 1, 100);
	gluLookAt(0, 0, camera, 0, 0, 0, 0, 1, 0);



	glRotated(cameraAngle, 0, 0, 1);



	glMatrixMode(GL_MODELVIEW);
}

void display() {
	//simulate
	if (pause) {
		previousTime = GetCurrentTime();
	}
	else {
		currentTime = GetCurrentTime();
		accumulator += currentTime - previousTime;
		previousTime = currentTime;
		if (accumulator > dt*maxSimulation) {
			accumulator = dt*maxSimulation;
		}
		if (currentTime - previousFpsCheck > 1000) {
			previousFpsCheck = currentTime;
			fpsReal = frames;
			frames = 0;
		}
		while (accumulator >= dt) {
			simulate();
			accumulator -= dt;
			frames++;
			simulationDuration += dt;
		}
	}
	//delete objects out of bound(too far from origin)
	for (int i = 0; i < objects.size(); i++) {
		if (objects[i]->position.Magnitude() > camera) {
			deleteObject(i--);
		}
	}
	//render
	glClear(GL_COLOR_BUFFER_BIT);
	for (int i = 0; i < objects.size(); i++) {
		objects[i]->draw();
	}
	//extra drawing
	glColor3d(1, 0, 0);
	glBegin(GL_POINTS);
	glVertex2d(mouseX, mouseY);
	glEnd();
	switch (mode) {
	case DRAG:
		if (drag != nullptr && !pause) {
			glBegin(GL_LINES);
			glVertex2d(mouseX, mouseY);
			glVertex2d(mouseX - dragVector.x, mouseY - dragVector.y);
			glEnd();
		}
		break;
	case POLY:
		glBegin(GL_POINTS);
		for (int i = 0; i < vertices.size(); i++) {
			glVertex2d(vertices[i].x, vertices[i].y);
		}
		glEnd();
		break;
	case SPRING:
		if (o1 != nullptr) {
			Vector p = o1->position + p1.Rotated(o1->angle);
			glBegin(GL_LINES);
			glVertex2d(p.x, p.y);
			glVertex2d(mouseX, mouseY);
			glEnd();
		}
		break;
	}
	if (showSpring) {
		glColor3d(0, 0, 1);
		glBegin(GL_LINES);
		for (int i = 0; i < springs.size(); i++) {
			if(springs[i]->isJoint)
				glColor3d(0, 0, 1);
			else
				glColor3d(1, 0, 1);
			glVertex2d(springs[i]->v1.x, springs[i]->v1.y);
			glVertex2d(springs[i]->v2.x, springs[i]->v2.y);
		}
		glEnd();
	}
	if (showCollision) {
		glColor3d(0, 1, 0);
		glBegin(GL_POINTS);
		for (int i = 0; i < contacts.size(); i++) {
			for (int j = 0; j < contacts[i].contacts.size(); j++) {
				glVertex2d(contacts[i].contacts[j].x, contacts[i].contacts[j].y);
			}
		}
		glEnd();
		glBegin(GL_LINES);
		for (int i = 0; i < contacts.size(); i++) {
			for (int j = 0; j < contacts[i].contacts.size(); j++) {
				Vector c = contacts[i].contacts[j];
				glVertex2d(c.x, c.y);
				c += contacts[i].normal*0.75;
				glVertex2d(c.x, c.y);
			}
		}
		glEnd();
	}
	//texts
	if (showInformation) {
		glColor3d(0, 0, 0);
		double x = -viewWidth / 2 + 1;
		double y = viewHeight / 2 - 1;
		drawText(x, y - 1, "mode : %s", modeString[mode]);
		drawText(x, y - 2, "real fps : %d", fpsReal);
		drawText(x, y - 3, "target fps : %d", (int)fps);
		drawText(x, y - 4, "timescale : %g", fpsReal / fps);
		drawText(x, y - 5, "total objects : %d", objects.size());
		drawText(x, y - 6, "simulation duration : %g", simulationDuration / 1000.0);
		drawText(x, y - 7, "[S]how collision : %s", (showCollision) ? "true" : "false");
		drawText(x, y - 8, "show sp[R]ing : %s", (showSpring) ? "true" : "false");
		drawText(x, y - 9, "[G]ravity : %s", (gravity) ? "true" : "false");
		drawText(x, y - 10, "perfect [C]orrection : %s", (perfectCorrection) ? "true" : "false");
		drawText(x, y - 11, "is [J]oint : %s", (isJoint) ? "true" : "false");
		drawText(x, y - 12, "[U]niform size : %s", (uniformSize) ? "true" : "false");
		drawText(x, y - 13, "da[M]ping : %s", (damping) ? "true" : "false");
		drawText(x, y - 14, "[D]rag : %s", (dragForce) ? "pull" : "move");
	}
	//finish
	glutSwapBuffers();
}

void keyboard(unsigned char key, int x, int y) {
	switch (key) {
	case 'i':
		showInformation = !showInformation;
		break;
	case 's':
		showCollision = !showCollision;
		break;
	case 'g':
		gravity = !gravity;
		break;
	case 'c':
		perfectCorrection = !perfectCorrection;
		break;
	case 'r':
		showSpring = !showSpring;
		break;
	case 'p':
		pause = !pause;
		break;
	case 'm':
		damping = !damping;
		break;
	case 'u':
		uniformSize = !uniformSize;
		break;
	case 'd':
		dragForce = !dragForce;
		break;
	case 'j':
		isJoint = !isJoint;
		break;
	case 'q':
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i]->shape->getType() == CIRCLE) {
				//objects[i]->angularVelocity += 3;
				objects[i]->angularForce += 3 * objects[i]->inertia * 1000 / dt;
			}
		}
		break;
	case 'w':
		for (int i = 0; i < objects.size(); i++) {
			if (objects[i]->shape->getType() == CIRCLE) {
				//objects[i]->angularVelocity -= 3;
				objects[i]->angularForce -= 3 * objects[i]->inertia * 1000 / dt;
			}
		}
		break;
	case '1':
		mode = DRAG;
		drag = nullptr;
		break;
	case '2':
		mode = POLY;
		vertices.clear();
		break;
	case '3':
		mode = CIRC;
		break;
	case '4':
		mode = BOX;
		break;
	case '5':
		mode = SPRING;
		o1 = nullptr;
		o2 = nullptr;
		break;
	case '6':
		mode = FREE;
		break;
	case '7':
		mode = DESTROY;
		break;
	}
}

void motion(int x, int y) {
	mouseX = (x / width - 0.5)*viewWidth;
	mouseY = (0.5 - y / height)*viewHeight;
	Vector m(mouseX, mouseY);
	Vector mr = m.Rotated(-radian(cameraAngle));
	mouseX = mr.x;
	mouseY = mr.y;
	//x*cos(angle) - y * sin(angle), y*cos(angle) + x * sin(angle)
}

void mouse(int button, int state, int x, int y) {
	//find object under cursor
	Vector mouse = Vector(mouseX, mouseY);
	Object* o = nullptr;
	int index;
	int size = objects.size();
	for (int i = 0; i < size; i++) {
		if (objects[i]->isInside(Vector(mouseX, mouseY))) {
			o = objects[i];
			index = i;
			break;
		}
	}
	//mouse event
	switch (mode) {
	case DRAG:
		if (button == LEFT) {
			if (state == PRESS && o != nullptr) {
				drag = o;
				o->dragPosition = (mouse - o->position).Rotated(-o->angle);
			}
			else if (state == RELEASE) {
				drag = nullptr;
			}
		}
		else if (button == RIGHT && state == PRESS) {
			Object* target = nullptr;
			if (drag != nullptr)
				target = drag;
			else if (o != nullptr)
				target = o;
			if (target != nullptr) {
				if (target->mass == 0 && target->inertia == 0) {
					target->set();
				}
				else {
					target->setMass(0);
					target->setInertia(0);
					target->linearVelocity = Vector();
					target->angularVelocity = 0;
				}
			}
			
		}
		break;
	case POLY:
		if (button == LEFT && state == PRESS) {
			vertices.push_back(mouse);
		}
		else if (button == RIGHT && state == PRESS) {
			if (vertices.size() > 2) {
				Polygon p = Polygon(vertices);
				if (p.isConvex()) {
					Object* o = new Object();
					o->shape = new Polygon(vertices);
					o->set();
					o->position = Vector(mouseX, mouseY);
					objects.push_back(o);
				}
				vertices.clear();
			}
		}
		break;
	case CIRC:
		if (button == LEFT && state == PRESS) {
			double radius;
			if (uniformSize)
				radius = 1;
			else
				radius = (double)rand() / RAND_MAX * 2 + 0.5;
			o = new Object();
			o->shape = new Circle(radius);
			o->set();
			o->position = mouse;
			objects.push_back(o);
		}
		break;
	case BOX:
		if (button == LEFT && state == PRESS) {
			double width, height;
			if (uniformSize) {
				width = 1;
				height = 1;
			}
			else {
				width = (double)rand() / RAND_MAX * 2 + 0.5;
				height = (double)rand() / RAND_MAX * 2 + 0.5;
			}
			o = new Object();
			o->shape = new Polygon({
				Vector(width,height),
				Vector(-width,height),
				Vector(-width,-height),
				Vector(width,-height)
			});
			o->set();
			o->position = mouse;
			objects.push_back(o);
		}
		break;
	case DESTROY:
		if (button == LEFT && state == PRESS && o!=nullptr) {
			deleteObject(index);
		}
		break;
	case SPRING:
		if (button == LEFT) {
			if (o != nullptr) {
				if (state == PRESS) {
					o1 = o;
					if (toCenter)
						p1 = Vector();
					else
						p1 = (mouse - o1->position).Rotated(-o1->angle);
				}
				else if (o1 != nullptr && state == RELEASE) {
					o2 = o;
					if (toCenter)
						p2 = Vector();
					else
						p2 = (mouse - o2->position).Rotated(-o2->angle);

					if (o1 != o2) {
						Spring* s = new Spring(o1, o2, p1, p2);
						s->isJoint = isJoint;
						springs.push_back(s);
					}
					o1 = nullptr;
					o2 = nullptr;
				}
			}
			else {
				o1 = nullptr;
				o2 = nullptr;
			}
		}
		else if (button == RIGHT && state == PRESS) {
			toCenter = true;
		}
		else if (button == RIGHT && state == RELEASE) {
			toCenter = false;
		}
		break;
	case FREE:
		if (button == LEFT && state == PRESS && o != nullptr) {
			for (int i = 0; i < springs.size(); i++) {
				if (springs[i]->o1 == o || springs[i]->o2 == o) {
					delete springs[i];
					springs.erase(springs.begin() + i--);
				}
			}
		}
		break;
	}
}

int main() {
	//window
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(400, 100);
	glutInitWindowSize(width, height);
	glutCreateWindow("");
	//functions
	glutDisplayFunc(display);
	glutIdleFunc(display);
	glutReshapeFunc(reshape);
	glutKeyboardFunc(keyboard);
	glutPassiveMotionFunc(motion);
	glutMotionFunc(motion);
	glutMouseFunc(mouse);
	//setting
	glClearColor(1, 1, 1, 1);
	glPointSize(4.0f);
	//scene setup
	init();
	//start
	glutMainLoop();
}
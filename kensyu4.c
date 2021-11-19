#include <stdlib.h>
#include <GL/glut.h>
#include <math.h>
#include <stdio.h>
#include "vec3.h"
#include "qua.h"

int left_flag = 0;
int middle_flag = 0;
int right_flag = 0;

int left_x;
int left_y;
int right_x;
int right_y;
int middle_x;
int middle_y;

double eye[3] = {3.0, 4.0, 5.0};
double pov[3] = {0.0, 0.0, 0.0};
double up[3] = {0.0, 1.0, 0.0};

double eye_mem[3];
double pov_mem[3];
double up_mem[3];

GLdouble vertex[][3]={
  {0.0, 0.0, 0.0},
  {1.0, 0.0, 0.0},
  {1.0, 1.0, 0.0},
  {0.0, 1.0, 0.0},
  {0.0, 0.0, 1.0},
  {1.0, 0.0, 1.0},
  {1.0, 1.0, 1.0},
  {0.0, 1.0, 1.0}
};

int face[][4] = {
  {0, 3, 2, 1},
  {1, 2, 6, 5},
  {5, 6, 7, 4},
  {4, 7, 3, 0},
  {4, 0, 1, 5},
  {3, 7, 6, 2}
};

GLdouble color[][3] = {
  {1.0, 0.0, 0.0},
  {0.0, 1.0, 0.0},
  {0.0, 0.0, 1.0},
  {1.0, 1.0, 0.0},
  {1.0, 0.0, 1.0},
  {0.0, 1.0, 1.0}
};

/*
void idle(void){
  glutPostRedisplay();
}
*/

void display(void){
  int i;
  int j;
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
  glLoadIdentity();
  gluLookAt(eye[0], eye[1], eye[2], pov[0], pov[1], pov[2], up[0], up[1], up[2]);
  glBegin(GL_QUADS);
  for(i=0;i<6;i++){
    glColor3dv(color[i]);
    for(j=0;j<4;j++){
      glVertex3dv(vertex[face[i][j]]);
    }
  }
  glEnd();
  glutSwapBuffers();
}

void resize(int w,int h){
  glViewport(0,0,w,h);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  gluPerspective(30.0,(double)w/(double)h,1.0,100.0);
  glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y){
  if(state == GLUT_DOWN){
    for(int i = 0; i < 3; i++){
      eye_mem[i] = eye[i];
      pov_mem[i] = pov[i];
      up_mem[i] = up[i];
    }
  }
  switch(button){
  case GLUT_LEFT_BUTTON:
    if(state == GLUT_DOWN){
      left_flag = 1;
      left_x = x;
      left_y = y;
    }
    else{
      left_flag = 0;
    }
    break;
  case GLUT_RIGHT_BUTTON:
    if(state == GLUT_DOWN){
      right_flag = 1;
      right_x = x;
      right_y = y;
    }
    else{
      right_flag = 0;
    }
    break;
  case GLUT_MIDDLE_BUTTON:
    if(state == GLUT_DOWN){
      middle_flag = 1;
      middle_x = x;
      middle_y = y;
    }
    else{
      middle_flag = 0;
    }
    break;
  default:
    break;
  }
}

void motion(int x, int y){

  if(right_flag){
    int right2 = (x - right_x) * (x - right_x) + (y - right_y) * (y - right_y);
    double right = sqrt((double)right2);
    printf("px:%f\n", right);
    double deg = right * 0.9;
    printf("deg:%f\n",deg);
    double rad = deg * M_PI / 180.0;
    printf("rad:%f\n\n", rad);
    if(x - right_x > 0){
      qua_rot(eye_mem, up, pov, rad, eye);
    }
    else{
      qua_rot(eye_mem, up, pov, -rad, eye);
    }
    glutPostRedisplay();
  }

  if(left_flag){
    int leftx2_int = left_x - x;
    int lefty2_int = left_y - y;
    double leftx2_double = (double)leftx2_int;
    double lefty2_double = (double)lefty2_int;
    printf("$$$$$$$$$$$$$$$$$$$\nleft\nx : %f, y : %f\n$$$$$$$$$$$$$$$$$\n", leftx2_double, lefty2_double);
    eye[0] = eye_mem[0] + leftx2_double / 1000.0;
    eye[2] = eye_mem[2] + lefty2_double / 1000.0;
    pov[0] = pov_mem[0] + leftx2_double / 1000.0;
    pov[2] = pov_mem[2] + lefty2_double / 1000.0;
    glutPostRedisplay();
  }
  
  if(middle_flag){
    double eye_pov_vec[3];
    vec3sub(pov_mem, eye_mem, eye_pov_vec);
    double eye_pov_vec_nrm[3];
    vec3nrm(eye_pov_vec, eye_pov_vec_nrm);
    int middlex2_int = x - middle_x;
    int middley2_int = y - middle_y;
    double middlex2_double = (double)middlex2_int;
    double middley2_double = (double)middley2_int;
    double distance_middle = sqrt(middlex2_double * middlex2_double + middley2_double * middley2_double);
    double move[3];
    vec3mul(distance_middle / 100.0, eye_pov_vec_nrm, move);
    if(middley2_int > 0){
      vec3add(eye_mem, move, eye);
    }
    else{
      vec3sub(eye_mem, move, eye);
    }
    glutPostRedisplay();
  }
}

void keyboard(unsigned char key, int x, int y){
  switch(key){
  case 'q':
  case 'Q':
  case '\033':
    exit(0);
  default:
    break;
  }
}

void init(void){
  glClearColor(1.0,1.0,1.0,1.0);
  glEnable(GL_DEPTH_TEST);
  glEnable(GL_CULL_FACE);
  glCullFace(GL_BACK);
}

int main(int argc,char *argv[]){
  vec3show(eye);
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
  glutCreateWindow(argv[0]);
  glutDisplayFunc(display);
  glutReshapeFunc(resize);
  glutMouseFunc(mouse);
  glutMotionFunc(motion);
  glutKeyboardFunc(keyboard);
  init();
  glutMainLoop();
  return 0;
}

// twoturbines.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include<GLUT/glut.h>
#include<stdio.h>
//#include<stdlib.h>
#include<math.h>
#include<string.h>

#define deg2grad 3.14/180.0
GLfloat rotateAngle=0;

static int slideNumber = 1,workingSlideNumber=1;
//colors
GLfloat cylinderFacesColor[] = {0.3,0.3,0.3,0.8};
GLfloat cylinderStripsColor[] = {0.3,.3,.3};
GLfloat cylinderSmallCirclesColor[] = {0.3,.3,.3,0.5};
GLfloat innerObjectsColor[] = {0.698,0.1333,0.1333};
GLfloat discsColor[] = {0.8627,0.0784,0.2352};

GLfloat smallCirclesCenters[5][2];
bool labels[] = {false,false,false,false,false};

static GLfloat theta[] = {0,0,0};

static int boolTest = 0;

void *fonts[]=                                        //various fonts
{
GLUT_BITMAP_9_BY_15,
GLUT_BITMAP_TIMES_ROMAN_10,
GLUT_BITMAP_TIMES_ROMAN_24,
GLUT_BITMAP_HELVETICA_10,
GLUT_BITMAP_HELVETICA_18,
GLUT_BITMAP_HELVETICA_12,
GLUT_BITMAP_8_BY_13

};

//balls
float r0=0,r1=0,r2=0.4,r3=0.12;
GLfloat X,Y,Z,X1,Y1,Z1,X2,Y2,X3,Y3;

void output(int x, int y, char *string,void *font)
 /* x and y are coordinates,
 string->msg to display */
 {
  int len, i;

  glRasterPos2f(x, y);
  len = (int) strlen(string);
  for (i = 0; i < len; i++) {
    glutBitmapCharacter(font, string[i]);
  }
}

void output2(GLfloat x, GLfloat y, char* text)
{
    glPushMatrix();
    glTranslatef(x, y, 0);
    for( char* p = text; *p; p++)
    {
        glutStrokeCharacter(GLUT_STROKE_ROMAN, *p);
    }
    glPopMatrix();
}


void draw_arrow(GLfloat fromX,GLfloat fromY,GLfloat fromZ,GLfloat toX,GLfloat toY,GLfloat toZ){

  glBegin(GL_TRIANGLES);		// Drawing Using Triangles
 	glVertex3f(-1.0f, 1.0f, fromZ);		// Top
 	glVertex3f(-1.0f,-1.0f, fromZ);		// Bottom Left
 	glVertex3f( 1.0f,0.0f, fromZ);		// Bottom Right
  glEnd();

}



void draw_small_circle(GLfloat h,GLfloat k,GLfloat radius,GLfloat base){

  GLfloat x,y;


  glBegin(GL_POLYGON);

    for(int i=0;i<=360;i++){
      x = h+radius*cos(i*deg2grad);
      y = k+radius*sin(i*deg2grad);
      glVertex3f(x,y,base);
    }

    glVertex3f(h+radius,k+0,base);

  glEnd();

}

void draw_baseRectangle(GLfloat radius,GLfloat height,GLfloat base){

  GLfloat x,y;
  glColor3f(0.2,0.2,0.2);
  //left towards base
      x=radius*cos(225*deg2grad)-0.1;
      y=radius*sin(225*deg2grad);

      glVertex3f(x,y,height);
      glVertex3f(x,y,base);
  //down for 1 unit
      y-=0.07;

      glVertex3f(x,y,height);
      glVertex3f(x,y,base);

  //go to extreme right

      x=radius*cos(-45*deg2grad)+0.1;

      glVertex3f(x,y,height);
      glVertex3f(x,y,base);

  //go to top

      y+=0.07;

      glVertex3f(x,y,height);
      glVertex3f(x,y,base);

      //join back

      x=radius*cos(-45*deg2grad);

      glVertex3f(x,y,height);
      glVertex3f(x,y,base);

}

void draw_baseRectangleSides(GLfloat radius,GLfloat height,GLfloat base)
{

  GLfloat x,y;
  glColor3f(0.2,0.2,0.2);
  glBegin(GL_POLYGON);
  x=radius*cos(225*deg2grad)-0.1;
  y=radius*sin(225*deg2grad);

  glVertex3f(x,y,height);
  glVertex3f(x,y,base);
//down for 1 unit
  y-=0.07;

  glVertex3f(x,y,height);
  glVertex3f(x,y,base);

//go to extreme right

  x=radius*cos(-45*deg2grad)+0.1;

  glVertex3f(x,y,height);
  glVertex3f(x,y,base);

//go to top

  y+=0.07;

  glVertex3f(x,y,height);
  glVertex3f(x,y,base);

  //join back

  x=radius*cos(-45*deg2grad);

  glVertex3f(x,y,height);
  glVertex3f(x,y,base);

  glEnd();
}

void draw_curve(float innerRadius,float outerRadius,float startAngle,float endAngle,GLfloat height,GLfloat base)
{

  GLfloat x,y,x1,y1;
  GLfloat angle = 0;
  glColor3fv(innerObjectsColor);

  glBegin(GL_QUAD_STRIP);

  for(int i = startAngle ; i<=endAngle ; i++ )
  {

    x = innerRadius*cos(i*deg2grad);
    y = innerRadius*sin(i*deg2grad);
    glVertex3f(x,y,height);
    glVertex3f(x,y,base);

  }


  for(int i = endAngle ; i>=startAngle ; i-- )
  {

    x1 = outerRadius*cos(i*deg2grad);
    y1 = outerRadius*sin(i*deg2grad);
    glVertex3f(x1,y1,height);
    glVertex3f(x1,y1,base);

  }

  x = innerRadius*cos(startAngle*deg2grad);
  y = innerRadius*sin(startAngle*deg2grad);

  glVertex3f(x,y,height);
  glVertex3f(x,y,base);

  glEnd();

}

void draw_cylinder(float radius,float height,float base){

  GLfloat x,y,x1,y1;
  GLfloat angle = 0;
//  glColor3f(0.8,0,0.3);

  glBegin(GL_QUAD_STRIP);

  for(int i=0;i<=360;i++){
    x = radius*cos(i*deg2grad);
    y = radius*sin(i*deg2grad);
    glVertex3f(x,y,height);
    glVertex3f(x,y,base);
  }

  glVertex3f(radius,0,height);
  glVertex3f(radius,0,base);

  glEnd();

  glColor3fv(discsColor);
 // glColor3f(1,0,0.5);

  glBegin(GL_POLYGON);

  for(int i=0;i<=360;i++){
    x = radius*cos(i*deg2grad);
    y = radius*sin(i*deg2grad);
    glVertex3f(x,y,base);
  }

  glVertex3f(radius,0,base);

  glEnd();

}


void draw_inner_objects(){

    glPushMatrix();
	if(workingSlideNumber!=1 || slideNumber==2 || slideNumber==5 || slideNumber==7){
    glRotatef(rotateAngle,0,0,1);
	}
	


    //inner blade 1
  glColor3fv(innerObjectsColor);

    draw_cylinder(2,0.5,0);

      //float innerRadius,float outerRadius,
      //float startAngle,float endAngle,
      //GLfloat height,GLfloat base
      draw_curve(0.5,1,20,120,0.5,0);
      draw_curve(.5,1,140,240,0.5,0);
      draw_curve(.5,1,260,360,0.5,0);


    //inner blade 2
  glColor3fv(innerObjectsColor);

    draw_cylinder(2,1.5,1);


      draw_curve(.5,1,20,120,1.5,1);
      draw_curve(.5,1,140,240,1.5,1);
      draw_curve(.5,1,260,360,1.5,1);

    //inner blade 2
  glColor3fv(innerObjectsColor);

    draw_cylinder(2,2.5,2);


      draw_curve(.5,1,20,120,2.5,2);
      draw_curve(.5,1,140,240,2.5,2);
      draw_curve(.5,1,260,360,2.5,2);

      //center axis rod
  glColor3fv(innerObjectsColor);

      draw_cylinder(0.5,3,-0.5);


    glPopMatrix();

}



void draw_outer_cylinder(float radius,float height,float base){

  GLfloat x,y,x1,y1;
  GLfloat angle = 0;

  // glRotatef(rotateAngle,1,1,0);

  //circle front
  glColor4fv(cylinderFacesColor);

  glBegin(GL_POLYGON);

    for(int i=0;i<=360;i++){
      x = radius*cos(i*deg2grad);
      y = radius*sin(i*deg2grad);
      glVertex3f(x,y,height);

    }

  glVertex3f(radius,0,height);

  glEnd();


  glColor4fv(cylinderSmallCirclesColor);

  for(int i=0;i<360;i++){
    // smaller circles with radius and
    // x1,y1 as centers of the circles
    int count=0;
    if(i%72==0){
      x1 = (radius/2)*cos(i*deg2grad);
      y1 = (radius/2)*sin(i*deg2grad);
      smallCirclesCenters[count][0]=x1;
      smallCirclesCenters[count][1]=y1;
      draw_small_circle(x1,y1,(radius/5),height+0.0001);
    }

  }


  //circle behind

  glColor4fv(cylinderFacesColor);

  glBegin(GL_POLYGON);

    for(int i=0;i<=360;i++){
      x = radius*cos(i*deg2grad);
      y = radius*sin(i*deg2grad);
      glVertex3f(x,y,base);
    }

    glVertex3f(radius,0,base);

  glEnd();

  glColor4fv(cylinderSmallCirclesColor);

  for(int i=0;i<360;i++){
    // smaller circles with radius and
    // x1,y1 as centers of the circles
    if(i%72==0){
      //calculating centers of the small circles
      x1 = (radius/2)*cos(i*deg2grad);
      y1 = (radius/2)*sin(i*deg2grad);
      draw_small_circle(x1,y1,(radius/5),base-0.0001);
    }

  }



  //strips



  glColor3fv(cylinderStripsColor);

  glBegin(GL_QUAD_STRIP);


  for(int i=-45;i<=45;i++){
    x = radius*cos(i*deg2grad);
    y = radius*sin(i*deg2grad);
    glVertex3f(x,y,height);
    glVertex3f(x,y,base);
  }

// i need 50 and 140

  y+=0.1;

  glVertex3f(x,y,height);
  glVertex3f(x,y,base);

  x=radius*cos(55*deg2grad);
  y=radius*sin(55*deg2grad)+0.07;

  glVertex3f(x,y,height);
  glVertex3f(x,y,base);

  for(int i=55;i<=125;i++){
    x = radius*cos(i*deg2grad);
    y = radius*sin(i*deg2grad);
    glVertex3f(x,y,height);
    glVertex3f(x,y,base);
  }

  y+=0.07;
  glVertex3f(x,y,height);
  glVertex3f(x,y,base);

  x=radius*cos(135*deg2grad);
  y=radius*sin(135*deg2grad)+0.1;

  glVertex3f(x,y,height);
  glVertex3f(x,y,base);


  for(int i=135;i<=225;i++){
    x = radius*cos(i*deg2grad);
    y = radius*sin(i*deg2grad);
    glVertex3f(x,y,height);
    glVertex3f(x,y,base);
  }

  draw_baseRectangle(radius,height,base);

glEnd();

  draw_baseRectangleSides(radius,height,base);




}

//for two turbines

void draw_outer_cylinder0(float radius,float height,float base){

  GLfloat x,y,x1,y1;
  GLfloat angle = 0;
  //circle behind

  glColor4fv(cylinderFacesColor);

  glBegin(GL_POLYGON);

    for(int i=0;i<=360;i++){
      x = radius*cos(i*deg2grad);
      y = radius*sin(i*deg2grad);
      glVertex3f(x,y,base);
    }

    glVertex3f(radius,0,base);

  glEnd();

  glColor4fv(cylinderSmallCirclesColor);

  for(int i=0;i<360;i++){
    // smaller circles with radius and
    // x1,y1 as centers of the circles
    if(i%72==0){
      //calculating centers of the small circles
      x1 = (radius/2)*cos(i*deg2grad);
      y1 = (radius/2)*sin(i*deg2grad);
      draw_small_circle(x1,y1,(radius/5),base-0.0001);
    }

  }
  //strips
 glColor3fv(cylinderStripsColor);

  glBegin(GL_QUAD_STRIP);


  for(int i=-45;i<=45;i++){
    x = radius*cos(i*deg2grad);
    y = radius*sin(i*deg2grad);
    glVertex3f(x,y,height);
    glVertex3f(x,y,base);
  }

  y+=0.1;

  glVertex3f(x,y,height);
  glVertex3f(x,y,base);

  x=radius*cos(55*deg2grad);
  y=radius*sin(55*deg2grad)+0.07;

  glVertex3f(x,y,height);
  glVertex3f(x,y,base);

  for(int i=55;i<=125;i++){
    x = radius*cos(i*deg2grad);
    y = radius*sin(i*deg2grad);
    glVertex3f(x,y,height);
    glVertex3f(x,y,base);
  }

  y+=0.07;

  glVertex3f(x,y,height);
  glVertex3f(x,y,base);

  x=radius*cos(135*deg2grad);
  y=radius*sin(135*deg2grad)+0.1;

  glVertex3f(x,y,height);
  glVertex3f(x,y,base);


  for(int i=135;i<=225;i++){
    x = radius*cos(i*deg2grad);
    y = radius*sin(i*deg2grad);
    glVertex3f(x,y,height);
    glVertex3f(x,y,base);
  }

  draw_baseRectangle(radius,height,base);

glEnd();

  draw_baseRectangleSides(radius,height,base);
}


void displayFirstSlide(){



  glOrtho(-500,500,-500,500,-500,500);
  //purple background
  glClearColor(0.5764,0.4392,0.8588,1);
  
  glColor3f(1,1,1);

  // output(1,1,"hello world",fonts[2]);
  glPushMatrix();
  glScalef(0.5,0.5,1);
  output2(-400,20,"Tesla Turbine");
  glPopMatrix();

  glPushMatrix();
  glScalef(0.3,0.3,1);
  output2(-1500,-1000,"Anitha V");
  output2(-1500,-1200,"Darshan N");
  output2(-1500,-1400,"Meghana N");
  glPopMatrix();

  glPushMatrix();
  glScalef(0.2,0.2,1);
  output2(1750,-2000,"N - next");
  glPopMatrix();

  // glBegin(GL_POINTS);
  // glPointSize(5.0);
  // GLfloat x,y;
  // float r = 0;
  // // for(int r=0;r<400;r+=5){
  //   for(int i=0;i<3600;i++){
  //     x=r*cos(i*deg2grad);
  //     y=r*sin(i*deg2grad);
  //     r+=0.1;
  //     if(r>=500)break;
  //     glTranslatef(x,y,0);
  //     draw_small_circle(0,0,3,0);
  //   }
  // // }
  // glEnd();
  /*   for(float i=0;i<10;i+=0.01){
		 glTranslatef(0,50+i,0);
      draw_small_circle(0,0,3,0);
	 }*/
 
  // if(boolTest==1){
  //   output(0,0,"darshan",fonts[2]);
  // }

}

void draw_line(GLfloat fromX,GLfloat fromY,GLfloat toX,GLfloat toY,GLfloat z){
  glBegin(GL_LINES);
  glPointSize(5);
    glVertex3f(fromX,fromY,z);
    glVertex3f(toX,toY,z);
  glEnd();
}

void displaySecondSlide(){

    glClearColor(1,1,1,1);

    double winht = 1.0;
    // glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);
    glOrtho(-winht *64/48.0,winht*64/48.0,-winht,winht,0.1,100.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // gluLookAt(0.8,0.5,-1,0,0,0,0,1,0);
    gluLookAt(2.3,1.3,6,0,0.25,0,0,1,0);

    glPushMatrix();

    glRotatef(theta[0],1,0,0);
    glRotatef(theta[1],0,1,0);
    glRotatef(theta[2],0,0,1);

        glPushMatrix();
        glTranslatef(0,0.1,0);
glScalef(0.9,0.9,0.9);
// // innner blades
        // glScalef(0.08,0.08,0.08);
        glPushMatrix();
        glScalef(0.2,0.2,0.2);
        glTranslatef(0,0,-1);
        draw_inner_objects();
        glPopMatrix();

//outer cylinder
    glPushMatrix();

    glScalef(2.5,2.5,2.5);
    draw_outer_cylinder(0.2,0.25,-0.1);

    glPopMatrix();

    glPopMatrix();
    glPopMatrix();

    glPushMatrix();

    glScalef(0.0005,0.0005,0.0005);
    output(-2400,2100,"OVERVIEW OF THE TURBINE",fonts[2]);

	glColor3f(0,0,1);
    // output2(-400,-600,"output");
	output(-2800,1900,"-->Tesla turbine is a bladeless centripetal turbine patented by NIKOLA TESLA in 1913.",fonts[4]);
	output(-2800,1750,"-->This turbine is an efficient self-starting prime mover which may be operated as a steam or mixed fluid turbine at will.",fonts[4]);
	output(-2800,1600,"-->It uses boundary layer effect hence it is also called boundary layer turbine.",fonts[4]);
	output(-2800,1450,"-->Few applications are The tesla pump and multiple-disk centrifugal blood pump.  ",fonts[4]);
	output(-2800,1300,"-->As it is bladeless,it's efficiency is high compared to conventional turbines.   ",fonts[4]);
	
    glPopMatrix();
   
	glScalef(0.001,0.001,0.001);
	glColor3f(0.3,0.3,0.3);
   	output(-1200,-700,"Press 'a' & 'd' to navigate and 'numbers'(1-3) for more options",fonts[2]);

	

    //
    // output(0,0,"darshan",fonts[2]);
    // glScalef(0.0002,0.0002,0.0002);
    // output2(0,0,"darshan");
    //

    // if(boolTest==1){
    //   glColor3f(1,0,0);
    //   output(1,1,"darshan",fonts[2]);
    //   draw_line(0,0,1,1,1);
    //   draw_line(smallCirclesCenters[0][0],smallCirclesCenters[0][1],1,1,1);
    //   }

}

void displaySecondSlidePartOne(){
  glClearColor(1,1,1,1);

  double winht = 1.0;
  glOrtho(-winht *64/48.0,winht*64/48.0,-winht,winht,0.1,100.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(2.3,1.3,6,0,0.25,0,0,1,0);

  output(-1,1,"Outer turbine case",fonts[2]);

  // output(-0.5,-1.5,"press 'numbers' to view the parts",fonts[0]);

  glPushMatrix();

  glRotatef(theta[0],1,0,0);
  glRotatef(theta[1],0,1,0);
  glRotatef(theta[2],0,0,1);

  glTranslatef(0,0.1,0);
  glScalef(1.5,1.5,1.5);
  draw_outer_cylinder(0.2,0.25,-0.1);

  //calculate
  GLfloat x,y,x1,y1;
  x = 0.2*cos(50*deg2grad);
  y = 0.2*sin(50*deg2grad)+0.1;

  draw_line(x,y,x+0.35,y+0.35,0.25);

  x1 = 0.2*cos(135*deg2grad);
  y1 = 0.2*sin(135*deg2grad)+0.1;
  draw_line(x1,y1,x+0.35,y+0.35,0.25);

  // output(x+0.35,y+0.35,"inlets",fonts[2]);
  glScalef(0.001,0.001,0.001);
  output(400,600,"inlets",fonts[2]);
  glPopMatrix();

  glScalef(0.001,0.001,0.001);
  // output2(-400,-600,"output");
  
  output(-1200,-600,"press 'b' for back and 'numbers' for more parts",fonts[2]);

}

void displaySecondSlidePartTwo(){

  glClearColor(1,1,1,1);

  double winht = 1.0;
  glOrtho(-winht *64/48.0,winht*64/48.0,-winht,winht,0.1,100.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(2.3,1.3,6,0,0.25,0,0,1,0);
  glColor3f(0.2,0.3,0.4);

  output(-1,1,"Turbine outlets (small holes in the plate are the outlets )",fonts[2]);

  glPushMatrix();
  glTranslatef(0,0.3,0);

  GLfloat x,y,x1,y1;
  GLfloat angle = 0;
  GLfloat radius = 0.6,height = 0;

  glRotatef(theta[0],1,0,0);
  glRotatef(theta[1],0,1,0);
  glRotatef(theta[2],0,0,1);

  // glRotatef(rotateAngle,1,1,0);

  glRotatef(rotateAngle,0,0,1);

  //circle front
  glColor4fv(cylinderFacesColor);

  glBegin(GL_POLYGON);

    for(int i=0;i<=360;i++){
      x = radius*cos(i*deg2grad);
      y = radius*sin(i*deg2grad);
      glVertex3f(x,y,height);

    }

  glVertex3f(radius,0,height);

  glEnd();


  glColor4fv(cylinderSmallCirclesColor);

  for(int i=0;i<360;i++){
    // smaller circles with radius and
    // x1,y1 as centers of the circles
    int count=0;
    if(i%72==0){
      x1 = (radius/2)*cos(i*deg2grad);
      y1 = (radius/2)*sin(i*deg2grad);
      smallCirclesCenters[count][0]=x1;
      smallCirclesCenters[count][1]=y1;
      draw_small_circle(x1,y1,(radius/5),height+0.0001);
    }

  }

  glPopMatrix();
  glColor3f(0.2,0.3,0.4);

  glScalef(0.001,0.001,0.001);
  // output2(-400,-600,"output");
  output(-1200,-600,"Press 'b' for back and 'numbers' for more parts",fonts[2]);


}

void displaySecondSlidePartThree(){

  glClearColor(1,1,1,1);

  double winht = 1.0;
  glOrtho(-winht *64/48.0,winht*64/48.0,-winht,winht,0.1,100.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(2.3,1.3,6,0,0.25,0,0,1,0);

  output(-1,1,"Turbine disks (small holes in the center acts as vortex )",fonts[2]);

  glPushMatrix();

  GLfloat x,y,x1,y1;
  GLfloat angle = 0;
  GLfloat radius = 0.6,height = 0;
  glTranslatef(0,0.3,0);

  glRotatef(theta[0],1,0,0);
  glRotatef(theta[1],0,1,0);
  glRotatef(theta[2],0,0,1);

  glPushMatrix();
  glScalef(0.2,0.2,0.2);
  glTranslatef(0,0,-1);
  draw_inner_objects();
  glPopMatrix();

  glPopMatrix();

  glScalef(0.001,0.001,0.001);
  // output2(-400,-600,"output");
  output(-1200,-600,"Press 'a'&'d' to navigate , 'b' for back and 'numbers' for more parts",fonts[2]);

}


void displayWorkingOfTurbine(){

  glClearColor(1,1,1,1);

  double winht = 1.0;
  glOrtho(-winht *64/48.0,winht*64/48.0,-winht,winht,0.1,100.0);
  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();
  gluLookAt(2.3,1.3,6,0,0.25,0,0,1,0);
  glColor3f(0,0,0);
  output(-1,1,"WORKING OF THE TURBINE",fonts[2]);
  
  glPushMatrix();

  glTranslatef(-0.6,0.3,0);
  glRotatef(theta[0],1,0,0);
  glRotatef(theta[1],0,1,0);
  glRotatef(theta[2],0,0,1);
 
  // // innner blades
      // glScalef(0.08,0.08,0.08);
      glPushMatrix();
      glScalef(0.2,0.2,0.2);
      glTranslatef(0,0,-1);
      draw_inner_objects();
      glPopMatrix();

  //outer cylinder
  glPushMatrix();
  glPopMatrix();

  glScalef(2.5,2.5,2.5);
  if(workingSlideNumber==1||workingSlideNumber==2)
    draw_outer_cylinder0(0.2,0.25,-0.1);

  glPopMatrix();
  glPopMatrix();

  switch (workingSlideNumber){ //if(workingSlideNumber==1){
  case 1: glPushMatrix();
  glScalef(2.5,2.5,2.5);
 	 glColor4f(0.2823,0.4627,1,0.6);

  if(r2>=0.2){
  glPushMatrix();
  glTranslatef(X3,Y3,0.2);
  draw_small_circle(0,0,0.008,0); 
  glPopMatrix();
  r3=0.12;
  }
  else{ 
		glPushMatrix();
		glTranslatef(-0.05,0.145,0.2);
        glTranslatef(X2,Y2,Z1);
		draw_small_circle(0,0,0.008,0);
		glPopMatrix();
	  }
	glPopMatrix();
	break;
  case 2:
	   glPushMatrix();
  glScalef(2.5,2.5,2.5);
 	 glColor4f(0.2823,0.4627,1,0.6);

  glPushMatrix();
		glTranslatef(-0.04,0.16,0.2);
        glTranslatef(X2,Y2,Z1);
      draw_small_circle(0,0,0.008,0);
	 
glPopMatrix();
glPopMatrix();
break;

  }

    glScalef(0.001,0.001,0.001);
	glColor3f(0,0,0);
  // output2(-400,-600,"output");
  if(workingSlideNumber!=1)
	  output(-1200,-500,"Press 'a' & 'd' to navigate and 'p' for previous and 'n' for next slide",fonts[2]);
  output(-1000,-600,"Press '<' and '>' to continue ",fonts[2]);

  switch (workingSlideNumber) {
    case 1:
    glPushMatrix();
    glColor3f(0,0,1);
    output(200,1000,"As the air enters the turbine",fonts[2]);
    output(200,900,"housing it is forced to",fonts[2]);
    output(200,800,"create a vortex due to",fonts[2]);
    output(200,700,"the shape of the turbine.",fonts[2]);
    output(200,600,"Remember that the front covers",fonts[2]);
    output(200,500,"would also be in place",fonts[2]);
    output(200,400,"so the air has no choice",fonts[2]);
    output(200,300,"but to exit through the front or",fonts[2]);
    output(200,200,"rear exaust ports.",fonts[2]);
    output(200,100,"",fonts[2]);
    output(200,00,"",fonts[2]);
    output(200,-100,"",fonts[2]);
    glPopMatrix();
	
	
    break;

    case 2:
    glPushMatrix();
    glColor3f(0,0,1);
    output(200,1000,"As the air molecules pass",fonts[2]);
    output(200,900,"discs, they create a drag",fonts[2]);
    output(200,800,"on them, this drag pulls the discs",fonts[2]);
    output(200,700,"with air,this is the ",fonts[2]);
    output(200,600,"basic explanation of the ",fonts[2]);
    output(200,500,"boundary layer effect.",fonts[2]);
    output(200,400,"",fonts[2]);
    output(200,300,"",fonts[2]);
    output(200,200,"",fonts[2]);
    output(200,100,"",fonts[2]);
    output(200,00,"",fonts[2]);
    output(200,-100,"",fonts[2]);
    glPopMatrix();
    break;

    case 3:
    glPushMatrix();
    glColor3f(0,0,1);
    output(200,1000,"As we already know",fonts[2]);
    output(200,900,"there are vortices ",fonts[2]);
    output(200,800,"created by the air",fonts[2]);
    output(200,700,"entering the Tesla turbine.",fonts[2]);
    output(200,600,"But the interaction ",fonts[2]);
    output(200,500,"between the two is the",fonts[2]);
    output(200,400,"key to the turbine's efficiency",fonts[2]);
    output(200,300,"and simplicity.",fonts[2]);
    output(200,200,"",fonts[2]);
    output(200,100,"",fonts[2]);
    output(200,00,"",fonts[2]);
    output(200,-100,"",fonts[2]);
    glPopMatrix();
    break;

    case 4:
    glPushMatrix();
    glColor3f(0,0,1);
    output(200,1000,"At low speed and air",fonts[2]);
    output(200,900,"pressures the profile of the",fonts[2]);
    output(200,800,"air vortex, there is only ",fonts[2]);
    output(200,700,"a small area of contact between",fonts[2]);
    output(200,600,"the rotating discs and the vortex.",fonts[2]);
    output(200,500,"However, as the speed and",fonts[2]);
    output(200,400,"air pressure is increased the ",fonts[2]);
    output(200,300,"centrifugal force of the spinning ",fonts[2]);
    output(200,200,"discs increases.",fonts[2]);
    output(200,100,"",fonts[2]);
    output(200,00,"",fonts[2]);
    output(200,-100,"",fonts[2]);
    glPopMatrix();
    break;

    case 5:
    glPushMatrix();
    glColor3f(0,0,1);
    output(200,1000,"The centrifugal force of the ",fonts[2]);
    output(200,900,"spinning discs is trying to",fonts[2]);
    output(200,800,"force the air outwards, while air",fonts[2]);
    output(200,700,"has no choice but to make it to the",fonts[2]);
    output(200,600,"exhaust ports. This battle between",fonts[2]);
    output(200,500,"airflow & the centrifugal force enables ",fonts[2]);
    output(200,400,"the vortex to become so tight that ",fonts[2]);
    output(200,300,"virtually 100% of the air flow's power",fonts[2]);
    output(200,200,"is going into the rotating discs.",fonts[2]);
    output(200,100,"At this point, the speed of discs",fonts[2]);
    output(200,00,"is equal to the speed of airflow.",fonts[2]);
    output(200,-100,"",fonts[2]);
    glPopMatrix();
    break;
  }
}
void displayTwoTurbines(){

    glClearColor(1,1,1,1);
	double winht = 1.0;
    // glFrustum(-1.0, 1.0, -1.0, 1.0, 1.5, 20.0);

    glOrtho(-winht *64/48.0,winht*64/48.0,-winht,winht,0.1,100.0);

	glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    // gluLookAt(0.8,0.5,-1,0,0,0,0,1,0);
    gluLookAt(1.8,0.9,6,0,0.25,0,0,1,0);
	glColor3f(0,0,0);
    output(-1.3,1.8,"TESLA TUBINE AS A PUMP",fonts[2]);
	glPushMatrix();
	glTranslatef(0,-0.35,0);
	
	glPushMatrix();
	glColor3f(1,0,0);
	glRotatef(theta[0],1,0,0);
    glRotatef(theta[1],0,1,0);
    glRotatef(theta[2],0,0,1);

	glPushMatrix();
	glTranslatef(-0.63,0.84,0.3);
	glRotatef(90,0,1,0);
	glColor4f(0.25,0.25,0.25,0.5);
	draw_cylinder(0.05,1.08,0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(-0.4,1,0.3);
	glRotatef(90,1,0,0);
	glColor4f(0.2,0.2,0.2,0.5);
	draw_cylinder(0.03,0.35,0.2);
	glPopMatrix();

	glPushMatrix();
	glTranslatef(0.42,1,0.3);
	glRotatef(90,1,0,0);
	glColor4f(0.2,0.2,0.2,0.5);
	draw_cylinder(0.03,0.35,0.2);
	glPopMatrix();

//left turbine
	glPushMatrix();
	glTranslatef(-0.6,0.3,0);
	glScalef(0.6,0.6,0.6);
 // innner blades
        glPushMatrix();
        // glScalef(0.08,0.08,0.08);
        glScalef(0.2,0.2,0.2);
        glTranslatef(0,0,-1);
        draw_inner_objects();
        glPopMatrix();

//outer cylinder
    glPushMatrix();

    glScalef(2.5,2.5,2.5);
    draw_outer_cylinder0(0.2,0.25,-0.1);

    glPopMatrix();
    glPopMatrix();
//right turbine

	glPushMatrix();
	glTranslatef(0.6,0.3,0);
	glScalef(0.6,0.6,0.6);

 // innner blades
        glPushMatrix();
        // glScalef(0.08,0.08,0.08);
        glScalef(0.2,0.2,0.2);
        glTranslatef(0,0,-1);
        draw_inner_objects();
        glPopMatrix();

//outer cylinder
    glPushMatrix();

    glScalef(2.5,2.5,2.5);
    draw_outer_cylinder0(0.2,0.25,-0.1);

    glPopMatrix();
    glPopMatrix();
	
	if(boolTest==2)
	{
	   
		glPushMatrix();
		glScalef(0.03,0.03,0.03);
		glTranslatef(26.5,30,0);
		glColor3f(0,0,1);
		glRotatef(90,0,0,1);
		draw_arrow(0,0,0.2,0,0,0);
		 glPopMatrix();

		glPushMatrix();
		glScalef(0.03,0.03,0.03);
		glTranslatef(0,29.7,0);
		glColor3f(0,0,1);
		//glPointSize(10);
		draw_arrow(0,0,0.2,0.0,0,0);
		 glPopMatrix();
		glPushMatrix();
		glScalef(0.2,0.2,0.2);
		glTranslatef(0,4.5,0.03);
		glColor3f(0,0,1);
		//glPointSize(10);
		draw_line(-1.5,-0.05,-0.1,-0.05,0);
		draw_line(4,-1,4,0.05,0);
	 glScalef(0.01,0.01,0.01);

	output(420,10,"liquid outlet",fonts[2]);

    glPopMatrix();
	 glScalef(0.001,0.001,0.001);
	 glColor4f(0.2823,0.4627,1,0.6);
	Z=500;Z1=500;
	 if(r0<224){
	 glPushMatrix();
	 glTranslatef(X,Y,Z);
	draw_small_circle(-570,350,12,0);
	glPopMatrix();
	r1=0;
	} 
	
	 else
	 {
			 glPushMatrix();
			 if(r1>208){ X1=190;Y1=620;Z1=0;}
			  glTranslatef(X1,Y1,Z1);
			  draw_small_circle(620,345,12,0);
			 glPopMatrix();
	 } 
}
	glPopMatrix();
	glPopMatrix();
	glColor3f(0,0,0);
	 glScalef(0.001,0.001,0.001);
 	
	glColor3f(0,0,1);
  output(-1300,880,"--> As mentioned before,if you put a vortex into the turbine it will spin, ",fonts[2]);
  output(-1300,800,"      similarly, if you spin the turbine,a vortex is created ,allowing it to operate as a pump. ",fonts[2]);
  output(-1300,720,"--> The turbine is spun by a motor.It will suck in at what were previously the outlets  ",fonts[2]);
  output(-1300,640,"      and expels at what were previously the air inlets,complete reversal!!",fonts[2]);
	 
  glColor3f(0.2,0.3,0.4);
  output(-1200,-600,"Press 'p' for previous slide and 'z' for working !",fonts[2]);
 	

}



void display(void){

  glClear(GL_COLOR_BUFFER_BIT|GL_DEPTH_BUFFER_BIT);
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();

  switch (slideNumber) {
    case 1:
      displayFirstSlide();
     
      break;
    case 2:
      displaySecondSlide();
      break;
    case 3:
      displaySecondSlidePartOne();
      break;
    case 4:
      displaySecondSlidePartTwo();
      break;
    case 5:
      displaySecondSlidePartThree();
      break;
    case 6:
      displayWorkingOfTurbine();
	  break;
	case 7:
		displayTwoTurbines();
		break;
  }

  // displayFirstSlide();
  // displaySecondSlide();

  // output(1,1, "hello world",fonts[2]);
  //
  // draw_arrow(5,5,8,5);

  glFlush();

  glutSwapBuffers();

}

void keys(unsigned char key,int x,int y)
{
  if(key=='s')
  {
    theta[0]+=5;
    if(theta[0]>360) theta[0]-=360;
  }
  if(key=='w')
  {
    theta[0]-=5;
    if(theta[0]<0) theta[0]+=360;
  }
  if(key=='d')
  {
	if(workingSlideNumber!=1 || slideNumber==2 || slideNumber==3 || slideNumber==4 || slideNumber==5 ||slideNumber==7){
    
    theta[1]+=5;
    if(theta[1]>360) theta[1]-=360;
	}
  }
  if(key=='a')
  {
	if(workingSlideNumber!=1 || slideNumber==2 || slideNumber==3 || slideNumber==4 || slideNumber==5 ||slideNumber==7){

 		theta[1]-=5;
		if(theta[1]<0) theta[1]+=360;
	}
 
  }
  if(key=='z'){
	  if(slideNumber==7){
			boolTest=2;
			r0=0;r1=0;
	  }
  }
  if(key=='q')
  {
    theta[2]+=5;
    if(theta[2]>360) theta[2]-=360;
  }
  if(key=='e')
  {
    theta[2]-=5;
    if(theta[2]<0) theta[2]+=360;
  }
 
  if(key=='n')
  {
    switch (slideNumber) {
      case 1:
              theta[0]=0;
              theta[1]=0;
              theta[2]=0;
              slideNumber++;
              break;

      case 2:
              theta[0]=0;
              theta[1]=35;
              theta[2]=0;
		  r2=0.4;r3=0.12;

			  workingSlideNumber=1;
			  slideNumber=6;
              break;

      case 6:
              theta[0]=0;
		  theta[1]=10;
		  theta[2]=0;
		  slideNumber=7;
          break;
    }
  }
  if(key=='p')
  {
    switch (slideNumber) {
      case 2:
            slideNumber=1;
            break;
      case 6:
              theta[0]=0;
              theta[1]=0;
              theta[2]=0;
          slideNumber = 2;
          break;
      case 7:
              theta[0]=0;
		      theta[1]=35;
              theta[2]=0;
		  slideNumber = 6;
		  r2=0.4;r3=0.12;
		  workingSlideNumber = 1;
		  boolTest=0;
		  break;

    }
  }
  if(key=='1'){
	  theta[0]=0;
          theta[1]=0;
              theta[2]=0;
    slideNumber = 3;
  }
  if(key=='2'){
	      slideNumber = 4;
  }
  if(key=='3'){
	 
    slideNumber = 5;
  }
  if(key=='b'){
	  theta[0]=0;
          theta[1]=0;
              theta[2]=0;
    slideNumber=2;
  }
  if(key=='.'){
		  r2=0.4;r3=0.12;

    if(workingSlideNumber!=5)

    workingSlideNumber++;
  }
  if(key==','){
		  r2=0.4;r3=0.12;
  if(workingSlideNumber==3 || workingSlideNumber==2 )
  {
	   theta[0]=0;
	  theta[1]=35;
	  theta[2]=0;
  }
  	          
  if(workingSlideNumber!=1)
  {	
	  workingSlideNumber--;
  }
  }
  
glutPostRedisplay();

}
void reshape(int x, int y)
{
    glViewport(0,0,x,y);  //Use the whole window for rendering
    glMatrixMode(GL_PROJECTION);  
    glLoadIdentity(); 
	if(x<=y)
		glOrtho(-2.0,2.0,-2.0*(GLfloat)y/(GLfloat)x,2.0*(GLfloat)y/(GLfloat)x,-10.0,10.0);
	else
		glOrtho(-2.0*(GLfloat)x/(GLfloat)y,2.0*(GLfloat)x/(GLfloat)y,-2.0,2.0,-10.0,10.0);

    glMatrixMode(GL_MODELVIEW);
	glutPostRedisplay();

} 

void spin (){
  rotateAngle += 1.5;
  if(rotateAngle>360)
    rotateAngle -=360;
  X=r0*cos(rotateAngle*deg2grad);
  Y=r0*sin(rotateAngle*deg2grad);
  X1=r1*cos(rotateAngle*deg2grad);
  Y1=r1*sin(rotateAngle*deg2grad);
  if(r3>=0){
  X2=r3*cos(rotateAngle*deg2grad);
  Y2=r3*sin(rotateAngle*deg2grad);
  Z1=0.2;
  r3-=0.00016;
  }
  else{ X2=0,Y2=0,Z1=-2.4; }
  
  X3=-0.24;
  Y3=r2;
  
  r0+=0.3;
  r1+=0.3;
  r2-=0.0006;
  
  glutPostRedisplay();
}


int main(int argc,char **argv)
{
  glutInit(&argc,argv);
  glutInitDisplayMode(GLUT_DOUBLE|GLUT_RGB|GLUT_DEPTH);
  glutInitWindowSize(1000,1000);
  glutInitWindowPosition(0,0);
  glutCreateWindow("turbine");
  glutReshapeFunc(reshape);
  glutDisplayFunc(display);
  glutIdleFunc(spin);
  glutKeyboardFunc(keys);
  glEnable(GL_DEPTH_TEST);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
  glEnable( GL_BLEND );
  glClearColor(1,1,1,0.0);
  glViewport(0,0,640,480);
  glutMainLoop();
}

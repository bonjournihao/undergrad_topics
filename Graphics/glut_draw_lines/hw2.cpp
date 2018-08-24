//created by Xie Zhou on 10/25/2017

#ifdef WIN32
#include <windows.h>
#endif

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
//#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

//other includes
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>
#include <sstream>
#include <algorithm>
#include <string>
#include <fstream>
#include <math.h>
using namespace std;

#define SCALE 0
#define TRANSLATE 1
#define ROTATE 2
#define POLY 3
#define RESET 4
#define QUIT 5
#define XY 6
#define XZ 7
#define YZ 8

#define PI 3.14159265

/****set in main()****/
//the number of pixels in the grid
int grid_width;
int grid_height;

//the size of pixels sets the inital window height and width
//don't make the pixels too large or the screen size will be larger than
//your display size
float pixel_size;

/*Window information*/
int win_height;
int win_width;


void init();
void idle();
void createMenu();
void processMenuEvents(int option);
void display();
void draw_line(vector<float> p1, vector<float> p2);
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();
void load_data();
int menu();
void scale_poly();
void draw_axis(vector<float> p1, vector<float> p2);
vector<float> rotation_z_single(float degree, vector<float> point_vector);
vector<vector<float> > scaling(vector<vector<float> > list, float scale_vector);
void translate_poly();
vector<vector<float> > translation(vector<vector<float> > list, float x, float y, float z);
void rotate_poly();
vector<vector<float> > rotation_x(float degree, vector<vector<float> > list);
vector<vector<float> > rotation_y(float degree, vector<vector<float> > list);
vector<vector<float> > rotation_z(float degree, vector<vector<float> > list);
vector<float> get_center(vector<vector<float> > list);
void draw_polygons(vector<vector<vector<float> > > point_lists, vector<vector<vector<int> > > connections);
void save_points();

//polygon info
int polyNum;
string filename;
vector<vector<vector<float> > > point_lists;
vector<vector<vector<float> > > point_lists_copy;
vector<vector<vector<float> > > edge_lists;
vector<vector<vector<int> > > edge_connections;
vector<float> point1;
vector<float> point2;

//variables
int which_poly = 0;
int choose_ploy = 0;
int set_scale = 0;
int set_translate = 0;
int set_rotate = 0;
int set_reset = 0;
int set_quit = 0;
int plane = 0;
int draw_rotation_axis = 0;

int win;
int main(int argc, char **argv)
{
    
    //the number of pixels in the grid
    grid_width = 100;
    grid_height = 100;
    
    //the size of pixels sets the inital window height and width
    //don't make the pixels too large or the screen size will be larger than
    //your display size
    pixel_size = 5;
    
    /*Window information*/
    win_height = grid_height*pixel_size;
    win_width = grid_width*pixel_size;
    
	/*Set up glut functions*/
    /** See https://www.opengl.org/resources/libraries/glut/spec3/spec3.html ***/
    
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
    /*initialize variables, allocate memory, create buffers, etc. */
    //create window of size (win_width x win_height
    glutInitWindowSize(win_width,win_height);
    //windown title is "glut demo"
	win = glutCreateWindow("glut demo");
    
	/*defined glut callback functions*/
	glutDisplayFunc(display); //rendering calls here
	glutReshapeFunc(reshape); //update GL on window size change
	glutMouseFunc(mouse);     //mouse button events
	glutMotionFunc(motion);   //mouse movement events
	glutKeyboardFunc(key);    //Keyboard events
	glutIdleFunc(idle);       //Function called while program is sitting "idle"
    
    //initialize opengl variables
    init();
    //create a menu
    createMenu();
    //load_data
    load_data();
    //
    for(int i = 0; i < 3; i++){
        float x = 0;
        point1.push_back(x);
        point2.push_back(x);
    }
    //start glut event loop
	glutMainLoop();
	return 0;
}

/*initialize gl stufff*/
void init()
{
    //set clear color (Default background to white)
	glClearColor(1.0,1.0,1.0,1.0);
    //checks for OpenGL errors
	check();
}

void draw_axis(){

    if(plane == 0)
        glColor3f(1.0,0.0,0.0); //xy plane, red
    else if(plane == 1)
        glColor3f(1.0,0.0,1.0); //xz plane purple
    else
        glColor3f(0.0,1.0,0.0); // yz plane green

    glBegin(GL_LINES);
    // x axis
    glVertex3f(-1.0, 0.0f, 0.0f);
    glVertex3f(1.0, 0.0f, 0.0f);
    // arrow
    glVertex3f(1.0, 0.0f, 0.0f);
    glVertex3f(0.95, 0.05f, 0.0f);
 
    glVertex3f(1.0, 0.0f, 0.0f);
    glVertex3f(0.95, -0.05f, 0.0f);

    // y axis
    glVertex3f(0.0, -1.0f, 0.0f);
    glVertex3f(0.0, 1.0f, 0.0f);
    // arrow
    glVertex3f(0.0, 1.0f, 0.0f);
    glVertex3f(0.05, 0.95f, 0.0f);
    glVertex3f(0.0, 1.0f, 0.0f);
    glVertex3f(-0.05, 0.95f, 0.0f);

    // z axis
    glVertex3f(0.0, 0.0f ,-1.0f );
    glVertex3f(0.0, 0.0f ,1.0f );
    // arrow
    glVertex3f(0.0, 0.0f ,1.0f );
    glVertex3f(0.0, 0.05f ,0.95f );
    glVertex3f(0.0, 0.0f ,1.0f );
    glVertex3f(0.0, -0.05f ,0.95f );
    glEnd();
    glFlush();
}

void createMenu() {
    int menu;

    // create the menu and
    // tell glut that "processMenuEvents" will
    // handle the events
    menu = glutCreateMenu(processMenuEvents);

    //add entries to our menu
    glutAddMenuEntry("choose polyhedron",POLY);
    glutAddMenuEntry("set scaling factor",SCALE);
    glutAddMenuEntry("set translation vector",TRANSLATE);
    glutAddMenuEntry("set rotation axis and angle",ROTATE);
    glutAddMenuEntry("projection XY (red)",XY);
    glutAddMenuEntry("projection XZ (purple)",XZ);
    glutAddMenuEntry("projection YZ (green)",YZ);
    glutAddMenuEntry("reset all points",RESET);
    glutAddMenuEntry("--Quit--",QUIT);

    // attach the menu to the right button
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void processMenuEvents(int option) {

    switch (option) {
        case POLY :
            choose_ploy = 1;
            break;
        case SCALE :
            set_scale = 1;
            break;
        case TRANSLATE :
            set_translate = 1;
            break;
        case ROTATE :
            set_rotate = 1;
            break;
        case XY :
            plane = 0;
            break;
        case XZ :
            plane = 1;
            break;
        case YZ :
            plane = 2;
            break;
        case RESET :
            set_reset = 1;
            break;
        case QUIT :
            set_quit = 1;
            break;
    }
}


//called repeatedly when glut isn't doing anything else
void idle()
{
    //redraw the scene over and over again
	glutPostRedisplay();	
}

void print_vertex_info(){
    cout << "Vertices of polygon -> " << which_poly << endl;
    for(int i = 0; i < point_lists[which_poly].size(); i++){
        cout << "(" << point_lists[which_poly][i][0] << "," << point_lists[which_poly][i][1] << "," << point_lists[which_poly][i][2] << ")\n";
    }
}

//this is where we render the screen
void display()
{
    //clears the screen
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix
	glLoadIdentity();

    draw_axis();
    draw_polygons(point_lists,edge_connections);

    if(choose_ploy == 1){
        cout << "Please choose a polygon: ";
        cin >> which_poly;
        choose_ploy = 0;
    }
    if(set_scale == 1){
        print_vertex_info();
        scale_poly();
        print_vertex_info();
        set_scale = 0;
    }
    if(set_translate == 1){
        print_vertex_info();
        translate_poly();
        print_vertex_info();
        set_translate = 0;
    }
    if(set_rotate == 1){
        print_vertex_info();
        rotate_poly();

        print_vertex_info();
        set_rotate = 0;
    }
    if(set_reset == 1){
        point_lists = point_lists_copy;
        set_reset = 0;
    }
    if(set_quit == 1){
        glutDestroyWindow(win);
        save_points();
        exit(0);
    }

    //blits the current opengl framebuffer on the screen
    glutSwapBuffers();
    //checks for opengl errors
	check();
}

void scale_poly(){
    float scale_factor;
    cout << "scaling factor: ";
    cin >> scale_factor; 

    //translate to origin
    vector<float> center = get_center(point_lists[which_poly]);
    float x_diff = 0 - center[0];
    float y_diff = 0 - center[1];
    float z_diff = 0 - center[2];
    vector<vector<float> > temp0 = translation(point_lists[which_poly],x_diff,y_diff,z_diff);
    //scaling
    vector<vector<float> > temp1 = scaling(temp0, scale_factor);
    //translate back
    point_lists[which_poly] = translation(temp1, -x_diff,-y_diff,-z_diff);
}


vector<vector<float> > scaling(vector<vector<float> > list, float scale_vector){
    for(int i = 0; i < list.size(); i++){
        list[i][0] *= scale_vector;
        list[i][1] *= scale_vector;
        list[i][2] *= scale_vector;
    }
    return list;
}

void translate_poly(){
    float x_diff, y_diff, z_diff;
    cout << "Please enter translation vector: \nx: ";
    cin >> x_diff;
    cout << "y: ";
    cin >> y_diff;
    cout << "z: ";
    cin >> z_diff;
    point_lists[which_poly] = translation(point_lists[which_poly],x_diff,y_diff,z_diff);
}

vector<vector<float> > translation(vector<vector<float> > list, float x, float y, float z){
    vector<vector<float> > temp_list;

    int numVertices = list.size();
    for(int i= 0; i < numVertices;i++){
        vector<float> temp_point;
        temp_point.push_back(x + list[i][0]);
        temp_point.push_back(y + list[i][1]);
        temp_point.push_back(z + list[i][2]);
        temp_list.push_back(temp_point);
    }
    return temp_list;
}

void rotate_poly(){
    float x1,y1,z1,x2,y2,z2;
    float degree;

    cout <<"Please enter a vector (2 points) to define the rotation axis\n";
    cout << "--Point 1--\nx: ";

    cin >> x1;
    cout << "y: ";
    cin >> y1;
    cout << "z: ";
    cin >> z1;
    cout << "--Point 2--\nx: ";
    cin >> x2;
    cout << "y: ";
    cin >> y2;
    cout << "z: ";
    cin >> z2;

    draw_rotation_axis = 1;
    point1[0] = x1;
    point1[1] = y1;
    point1[2] = z1;
    point2[0] = x2;
    point2[1] = y2;
    point2[2] = z2;

    draw_axis(point1,point2);

    cout << "Rotation angle in degrees: ";
    cin >> degree;


    float delta_x = x2 - x1;
    float delta_y = y2 - y1;
    float delta_z = z2 - z1;
    if(delta_x == 0 && delta_y == 0 && delta_z == 0){
        cout << "!!!Note: 2 points cannot be the same!!!\n";
        return;
    }

    if(delta_y != 0){
        // 1)translate axis to origin (find the xz plane intersection, y = 0)
        float x_intercept = x1 - (y1/delta_y)*delta_x;
        float z_intercept = z1 - (y1/delta_y)*delta_z;

        float x_diff = 0 - x_intercept;
        float y_diff = 0;
        float z_diff = 0 - z_intercept;

        vector<vector<float> > temp1 = translation(point_lists[which_poly],x_diff,y_diff,z_diff);

        if(delta_z != 0){
            if(delta_x != 0){
                vector<float> direction_vector,resulting_vector;
                direction_vector.push_back(delta_x);
                direction_vector.push_back(delta_y);
                direction_vector.push_back(delta_z);
                resulting_vector = rotation_z_single(degree,direction_vector);

                float alpha = (atan(delta_x/delta_z)* 180)/PI;

                //2)rotation to yz plane (around z)
                vector<vector<float> > temp2 = rotation_z(alpha,temp1);
                float beta = (atan(resulting_vector[2]/resulting_vector[1])* 180)/PI;
                /*
                cout <<"rot z";
                for(int i = 0; i < temp2.size();i++){
                    float x = temp2[i][0];
                    float y = temp2[i][1];
                    float z = temp2[i][2];
                    cout << x << " " << y << " " << z << endl;
                }
                */
                    
                //3)rotation to yx plane (around x)
                vector<vector<float> > temp3 = rotation_x(-beta,temp2);

                /*
                cout <<"rot x";
                for(int i = 0; i < temp3.size();i++){
                    float x = temp3[i][0];
                    float y = temp3[i][1];
                    float z = temp3[i][2];
                    cout << x << " " << y << " " << z << endl;
                }
                */
                
                //4)rotation around y
                vector<vector<float> > temp4 = rotation_y(degree,temp3);
                
                /*
                cout <<"rot y!!";
                for(int i = 0; i < temp3.size();i++){
                    float x = temp4[i][0];
                    float y = temp4[i][1];
                    float z = temp4[i][2];
                    cout << x << " " << y << " " << z << endl;
                }
                */

                //5)inverse of 3)
                vector<vector<float> > temp5 = rotation_x(beta,temp4);
                
                /*
                cout <<"rot -x";
                for(int i = 0; i < temp3.size();i++){
                    float x = temp5[i][0];
                    float y = temp5[i][1];
                    float z = temp5[i][2];
                    cout << x << " " << y << " " << z << endl;
                }
                */
                
                //6)inverse of 2)
                vector<vector<float> > temp6 = rotation_z(-alpha,temp5);
                
                /*
                cout <<"rot -z";
                for(int i = 0; i < temp3.size();i++){
                    float x = temp6[i][0];
                    float y = temp6[i][1];
                    float z = temp6[i][2];
                    cout << x << " " << y << " " << z << endl;
                }
                */
                
                //7)translate back
                vector<vector<float> > temp7 = translation(temp6,-x_diff,-y_diff,-z_diff);
                point_lists[which_poly] = temp7;//delta_y != 0, delta_x != 0, delta_z != 0
            }
            else{ //delta_y != 0, delta_x = 0, delta_z != 0 rotate x, y
                //cout <<"x is 0" << endl;
                float alpha = (atan(delta_z/delta_y)*180)/PI;
                vector<vector<float> > temp2 = rotation_x(-alpha,temp1);
                vector<vector<float> > temp3 = rotation_y(degree,temp2);
                vector<vector<float> > temp4 = rotation_x(alpha,temp3);
                vector<vector<float> > temp5 = translation(temp4,-x_diff,-y_diff,-z_diff);
                point_lists[which_poly] = temp5;
            }
        }
        else{
            if(delta_x != 0){//delta_y = !0, delta_x != 0, delta_z = 0 rotate z,y
                float alpha = (atan(delta_x/delta_y)*180)/PI;
                vector<vector<float> > temp2 = rotation_z(alpha,temp1);
                vector<vector<float> > temp3 = rotation_y(degree,temp2);
                vector<vector<float> > temp4 = rotation_z(-alpha,temp3);
                vector<vector<float> > temp5 = translation(temp4,-x_diff,-y_diff,-z_diff);
                point_lists[which_poly] = temp5;
            }
            else{//delta_y = !0, delta_x = 0, delta_z = 0 rotate y
                //cout << "rotate around y" << endl;
                //cout << "x_diff: " << x_diff << " y_diff: "<< y_diff <<" z_diff: "<< z_diff << endl;
                vector<vector<float> > temp2 = rotation_y(degree,temp1);
                vector<vector<float> > temp3 = translation(temp2,-x_diff,-y_diff,-z_diff);
                point_lists[which_poly] = temp3;
            }
        }
    }
    else{
        if(delta_x != 0){
            if(delta_z != 0){//delta_y = 0, delta_x != 0, delta_z != 0 rotate y,z
                float x_intercept = (delta_x/delta_z)*((-1)*z1)+x1;
                float x_diff = 0 - x_intercept;
                float y_diff = 0 - y1;
                float z_diff = 0;
                float alpha = (atan(delta_x/delta_z)*180)/PI;
                vector<vector<float> > temp1 = translation(point_lists[which_poly],x_diff,y_diff,z_diff);
                vector<vector<float> > temp2 = rotation_y(-alpha,temp1);
                vector<vector<float> > temp3 = rotation_z(degree,temp2);
                vector<vector<float> > temp4 = rotation_y(alpha,temp3);
                vector<vector<float> > temp5 = translation(temp4,-x_diff,-y_diff,-z_diff);
                point_lists[which_poly] = temp5;
            }
            else{ //delta_y = 0, delta_x != 0, delta_z = 0 rotate x
                float x_diff = 0;
                float y_diff = 0 - y1;
                float z_diff = 0 - z1;
                vector<vector<float> > temp1 = translation(point_lists[which_poly],x_diff,y_diff,z_diff); 
                vector<vector<float> > temp2 = rotation_x(degree,temp1);
                vector<vector<float> > temp3 = translation(temp2,-x_diff,-y_diff,-z_diff);
                point_lists[which_poly] = temp3;
            }

        }
        else{//delta_y = 0, delta_x = 0, delta_z != 0 rotate z
            float x_diff = 0 - x1;
            float y_diff = 0 - y1;
            float z_diff = 0;
            vector<vector<float> > temp1 = translation(point_lists[which_poly],x_diff,y_diff,z_diff); 
            vector<vector<float> > temp2 = rotation_z(degree,temp1);
            vector<vector<float> > temp3 = translation(temp2,-x_diff,-y_diff,-z_diff);
            point_lists[which_poly] = temp3;
        }
    }
}

vector<vector<float> > rotation_x(float degree, vector<vector<float> > list){
    //cout << "rotation_x" << endl;
    float radian = (degree * PI)/180.0;
    for(int i = 0; i < list.size(); i++){
        float x = list[i][0];
        float y = cos(radian)*list[i][1] + (-1)*sin(radian)*list[i][2];
        float z = sin(radian)*list[i][1] + cos(radian)*list[i][2];

        list[i][0] = x;
        list[i][1] = y;
        list[i][2] = z;
    }
    return list;
}

vector<vector<float> > rotation_y(float degree, vector<vector<float> > list){
    float radian = (degree * PI)/180.0;
    for(int i = 0; i < list.size(); i++){
        float x = cos(radian)*list[i][0] + sin(radian)*list[i][2];
        float y = list[i][1];
        float z = (-1)*sin(radian)*list[i][0] + cos(radian)*list[i][2];
        list[i][0] = x;
        list[i][1] = y;
        list[i][2] = z;
    }
    return list;
}

vector<float> rotation_z_single(float degree, vector<float> point_vector){
    vector<float> resulting_vector;
    float radian = (degree * PI)/180.0;
    resulting_vector.push_back(cos(radian)*point_vector[0] + (-1)*sin(radian)*point_vector[1]);
    resulting_vector.push_back(sin(radian)*point_vector[0] + cos(radian)*point_vector[1]);
    resulting_vector.push_back(point_vector[2]);
    return resulting_vector;
}

vector<vector<float> > rotation_z(float degree, vector<vector<float> > list){
    float radian = (degree * PI)/180.0;
    //cout << "rotation_z" << endl;
    for(int i = 0; i < list.size(); i++){
        float x = cos(radian)*list[i][0] + (-1)*sin(radian)*list[i][1];
        float y = sin(radian)*list[i][0] + cos(radian)*list[i][1];
        float z = list[i][2];
        /*
        cout <<"x:   " << list[i][0] << " to " << x << endl;
        cout <<"y:   " << list[i][1] << " to " << y << endl;
        cout <<"z:   " << list[i][2] << " to " << z << endl;*/
        list[i][0] = x;
        list[i][1] = y;
        list[i][2] = z;
    }
    return list;
}

//load point_lists and edge_connections
void load_data(){
    //filename = "datafile.data";
    //filename;

    
    cout << "Please enter the data filename: ";
    while(!(cin >> filename)){
        cout << "Please enter the correct filename: ";
        cin >> filename;
    }
    

    //open file
    ifstream infile(filename.c_str());
    string line;
    //vector<int> num_point_lists;

    //number of polygons
    infile >> polyNum;

    edge_connections.clear();
    //read line by line
    int num_points = 0;
    while(infile>>num_points){
        //number of points in nth polygon
        //load points in nth polygon
        vector<vector<float> > single_point_list;
        for(int i = 0; i < num_points; i++){
            float x,y,z;
            vector<float> temp_point;
            infile >> x >> y >> z;
            //cout << "x: " << x << " y: " << y <<" z: " << z << endl;  
            temp_point.push_back(x);
            temp_point.push_back(y);
            temp_point.push_back(z);
            single_point_list.push_back(temp_point);
        }
        point_lists.push_back(single_point_list);
        point_lists_copy.push_back(single_point_list);
        //number of edges in nth polygon
        int num_edges = 0;
        infile >> num_edges;
        //load edge_connections
        vector<vector<int> > single_edge_connection;
        for(int i = 0; i < num_edges; i++){
            vector<int> connection;
            int index1, index2;
            infile >> index1 >> index2;
            //cout << "index1: " << index1 << " index2: " << index2 << endl;
            connection.push_back(index1);
            connection.push_back(index2);
            single_edge_connection.push_back(connection);
        }
        edge_connections.push_back(single_edge_connection);
    }
}

void draw_polygons(vector<vector<vector<float> > > lists, vector<vector<vector<int> > > connections){
    if(draw_rotation_axis == 1)
        draw_axis(point1,point2);

    for(int i = 0; i < connections.size(); i++){
        //cout << "polygon: " << i << " " << endl;
        int index1,index2;
        for(int j = 0; j < connections[i].size(); j++){
            index1 = connections[i][j][0]-1;
            index2 = connections[i][j][1]-1;
            //cout << "    connection: " << j << " " << index1 << " " << index2 << endl;

            vector<float> p1 = lists[i][index1];
            //cout << "\nx1: " << p1[0] <<  " y1: " << p1[1] <<  " z1: " << p1[2] << endl;
            vector<float> p2 = lists[i][index2];
            //cout << "x2: " << p2[0] <<  " y2: " << p2[1] <<  " z2: " << p2[2] << endl;
            draw_line(p1,p2);
        }
    }
}

void draw_axis(vector<float> p1, vector<float> p2){
    glLineWidth(2.0);
    glBegin(GL_LINES);
    glColor3f(0,0,0);

    if(plane == 0){ //xy
        glVertex3f(p1[0],p1[1],p1[2]);
        glVertex3f(p2[0],p2[1],p2[2]);
    }
    else if(plane == 1){ //xz
        glVertex3f(p1[0],p1[2],p1[1]);
        glVertex3f(p2[0],p2[2],p2[1]);
    }
    else{ // yz
        glVertex3f(p1[1],p1[2],p1[0]);
        glVertex3f(p2[1],p2[2],p2[0]);
    }
    glEnd();
}

void draw_line(vector<float> p1, vector<float> p2){
    glLineWidth(1.0); //sets the "width" of each line we are rendering

    //tells opengl to interperate every two calls to glVertex as a line
    glBegin(GL_LINES);
    //first line will be blue    
    glColor3f(0,0,1.0);

    if(plane == 0){ //xy
        glVertex3f(p1[0],p1[1],p1[2]);
        glVertex3f(p2[0],p2[1],p2[2]);
    }
    else if(plane == 1){ //xz
        glVertex3f(p1[0],p1[2],p1[1]);
        glVertex3f(p2[0],p2[2],p2[1]);
    }
    else{ // yz
        glVertex3f(p1[1],p1[2],p1[0]);
        glVertex3f(p2[1],p2[2],p2[0]);
    }

    //this will be a red line
    //notice we can use 3d points too
    //how will this change if we project to the XZ or YZ plane?
    //glColor3f(1.0,0.0,0.0);
    //glVertex3f(0.1,0.9,0.5);
    //glVertex3f(0.9,0.1,0.3);
    glEnd();
}

vector<float> get_center(vector<vector<float> > list){
    vector<float> temp;
    float xSum = 0;
    float ySum = 0;
    float zSum = 0;
    float xCenter,yCenter,zCenter;
    int numVertices = list.size();
    for(int i= 0; i < numVertices;i++){
        xSum += list[i][0];
        ySum += list[i][1];
        zSum += list[i][2];
    }
    xCenter = xSum/numVertices;
    yCenter = ySum/numVertices;
    zCenter = zSum/numVertices;

    temp.push_back(xCenter);
    temp.push_back(yCenter);
    temp.push_back(zCenter);
    return temp;
}

void save_points(){
    //open file
    ofstream outfile(filename.c_str());

    //number of polygons
    outfile << polyNum;
    outfile << "\n";

    //write line by line
    for(int j = 0; j < polyNum; j++){
        //number of points in jth polygon
        int num_points = point_lists[j].size();
        outfile << "\n";
        outfile << num_points;
        outfile << "\n";
        for(int i = 0; i < num_points; i++){
            outfile << point_lists[j][i][0] << " " << point_lists[j][i][1] << " " << point_lists[j][i][2] << "\n";
        }
        //number of edges in jth polygon
        int num_edges = edge_connections[j].size();
        outfile << "\n";
        outfile << num_edges;
        outfile << "\n";
        for(int i = 0; i < num_edges; i++){
            outfile << edge_connections[j][i][0] << " " << edge_connections[j][i][1] << "\n";
        }
    }
}

/*Gets called when display size changes, including initial craetion of the display*/
/*this needs to be fixed so that the aspect ratio of the screen is consistant with the orthographic projection*/

void reshape(int width, int height)
{
	/*set up projection matrix to define the view port*/
    //update the ne window width and height
	win_width = width;
	win_height = height;
    
    //creates a rendering area across the window
	glViewport(0,0,width,height);

    // uses an orthogonal projection matrix so that
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    //you guys should look up this function
	//glOrtho(0,1.0,0.0,1.0,-10,10);
    glOrtho(-0.1,1.0,-0.1,1.0,-1,1.0);
    
    //clear the modelview matrix
    //the ModelView Matrix can be used in this project, to change the view on the projection
    //but you can also leave it alone and deal with changing the projection to a different view
    //for project 2, do not use the modelview matrix to transform the actual geometry, as you won't
    //be able to save the results
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //check for opengl errors
    check();
}

//gets called when a key is pressed on the keyboard
void key(unsigned char ch, int x, int y)
{
	switch(ch)
	{
		default:
            //prints out which key the user hit
            printf("User hit the \"%c\" key\n",ch);
			break;
	}
    //redraw the scene after keyboard input
	glutPostRedisplay();
}


//gets called when a mouse button is pressed
void mouse(int button, int state, int x, int y)
{
    //print the pixel location, and the grid location
    printf ("MOUSE AT PIXEL: %d %d, GRID: %d %d\n",x,y,(int)(x/pixel_size),(int)((win_height-y)/pixel_size));
	switch(button)
	{
		case GLUT_LEFT_BUTTON: //left button
            printf("LEFT ");
            break;
		case GLUT_RIGHT_BUTTON: //right button
            printf("RIGHT ");
		default:
            printf("UNKNOWN "); //any other mouse button
			break;
	}
    if(state !=GLUT_DOWN)  //button released
        printf("BUTTON UP\n");
    else
        printf("BUTTON DOWN\n");  //button clicked
    
    //redraw the scene after mouse click
    glutPostRedisplay();
}

//gets called when the curser moves accross the scene
void motion(int x, int y)
{
    //redraw the scene after mouse movement
	glutPostRedisplay();
}

//checks for any opengl errors in the previous calls and
//outputs if they are present
void check()
{
	GLenum err = glGetError();
	if(err != GL_NO_ERROR)
	{
		printf("GLERROR: There was an error %s\n",gluErrorString(err) );
		exit(1);
	}
}

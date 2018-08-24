/*
 * Simple glut demo that can be used as a template for
 * other projects by Garrett Aldrich
 */


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
#include <math.h>
#include <vector>
#include <cstring>
#include <string>
#include <iostream>
using namespace std;

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
void display();
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();

void processMenuEvents(int option);
void createGLUTMenus();

vector<int> make_point(int x, int y);
void draw_pix(float x, float y, int option);
void draw_line(vector<float> p1, vector<float> p2, int option);
void draw_lines(vector<vector<int> > list, int option);
int find_point(vector<vector<int> > list, vector<int> p);
void draw_points(vector<vector<int> > list, int option);
void draw_points(vector<vector<float> > list, int option);
vector<vector<float> > init_matrix(int w, int h);
vector<vector<float> > bazier(vector<vector<int> > points);
vector<vector<float> > bspline(vector<vector<int> > points, int line_index);
vector<float> calculate_each_t(vector<vector<int> > points, float t);
vector<float> make_point(float x, float y);
float deBoor_x(int degree, int i, float x, vector<float> knots, vector<vector<int> > list);
float deBoor_y(int degree, int i, float x, vector<float> knots, vector<vector<int> > list);

//containers
vector<vector<vector<int> > > point_lists;
vector<int> curve_choices;
vector<int> bspline_degrees;
vector<vector<float> > bspline_knots;
//variables
int which_line = 0;
int point_to_move = -1;
int point_selected = -1;

//states
int choose_line = 0;
int add_points = 1;
int connect_points = 1;
int delete_points = 0;
int bazier_curve = 0;
int bspline_curve = 0;
int move_points = 0;
int add_lines = 0;
int insert_points = 0;
int change_bspline = 0;
int set_quit = 0;
//
#define ADDPOINT 1
#define CONNECT 2
#define DELETE 3
#define MOVE 4
#define NEWLINE 5
#define INSERT 6
#define CHOOSE 7
#define BAZ 8
#define BSPL 9
#define CHANGE 10
#define QUIT 100

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
	win = glutCreateWindow("glut");
    
	/*defined glut callback functions*/
	glutDisplayFunc(display); //rendering calls here
	glutReshapeFunc(reshape); //update GL on window size change
	glutMouseFunc(mouse);     //mouse button events
	glutMotionFunc(motion);   //mouse movement events
	glutKeyboardFunc(key);    //Keyboard events
	glutIdleFunc(idle);       //Function called while program is sitting "idle"
    
    //create a menu
    createGLUTMenus();
    //initialize opengl variables
    init();
    //initialize point_lists
    vector<vector<int> > line0;
    point_lists.push_back(line0);
    curve_choices.push_back(-1);

    bspline_degrees.push_back(2);
    vector<float> temp_knot_list;
    bspline_knots.push_back(temp_knot_list);
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

//called repeatedly when glut isn't doing anything else
void idle()
{
    //redraw the scene over and over again
	glutPostRedisplay();	
}

void createGLUTMenus() {

    int menu;

    // create the menu and
    // tell glut that "processMenuEvents" will
    // handle the events
    menu = glutCreateMenu(processMenuEvents);

    //add entries to our menu
    glutAddMenuEntry("1) Choose a curve          -> in terminal",CHOOSE);
    glutAddMenuEntry("2) Add points                (click any where)",ADDPOINT);
    glutAddMenuEntry("3) Connect/disconnect points  ",CONNECT);
    glutAddMenuEntry("4) Delete points            (click point)",DELETE);
    glutAddMenuEntry("5) Insert points            (click leading point)",INSERT);
    glutAddMenuEntry("6) Move points              (click point and drag)",MOVE);
    glutAddMenuEntry("7) --Bazier-- ",BAZ);
    glutAddMenuEntry("8) --B-spline-- ",BSPL);
    glutAddMenuEntry("9) ----B-spline-changes----> different order OR knots ",CHANGE);
    glutAddMenuEntry("10) Add a new curve",NEWLINE);
    glutAddMenuEntry("11) --Quit--",QUIT);


    // attach the menu to the right button
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //glutDetachMenu(GLUT_RIGHT_BUTTON);
}

void processMenuEvents(int option) {

    switch (option) {
        case CHOOSE:
            choose_line = 1;
            add_points = 1;
            delete_points = 0;
            move_points = 0;
            insert_points = 0;
            bazier_curve = 0;
            bspline_curve = 0;
            break;            
        case ADDPOINT :
            add_points = 1;
            delete_points = 0;
            move_points = 0;
            insert_points = 0;
            break;
        case CONNECT :
            if(!connect_points)
                connect_points = 1;
            else
                connect_points = 0;
            break;
        case DELETE :
            add_points = 0;
            delete_points = 1;
            move_points = 0;
            insert_points = 0;
            break;
        case INSERT :
            add_points = 0;
            delete_points = 0;
            move_points = 0;
            insert_points = 1;
            break;
        case MOVE :
            add_points = 0;
            delete_points = 0;
            move_points = 1;
            insert_points = 0;
            break;
        case NEWLINE :
            add_lines = 1;
            add_points = 1;
            delete_points = 0;
            move_points = 0;
            insert_points = 0;
            bazier_curve = 0;
            bspline_curve = 0;
            break;
        case BAZ :
            bazier_curve = 1;
            bspline_curve = 0;
            break;
        case BSPL : 
            bspline_curve = 1;
            bazier_curve = 0;
            break;
        case CHANGE:
            if(bspline_curve == 1){
                change_bspline = 1;
            }
            else{
                cout << "current line is not a B_Spline curve\n";
            }
            break;
        case QUIT :
            set_quit = 1;
            break;
    }
}

//this is where we render the screen
void display()
{
    //clears the screen
	glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix
	glLoadIdentity();
	
    //draw all points 
    for(int i = 0; i < point_lists.size(); i++)
        draw_points(point_lists[i],i);

    if(choose_line){
        cout << "Choose a curve from 0 to " << point_lists.size()-1 << endl;
        string input;
        cin >> input;
        which_line = atoi(input.c_str());
        if(curve_choices[which_line] == 0)
            bazier_curve = 1;
        else if(curve_choices[which_line] == 1)
            bspline_curve = 1;
        choose_line = 0;
    }
    if(add_lines){
        cout << "adding line" << endl;
        vector<vector<int> > line_new;
        point_lists.push_back(line_new);
        curve_choices.push_back(-1);

        bspline_degrees.push_back(2);
        vector<float> temp_knot_list;
        bspline_knots.push_back(temp_knot_list);
        cout << "bspline_knots: size: "<< bspline_knots.size() << endl;

        int index = point_lists.size()-1;
        which_line = index;
        add_lines = 0;
    }
    //connect the points
    if(connect_points){
        for(int i = 0; i < point_lists.size(); i++)
            if(!point_lists[i].empty()&& point_lists[i].size()>1)
                draw_lines(point_lists[i], i);
    }

    if(bazier_curve){
        //cout << "Making curve --" << which_line << "-- using Bazier\n";
        curve_choices[which_line] = 0;
        //bazier_curve = 0;
    }
    if(bspline_curve){
        if(point_lists[which_line].size()>2){
            //cout << "Making curve --" << which_line << "-- using B-Spline\n";
            curve_choices[which_line] = 1;

            int num_knots = point_lists[which_line].size()-1 + 1 + bspline_degrees[which_line] + 1; //n+1+k+1
            //cout << "num_knots : "<< num_knots << endl;
            vector<float> temp_knots(num_knots,0.0);
            for(int i = 0; i < num_knots; i++)
                temp_knots[i] = i;
            bspline_knots[which_line] = temp_knots;
            //bspline_curve = 0;
        }
    }

    if(change_bspline){
        string input, input1, input2;

        cout << "\nCurrent order: " << bspline_degrees[which_line] << endl << "New order (>2): ";
        cin >> input;
        int new_degree = atoi(input.c_str());
        bspline_degrees[which_line] = new_degree;

        cout << "\nCurrent knots: " << endl;
        for(int t = 0; t < bspline_knots[which_line].size();t++){
            cout << t << ")" << bspline_knots[which_line][t] << endl;
        }
        cout << "\nWhich knot to change: ";
        cin >> input1;
        int which_knot = atoi(input1.c_str());
        float min_val;
        float max_val;
        if(which_knot == 0)
            min_val = bspline_knots[which_line][which_knot];
        else
            min_val = bspline_knots[which_line][which_knot-1];
        if(which_knot == bspline_knots[which_line].size()-1)
            max_val = bspline_knots[which_line][which_knot];
        else
            max_val = bspline_knots[which_line][which_knot+1];
        cout << "-----value range---- (" << min_val << "," << max_val << ")" << endl << "New knot position: ";
        cin >> input2;
        float new_knot_position =atof(input2.c_str());

        if(new_knot_position >= max_val || new_knot_position <= min_val)
            cout << "Not in range" << endl;
        else
            bspline_knots[which_line][which_knot] = new_knot_position;
        change_bspline = 0;
    }
    if(set_quit){
        glutDestroyWindow(win);
        exit(0);
    }

    //make curves if set
    for(int i = 0; i < curve_choices.size(); i++){
        int choice = curve_choices[i];
        //cout << "line: " << i << "  curve_choices: " << choice << endl;
        if(choice == 0){
            vector<vector<float> > temp = bazier(point_lists[i]);
            draw_points(temp,100);
        }
        else if(choice == 1){
            vector<vector<float> > temp = bspline(point_lists[i], i);
            draw_points(temp,200);
        }
    }

    //blits the current opengl framebuffer on the screen
    glutSwapBuffers();
    //checks for opengl errors
	check();
}

void draw_points(vector<vector<int> > list, int option){
    for(int i = 0; i < list.size(); i++){
        draw_pix((float)list[i][0],(float)list[i][1],option);
    }
}

void draw_points(vector<vector<float> > list, int option){
    for(int i = 0; i < list.size(); i++){
        draw_pix(list[i][0],list[i][1],option);
    }
}

void draw_pix(float x, float y, int option){
    if(option != 100 && option != 200)
        glPointSize(5);
    else
        glPointSize(1);

    glBegin(GL_POINTS);
    int choice = option % 3;
    if(option != 100 && option != 200){
        if(choice == 0)
            glColor3f(0,0,1.0);
        else if(choice ==1)
            glColor3f(1.0,0,0);
        else
            glColor3f(0,1.0,0);
    }
    else{
        if(option == 100)
            glColor3f(0,0,0);
        else
            glColor3f(1,0.5,0);
    }
    glVertex2i((int)(x*pixel_size),(int)(y*pixel_size));
    glEnd();
}


void draw_line(vector<int> p1, vector<int> p2, int option){
    glLineWidth(1.0); //sets the "width" of each line we are rendering

    //tells opengl to interperate every two calls to glVertex as a line
    glBegin(GL_LINES);

    int choice = option % 3;
    if(choice == 0)
        glColor3f(0,0,1.0);
    else if(choice ==1)
        glColor3f(1.0,0,0);
    else
        glColor3f(0,1.0,0);
    
    glVertex2f(p1[0]*pixel_size,p1[1]*pixel_size);
    glVertex2f(p2[0]*pixel_size,p2[1]*pixel_size);

    glEnd();
}

void draw_lines(vector<vector<int> > list, int option){
    for(int i = 0; i < list.size()-1; i++){
        draw_line(list[i],list[i+1], option);
    }
}

int find_point(vector<vector<int> > list, vector<int> p){
    for(int i = 0; i < list.size(); i++){
        if(list[i][0] == p[0] && list[i][1] == p[1]){
            return i;
        }
    }
    return -1;
}

//input: points
//output: a curve
vector<vector<float> > bazier(vector<vector<int> > points){
    float t_inc = 0.001;
    int steps = 1/t_inc;
    vector<vector<float> > points_on_curve;
    float t = 0;
    for(int i = 0; i < steps; i++){
        vector<float> temp;
        temp = calculate_each_t(points,t);
        points_on_curve.push_back(temp);
        t += t_inc;
    }
    return points_on_curve;
}

//input: points, t
//ouput: a point on curve
vector<float> calculate_each_t(vector<vector<int> > points, float t){
    int height = points.size();
    int width = points.size();
    vector<vector<float> > b_x = init_matrix(width,height);
    vector<vector<float> > b_y = init_matrix(width,height);
    vector<float> temp;

    //initialize left most column
    for(int i = 0; i < height; i++){
        b_x[0][i] = (float)points[i][0];
        b_y[0][i] = (float)points[i][1];
    }

    for(int j = 1; j < width; j++){
        for(int i = 0; i < height-j; i++){
            b_x[j][i] = (1-t)*b_x[j-1][i]+t*b_x[j-1][i+1];
            b_y[j][i] = (1-t)*b_y[j-1][i]+t*b_y[j-1][i+1];
        }
    }
    //cout << "t: " << t << " x: " << b_x[width-1][0] << " y: " << b_y[width-1][0] << endl;
    temp.push_back(b_x[width-1][0]);
    temp.push_back(b_y[width-1][0]);
    return temp;
}

vector<vector<float> > init_matrix(int w, int h){
    vector<vector<float> > temp;
    vector<float> col(h,0.0);
    for(int i = 0; i < w; i++){
        temp.push_back(col);
    }
    return temp;
}

vector<vector<float> > bspline(vector<vector<int> > points, int line_index){
    vector<vector<float> > temp;

    int n = points.size() - 1;
    vector<float> knots = bspline_knots[line_index];
    int degree = bspline_degrees[line_index];
    //cout << "here" << endl;
    for(int i = degree; i <= n; i++){
        float t = knots[i];
        float t_inc = 0.001;
        while(t < knots[i+1]){
            float x = deBoor_x(degree,i,t,knots,points);
            //cout << "x: " << x << endl;
            float y = deBoor_y(degree,i,t,knots,points);
            //cout << "y: " << y << endl;
            vector<float> temp_point = make_point(x,y);
            temp.push_back(temp_point);
            t += t_inc;
        }
    }
    return temp;
}

float deBoor_x(int degree, int i, float x, vector<float> knots, vector<vector<int> > list){   
    //i: index of knot interval that contains x
    //x: position
    //knots: array of knot positions, needs to be padded as described above
    //list: array of control points
    //degree: degree of B-spline

    vector<float> d(degree+1,0.0);
    for(int j = 0; j < degree + 1; j++){
        d[j] = list[j+i-degree][0];
    }
    //cout << "here" << endl;
    for(int r = 1; r < degree+1; r++){
        //cout << "r: " << r << endl;
        for(int j = degree; j > r-1; j--){
            //cout << "j: " << j << endl;
            //cout << "tmin: " << knots[j+i-degree] << " t: " << x << " tmax: " << knots[j+1+i-r] << endl;
            float alpha = (x - knots[j+i-degree]) / (knots[j+1+i-r] - knots[j+i-degree]);
            d[j] = (1.0 - alpha) * d[j-1] + alpha * d[j];
        }
    }
    return d[degree];
}

float deBoor_y(int degree, int i, float x, vector<float> knots, vector<vector<int> > list){   
    vector<float> d(degree+1,0.0);
    for(int j = 0; j < degree + 1; j++){
        d[j] = list[j+i-degree][1];
    }
    for(int r = 1; r < degree+1; r++){
        for(int j = degree; j > r-1; j--){
            float alpha = (x - knots[j+i-degree]) / (knots[j+1+i-r] - knots[j+i-degree]);
            d[j] = (1.0 - alpha) * d[j-1] + alpha * d[j];
        }
    }
    return d[degree];
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
	glOrtho(0.0,win_width,0.0,win_height,1,-1);
    
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

vector<int> make_point(int x, int y){
    vector<int> temp;
    temp.push_back(x);
    temp.push_back(y);
    return temp;
}

vector<float> make_point(float x, float y){
    vector<float> temp;
    temp.push_back(x);
    temp.push_back(y);
    return temp;
}

//gets called when a mouse button is pressed
void mouse(int button, int state, int x, int y)
{
    int grid_x = (int)(x/pixel_size);
    int grid_y = (int)((win_height-y)/pixel_size);
    vector<int> temp_point;
    temp_point = make_point(grid_x,grid_y);
    //add points
    
    //print the pixel location, and the grid location
    //printf ("MOUSE AT PIXEL: %d %d, GRID: %d %d\n",x,y,grid_x,grid_y);
	switch(button)
	{
		case GLUT_LEFT_BUTTON: //left button
            //printf("LEFT ");
            break;
		case GLUT_RIGHT_BUTTON: //right button
            printf("RIGHT ");
		default:
            printf("UNKNOWN "); //any other mouse button
			break;
	}

    if(add_points){
        if(state == GLUT_DOWN){
            //!check for duplicates
            if(find_point(point_lists[which_line], temp_point) == -1){
                point_lists[which_line].push_back(temp_point);
                //cout << "BUTTON DOWN ";
                cout << grid_x<<" " << grid_y<<endl;
                cout << "-> point added to line" << which_line << endl;
            }
            else{
                cout << "dont add a duplicate point\n";
            }
        }
    }
    else if(delete_points){
        if(state == GLUT_DOWN){
            int index = find_point(point_lists[which_line], temp_point);
            //!check for existance
            //cout << "BUTTON DOWN ";
            if(index != -1){
                cout << "->point deleted\n";
                point_lists[which_line].erase(point_lists[which_line].begin()+index);
            }
            else{
                cout <<"Not clicking on any points\n";
            }
        }
    }
    else if(move_points){
        if(state !=GLUT_DOWN){//final position
            //cout << "BUTTON UP\n";
            if(point_to_move != -1){
                point_lists[which_line][point_to_move] = temp_point;
                cout << "->point moved\n";
            }
        }
        else{//initial position
            //cout << "BUTTON DOWN ";
            point_to_move = find_point(point_lists[which_line], temp_point);
            if(point_to_move != -1){
                cout << "Point clicked\n";
            }
            else{
                cout <<"Not clicking on any points\n";
            }
        }
    }
    else if(insert_points){
        if(state == GLUT_DOWN){
            //cout << "BUTTON DOWN ";
            point_selected = find_point(point_lists[which_line], temp_point);
            if(point_selected != -1){
                cout << "Point clicked\n";
            }
            else{
                cout <<"Not clicking on any points\n";
            }
        }
        else{
            //cout << "BUTTON UP\n";
            if(point_selected != -1){
                vector<int> temp_point;
                temp_point = make_point(point_lists[which_line][point_selected][0]+2,point_lists[which_line][point_selected][1]+2);
                point_lists[which_line].insert(point_lists[which_line].begin()+1+point_selected,temp_point);
                cout << "-> point Inserted to line" << which_line << endl;  //button clicked
            }
        }
    }


    
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

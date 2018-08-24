/*
 * Simple glut demo that can be used as a template for
 * other projects by Garrett Aldrich
 */


#ifdef WIN32
#include <windows.h>
#endif

#if defined (__APPLE__) || defined(MACOSX)
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <GLUT/glut.h>

#else //linux
#include <GL/gl.h>
#include <GL/glut.h>
#endif

//other includes
#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <string>
#include <fstream>
#include <math.h>
#include <iostream>
using namespace std;

#define OUT 1
#define IN 2

#define VERTICES 12
#define DDA 10
#define BRES 11
#define RASTER 13
#define CLIPP 14
#define FINISHED 30
#define VIEW 40
#define SCALE 50
#define TRANSLATE 51
#define ROTATE 52
#define QUIT 100

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
GLint m_viewport[4];



void init();
void idle();
void display();
void draw_pix(int x, int y);
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state, int x, int y);
void motion(int x, int y);
void check();
void clear_board();
vector<vector<int> > make_lines(vector<vector<int> > list);
vector<vector<int> > make_line_DDA(vector<int> start, vector<int> end);
vector<vector<int> > make_line_Bres(vector<int> start, vector<int> end);
vector<int> get_center(vector<vector<int> > list);
void scale_polygon();
void translate_polygon();
vector<vector<int> > translation(vector<vector<int> > list, int x, int y);
void rotate_polygon();
vector<vector<int> > rotation(double degree, vector<vector<int> > list);
vector<vector<int> > rasterize(vector<vector<vector<int> > > vertices, vector<vector<int> > lined_polygons);
vector<vector<int> > clip_polygon(int x_min, int x_max, int y_min, int y_max, vector<vector<vector<int> > > lines, vector<vector<int> > vert);
int x_intersect(vector<vector<int> > line, int x);
int y_intersect(vector<vector<int> > line, int y);
void save_points_to_file();
void make_polygons();
void draw_polygons();
void make_board(vector<vector<int> > list);
void loadPoints();
void draw_points(vector<vector<int> > list);
void processMenuEvents(int option);
void createGLUTMenus();

bool isEqual(vector<int> p1, vector<int> p2);
vector<int> find_max(vector<vector<int> > list);
vector<int> find_min(vector<vector<int> > list);
bool from_same_line(vector<int> p1, vector<int> p2);
bool is_in_list(vector<int> point, vector<vector<int> > list);



//BOOLS for menu triggering
int choose_ploy = 0; //bool
int set_viewport = 0;
int set_scale = 0;
int set_translate = 0;
int set_rotate = 0;
int set_quit = 0;
int set_rasterize = 0;
int set_clip = 0;

int which_alg = 0;// 1 for DDA, 2 for Bresenham's
int which_ploy = 0; // 0, 1, 2

int x_lowleft,y_lowleft,width,height; // viewpoint inputs
float scale = 1;
double rotat = 0;
vector<int> translat;


//points and polygons
vector<vector<vector<int> > > point_lists; //vertex list
vector<vector<vector<int> > > lines; //all the lines in the frame
vector<vector<vector<vector<int> > > > lines_in_polygon; //list of lines in each polygon
vector<vector<vector<int> > > lined_polygons; //polygons with edges overlapped
vector<vector<int> > lined_board; //polygons in a big matrix form 
vector<vector<int> > filled_polygons; //rasterization part

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

    translat.push_back(0);
    translat.push_back(0);
    
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

    //create a menu
    createGLUTMenus();

    //initialize lined_polygons
    vector<vector<int> > temp_polygon;
    lined_polygons.push_back(temp_polygon);
    lined_polygons.push_back(temp_polygon);
    lined_polygons.push_back(temp_polygon);
    

    //initialize opengl variables
    init();

    //load initial polygons
    loadPoints();
    //initialize lined_board
    vector<int> temp_polygon2(grid_height,0);
    for(int i = 0; i < grid_width; i++){
        lined_board.push_back(temp_polygon2);
    }

    //save initial viewport for resetting
    glGetIntegerv( GL_VIEWPORT, m_viewport );

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
    glutAddMenuEntry("VERTICES",VERTICES);
    glutAddMenuEntry("DDA",DDA);
    glutAddMenuEntry("BRES",BRES);
    //glutAddMenuEntry("manipulate 0",POLY0);
    //glutAddMenuEntry("manipulate 1",POLY1);
    //glutAddMenuEntry("manipulate 2",POLY2);
    glutAddMenuEntry("Rasterize",RASTER);
    glutAddMenuEntry("Clip",CLIPP);
    glutAddMenuEntry("Finished drawing/rasterizing; choose polygon",FINISHED);
    glutAddMenuEntry("Set viewpoint",VIEW);
    glutAddMenuEntry("Set scaling factor",SCALE);
    glutAddMenuEntry("Set translation vector",TRANSLATE);
    glutAddMenuEntry("Set rotation angle",ROTATE);
    glutAddMenuEntry("Quit",QUIT);


    // attach the menu to the right button
    glutAttachMenu(GLUT_RIGHT_BUTTON);

    //glutDetachMenu(GLUT_RIGHT_BUTTON);
}

void processMenuEvents(int option) {

    switch (option) {
        case VERTICES :
            which_alg = 0;
            break;
        case DDA :
            which_alg = 1;
            break;
        case BRES :
            which_alg = 2;
            break;
        case RASTER :
            set_rasterize = 1;
            break;
        case CLIPP :
            set_clip = 1;
            break;
        case FINISHED :
            choose_ploy = 1;
            break;
        case VIEW :
            set_viewport = 1;
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
        case QUIT :
            set_quit = 1;
            break;
    }
}



//load polygons points from 3 files
void loadPoints(){
    vector<string> filenames;
    filenames.push_back("new_polygon0.data");
    filenames.push_back("new_polygon1.data");
    filenames.push_back("new_polygon2.data");

    //create 3 vertex point_lists for 3 polygons
    //vect2 -> point
    for(int i = 0; i < 3; i++){
        vector<vector<int> > temp_pointlist;

        ifstream infile(filenames[i]);
        //get number of points in the polygon
        int pointNum;
        infile >> pointNum;
        //get pairs of <x,y>
        int x, y;
        for(int j = 0; j < pointNum; j++){
            infile >> x >> y;
            vector<int> temp_point;
            temp_point.push_back(x);
            temp_point.push_back(y);
            temp_pointlist.push_back(temp_point);
        }
        point_lists.push_back(temp_pointlist);
        //printf("points loaded from polygon: %d\n", i);
    }
}

void make_polygons(){
    //make 3 polygons
    lines.clear();
    lines_in_polygon.clear();
    for(int i = 0; i < 3; i++){
        lined_polygons[i] = make_lines(point_lists[i]);
        //draw_points(point_lists[i]);
        //draw_points(lined_polygons[i]);
    }
    /*
    
    for(int i = 0; i < lines.size(); i++){
        cout << "line=" << i << endl;
        for(int j = 0; j < lines[i].size();j++){
            cout << "("<<lines[i][j][0] << " , " << lines[i][j][1] << endl;
        }
    }
    */
    
}

void clear_board(){
    for(int i = 0; i < grid_width;i++){
        for(int j = 0; j < grid_height; j++){
            lined_board[i][j] = 0;
        }
    }
}

void draw_polygons(){
    clear_board();
    //draw 3 polygons (points); 
    for(int i = 0; i < 3; i++){
        //draw_points(point_lists[i]);
        draw_points(lined_polygons[i]);
        make_board(lined_polygons[i]);
    }
}

//this is where we render the screen
void display()
{
    //clears the screen
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix
    glLoadIdentity();

    //display points
    if(which_alg == 0){
        set_rasterize = 0;
        cout << "showing vertices" << endl;
        for(int i = 0; i < 3; i++)
            draw_points(point_lists[i]);
    }
    else{ //display polygons
        //make lined_polygons
        make_polygons();
        //draw polygons,  make lined_board
        draw_polygons();
        //rasterization

        /*
        for(int i = 0; i < lines_in_polygon.size();i++){
            cout << "poly=" << i << endl;
            for(int j = 0; j < lines_in_polygon[i].size();j++){
                cout << "  line=" << j << endl;
                for(int k = 0; k < lines_in_polygon[i][j].size();k++){
                    cout<< "(" << lines_in_polygon[i][j][k][0] << "," << lines_in_polygon[i][j][k][1] << ")" << endl;
                }
            }
        }
        */

        if(set_rasterize){
            filled_polygons = rasterize(point_lists, lined_board);
            draw_points(filled_polygons);
        }
    }

    //menu
    string input;
    if(choose_ploy){
        cout << "which polygon (0 or 1 or 2): ";
        cin >> which_ploy;

        //reset scale, trans, rotation for a new polygon
        scale = 1;
        translat[0] = 0;
        translat[1] = 0;
        rotat = 0;
        set_rasterize = 0;
        //reset viewport
        glViewport(m_viewport[0],m_viewport[1],m_viewport[2],m_viewport[3]);
        //reset choice
        choose_ploy = 0;
    }
    if(set_viewport){
        set_rasterize = 0;
        cout << "x: ";
        cin >> x_lowleft;
        cout << "y: ";
        cin >> y_lowleft;
        cout << "width: ";
        cin >> width;
        cout << "height: ";
        cin >> height;
        glViewport(x_lowleft,y_lowleft,width,height);
        set_viewport = 0;
    }
    if(set_scale){
        set_rasterize = 0;
        cout << "scaling factor: ";
        cin >> scale;
        scale_polygon();
        set_scale = 0;
    }
    if(set_translate){
        set_rasterize = 0;
        cout << "translation vector, x: ";
        cin >> translat[0];
        cout << "translation vector, y: ";
        cin >> translat[1];
        translate_polygon();
        set_translate = 0;
    }
    if(set_rotate){
        set_rasterize = 0;
        cout << "rotation angle (positive direction: counter-clockwise): ";
        cin >> rotat;
        rotate_polygon();
        set_rotate = 0;
    }

    if(set_clip){
        int x_min,x_max,y_min,y_max;
        vector<vector<int> > new_vertices;
        set_rasterize = 0;
        cout << "x_min";
        cin >> x_min;
        cout << "x_max";
        cin >> x_max;
        cout << "y_min";
        cin >> y_min;
        cout << "y_max";
        cin >> y_max;

        new_vertices = clip_polygon(x_min,x_max,y_min,y_max,lines_in_polygon[which_ploy],point_lists[which_ploy]);
        point_lists.erase(point_lists.begin()+which_ploy);
        point_lists.insert(point_lists.begin()+which_ploy, new_vertices);

        set_clip = 0;
    }
    if(set_quit){
        glutDestroyWindow(win);
        save_points_to_file();
        exit(0);
    }

    //blits the current opengl framebuffer on the screen
    glutSwapBuffers();
    //checks for opengl errors
    check();
}

vector<int> get_center(vector<vector<int> > list){
    vector<int> temp;
    int xSum = 0;
    int ySum = 0;
    int xCenter,yCenter;
    int numVertices = list.size();
    for(int i= 0; i < numVertices;i++){
        xSum += list[i][0];
        ySum += list[i][1];
    }
    xCenter = round(xSum/numVertices);
    yCenter = round(ySum/numVertices);

    temp.push_back(xCenter);
    temp.push_back(yCenter);
    return temp;
}

//need to change this to translation+scale
void scale_polygon(){
    vector<vector<int> > temp_list;
    vector<int> center = get_center(point_lists[which_ploy]);//get centroid
    int xCenter = center[0];
    int yCenter = center[1];

    int numVertices = point_lists[which_ploy].size();
    for(int i= 0; i < numVertices;i++){
        vector<int> temp_point;
        int x_diff_double = ceil(scale*(point_lists[which_ploy][i][0]-xCenter));
        int y_diff_double = ceil(scale*(point_lists[which_ploy][i][1]-yCenter));
        temp_point.push_back(x_diff_double + xCenter);
        temp_point.push_back(y_diff_double + yCenter);
        temp_list.push_back(temp_point);
    }
    point_lists[which_ploy] = temp_list;
}


void translate_polygon(){
    point_lists[which_ploy] = translation(point_lists[which_ploy],translat[0],translat[1]);
}

vector<vector<int> > translation(vector<vector<int> > list, int x, int y){
    int x_diff = x;// translation vector
    int y_diff = y;
    vector<vector<int> > temp_list;

    int numVertices = list.size();
    for(int i= 0; i < numVertices;i++){
        vector<int> temp_point;
        temp_point.push_back(x_diff + list[i][0]);
        temp_point.push_back(y_diff + list[i][1]);
        temp_list.push_back(temp_point);
    }
    return temp_list;
}

void rotate_polygon(){
    //move polygon's center to origin: translation vector = origin-centroid
    //perform rotation: [x';y'] = [cosh  -sinh;sinh  cosh]*[x;y]
    vector<int> center = get_center(point_lists[which_ploy]);
    int x_diff = 0 - center[0];
    int y_diff = 0 - center[1];

    vector<vector<int> > centered_points;
    centered_points = translation(point_lists[which_ploy], x_diff, y_diff);

    vector<vector<int> > rotated_points;
    rotated_points = rotation(rotat, centered_points);

    //move back: point - (diff:origin-centroid)
    point_lists[which_ploy] = translation(rotated_points,(-1)*x_diff, (-1)*y_diff);
}

vector<vector<int> > rotation(double degree, vector<vector<int> > list){
    double radian = (degree * PI)/180.0;
    double param11 = cos(radian);
    double param12 = (-1)*sin(radian);
    double param21 = sin(radian);
    double param22 = cos(radian);
    string input;

    vector<vector<int> > temp_list;
    int numVertices = list.size();

    for(int i= 0; i < numVertices;i++){
        vector<int> temp_point;
        int x = list[i][0];
        int y = list[i][1];
        temp_point.push_back(round(param11*x + param12*y));
        temp_point.push_back(round(param21*x + param22*y));
        temp_list.push_back(temp_point);
    }
    return temp_list;
}

int switch_state(int state){
    if(state == IN)
        return OUT;
    else
        return IN;
}

//return filled_polygons list (w/o edges)
vector<vector<int> > rasterize(vector<vector<vector<int> > > vertices, vector<vector<int> > lined_board){
    vector<vector<int> > temp_list;
    //find the top/bottom vertices
    vector<vector<int> > extrema_list;
    extrema_list.push_back(find_max(vertices[0]));
    extrema_list.push_back(find_max(vertices[1]));
    extrema_list.push_back(find_max(vertices[2]));
    extrema_list.push_back(find_min(vertices[0]));
    extrema_list.push_back(find_min(vertices[1]));
    extrema_list.push_back(find_min(vertices[2]));


    for (int j = 0; j < grid_height; j++){
        //reset in/out param for a new line
        int state = OUT;
        for (int i = 0; i < grid_width; i++){
            vector<int> current;
            current.push_back(i);
            current.push_back(j);

            vector<int> previous;
            if(i > 1){
                previous.push_back(i-1);
                previous.push_back(j);
            }

            //touching an edge
            if(lined_board[i][j] == 1){

                bool top_or_bottom = false;
                for(int k = 0; k < extrema_list.size(); k++){
                    if(isEqual(extrema_list[k],current)){
                        top_or_bottom = true;
                        break;
                    }
                }
                //if not a top/bottom vertex, switch state; else: remain state
                //cout << "edge! i: " << i << "  j: " << j;
                if(top_or_bottom == false){
                    //cout << " not extrema";
                    if(i > 1){
                        //if the previous pixel is an edge
                        if(lined_board[i-1][j] == 1){
                            if(from_same_line(current,previous) == true){ //from the same line,same state
                               //cout << "previous edge from same line, not switching state\n";
                            }
                            else{ //not from the same line, change state
                                state = switch_state(state);
                                //cout << "previous edge not from same line, switching state to --- " << state;
                            }
                        }
                        else{
                            state = switch_state(state);
                            //cout << "switching state to --- " << state;
                        }
                    }
                    else{ //switch state if lined_board[i>1][]is an edge
                        state = switch_state(state);
                        //cout << "switching state to --- " << state;
                    }
                    //string input;
                    //cin >> input;
                }
                else{
                    //cout << " extrema ";
                    //string input;
                    //cin >> input;
                }
            }
            //not touching an edge
            else{
                //if state is IN, fill pixel
                if(state == IN){
                    temp_list.push_back(current);
                    //cout << "not edge! i: " << i << "  j: " << j << endl;;
               }
            }
        }
    }
    return temp_list;
}

bool from_same_line(vector<int> p1, vector<int> p2){
    vector<int> index_list1; //reason for vector<int>: p1 in l1&l2, p2 in l2, need to match p1 and p2
    vector<int> index_list2;
    for(int i = 0; i < lines.size(); i++){
        for(int j = 0; j < lines[i].size(); j++){
            if(isEqual(lines[i][j],p1))
                index_list1.push_back(i);
            if(isEqual(lines[i][j],p2))
                index_list2.push_back(i);
        }
    }

    for (int i = 0; i < index_list1.size(); ++i){
        for (int j = 0; j < index_list2.size(); ++j){
            if(index_list1[i] == index_list2[j]){
                return true;
            }
        }
    }
    return false;
}

bool isEqual(vector<int> p1, vector<int> p2){
    if(p1[0] == p2[0] && p1[1] == p2[1])
        return true;
    else 
        return false;
}

//top point
vector<int> find_max(vector<vector<int> > list){
    int y_max = 0;
    int index = 0;
    for(int i = 0; i < list.size(); i++){
        if(y_max < list[i][1]){
            y_max = list[i][1];
            index = i;
        }
    }

    return list[index];
}

//bottom point
vector<int> find_min(vector<vector<int> > list){
    int y_min = 10000;
    int index = 0;
    for(int i = 0; i < list.size(); i++){
        if(y_min > list[i][1]){
            y_min = list[i][1];
            index = i;
        }
    }

    return list[index];
}


//bottom point index
int find_y_min_index(vector<vector<int> > list, vector<int> indices){
    int y_min = 10000;
    int index = 0;
    for(int i = 0; i < list.size(); i++){
        for(int j = 0; j < indices.size(); j++){
            if(i == indices[j]){
                if(y_min > list[i][1]){
                    y_min = list[i][1];
                    index = i;
                }
            }
        }
    }
    return index;
}

int find_x_min_index(vector<vector<int> > list, vector<int> indices){
    int x_min = 10000;
    int index = 0;
    for(int i = 0; i < list.size(); i++){
        for(int j = 0; j < indices.size(); j++){
            if(i == indices[j]){
                if(x_min > list[i][0]){
                    x_min = list[i][0];
                    index = i;
                }
            }
        }
    }
    return index;
}

int find_y_max_index(vector<vector<int> > list, vector<int> indices){
    int y_max = 0;
    int index = 0;
    for(int i = 0; i < list.size(); i++){
        for(int j = 0; j < indices.size(); j++){
            if(i == indices[j]){
                if(y_max < list[i][1]){
                    y_max = list[i][1];
                    index = i;
                }
            }
        }
    }
    return index;
}

int find_x_max_index(vector<vector<int> > list, vector<int> indices){
    int x_max = 0;
    int index = 0;
    for(int i = 0; i < list.size(); i++){
        for(int j = 0; j < indices.size(); j++){
            if(i == indices[j]){
                if(x_max < list[i][0]){
                    x_max = list[i][0];
                    index = i;
                }
            }
        }
    }
    return index;
}

//find if point is in list
bool is_in_list(vector<int> point, vector<vector<int> > list){
    for(int i = 0; i < list.size(); i++){
        if(point[0] == list[i][0] && point[1] == list[i][1])
            return true;
    }
    return false;
}

vector<vector<int> > clip_polygon(int x_min, int x_max, int y_min, int y_max, vector<vector<vector<int> > > lines, vector<vector<int> > vert){
    vector<vector<int> > vertices = vert;
    vector<int> delete_indices;
    vector<int> temp_point(2,0);
    vector<int> x_list;
    vector<int> y_list;
    int deleteNum = 0;
    int index = -1;
    string input;

    //cout << x_min << " "  << x_max << "  " << y_min << "  " << y_max;
    //cin >> input;

    //   x_min
    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i][0] < x_min){ // there is an intersection
            //find smallest index's loacation m, delete the indices, insert new indicies at m
            delete_indices.push_back(i);
        }
    }
    index = find_y_min_index(vertices, delete_indices);
    deleteNum = delete_indices.size();
    //cout << "delete num: " << deleteNum;
    //cin >> input;
    if(deleteNum > 0){
        vertices.erase(vertices.begin()+index, vertices.begin()+index+deleteNum);
        
        y_list.clear();
        for(int j = 0; j < lines.size(); j++){
            int y = x_intersect(lines[j], x_min);
            if(y != -1000)
                y_list.push_back(y);
        }
        //x_min, sort y coordinates to insert clockwise
        sort(y_list.begin(),y_list.end());
        for(int i = 0; i < y_list.size(); i++){
            temp_point[0] = x_min;
            temp_point[1] = y_list[i];
            vertices.insert(vertices.begin()+index+i, temp_point);
        } 
    }

    //    y_max
    delete_indices.clear();
    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i][1] > y_max){ // there is an intersection
            //find smallest index's loacation m, delete the indices, insert new indicies at m
            delete_indices.push_back(i);
        }
    }
    index = find_x_min_index(vertices, delete_indices);
    deleteNum = delete_indices.size();

    if(deleteNum > 0){
        vertices.erase(vertices.begin()+index, vertices.begin()+index+deleteNum);
        
        x_list.clear();
        for(int j = 0; j < lines.size(); j++){
            int x = y_intersect(lines[j], y_max);
            if(x != -1000)
                x_list.push_back(x);
        }
        //y_max, sort x coordinates to insert clockwise
        sort(x_list.begin(),x_list.end());
        for(int i = 0; i < x_list.size(); i++){
            temp_point[0] = x_list[i];
            temp_point[1] = y_max;
            vertices.insert(vertices.begin()+index+i, temp_point);
        }
    }


    //    x_max
    delete_indices.clear();
    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i][0] > x_max){ // there is an intersection
            //find smallest index's loacation m, delete the indices, insert new indicies at m
            delete_indices.push_back(i);
        }
    }
    index = find_y_max_index(vertices, delete_indices);
    deleteNum = delete_indices.size();
    if(deleteNum > 0){
        vertices.erase(vertices.begin()+index, vertices.begin()+index+deleteNum);
        
        y_list.clear();
        for(int j = 0; j < lines.size(); j++){
            int y = x_intersect(lines[j], x_max);
            if(y != -1000)
                y_list.push_back(y);
        }
        //x_max, inversely sort y coordinates to insert clockwise
        sort(y_list.begin(),y_list.end());
        reverse(y_list.begin(),y_list.end());
        for(int i = 0; i < y_list.size(); i++){
            temp_point[0] = x_max;
            temp_point[1] = y_list[i];
            vertices.insert(vertices.begin()+index+i, temp_point);
        }
    }


    //y_min
    delete_indices.clear();
    for(int i = 0; i < vertices.size(); i++){
        if(vertices[i][1] < y_min){ // there is an intersection
            //find smallest index's loacation m, delete the indices, insert new indicies at m
            delete_indices.push_back(i);
        }
    }
    index = find_x_max_index(vertices, delete_indices);
    deleteNum = delete_indices.size();
    if(deleteNum > 0){
        vertices.erase(vertices.begin()+index, vertices.begin()+index+deleteNum);
        
        x_list.clear();
        for(int j = 0; j < lines.size(); j++){
            int x = y_intersect(lines[j], y_min);
            if(x != -1000)
                x_list.push_back(x);
        }
        //y_min, inversely sort x coordinates to insert clockwise
        sort(x_list.begin(),x_list.end());
        reverse(x_list.begin(),x_list.end());
        for(int i = 0; i < x_list.size(); i++){
            temp_point[0] = x_list[i];
            temp_point[1] = y_min;
            vertices.insert(vertices.begin()+index+i, temp_point);
        }
    }

    return vertices;
}

//returns y value if intersects, else, return -1000
int x_intersect(vector<vector<int> > line, int x){
    for (int i = 0; i < line.size(); i++){
        if(x == line[i][0])
            return line[i][1];
    }
    return -1000;
}


//returns x value if intersects, else, return -1000
int y_intersect(vector<vector<int> > line, int y){
    for (int i = 0; i < line.size(); i++){
        if(y == line[i][1])
            return line[i][0];
    }
    return -1000;
}



void save_points_to_file(){
    vector<string >filenames;
    filenames.push_back("new_polygon0.data");
    filenames.push_back("new_polygon1.data");
    filenames.push_back("new_polygon2.data");


    for(int i = 0; i < 3; i++){
         ofstream myfile;
         myfile.open(filenames[i]);

        int pointNum = point_lists[i].size();
        myfile << pointNum << "\n";

        for(int j = 0; j < pointNum;j++){
            string x = to_string(point_lists[i][j][0]);
            string y = to_string(point_lists[i][j][1]);
            myfile << x << " " << y << "\n";
        }
        myfile.close();
    }

}

//input: vertices of a polygon
//output: lined polygon
vector<vector<int> > make_lines(vector<vector<int> > list){
    vector<vector<int> > temp_polygon;
    vector<vector<vector<int> > > these_lines;
    //total number of lines in the polygon
    int pointNum = list.size();
    for(int i = 0; i < pointNum; i++){//for each vertex
        vector<int> point1,point2;
        point1.push_back(list[i][0]);
        point1.push_back(list[i][1]);
        //concatnate end point to start point
        if(i == pointNum-1){
            point2.push_back(list[0][0]);
            point2.push_back(list[0][1]);
        }
        else{
            point2.push_back(list[i+1][0]);
            point2.push_back(list[i+1][1]);
        }

        
        vector<vector<int> > newline;
        if(which_alg == 1)
            newline = make_line_DDA(point1, point2);
        else if(which_alg == 2)
            newline = make_line_Bres(point1, point2);

        vector<vector<int> > temp = newline;
        these_lines.push_back(newline);
        lines.push_back(newline);
        /*
        for(int j = 0; j < newline.size();j++)
            printf("%d,%d  ",newline[j][0],newline[j][1]);
        printf("\nchanged to\n");
        */
        
        if(i != 0)//if not the 1st line, delete first point
            temp.erase(temp.begin());
        if(i == pointNum-1)//if the last line, delete the last point
            temp.pop_back();

        /*
        for(int j = 0; j < newline.size();j++)
            printf("%d,%d  ",newline[j][0],newline[j][1]);
        printf("\n\n\n\n");
        */

        for(int j = 0; j < temp.size(); j++)
            temp_polygon.push_back(temp[j]);//add points to polygon
    }

    lines_in_polygon.push_back(these_lines);//add a line_list for a polygon

    return temp_polygon;
}

//input: end points of a line
//output: a line
vector<vector<int> > make_line_DDA(vector<int> start, vector<int> end){
    cout << "using DDA" << endl;
    vector<vector<int> > temp_line;
    int diff_x = end[0] - start[0];
    int diff_y = end[1] - start[1];
    int steps;

    if(abs(diff_x) > abs(diff_y))
        steps = abs(diff_x);
    else
        steps = abs(diff_y);

    float x_inc = (float)diff_x/(float)steps;
    float y_inc = (float)diff_y/(float)steps;

    float curr_x = (float)start[0];
    float curr_y = (float)start[1];

    temp_line.push_back(start);
    //increment current x and y
    for(int i = 0; i < steps; i++){
        vector<int> temp_point;
        curr_x = curr_x + x_inc;
        curr_y = curr_y + y_inc;
        temp_point.push_back((int)round(curr_x));
        temp_point.push_back((int)round(curr_y));
        temp_line.push_back(temp_point);
    }
    return temp_line;
}

int sign(int num){
    if(num > 0)
        return 1;
    else if(num == 0)
        return 0;
    else 
        return -1;
}

vector<vector<int> > make_line_Bres(vector<int> start, vector<int> end){
    cout << "using BRES" << endl;
    vector<vector<int> > temp_line;

    int x_diff = end[0] - start[0];
    int y_diff = end[1] - start[1];

    int dx = abs(x_diff);
    int dy = abs(y_diff);

    int x = start[0], y = start[1];

    //vertical line, return right after this
    if(dx == 0){
        temp_line.push_back(start);
        while(dy){
            vector<int> temp_point;

            y = y + sign(y_diff); //go down/up the vertical line
            temp_point.push_back(x);
            temp_point.push_back(y);
            temp_line.push_back(temp_point);
            dy--;
        }
        return temp_line;
    }
    //horizontal line, return right after this
    if(dy == 0){
        temp_line.push_back(start);
        while(dx){
            vector<int> temp_point;

            x = x + sign(x_diff); //go down/up the vertical line
            temp_point.push_back(x);
            temp_point.push_back(y);
            temp_line.push_back(temp_point);
            dx--;
        }
        return temp_line;
    }


    //if dx is less than dy, swap dx and dy
    int temp;
    int swapped = 0;

    if(dx < dy){
        temp = dy;
        dy = dx;
        dx = temp;
        swapped = 1;
    }

    //set decision parameters
    int P = 2*dy-dx;
    int Ps = 2*dy;
    int Pt = 2*(dy-dx);

    //set initial point (left most)
    int endx = end[0],endy = end[1];
    if(swapped == 0){
        if(start[0] < end[0]){
            x = start[0];
            y = start[1];
        }
        else{
            x = end[0];
            y = end[1];
            endx = start[0];
            endy = start[1];
        }
    }
    else{
        if(start[1] < end[1]){
            x = start[0];
            y = start[1];
        }
        else{
            x = end[0];
            y = end[1];
            endy = start[1];
            endx = start[0];
        }
    }

    vector<int> initial;
    initial.push_back(x);
    initial.push_back(y);
    
    temp_line.push_back(initial);//set first point

    if(swapped == 0){//dx > dy
        while(x < endx){
            vector<int> temp_point;
            if(P < 0)
                P = P + Ps;
            else{
                if(y < endy)
                    y++;
                else
                    y--;
                P = P + Pt;
            }
            x++;
            temp_point.push_back(x);
            temp_point.push_back(y);
            temp_line.push_back(temp_point);
        }
    }
    else{ //dy > dx
        while(y < endy){
            vector<int> temp_point;
            if(P < 0)
                P = P + Ps;
            else{
                if(x < endx)
                    x++;
                else
                    x--;
                P = P + Pt;
            }
            y++;
            temp_point.push_back(x);
            temp_point.push_back(y);
            temp_line.push_back(temp_point);
        }
    }
    return temp_line;
}

void draw_points(vector<vector<int> > list){
    for (int j = 0; j < grid_height; j+=1){
        for (int i = 0; i < grid_width; i+=1){
            for(int k = 0; k < list.size(); k++){
                int tempx = list[k][0];
                int tempy = list[k][1];
                if(i == tempx && j == tempy){
                    draw_pix(i,j);
                }
                else{
                    //draw_pix(i,j);
                }
            }
        }
    }
}

void make_board(vector<vector<int> > list){
    for (int j = 0; j < grid_height; j++){
        for (int i = 0; i < grid_width; i++){
            for(int k = 0; k < list.size(); k++){
                int tempx = list[k][0];
                int tempy = list[k][1];
                if(i == tempx && j == tempy){
                    lined_board[i][j] = 1;
                    //cout << "i: " << i << "  j: " << j << endl;
                }
                else{
                    //lined_board[i][j] = 0;
                }
            }
        }
    }
}


//Draws a single "pixel" given the current grid size
//don't change anything in this for project 1
void draw_pix(int x, int y){
    glBegin(GL_POINTS);
    glColor3f(.2,.2,1.0);
    glVertex3f(x+.5,y+.5,0);
    glEnd();
}

/*Gets called when display size changes, including initial craetion of the display*/
void reshape(int width, int height)
{
    /*set up projection matrix to define the view port*/
    //update the ne window width and height
    win_width = width;
    win_height = height;
    
    //creates a rendering area across the window
    glViewport(0,0,width,height);
    // up an orthogonal projection matrix so that
    // the pixel space is mapped to the grid space
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    glOrtho(0,grid_width,0,grid_height,-10,10);
    
    //clear the modelview matrix
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();
    
    //set pixel size based on width, if the aspect ratio
    //changes this hack won't work as well
    pixel_size = width/(float)grid_width;
    
    //set pixel size relative to the grid cell size
    glPointSize(pixel_size);
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

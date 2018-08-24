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
    
#define OUT 1
#define IN 2

#define QUIT 5
#define XY 6
#define XZ 7
#define YZ 8
#define KAA 9
#define KDD 10
#define KSS 11
#define IAA 12
#define ILL 13
#define KK 14
#define NN 15
#define FF 16

#define PI 3.14159265

/****set in main()****/
//the number of pixels in the grid
int grid_width;
int grid_height;
int board_size;

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
void mouse(int button, int state_in_out, int x, int y);
void motion(int x, int y);
void check();
void load_data();
void print_point(vector<float> p);
void print_point(vector<int> p);
vector<float> make_point(float x, float y, float z);
vector<float> create_vector(vector<float> p1,vector<float> p2);
float get_dot_product(vector<float> a, vector<float> b);
vector<float> get_cross_product(vector<float> v1,vector<float> v2);
vector<float> to_unit(vector<float> input);
void append_normal_to_point(vector<float> n, int obj, int point_index);
int get_empty_entry(vector<vector<float> > normals);
vector<float> normalize_intensities(vector<float> values, float max);
float calculate_average(vector<float> list);
void get_intensities();
void get_triangles();
bool point_in_the_list(vector<vector<int> > list, vector<int> p);
vector<int> to_pixel_coord(vector<float> p);
bool isEqual(vector<int> p1, vector<int> p2);
int menu();
vector<vector<int> > make_line_DDA(vector<int> start, vector<int> end);
void get_vertices_in_pixels();
vector<vector<vector<int> > > make_lines(vector<vector<int> > list, int which_proj);
void make_all_lines();
void get_normals();
float get_magnitude(vector<float> v);
void draw_object_polygons(int which_proj);
void draw_points(vector<vector<int> > list, vector<float> intensities, int which_proj);
void draw_pix(int x, int y);
vector<float> get_line_intensity(int linesize, float int1, float int2);
void initialize_lined_boards();
void initialize_filled_polygons();
void make_boards(vector<vector<vector<vector<vector<int> > > > > line_list, vector<vector<vector<vector<float> > > > intens_list,int which_proj);
vector<vector<float> > make_line_intensity(vector<vector<int> > list, int obj, int tri, int which_proj);
void rasterize(vector<vector<vector<int> > > v, int obj, int which_proj);
void draw_polygons(vector<vector<vector<float> > > point_lists, vector<vector<vector<int> > > connections);
void print_to_screen(int plane);

//input info
int objectNum;
string material_filename;
string light_filename;
string obj_filename;
//light
vector<float> X; //light position
vector<float> La; //light ambient intensity
vector<float> Li; // light source intensity
vector<float> f_xy; // distance of centerOfScreen from objects
vector<float> f_xz;
vector<float> f_yz;
float K; // average “distance” of objects from light

//materials
vector<vector<float> > Ka; //ambient reflection coeff
vector<vector<float> > Kd; //diffuse reflection coeff
vector<vector<float> > Ks; // specular reflection coeff
vector<float> n_constant; //Phong constant

//obj
vector<int> obj_material;
vector<vector<vector<float> > > point_lists; //normal = point_lists[i]<->point_normal_lists[i]
//map points to normal lists
vector<vector<vector<float> > > point_normals; // 1 points/1 normal at a vertex
vector<vector<vector<float> > > point_normal_lists; // 3*number-of-normals = vector size (at a vertex)
//vector<vector<vector<float> > > point_lists_copy;
vector<vector<vector<vector<float> > > > triangle_lists; //triangle lists; float->point->triangle->triangles->object
vector<vector<vector<int> > > triangle_connections; //mapping from triangles to points
vector<vector<float> > point_intensities_xy;
vector<vector<float> > point_intensities_xz;
vector<vector<float> > point_intensities_yz;

vector<vector<vector<int> > > picked_triangles_xy;//indices of picked triangles, 
vector<vector<vector<int> > > picked_triangles_xz;
vector<vector<vector<int> > > picked_triangles_yz;
vector<vector<float> > depths_triangles_xy; //triangle depths
vector<vector<float> > depths_triangles_xz;
vector<vector<float> > depths_triangles_yz;
vector<vector<vector<int> > > sorted_triangles_xy; //indices of sorted triangles
vector<vector<vector<int> > > sorted_triangles_xz;
vector<vector<vector<int> > > sorted_triangles_yz;

vector<vector<vector<vector<int> > > > triangle_vertices_xy;//verices 3d
vector<vector<vector<vector<int> > > > triangle_vertices_xz;
vector<vector<vector<vector<int> > > > triangle_vertices_yz;

vector<vector<vector<float> > > triangle_intensities_xy;
vector<vector<vector<float> > > triangle_intensities_xz;
vector<vector<vector<float> > > triangle_intensities_yz;

vector<vector<vector<vector<vector<int> > > > > lines_xy; //point 2d
vector<vector<vector<vector<vector<int> > > > > lines_xz;
vector<vector<vector<vector<vector<int> > > > > lines_yz;

vector<vector<vector<vector<float> > > > lines_intensities_xy;
vector<vector<vector<vector<float> > > > lines_intensities_xz;
vector<vector<vector<vector<float> > > > lines_intensities_yz;

vector<vector<vector<vector<int> > > > lined_board_xy; //0/1
vector<vector<vector<vector<int> > > > lined_board_xz;
vector<vector<vector<vector<int> > > > lined_board_yz;

vector<vector<vector<vector<float> > > > lined_board_intensities_xy;
vector<vector<vector<vector<float> > > > lined_board_intensities_xz;
vector<vector<vector<vector<float> > > > lined_board_intensities_yz;

//rasterize
vector<vector<vector<vector<int> > > > filled_polygons_xy; //obj->tri->i->j
vector<vector<vector<vector<int> > > > filled_polygons_xz;
vector<vector<vector<vector<int> > > > filled_polygons_yz;

vector<vector<vector<vector<float> > > > filled_polygons_int_xy; //obj->tri->line->point (j->i)
vector<vector<vector<vector<float> > > > filled_polygons_int_xz;
vector<vector<vector<vector<float> > > > filled_polygons_int_yz;


//variables
int which_poly = 0;
int set_ka = 0;
int set_kd = 0;
int set_ks = 0;
int set_la = 0;
int set_li = 0;
int set_f = 0;
int set_k = 0;
int set_n = 0;

int set_quit = 0;
int plane = 0;

int win;

float abs_val(float value){
    if(value < 0)
        return value*(-1);
    else
        return value;
}

int main(int argc, char **argv)
{   
    board_size = 100;
    //the number of pixels in the grid
    grid_width = 3*board_size;
    grid_height = 3*board_size;
    
    //the size of pixels sets the inital window height and width
    //don't make the pixels too large or the screen size will be larger than
    //your display size
    pixel_size = 1;
    
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
    glClearColor(0,0,0,0);
    
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
    //get normals
    get_normals();
    //calculate intensities at each vertex
    get_intensities();
    //cout <<"here" << endl;
    //pick triangles for each projection and sort by triangle depths
    get_triangles();
    //cout <<"here" << endl;
    //makelines
    get_vertices_in_pixels();
    //cout <<"here" << endl;
    make_all_lines();
    //cout <<"here" << endl;
    // rasterize
    initialize_lined_boards();
    initialize_filled_polygons();
    //cout << "here" << endl;
    make_boards(lines_xy,lines_intensities_xy,0);
    make_boards(lines_xz,lines_intensities_xz,1);
    make_boards(lines_yz,lines_intensities_yz,2);
    //cout <<"here" << endl;

    for(int i = 0; i < objectNum; i++){
        rasterize(triangle_vertices_xy[i],i,0);
        rasterize(triangle_vertices_xz[i],i,1);
        rasterize(triangle_vertices_yz[i],i,2);
    }
    //cout <<"here" << endl;
    //start glut event loop
    glutMainLoop();
    return 0;
}


void draw_points(vector<vector<int> > list, vector<float> intensities, int which_proj){
    //cout << "  size: " << list.size()<< endl;
    for(int k = 0; k < list.size(); k++){
        //cout << "k: " << k << endl;
        int coord1 = list[k][0];
        int coord2 = list[k][1];
        //cout << "tempx: "<<tempx << "tempy: " << tempy << endl; 
        float intensity = intensities[k];
        int factor = 3;
        int offset1 = coord1*factor;
        int offset2 = coord2*factor;
        
        srand (k);
        int num_grids = round(intensity*10-0.5);
        if(num_grids > 9)
            num_grids = 9;
        //cout << "   "<<num_grids << endl;
        vector<int> this_point(2,-1);
        vector<vector<int> > position_list;
        for(int l = 0; l < num_grids; l++){
            int x = rand() % 3;
            int y = rand() % 3;
            this_point[0] = x;
            this_point[1] = y;
            while(point_in_the_list(position_list,this_point)){
                int x = rand() % 3;
                int y = rand() % 3;
                this_point[0] = x;
                this_point[1] = y;
            }
            position_list.push_back(this_point);
        }
        //cout << "   "<<intensity << endl;

        for(int l = 0; l < position_list.size(); l++){
            int x = position_list[l][0];
            int y = position_list[l][1];
            int x_coord = offset1 + x;
            int y_coord = offset2 + y;
            draw_pix(x_coord,y_coord);
        }
    }
    //cout << "    cout. "  <<endl; 
}

void initialize_lined_boards(){
    for(int k = 0; k < triangle_intensities_xy.size();k++){
        vector<vector<vector<int> > > temp_triangle;
        vector<vector<vector<float> > > temp_triangle2;
        for(int j = 0; j < triangle_intensities_xy[k].size(); j++){
            vector<vector<int> > temp_board;
            vector<vector<float> > temp_board2;
            for(int i = 0; i < board_size; i++){
                vector<int> temp_line(board_size,0);
                vector<float> temp_line2(board_size,0.0);
                temp_board.push_back(temp_line);
                temp_board2.push_back(temp_line2);
            }
            temp_triangle.push_back(temp_board);
            temp_triangle2.push_back(temp_board2);
        }
        lined_board_xy.push_back(temp_triangle);
        lined_board_intensities_xy.push_back(temp_triangle2);
    }
    for(int k = 0; k < triangle_intensities_xz.size();k++){
        vector<vector<vector<int> > > temp_triangle;
        vector<vector<vector<float> > > temp_triangle2;
        for(int j = 0; j < triangle_intensities_xz[k].size(); j++){
            vector<vector<int> > temp_board;
            vector<vector<float> > temp_board2;
            for(int i = 0; i < board_size; i++){
                vector<int> temp_line(board_size,0);
                vector<float> temp_line2(board_size,0.0);
                temp_board.push_back(temp_line);
                temp_board2.push_back(temp_line2);
            }
            temp_triangle.push_back(temp_board);
            temp_triangle2.push_back(temp_board2);
        }
        lined_board_xz.push_back(temp_triangle);
        lined_board_intensities_xz.push_back(temp_triangle2);
    }
    for(int k = 0; k < triangle_intensities_yz.size();k++){
        vector<vector<vector<int> > > temp_triangle;
        vector<vector<vector<float> > > temp_triangle2;
        for(int j = 0; j < triangle_intensities_yz[k].size(); j++){
            vector<vector<int> > temp_board;
            vector<vector<float> > temp_board2;
            for(int i = 0; i < board_size; i++){
                vector<int> temp_line(board_size,0);
                vector<float> temp_line2(board_size,0.0);
                temp_board.push_back(temp_line);
                temp_board2.push_back(temp_line2);
            }
            temp_triangle.push_back(temp_board);
            temp_triangle2.push_back(temp_board2);
        }
        lined_board_yz.push_back(temp_triangle);
        lined_board_intensities_yz.push_back(temp_triangle2);
    }
}

void initialize_filled_polygons(){
    vector<vector<vector<float> > > temp;
    vector<vector<vector<int> > > temp1;
    for(int i = 0; i < objectNum; i++){
        filled_polygons_xy.push_back(temp1);
        filled_polygons_xz.push_back(temp1);
        filled_polygons_yz.push_back(temp1);
        filled_polygons_int_xy.push_back(temp);
        filled_polygons_int_xz.push_back(temp);
        filled_polygons_int_yz.push_back(temp);
    }
}
void draw_pix(int x, int y){
    glBegin(GL_POINTS);
    glColor3f(1,1,1);
    glVertex3f(x,y,0);
    glEnd();
}

/*initialize gl stufff*/
void init()
{
    //set clear color (Default background to white)
    glClearColor(1.0,1.0,1.0,1.0);
    //checks for OpenGL errors
    check();
}

void createMenu() {
    int menu;

    // create the menu and
    // tell glut that "processMenuEvents" will
    // handle the events
    menu = glutCreateMenu(processMenuEvents);

    //add entries to our menu
    glutAddMenuEntry("change ka",KAA);
    glutAddMenuEntry("change kd",KDD);
    glutAddMenuEntry("change ks",KSS);
    glutAddMenuEntry("change IA",IAA);
    glutAddMenuEntry("change IL",ILL);
    glutAddMenuEntry("change K",KK);
    glutAddMenuEntry("change n",NN);
    glutAddMenuEntry("change F",FF);

    glutAddMenuEntry("projection XY (red)",XY);
    glutAddMenuEntry("projection XZ (purple)",XZ);
    glutAddMenuEntry("projection YZ (green)",YZ);
    glutAddMenuEntry("--Quit--",QUIT);

    // attach the menu to the right button
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void processMenuEvents(int option) {

    switch (option) {
        case XY :
            plane = 0;
            break;
        case XZ :
            plane = 1;
            break;
        case YZ :
            plane = 2;
            break;
        case KAA:
            set_ka = 1;
            break;
        case KDD:
            set_kd = 1;
            break;
        case KSS:
            set_ks = 1;
            break;
        case IAA:
            set_la = 1;
            break;
        case ILL:
            set_li = 1;
            break;
        case KK:
            set_k = 1;
            break;
        case NN:
            set_n = 1;
            break;
        case FF:
            set_f = 1;
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
        print_point(point_lists[which_poly][i]);
    }
}

void print_point(vector<float> p){
    cout << "(" << p[0] << "," << p[1] << "," << p[2] << ")" << endl;
}

void print_point(vector<int> p){
    cout << "(" << p[0] << "," << p[1] << "," << p[2] << ")" << endl;
}

void clearboards(){
    for(int i = 0; i < lined_board_xy.size();i++){
        for(int j = 0; j < lined_board_xy[i].size();j++){
            for(int m = 0; m < lined_board_xy[i][j].size();m++){
                for(int n = 0; n < lined_board_xy[i][j][m].size(); n++){
                    lined_board_xy[i][j][m][n] = 0;
                }
            }
        }
    }
    for(int i = 0; i < lined_board_xz.size();i++){
        for(int j = 0; j < lined_board_xz[i].size();j++){
            for(int m = 0; m < lined_board_xz[i][j].size();m++){
                for(int n = 0; n < lined_board_xz[i][j][m].size(); n++){
                    lined_board_xz[i][j][m][n] = 0;
                }
            }
        }
    }
    for(int i = 0; i < lined_board_yz.size();i++){
        for(int j = 0; j < lined_board_yz[i].size();j++){
            for(int m = 0; m < lined_board_yz[i][j].size();m++){
                for(int n = 0; n < lined_board_yz[i][j][m].size(); n++){
                    lined_board_yz[i][j][m][n] = 0;
                }
            }
        }
    }

    for(int i = 0; i < lined_board_intensities_xy.size();i++){
        for(int j = 0; j < lined_board_intensities_xy[i].size();j++){
            for(int m = 0; m < lined_board_intensities_xy[i][j].size();m++){
                for(int n = 0; n < lined_board_intensities_xy[i][j][m].size(); n++){
                    lined_board_intensities_xy[i][j][m][n] = 0;
                }
            }
        }
    }
    for(int i = 0; i < lined_board_intensities_xz.size();i++){
        for(int j = 0; j < lined_board_intensities_xz[i].size();j++){
            for(int m = 0; m < lined_board_intensities_xz[i][j].size();m++){
                for(int n = 0; n < lined_board_intensities_xz[i][j][m].size(); n++){
                    lined_board_intensities_xz[i][j][m][n] = 0;
                }
            }
        }
    }
    for(int i = 0; i < lined_board_intensities_yz.size();i++){
        for(int j = 0; j < lined_board_intensities_yz[i].size();j++){
            for(int m = 0; m < lined_board_intensities_yz[i][j].size();m++){
                for(int n = 0; n < lined_board_intensities_yz[i][j][m].size(); n++){
                    lined_board_intensities_yz[i][j][m][n] = 0;
                }
            }
        }
    }
}

void check_changes(){
    if(set_ka == 1){
        int obj;
        float new_ka;
        cout << "Which object (0,1,..): ";
        cin >> obj;
        cout << "-Ka- value: ";
        cin >> new_ka;
        Ka[obj][0] = new_ka;

        get_intensities();
        get_vertices_in_pixels();
        make_all_lines();
        clearboards();
        make_boards(lines_xy,lines_intensities_xy,0);
        make_boards(lines_xz,lines_intensities_xz,1);
        make_boards(lines_yz,lines_intensities_yz,2);

        for(int i = 0; i < objectNum; i++){
            rasterize(triangle_vertices_xy[i],i,0);
            rasterize(triangle_vertices_xz[i],i,1);
            rasterize(triangle_vertices_yz[i],i,2);
        }
        set_ka = 0;
    }
    if(set_kd == 1){
        int obj;
        float new_kd;
        cout << "Which object (0,1,..): ";
        cin >> obj;
        cout << "-Kd- value: ";
        cin >> new_kd;
        Kd[obj][0] = new_kd;
        get_intensities();
        get_vertices_in_pixels();
        make_all_lines();
        clearboards();
        make_boards(lines_xy,lines_intensities_xy,0);
        make_boards(lines_xz,lines_intensities_xz,1);
        make_boards(lines_yz,lines_intensities_yz,2);

        for(int i = 0; i < objectNum; i++){
            rasterize(triangle_vertices_xy[i],i,0);
            rasterize(triangle_vertices_xz[i],i,1);
            rasterize(triangle_vertices_yz[i],i,2);
        }
        set_kd = 0;
    }
    if(set_ks == 1){
        int obj;
        float new_ks;
        cout << "Which object (0,1,..): ";
        cin >> obj;
        cout << "-Ks- value: ";
        cin >> new_ks;
        Ks[obj][0] = new_ks;
        get_intensities();
        get_vertices_in_pixels();
        make_all_lines();
        clearboards();
        make_boards(lines_xy,lines_intensities_xy,0);
        make_boards(lines_xz,lines_intensities_xz,1);
        make_boards(lines_yz,lines_intensities_yz,2);

        for(int i = 0; i < objectNum; i++){
            rasterize(triangle_vertices_xy[i],i,0);
            rasterize(triangle_vertices_xz[i],i,1);
            rasterize(triangle_vertices_yz[i],i,2);
        }
        set_ks = 0;
    }
    if(set_la == 1){
        float new_la;
        cout << "-IA- value: ";
        cin >> new_la;
        La[0] = new_la;
        get_intensities();
        get_vertices_in_pixels();
        make_all_lines();
        clearboards();
        make_boards(lines_xy,lines_intensities_xy,0);
        make_boards(lines_xz,lines_intensities_xz,1);
        make_boards(lines_yz,lines_intensities_yz,2);

        for(int i = 0; i < objectNum; i++){
            rasterize(triangle_vertices_xy[i],i,0);
            rasterize(triangle_vertices_xz[i],i,1);
            rasterize(triangle_vertices_yz[i],i,2);
        }
        set_la = 0;
    }
    if(set_li == 1){
        float new_li;
        cout << "-IL- value: ";
        cin >> new_li;
        Li[0] = new_li;
        get_intensities();
        get_vertices_in_pixels();
        make_all_lines();
        clearboards();
        make_boards(lines_xy,lines_intensities_xy,0);
        make_boards(lines_xz,lines_intensities_xz,1);
        make_boards(lines_yz,lines_intensities_yz,2);

        for(int i = 0; i < objectNum; i++){
            rasterize(triangle_vertices_xy[i],i,0);
            rasterize(triangle_vertices_xz[i],i,1);
            rasterize(triangle_vertices_yz[i],i,2);
        }
        set_li = 0;
    }
    if(set_f == 1){
        float new_f;
        cout << "-F- (from point to screen center) value: ";
        cin >> new_f;
        f_xy = make_point(0.5,0.5,new_f);
        f_xz = make_point(0.5,new_f,0.5);
        f_yz = make_point(new_f,0.5,0.5);
        get_intensities();
        get_vertices_in_pixels();
        make_all_lines();
        clearboards();
        make_boards(lines_xy,lines_intensities_xy,0);
        make_boards(lines_xz,lines_intensities_xz,1);
        make_boards(lines_yz,lines_intensities_yz,2);

        for(int i = 0; i < objectNum; i++){
            rasterize(triangle_vertices_xy[i],i,0);
            rasterize(triangle_vertices_xz[i],i,1);
            rasterize(triangle_vertices_yz[i],i,2);
        }
        set_f = 0;
    }
    if(set_k == 1){
        float new_k;
        cout << "-K- (average distance) value: ";
        cin >> new_k;
        K = new_k;
        get_intensities();
        get_vertices_in_pixels();
        make_all_lines();
        clearboards();
        make_boards(lines_xy,lines_intensities_xy,0);
        make_boards(lines_xz,lines_intensities_xz,1);
        make_boards(lines_yz,lines_intensities_yz,2);

        for(int i = 0; i < objectNum; i++){
            rasterize(triangle_vertices_xy[i],i,0);
            rasterize(triangle_vertices_xz[i],i,1);
            rasterize(triangle_vertices_yz[i],i,2);
        }
        set_k = 0;
    }
    if(set_n == 1){
        int obj;
        float new_n;
        cout << "Which material (0,1,..): ";
        cin >> obj;
        cout << "-n-: ";
        cin >> new_n;
        n_constant[obj] = new_n;
        get_intensities();
        get_vertices_in_pixels();
        make_all_lines();
        clearboards();
        make_boards(lines_xy,lines_intensities_xy,0);
        make_boards(lines_xz,lines_intensities_xz,1);
        make_boards(lines_yz,lines_intensities_yz,2);

        for(int i = 0; i < objectNum; i++){
            rasterize(triangle_vertices_xy[i],i,0);
            rasterize(triangle_vertices_xz[i],i,1);
            rasterize(triangle_vertices_yz[i],i,2);
        }
        set_n = 0;
    }

    if(set_quit == 1){
        glutDestroyWindow(win);
        exit(0);
    }
}

//this is where we render the screen
void display()
{
    //clears the screen
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix
    glLoadIdentity();

    glClearColor(0,0,0,0);
    check_changes();//check changes in parameters
    
    draw_object_polygons(plane);
    print_to_screen(plane);
    //blits the current opengl framebuffer on the screen
    glutSwapBuffers();
    //checks for opengl errors
    check();
}

bool point_in_the_list(vector<vector<int> > list, vector<int> p){
    for(int i = 0; i < list.size(); i++){
        if(list[i][0] == p[0] && list[i][1] == p[1]){
            return true;
        }
    }
    return false;
}

void print_to_screen(int plane){
    vector<vector<vector<vector<int> > > > filled_polygons;
    vector<vector<vector<vector<float> > > > filled_polygons_int;

    if(plane == 0){
        filled_polygons = filled_polygons_xy;
        filled_polygons_int = filled_polygons_int_xy;
    }
    else if(plane == 1){
        filled_polygons = filled_polygons_xz;
        filled_polygons_int = filled_polygons_int_xz;
    }
    else{
        filled_polygons = filled_polygons_yz;
        filled_polygons_int = filled_polygons_int_yz;
    }
    for(int m = 0; m < filled_polygons.size(); m++){
        for(int n = 0; n <filled_polygons[m].size();n++){
            for(int i = 0; i < filled_polygons[m][n].size();i++){
                    vector<int> temp_point = filled_polygons[m][n][i];
                    int coord1 = temp_point[0];
                    int coord2 = temp_point[1];
                    float intensity = filled_polygons_int[m][n][coord2][coord1];

                    int factor = 3;
                    int offset1 = coord1*factor;
                    int offset2 = coord2*factor;
                    
                    int seed = m*n*i;
                    srand (seed);
                    int num_grids = round(intensity*10);
                    vector<int> this_point(2,-1);
                    vector<vector<int> > position_list;
                    for(int l = 0; l < num_grids; l++){
                        int x = rand() % 3;
                        int y = rand() % 3;
                        this_point[0] = x;
                        this_point[1] = y;
                        while(point_in_the_list(position_list,this_point)){
                            int x = rand() % 3;
                            int y = rand() % 3;
                            this_point[0] = x;
                            this_point[1] = y;
                        }
                        position_list.push_back(this_point);
                    }
                    //cout << intensity << endl;
                    for(int l = 0; l < position_list.size(); l++){
                        int x = position_list[l][0];
                        int y = position_list[l][1];
                        int x_coord = offset1 + x;
                        int y_coord = offset2 + y;
                        draw_pix(x_coord,y_coord);
                    }
            }
        }
    }
}

//get point_normals list
//for each triangle(connection), get normal of the vertices
    //for each of the 3 vertices on the plane, (map to right index in point_normal_lists from point_lists) 
    //append the normal (NOTE! [0] -- [2] !!Not push_back!!)
//get point_normal_lists list
void get_normals(){
    for(int i = 0; i < objectNum; i++){
        for(int j = 0; j < triangle_connections[i].size(); j++){
            vector<int> current_tri = triangle_connections[i][j];
            int index1 = current_tri[0];
            int index2 = current_tri[1];
            int index3 = current_tri[2];
            //counter-clockwise vertices order
            //get normal for each vertice
            vector<float> vertex1 = point_lists[i][index1];
            vector<float> vertex2 = point_lists[i][index2];
            vector<float> vertex3 = point_lists[i][index3];
            vector<float> vector12 = create_vector(vertex1,vertex2);
            vector<float> vector13 = create_vector(vertex1,vertex3);
            vector<float> cross = get_cross_product(vector12,vector13);
            vector<float> normal = to_unit(cross);
            //print_point(normal);
            append_normal_to_point(normal,i,index1);
            append_normal_to_point(normal,i,index2);
            append_normal_to_point(normal,i,index3);
        }
    }
    for(int i = 0; i < objectNum; i++){
        //cout << "obj:" << i << endl;
        //cout << "  points count: "<< point_normal_lists[i].size() << endl;
        for(int j = 0; j < point_normal_lists[i].size(); j++){
            //cout << "    Normal count: " << point_normal_lists[i][j].size()/3 << endl;
            vector<float> x_values;
            vector<float> y_values;
            vector<float> z_values;
            for(int k = 0; k < point_normal_lists[i][j].size(); k+=3){
                //cout << "     normal num: " << k/3 << " ->";
                x_values.push_back(point_normal_lists[i][j][k]);
                y_values.push_back(point_normal_lists[i][j][k+1]);
                z_values.push_back(point_normal_lists[i][j][k+2]);
                
                float x = point_normal_lists[i][j][k];
                float y = point_normal_lists[i][j][k+1];
                float z = point_normal_lists[i][j][k+2];
                vector<float> temp;
                temp.push_back(x);
                temp.push_back(y);
                temp.push_back(z);
                //print_point(temp);
            }
            float x_mean = calculate_average(x_values);
            float y_mean = calculate_average(y_values);
            float z_mean = calculate_average(z_values);
            point_normals[i][j].push_back(x_mean);
            point_normals[i][j].push_back(y_mean);
            point_normals[i][j].push_back(z_mean);
        }
    }
}

//get intensity at each vertex using the Phong light model
void get_intensities(){
    //only use red for now
    float la = La[0];
    //cout << "la: " << la << endl;
    float li = Li[0];
    //cout << "li: "<< li << endl;
    //get K = K

    //for each object
    for(int i = 0; i < point_lists.size(); i++){
        int which_material = obj_material[i];
        //get n
        int n = n_constant[which_material];
        //cout << "n: " << n << endl;

        float ka = Ka[which_material][0];
        float kd = Kd[which_material][0];
        float ks = Ks[which_material][0];

        //cout << "ka kd ks:  " << ka << " " << kd << " " << ks << endl;
        //for each point in the object
        for(int j = 0; j < point_lists[i].size(); j++){
            //cout << "point: " << j << endl;
            //get ||f-p||
            vector<float> pf_xy = create_vector(point_lists[i][j],f_xy);
            vector<float> pf_xz = create_vector(point_lists[i][j],f_xz);
            vector<float> pf_yz = create_vector(point_lists[i][j],f_yz);
            float pf_val_xy = get_magnitude(pf_xy);
            float pf_val_xz = get_magnitude(pf_xz);
            float pf_val_yz = get_magnitude(pf_yz);
            //get n
            vector<float> normal = point_normals[i][j];
            //cout << "normal vector: ";
            //print_point(normal);

            //get l(unit)
            vector<float> light_vector;
            vector<float> px = create_vector(point_lists[i][j],X);
            float px_val = get_magnitude(px);
            light_vector = make_point(px[0]/px_val, px[1]/px_val, px[2]/px_val);
            //cout << "light vector: ";
            //print_point(light_vector);

            //get r(unit) 2((n dot l)*n) − l
            float dot_product = normal[0]*light_vector[0] + normal[1]*light_vector[1] + normal[2]*light_vector[2];
            vector<float> reflection_vector;
            reflection_vector.push_back(2*dot_product*normal[0]-light_vector[0]);
            reflection_vector.push_back(2*dot_product*normal[1]-light_vector[1]);
            reflection_vector.push_back(2*dot_product*normal[2]-light_vector[2]);
            //cout << "reflection vector: ";
            //print_point(reflection_vector);

            //get v = <f-p>/||f-p||
            vector<float> view_vector_xy,view_vector_xz,view_vector_yz;
            view_vector_xy = make_point(pf_xy[0]/pf_val_xy, pf_xy[1]/pf_val_xy, pf_xy[2]/pf_val_xy);
            view_vector_xz = make_point(pf_xz[0]/pf_val_xz, pf_xz[1]/pf_val_xz, pf_xz[2]/pf_val_xz);
            view_vector_yz = make_point(pf_yz[0]/pf_val_yz, pf_yz[1]/pf_val_yz, pf_yz[2]/pf_val_yz);
            //cout << "viewing vector: ";
            //print_point(view_vector_xy);
            //print_point(view_vector_xz);
            //print_point(view_vector_yz);

            //get intensity
            float nl_product = get_dot_product(normal,light_vector);
            float nv_product_xy = get_dot_product(normal,view_vector_xy);
            float nv_product_xz = get_dot_product(normal,view_vector_xz);
            float nv_product_yz = get_dot_product(normal,view_vector_yz);
            float rv_product_xy = get_dot_product(reflection_vector,view_vector_xy);
            float rv_product_xz = get_dot_product(reflection_vector,view_vector_xz);
            float rv_product_yz = get_dot_product(reflection_vector,view_vector_yz);
            
            vector<float> intensities;
            intensities.push_back(ka*la + (li/(pf_val_xy + K))*(kd * nl_product + ks * pow(rv_product_xy,n)));
            intensities.push_back(ka*la + (li/(pf_val_xz + K))*(kd * nl_product + ks * pow(rv_product_xz,n)));
            intensities.push_back(ka*la + (li/(pf_val_yz + K))*(kd * nl_product + ks * pow(rv_product_yz,n)));
            for(int l = 0; l < intensities.size();l++){
                if(abs_val(intensities[l]) < 0.00001)
                    intensities[l] = 0;

            }
            point_intensities_xy[i][j] = intensities[0];
            point_intensities_xz[i][j] = intensities[1];
            point_intensities_yz[i][j] = intensities[2];
            /*
            if((nl_product > 0 && nv_product_xy > 0)||(nl_product < 0 && nv_product_xy < 0))

            if((nl_product > 0 && nv_product_xy > 0)||(nl_product < 0 && nv_product_xy < 0))

            if((nl_product > 0 && nv_product_xy > 0)||(nl_product < 0 && nv_product_xy < 0))            
            */
            //cout << "\n";
        }

        //normalize intensities
        float xy_max = *max_element(point_intensities_xy[i].begin(), point_intensities_xy[i].end());
        point_intensities_xy[i] = normalize_intensities(point_intensities_xy[i],xy_max);

        float xz_max = *max_element(point_intensities_xz[i].begin(), point_intensities_xz[i].end());
        point_intensities_xz[i] = normalize_intensities(point_intensities_xz[i],xz_max);

        float yz_max = *max_element(point_intensities_yz[i].begin(), point_intensities_yz[i].end());
        point_intensities_yz[i] = normalize_intensities(point_intensities_yz[i],yz_max);

    }
}

int which_object_to_compare;
int which_projection;
bool descending(int i, int j){
    if(which_projection == 0){
        return depths_triangles_xy[which_object_to_compare][i] > depths_triangles_xy[which_object_to_compare][j];
    }
    if(which_projection == 1){
        return depths_triangles_xz[which_object_to_compare][i] > depths_triangles_xz[which_object_to_compare][j];
    }
    if(which_projection == 2){
        return depths_triangles_yz[which_object_to_compare][i] > depths_triangles_yz[which_object_to_compare][j];
    }
}

//for each object, 1)pick v*n > 0 triangles + get depth, 2) sort depth of each triangle, get indices
void get_triangles(){
    //get valid triangles and depths
    //for each object
    for(int i = 0; i < triangle_connections.size();i++){
        vector<vector<int> > triangles_per_object_xy;
        vector<vector<int> > triangles_per_object_xz;
        vector<vector<int> > triangles_per_object_yz;
        vector<float> depths_xy;
        vector<float> depths_xz;
        vector<float> depths_yz;
        //for each triangle
        for(int j = 0; j < triangle_connections[i].size(); j++){
            //pick triangles whose v*n >0
            vector<float> p1 = triangle_lists[i][j][0];
            vector<float> p2 = triangle_lists[i][j][1];
            vector<float> p3 = triangle_lists[i][j][2];
            vector<int> this_tri = triangle_connections[i][j];
            vector<float> vector12 = create_vector(p1,p2);
            vector<float> vector13 = create_vector(p1,p3);
            vector<float> cross = get_cross_product(vector12,vector13);

            vector<float> point_on_xy = make_point(p1[0],p1[1],0);
            vector<float> point_on_xz = make_point(p1[0],0,p1[2]);
            vector<float> point_on_yz = make_point(0,p1[1],p1[2]);

            vector<float> vector_to_xy = create_vector(p1,point_on_xy);
            vector<float> vector_to_xz = create_vector(p1,point_on_xz);
            vector<float> vector_to_yz = create_vector(p1,point_on_yz);

            float dot_product_xy = get_dot_product(cross,vector_to_xy);
            float dot_product_xz = get_dot_product(cross,vector_to_xz);
            float dot_product_yz = get_dot_product(cross,vector_to_yz);
            if(dot_product_xy > 0){
                vector<float> dis;
                //find min distance of the 3 vertices
                dis.push_back(abs_val(p1[2]));
                dis.push_back(abs_val(p2[2]));
                dis.push_back(abs_val(p3[2]));
                float min = 100000;
                for(int p = 0; p < dis.size(); p++){
                    if(dis[p] < min)
                        min = dis[p];
                }
                //add depth to depth_triangle
                depths_xy.push_back(min);
                //add triangle to picked_triangles
                triangles_per_object_xy.push_back(this_tri);
            }
            if(dot_product_xz > 0){
                vector<float> dis;
                dis.push_back(abs_val(p1[1]));
                dis.push_back(abs_val(p2[1]));
                dis.push_back(abs_val(p3[1]));
                float min = 100000;
                for(int p = 0; p < dis.size(); p++){
                    if(dis[p] < min)
                        min = dis[p];
                }
                depths_xz.push_back(min);
                triangles_per_object_xz.push_back(this_tri);                
            }
            if(dot_product_yz > 0){
                vector<float> dis;
                dis.push_back(abs_val(p1[0]));
                dis.push_back(abs_val(p2[0]));
                dis.push_back(abs_val(p3[0]));
                float min = 100000;
                for(int p = 0; p < dis.size(); p++){
                    if(dis[p] < min)
                        min = dis[p];
                }
                depths_yz.push_back(min);
                triangles_per_object_yz.push_back(this_tri);
            }
        }
        picked_triangles_xy.push_back(triangles_per_object_xy);
        picked_triangles_xz.push_back(triangles_per_object_xz);
        picked_triangles_yz.push_back(triangles_per_object_yz);
        depths_triangles_xy.push_back(depths_xy);
        depths_triangles_xz.push_back(depths_xz);
        depths_triangles_yz.push_back(depths_yz);
    }

    //sort the triangles
    //for each object sort the picked triangles
    for(int i = 0; i < depths_triangles_xy.size(); i++){
        vector<vector<int> > sorted_xy;
        vector<int> list0;
        for(int j = 0; j < depths_triangles_xy[i].size(); j++){
            list0.push_back(j);
        }
        which_projection = 0;
        which_object_to_compare = i;

        sort(list0.begin(), list0.end(),descending);

        for(int j = 0; j < list0.size(); j++){
            int temp_index = list0[j];
            sorted_xy.push_back(picked_triangles_xy[i][temp_index]); 
        }
        sorted_triangles_xy.push_back(sorted_xy);
    }
    //for each object
    for(int i = 0; i < depths_triangles_xz.size(); i++){
        vector<vector<int> > sorted_xz;
        vector<int> list1;
        for(int j = 0; j < depths_triangles_xy[i].size(); j++){
            list1.push_back(j);
        }
        which_projection = 1;
        which_object_to_compare = i;

        sort(list1.begin(), list1.end(),descending);

        for(int j = 0; j < list1.size(); j++){
            int temp_index = list1[j];
            sorted_xz.push_back(picked_triangles_xz[i][temp_index]); 
        }
        sorted_triangles_xz.push_back(sorted_xz);
    }
    //for each object
    for(int i = 0; i < depths_triangles_yz.size(); i++){
        vector<vector<int> > sorted_yz;
        vector<int> list2;
        for(int j = 0; j < depths_triangles_xy[i].size(); j++){
            list2.push_back(j);
        }
        which_projection = 2;
        which_object_to_compare = i;

        sort(list2.begin(), list2.end(),descending);

        for(int j = 0; j < list2.size(); j++){
            int temp_index = list2[j];
            sorted_yz.push_back(picked_triangles_yz[i][temp_index]); 
        }
        sorted_triangles_yz.push_back(sorted_yz);
    }
}

// for each triangle
//  make vertex lists (in pixels)
//  make vertex intensity lists
void get_vertices_in_pixels(){
    //xy
    //cout << "\n\n";
    triangle_vertices_xy.clear();
    triangle_vertices_xz.clear();
    triangle_vertices_yz.clear();
    triangle_intensities_xy.clear();
    triangle_intensities_xz.clear();
    triangle_intensities_yz.clear();

    for(int i = 0; i < sorted_triangles_xy.size(); i++){
        //cout << "obj: " << i << endl;
        vector<vector<vector<int> > > vertices_list;
        vector<vector<float> > intensities_list;//list of intensities in triangles

        for(int j = 0; j < sorted_triangles_xy[i].size(); j++){
            //cout << "tri: " << j << endl;
            if(sorted_triangles_xy[i][j].size() == 0)
                continue;
            vector<vector<int> > vertices;
            int index1 = sorted_triangles_xy[i][j][0];
            int index2 = sorted_triangles_xy[i][j][1];
            int index3 = sorted_triangles_xy[i][j][2];
            vector<float> p1 = point_lists[i][index1];
            vector<float> p2 = point_lists[i][index2];
            vector<float> p3 = point_lists[i][index3];
            
            //print_point(p1);
            //print_point(p2);
            //print_point(p3);
            
            vertices.push_back(to_pixel_coord(p1));
            vertices.push_back(to_pixel_coord(p2));
            vertices.push_back(to_pixel_coord(p3));
            vertices_list.push_back(vertices);

            vector<float> intensities = make_point(point_intensities_xy[i][index1],point_intensities_xy[i][index2],point_intensities_xy[i][index3]);
            intensities_list.push_back(intensities);
        }
        triangle_vertices_xy.push_back(vertices_list);
        triangle_intensities_xy.push_back(intensities_list);
    }
    //xz
    for(int i = 0; i < sorted_triangles_xz.size(); i++){
        //cout << "obj1: " << i << endl;
        vector<vector<vector<int> > > vertices_list;
        vector<vector<float> > intensities_list;
        for(int j = 0; j < sorted_triangles_xz[i].size(); j++){
            //cout << "tri1: " << j << endl;
            if(sorted_triangles_xz[i][j].size() == 0)
                continue;
            vector<vector<int> > vertices;
            int index1 = sorted_triangles_xz[i][j][0];
            int index2 = sorted_triangles_xz[i][j][1];
            int index3 = sorted_triangles_xz[i][j][2];
            //cout << "indexs: "<<index1<<" " << index2 << " " << index3 << endl;
            vector<float> p1 = point_lists[i][index1];
            vector<float> p2 = point_lists[i][index2];
            vector<float> p3 = point_lists[i][index3];
            //print_point(p1);
            //print_point(p2);
            //print_point(p3);
            vertices.push_back(to_pixel_coord(p1));
            vertices.push_back(to_pixel_coord(p2));
            vertices.push_back(to_pixel_coord(p3));
            //cout << "here" << endl;
            vertices_list.push_back(vertices);
            //cout << "here" << endl;

            vector<float> intensities = make_point(point_intensities_xz[i][index1],point_intensities_xz[i][index2],point_intensities_xz[i][index3]);
            intensities_list.push_back(intensities);
        }
        triangle_vertices_xz.push_back(vertices_list);
        triangle_intensities_xz.push_back(intensities_list);
    }
    //yz
    for(int i = 0; i < sorted_triangles_yz.size(); i++){
        //cout << "obj2: " << i << endl;
        vector<vector<vector<int> > > vertices_list;
        vector<vector<float> > intensities_list;
        for(int j = 0; j < sorted_triangles_yz[i].size(); j++){
            //cout << "tri2: " << j << endl;
            if(sorted_triangles_yz[i][j].size() == 0)
                continue;
            vector<vector<int> > vertices;
            int index1 = sorted_triangles_yz[i][j][0];
            int index2 = sorted_triangles_yz[i][j][1];
            int index3 = sorted_triangles_yz[i][j][2];
            vector<float> p1 = point_lists[i][index1];
            vector<float> p2 = point_lists[i][index2];
            vector<float> p3 = point_lists[i][index3];
            vertices.push_back(to_pixel_coord(p1));
            vertices.push_back(to_pixel_coord(p2));
            vertices.push_back(to_pixel_coord(p3));
            //cout << "here" << endl;
            vertices_list.push_back(vertices);
            //cout << "here" << endl;

            vector<float> intensities = make_point(point_intensities_yz[i][index1],point_intensities_yz[i][index2],point_intensities_yz[i][index3]);
            intensities_list.push_back(intensities);
        }
        triangle_vertices_yz.push_back(vertices_list);
        triangle_intensities_yz.push_back(intensities_list);
        //cout << "here" << endl;
    }
    
}

//vector<vector<vector<vector<int> > > > triangle_vertices_xy;
//vector<vector<vector<vector<vector<int> > > > > lines_xy;
//vector<vector<vector<vector<float> > > > lines_intensities_xy;
void make_all_lines(){
    lines_xy.clear();
    lines_xz.clear();
    lines_yz.clear();
    lines_intensities_xy.clear();
    lines_intensities_xz.clear();
    lines_intensities_yz.clear();

    //each object
    //cout << "xy" << endl;
    for(int i = 0; i < triangle_vertices_xy.size(); i++){
        //cout << "obj:" << i << endl;
        vector<vector<vector<vector<int> > > > lines_per_object;
        vector<vector<vector<float> > > intensities_per_object;
        //each tri
        for(int j = 0; j < triangle_vertices_xy[i].size(); j++){
            //cout << "tri:" << j << endl;
            vector<vector<vector<int> > > lines_per_triangle = make_lines(triangle_vertices_xy[i][j],0);
            vector<vector<float> > intensities_per_triangle = make_line_intensity(triangle_vertices_xy[i][j],i,j,0);
            lines_per_object.push_back(lines_per_triangle);
            intensities_per_object.push_back(intensities_per_triangle);
        }
        lines_xy.push_back(lines_per_object);
        lines_intensities_xy.push_back(intensities_per_object);
    }
    //cout << "xz" << endl;
    for(int i = 0; i < triangle_vertices_xz.size(); i++){
        //cout << "obj:" << i << endl;
        vector<vector<vector<vector<int> > > > lines_per_object;
        vector<vector<vector<float> > > intensities_per_object;
        //each tri
        for(int j = 0; j < triangle_vertices_xz[i].size(); j++){
            //cout << "tri:" << j << endl;
            vector<vector<vector<int> > > lines_per_triangle = make_lines(triangle_vertices_xz[i][j],1);
            vector<vector<float> > intensities_per_triangle = make_line_intensity(triangle_vertices_xz[i][j],i,j,1);
            lines_per_object.push_back(lines_per_triangle);
            intensities_per_object.push_back(intensities_per_triangle);
        }
        lines_xz.push_back(lines_per_object);
        lines_intensities_xz.push_back(intensities_per_object);
    }
    //cout << "yz" << endl;
    for(int i = 0; i < triangle_vertices_yz.size(); i++){
        //cout << "obj:" << i << endl;
        vector<vector<vector<vector<int> > > > lines_per_object;
        vector<vector<vector<float> > > intensities_per_object;
        //each tri
        for(int j = 0; j < triangle_vertices_yz[i].size(); j++){
            //cout << "tri:" << j << endl;
            vector<vector<vector<int> > > lines_per_triangle = make_lines(triangle_vertices_yz[i][j],2);
            vector<vector<float> > intensities_per_triangle = make_line_intensity(triangle_vertices_yz[i][j],i,j,2);
            lines_per_object.push_back(lines_per_triangle);
            intensities_per_object.push_back(intensities_per_triangle);
        }
        lines_yz.push_back(lines_per_object);
        lines_intensities_yz.push_back(intensities_per_object);
    }
}

vector<int> to_pixel_coord(vector<float> p){
    vector<int> temp;
    temp.push_back((int)roundf(board_size*p[0]));
    temp.push_back((int)roundf(board_size*p[1]));
    temp.push_back((int)roundf(board_size*p[2]));
    return temp;
}

bool from_same_line(vector<int> p1, vector<int> p2, vector<vector<vector<int> > > lines){
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
vector<vector<int> > find_max(vector<vector<int> > list){
    int y_max = -1;
    vector<vector<int> > temp_list;

    for(int i = 0; i < list.size(); i++){
        if(y_max < list[i][1]){
            y_max = list[i][1]; // find the maximum value of y
        }
    }

    for(int i = 0; i < list.size(); i++){
        vector<int> temp;
        if(y_max == list[i][1]){
            temp.push_back(list[i][0]);
            temp.push_back(y_max);
            temp_list.push_back(temp);
        }
    }

    return temp_list;
}

//bottom point
vector<vector<int> > find_min(vector<vector<int> > list){
    int y_min = 10000;
    vector<vector<int> > temp_list;

    for(int i = 0; i < list.size(); i++){
        if(y_min > list[i][1]){
            y_min = list[i][1];//find the minimum y value
        }
    }

    for(int i = 0; i < list.size(); i++){
        vector<int> temp;
        if(y_min == list[i][1]){
            temp.push_back(list[i][0]);
            temp.push_back(y_min);
            temp_list.push_back(temp);
        }
    }

    return temp_list;
}

vector<vector<int> > local_extrema(vector<vector<int> > list){
    vector<vector<int> > extrema_lists;
    for(int i = 0; i < list.size(); i++){

    }
}

int switch_state_in_out(int state_in_out){
    if(state_in_out == IN)
        return OUT;
    else
        return IN;
}

//return a lined_board for all object
void make_boards(vector<vector<vector<vector<vector<int> > > > > line_list, vector<vector<vector<vector<float> > > > intens_list,int which_proj){
    //vector<vector<vector<vector<int> > > > lined_board;
    //vector<vector<vector<vector<float> > > > lined_board_intensities;

    //obj
    for(int m = 0; m < line_list.size(); m++){
        //tri
        for(int n = 0; n < line_list[m].size(); n++){
            //line
            for(int k = 0; k < line_list[m][n].size();k++){
                //point
                for(int p = 0; p < line_list[m][n][k].size(); p++){
                    //for (int j = 0; j < 100; j++){
                        //for (int i = 0; i < 100; i++){
                            int tempx = line_list[m][n][k][p][0];
                            int tempy = line_list[m][n][k][p][1];
                            //if(i == tempx && j == tempy){
                            if(which_proj == 0){
                                lined_board_xy[m][n][tempx][tempy] = 1;
                                lined_board_intensities_xy[m][n][tempx][tempy] = intens_list[m][n][k][p];
                            }
                            else if(which_proj == 1){
                                lined_board_xz[m][n][tempx][tempy] = 1;
                                lined_board_intensities_xz[m][n][tempx][tempy] = intens_list[m][n][k][p];
                            }
                            else{
                                lined_board_yz[m][n][tempx][tempy] = 1;
                                lined_board_intensities_yz[m][n][tempx][tempy] = intens_list[m][n][k][p];
                            }  
                                //cout << "i: " << i << "  j: " << j << endl;
                            //}
                        //}
                    //}
                }
            }
        }
    }
}

//per object
//input vertices list, object, which projection
void rasterize(vector<vector<vector<int> > > v, int obj, int which_proj){
    //cout << "proj:" << which_proj << endl;
    vector<vector<vector<int> > > temp_list;
    vector<vector<vector<float> > > int_list;
    vector<vector<vector<int> > > lined_board;
    vector<vector<vector<int> > > vertices;

    if(which_proj == 0){
        lined_board = lined_board_xy[obj];
        for(int i = 0; i < v.size();i++){
            vector<vector<int> > temp;
            for(int j = 0; j < v[i].size(); j++){
                vector<int> new_point;
                int coord1 = v[i][j][0];
                int coord2 = v[i][j][1];
                new_point.push_back(coord1);
                new_point.push_back(coord2);
                temp.push_back(new_point);
            }
            vertices.push_back(temp);
        }
    }
    else if(which_proj == 1){
        lined_board = lined_board_xz[obj];
        for(int i = 0; i < v.size();i++){
            vector<vector<int> > temp;
            for(int j = 0; j < v[i].size(); j++){
                vector<int> new_point;
                int coord1 = v[i][j][0];
                int coord2 = v[i][j][2];
                new_point.push_back(coord1);
                new_point.push_back(coord2);
                temp.push_back(new_point);
            }
            vertices.push_back(temp);
        }
    }
    else{
        lined_board = lined_board_yz[obj];
        for(int i = 0; i < v.size();i++){
            vector<vector<int> > temp;
            for(int j = 0; j < v[i].size(); j++){
                vector<int> new_point;
                int coord1 = v[i][j][1];
                int coord2 = v[i][j][2];
                new_point.push_back(coord1);
                new_point.push_back(coord2);
                temp.push_back(new_point);
            }
            vertices.push_back(temp);
        }
    }
    //find the top/bottom vertices

    vector<vector<vector<int> > > extrema_lists;
    for(int i = 0; i < vertices.size(); i++){
        //cout << "triangle: "<< i << endl;
        vector<vector<int> > temp_point_list;//one extrema list per triangle

        vector<vector<int> > temp_temp_point_list;

        temp_temp_point_list = find_max(vertices[i]);
        for(int j = 0; j < temp_temp_point_list.size(); j++){
            //cout << temp_temp_point_list[j][0] << " " << temp_temp_point_list[j][1] << endl;
            temp_point_list.push_back(temp_temp_point_list[j]);
        }
        temp_temp_point_list = find_min(vertices[i]);
        for(int j = 0; j < temp_temp_point_list.size(); j++){
            //cout << temp_temp_point_list[j][0] << " " << temp_temp_point_list[j][1] << endl;
            temp_point_list.push_back(temp_temp_point_list[j]);
        }
        extrema_lists.push_back(temp_point_list);
    }

    //cout << "rasterize: "<< endl;
    for(int m =0; m < vertices.size(); m++){
        //cout << "obj: " << obj << " tri: " << m<<endl;
        vector<vector<vector<int> > > lines;
        if(which_proj == 0)
            lines = lines_xy[obj][m];
        else if(which_proj == 1)
            lines = lines_xz[obj][m];
        else
            lines = lines_yz[obj][m];

        vector<vector<int> > temp_temp;
        vector<vector<float> > int_temp;
        for (int j = 0; j < board_size; j++){
            //reset in/out param for a new line
            int state_in_out = OUT;
            vector<vector<int> > in_list;
            for (int i = 0; i < board_size; i++){
                //cout << "j: " << j << " i: " << i << endl;
                vector<int> current;
                current.push_back(i);
                current.push_back(j);

                vector<int> previous;
                if(i > 1){
                    previous.push_back(i-1);
                    previous.push_back(j);
                }

                //touching an edge
                if(lined_board[m][i][j] == 1){

                    bool top_or_bottom = false;
                    //for(int n = 0; n < 3; n++){
                        for(int k = 0; k < extrema_lists[m].size(); k++){
                            if(isEqual(extrema_lists[m][k],current)){
                                top_or_bottom = true;
                                break;
                            }
                        }
                    //}
                    //if not a top/bottom vertex, switch state_in_out; else: remain state_in_out
                    //cout << "edge! i: " << i << "  j: " << j;
                    if(top_or_bottom == false){
                        //cout << " not extrema";
                        if(i > 1){
                            //if the previous pixel is an edge
                            if(lined_board[m][i-1][j] == 1){
                                if(from_same_line(current,previous,lines) == true){ //from the same line,same state_in_out
                                   //cout << "previous edge from same line, not switching state_in_out\n";
                                }
                                else{ //not from the same line, change state_in_out
                                    state_in_out = switch_state_in_out(state_in_out);
                                    //cout << "previous edge not from same line, switching state_in_out to --- " << state_in_out;
                                }
                            }
                            else{
                                state_in_out = switch_state_in_out(state_in_out);
                                //cout << "switching state_in_out to --- " << state_in_out;
                            }
                        }
                        else{ //switch state_in_out if lined_board[i>1][]is an edge
                            state_in_out = switch_state_in_out(state_in_out);
                            //cout << "switching state_in_out to --- " << state_in_out;
                        }
                        //string input;
                        //cin >> input;
                    }
                    else{
                        if(state_in_out == IN)
                            state_in_out = OUT;
                        //cout << " extrema ";
                        //string input;
                        //cin >> input;
                    }
                }
                //not touching an edge
                else{
                    //if state_in_out is IN, fill pixel
                    if(state_in_out == IN){
                        temp_temp.push_back(current);
                        in_list.push_back(current);
                        //cout << "not edge! i: " << i << "  j: " << j << endl;;
                   }
                }
            }
            //get intensity of the line
            if(!in_list.empty()){
                //for(int k = 0; k < in_list.size();k++)
                //    cout << in_list[k][0] << " " << in_list[k][1] << endl;
                vector<int> p1 = in_list.front();
                p1[0]--;
                vector<int> p2 = in_list.back();
                p2[0]++;
                float int1,int2;
                if(which_proj == 0){
                    int1 = lined_board_intensities_xy[obj][m][p1[0]][p1[1]];
                    int2 = lined_board_intensities_xy[obj][m][p2[0]][p2[1]];
                    //cout << int1<<" " << int2 << endl;
                }
                else if(which_proj == 1){
                    int1 = lined_board_intensities_xz[obj][m][p1[0]][p1[1]];
                    int2 = lined_board_intensities_xz[obj][m][p2[0]][p2[1]];
                }
                else{
                    int1 = lined_board_intensities_yz[obj][m][p1[0]][p1[1]];
                    int2 = lined_board_intensities_yz[obj][m][p2[0]][p2[1]];
                }
                //cout << int1<< int2<< endl;
                vector<float> intensities_per_line = get_line_intensity(in_list.size()+2,int1,int2);
                //cout << "size: " << intensities_per_line.size() << endl;
                intensities_per_line.erase(intensities_per_line.begin());
                intensities_per_line.pop_back();
                //fill the whole line
                for(int p = 0; p < p1[0]+1;p++)
                    intensities_per_line.insert(intensities_per_line.begin(),0.0);
                for(int p = 0; p < board_size - p2[0];p++) 
                    intensities_per_line.push_back(0.0);
                //cout<<"size: " << intensities_per_line.size()<< endl;
                //cout << "here" << endl;
                //intensity_per_line[j] 
                int_temp.push_back(intensities_per_line);
            }
            else{
                //cout << "line empty, j: " << j << endl;
                vector<float> intensities_per_line(board_size,0.0);
                int_temp.push_back(intensities_per_line);
            }
        }
        temp_list.push_back(temp_temp);
        int_list.push_back(int_temp);
        //cout << "line count: " << int_temp.size();
    }
    //cout << "done rasterize" << endl;
    if(which_proj == 0){
        filled_polygons_xy[obj] = temp_list;
        filled_polygons_int_xy[obj] = int_list;
    }
    else if(which_proj == 1){
        filled_polygons_xz[obj] = temp_list;
        filled_polygons_int_xz[obj] = int_list;
    }
    else{
        filled_polygons_yz[obj] = temp_list;
        filled_polygons_int_yz[obj] = int_list;
    }
}


//input: vertices
//output: list of lines
vector<vector<vector<int> > > make_lines(vector<vector<int> > list, int which_proj){
    vector<vector<vector<int> > > these_lines;
    int index1,index2;
    if(which_proj == 0){
        index1 = 0;
        index2 = 1;
    }
    else if(which_proj == 1){
        index1 = 0;
        index2 = 2;
    }
    else{
        index1 = 1;
        index2 = 2;
    }
    //total number of lines in the polygon
    int pointNum = list.size();
    for(int i = 0; i < pointNum; i++){//for each vertex
        vector<int> point1,point2;
        point1.push_back(list[i][index1]);
        point1.push_back(list[i][index2]);
        //concatnate end point to start point
        if(i == pointNum-1){
            point2.push_back(list[0][index1]);
            point2.push_back(list[0][index2]);
        }
        else{
            point2.push_back(list[i+1][index1]);
            point2.push_back(list[i+1][index2]);
        }
        
        vector<vector<int> > newline;
        newline = make_line_DDA(point1, point2);
        //cout<<"lineNum: "<<i <<"line size: " << newline.size()<< endl;
        these_lines.push_back(newline);
    }
    return these_lines;
}

vector<vector<float> > make_line_intensity(vector<vector<int> > list, int obj, int tri, int which_proj){
    vector<vector<float> > these_intensities;
    int coord1,coord2;
    if(which_proj == 0){
        coord1 = 0;
        coord2 = 1;
    }
    else if(which_proj == 1){
        coord1 = 0;
        coord2 = 2;
    }
    else{
        coord1 = 1;
        coord2 = 2;
    }
    //total number of lines in the polygon
    int pointNum = list.size();
    for(int i = 0; i < pointNum; i++){//for each vertex
        vector<int> point1,point2;
        int index1, index2;
        index1 = i;
        point1.push_back(list[i][coord1]);
        point1.push_back(list[i][coord2]);
        //concatnate end point to start point
        if(i == pointNum-1){
            index2 = 0;
            point2.push_back(list[0][coord1]);
            point2.push_back(list[0][coord2]);
        }
        else{
            index2 = i+1;
            point2.push_back(list[i+1][coord1]);
            point2.push_back(list[i+1][coord2]);
        }

        vector<vector<int> > newline;
        newline = make_line_DDA(point1, point2);
        //set intensities for points on a line
        float intensity1,intensity2;
        if(which_proj == 0){
            intensity1 = triangle_intensities_xy[obj][tri][index1];
            intensity2 = triangle_intensities_xy[obj][tri][index2];

        }
        else if(which_proj == 1){
            intensity1 = triangle_intensities_xz[obj][tri][index1];
            intensity2 = triangle_intensities_xz[obj][tri][index2];
        }
        else{
            intensity1 = triangle_intensities_yz[obj][tri][index1];
            intensity2 = triangle_intensities_yz[obj][tri][index2];
        }
        vector<float> line_intensity = get_line_intensity(newline.size(),intensity1,intensity2);
        //cout<<"lineNum: "<<i <<"line intensity size: " << line_intensity.size()<< endl;
        these_intensities.push_back(line_intensity);
    }
    return these_intensities;
}

vector<float> get_line_intensity(int linesize, float int1, float int2){
    vector<float> temp;
    temp.push_back(int1);
    float previous = int1;
    float inc = (int2-int1)/(linesize-1);

    for(int i = 1; i < linesize-1; i++){
        float current = previous + inc;
        temp.push_back(current);
        previous = current;
    }
    temp.push_back(int2);
    return temp;
}

//input: end points of a line
//output: a line
vector<vector<int> > make_line_DDA(vector<int> start, vector<int> end){
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

vector<float> normalize_intensities(vector<float> values, float max){
    vector<float> temp;
    for(int i = 0; i < values.size(); i++){
        temp.push_back(values[i]/max);
    }
    return temp;
}

float get_magnitude(vector<float> v){
    float sum = 0;
    for(int i = 0; i < v.size(); i++)
        sum += pow(v[i],2);
    return sqrt(sum);
}

float calculate_average(vector<float> list){
    float sum = 0;
    for(int i = 0; i < list.size(); i++)
        sum += list[i];
    float avg = sum/list.size();
    if(abs_val(avg) < 0.00001)
        avg = 0;
    return avg;
}

void append_normal_to_point(vector<float> n, int obj, int point_index){
    //cout << "index: " << point_index << endl;
    //int empty_entry = get_empty_entry(point_normal_lists[obj][point_index]);
    point_normal_lists[obj][point_index].push_back(n[0]);
    point_normal_lists[obj][point_index].push_back(n[1]);
    point_normal_lists[obj][point_index].push_back(n[2]);
}

vector<float> create_vector(vector<float> p1,vector<float> p2){
    vector<float> temp;
    float x = p2[0] - p1[0];
    float y = p2[1] - p1[1];
    float z = p2[2] - p1[2];
    temp.push_back(x);
    temp.push_back(y);
    temp.push_back(z);
    return temp;
}

float get_dot_product(vector<float> a, vector<float> b){
    float sum = a[0]*b[0]+a[1]*b[1]+a[2]*b[2];
    return sum;
}

vector<float> get_cross_product(vector<float> a,vector<float> b){
    vector<float> temp;
    float x = a[1]*b[2] - a[2]*b[1];
    float y = a[2]*b[0] - a[0]*b[2];
    float z = a[0]*b[1] - a[1]*b[0];
    temp.push_back(x);
    temp.push_back(y);
    temp.push_back(z);
    return temp;
}

vector<float> to_unit(vector<float> input){
    float sum = pow(input[0],2)+pow(input[1],2)+pow(input[2],2);
    float magn = sqrt(sum);
    vector<float> temp;
    float x = input[0]/magn;
    float y = input[1]/magn;
    float z = input[2]/magn;
    if(abs_val(x)==0)
        x = 0;
    if(abs_val(y)==0)
        y = 0;
    if(abs_val(z)==0)
        z = 0;
    temp.push_back(x);
    temp.push_back(y);
    temp.push_back(z);
    return temp;
}

vector<float> make_point(float x, float y, float z){
    vector<float> temp;
    temp.push_back(x);
    temp.push_back(y);
    temp.push_back(z);
    return temp;
}


//load point_lists and edge_connections
void load_data(){
    material_filename = "materials.txt";
    light_filename = "light.txt";
    obj_filename = "test_objs.txt";

    /*
    cout << "Please enter the object filename: ";
    while(!(cin >> obj_filename)){
        cout << "Please enter the correct filename: ";
        cin >> obj_filename;
    }
    */

    //material file
    ifstream material(material_filename.c_str());
    int num_material = 0;
    material >> num_material;
    int index = 0;
    //cout <<"num_material: " << num_material << endl;
    while(index < num_material){
        vector<float> temp_Ka, temp_Kd, temp_Ks;
        float ax,ay,az,dx,dy,dz,sx,sy,sz,n;
        material >> ax >> ay >> az >> dx >> dy >> dz >> sx >> sy >> sz >> n;
        temp_Ka = make_point(ax,ay,az);
        temp_Kd = make_point(dx,dy,dz);
        temp_Ks = make_point(sx,sy,sz);
        //cout << n << endl;
        Ka.push_back(temp_Ka);
        Kd.push_back(temp_Kd);
        Ks.push_back(temp_Ks);
        n_constant.push_back(n);
        index++;
    }

    //light file
    ifstream light(light_filename.c_str());
    float xx,xy,xz,lax,lay,laz,lix,liy,liz,F,k_temp;
    light >> xx >> xy >> xz >> lax >> lay >> laz >> lix >> liy >> liz >> F >> k_temp;
    X = make_point(xx,xy,xz);
    La = make_point(lax,lay,laz);
    Li = make_point(lix,liy,liz);

    f_xy = make_point(0.5,0.5,F);
    f_xz = make_point(0.5,F,0.5);
    f_yz = make_point(F,0.5,0.5);
    K = k_temp;

    //object file
    ifstream obj(obj_filename.c_str());
    //number of objects
    obj >> objectNum;

    //read line by line
    int matrial_id = 0;
    while(obj >> matrial_id){
        matrial_id--;
        //cout << "\nmatrial_id: " << matrial_id << endl;
        obj_material.push_back(matrial_id);
        //load points in object
        int num_points = 0;
        obj >> num_points;
        //cout << "num_points: "<< num_points << endl;
        vector<vector<float> > single_point_list;
        vector<vector<vector<float> > > empty_normal_list;
        vector<vector<float> > empty_normals;
        vector<float> empty_intensities;
        for(int i = 0; i < num_points; i++){
            float x,y,z;
            vector<float> temp_point;

            obj >> x >> y >> z;
            //cout << "x: " << x << " y: " << y <<" z: " << z << endl;  
            temp_point.push_back(x);
            temp_point.push_back(y);
            temp_point.push_back(z);
            single_point_list.push_back(temp_point);

            //initialize point normals
            vector<float> empty_point;
            empty_normals.push_back(empty_point);
            //initialize intensities for each point
            float zero = 0.0;
            empty_intensities.push_back(zero);
        }
        point_lists.push_back(single_point_list);
        //point_lists_copy.push_back(single_point_list);
        point_normal_lists.push_back(empty_normals);
        point_normals.push_back(empty_normals);
        point_intensities_xy.push_back(empty_intensities);
        point_intensities_xz.push_back(empty_intensities);
        point_intensities_yz.push_back(empty_intensities);

        //number of edges in object
        int num_triangles = 0;
        obj >> num_triangles;
        //cout << "num_triangles" << num_triangles << endl;
        //load triangles
        vector<vector<vector<float> > > single_triangle_list; //float->point->triangle->triangles
        vector<vector<int> > single_triangle_connection;
        for(int i = 0; i < num_triangles; i++){
            int index1, index2, index3;
            vector<float> p1,p2,p3;
            vector<vector<float> > temp_triangle;
            vector<int> temp_connection;

            obj >> index1 >> index2 >> index3;
            //out << "index1: " << index1 << " index2: " << index2 << " index3: " << index3 << endl;
            temp_connection.push_back(index1-1);
            temp_connection.push_back(index2-1);
            temp_connection.push_back(index3-1);

            p1 = make_point(single_point_list[index1-1][0],single_point_list[index1-1][1],single_point_list[index1-1][2]);
            p2 = make_point(single_point_list[index2-1][0],single_point_list[index2-1][1],single_point_list[index2-1][2]);
            p3 = make_point(single_point_list[index3-1][0],single_point_list[index3-1][1],single_point_list[index3-1][2]);
            temp_triangle.push_back(p1);
            temp_triangle.push_back(p2);
            temp_triangle.push_back(p3);
            //cout << "p1:" << p1[0] << "," << p1[1] << "," << p1[2] << " p2:" << p2[0] << "," << p2[1] << "," << p2[2] << " p3:" << p3[0] << "," << p3[1] << "," << p3[2] << endl;
           
            single_triangle_list.push_back(temp_triangle);
            single_triangle_connection.push_back(temp_connection);
        }
        //cout << endl;
        triangle_connections.push_back(single_triangle_connection);
        triangle_lists.push_back(single_triangle_list);
    }
}



void draw_object_polygons(int which_proj){
    vector<vector<vector<vector<vector<int> > > > > lines;
    vector<vector<vector<vector<float> > > > intensities;
    if(which_proj == 0){
        lines = lines_xy;
        intensities = lines_intensities_xy;
    }
    else if(which_proj == 1){
        lines = lines_xz;
        intensities = lines_intensities_xz;
    }
    else{
        lines = lines_yz;
        intensities = lines_intensities_yz;
    }

    for(int i = 0; i < lines.size();i++){
        //cout << "obj: "<< i << endl;
        for(int j = 0; j < lines[i].size();j++){
            //cout << "tri: "<< j<< endl;
            for(int k = 0; k < lines[i][j].size(); k++){
                //cout << "line: " << k << endl;
                draw_points(lines[i][j][k],intensities[i][j][k],which_proj);
            }
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
    glOrtho(0.0,grid_width,0.0,grid_height,-10,10);
    
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
void mouse(int button, int state_in_out, int x, int y)
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
    if(state_in_out !=GLUT_DOWN)  //button released
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

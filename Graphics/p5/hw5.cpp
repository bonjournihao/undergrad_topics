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

#define QUIT 5
#define CHANGE 6

#define PI 3.14159265


/****set in main()****/
//the number of pixels in the grid
int grid_width;
int grid_height;
int board_size;
float circle_R;
float circle_R2;
int set_change = 0;

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
void reshape(int width, int height);
void key(unsigned char ch, int x, int y);
void mouse(int button, int state_in_out, int x, int y);
void motion(int x, int y);
void check();
void load_data(vector<float> ff_in, vector<float> up_in, vector<float> at_in, float angle_in, vector<float> X_in, float K_in, vector<float> La_in, vector<float> Li_in, vector<float> ka_in0, vector<float> kd_in0, vector<float> ks_in0, vector<float> ka_in1, vector<float> kd_in1, vector<float> ks_in1, int n_in0, int n_in1);
float get_dot_product(vector<float> a, vector<float> b);
float abs_val(float value);
void drawing();
void printing();
vector<float> get_cross_product(vector<float> v1,vector<float> v2);
vector<float> create_vector(vector<float> p1,vector<float> p2);
vector<float> to_unit(vector<float> input);
vector<float> eye_to_world(vector<float> point);
float get_magnitude(vector<float> v);
void draw_pix(int x, int y, float R, float G, float B);
vector<float> make_point(float x, float y, float z);
vector<float> scale_vector(vector<float> v, float scal);
void print_point(vector<float> p);
void load_matrices();
vector<float> even_rgb(float r, float g, float b);
void get_intersect();

vector<vector<vector<float> > > world_location_matrix;
vector<vector<vector<float> > > world_location_matrix2;
vector<vector<vector<float> > > intersect_matrix;
vector<vector<vector<float> > > intersect_matrix2;
vector<vector<vector<float> > > normal_matrix;
vector<vector<vector<float> > > normal_matrix2;

vector<vector<float> > intensities_r;
vector<vector<float> > intensities_g;
vector<vector<float> > intensities_b;

vector<vector<float> > intensities2_r;
vector<vector<float> > intensities2_g;
vector<vector<float> > intensities2_b;

vector<float> La,Li,X;
float K;
vector<vector<float> > ka,kd,ks;
vector<int> n;

int set_quit = 0;

int win;
int N;
float angle;//radians
vector<float> ff(3,0);
vector<float> at(3,0);
vector<float> up(3,0);
vector<float> b1(3,0);
vector<float> b2(3,0);
vector<float> b3(3,0);
float fx,fy,fz;


int main(int argc, char **argv)
{   
	N = 300;
	circle_R = 0.5;
	circle_R2 = 0.2;

    vector<float> temp_;
    ka.push_back(temp_);
    ka.push_back(temp_);
    kd.push_back(temp_);
    kd.push_back(temp_);
    ks.push_back(temp_);
    ks.push_back(temp_);
    n.push_back(0);
    n.push_back(0);
    //cout << "here" << endl;
    //the number of pixels in the grid
    grid_width = N;
    grid_height = N;
    
    //the size of pixels sets the inital window height and width
    //don't make the pixels too large or the screen size will be larger than
    //your display size
    
    /*Window information*/
    win_height = grid_height;
    win_width = grid_width;
    
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
    vector<float> ff_in = make_point(0,0,10);
    vector<float> up_in = make_point(0,1,0);
    vector<float> at_in = make_point(0,0,0);
    float angle_in = 0.4;
    vector<float> X_in = make_point(2,2,2);
    float K_in = 3.4641;
    vector<float> La_in = make_point(.5,.5,.5);
	vector<float> Li_in = make_point(.7,.7,.7);
	vector<float> ka_in0 = make_point(0.3,0.1,0.0);
	vector<float> kd_in0 = make_point(0.2,0.4,0.1);
	vector<float> ks_in0 = make_point(0.1,0.8,0.3);
	vector<float> ka_in1 = make_point(0.0,0.05,0.0);
	vector<float> kd_in1 = make_point(0.4,0.4,0.4);
	vector<float> ks_in1 = make_point(0.9,0.9,0.9);
	int n_in0 = 5;
	int n_in1 = 20;

    load_data(ff_in,up_in,at_in,angle_in,X_in,K_in,La_in,Li_in,ka_in0,kd_in0,ks_in0,ka_in1,kd_in1,ks_in1,n_in0,n_in1);

    //get matrices;
    load_matrices();
    //compute intesects + normals
    get_intersect();

    
    //start glut event loop
    glutMainLoop();

    return 0;
}

void display()
{
    //clears the screen
    glClear(GL_DEPTH_BUFFER_BIT|GL_COLOR_BUFFER_BIT);
    //clears the opengl Modelview transformation matrix
    glLoadIdentity();

    //draw
    drawing();
    printing();

    if(set_quit == 1){
        glutDestroyWindow(win);
        exit(0);
    }

    if(set_change == 1){
    	cout << "--Current--\n";
    	cout << "1) Light position -----X : (" << X[0] << "," << X[1] << "," << X[2] << ")\n";
    	cout << "2) Light ambient ----- La: " << "(" << La[0] << "," << La[1] << "," << La[2] << ")\n";
    	cout << "3) Light intensity ----- Li: " << "(" << Li[0] << "," << Li[1] << "," << Li[2] << ")\n";
    	cout << "4) Material(Object) 0 ----- Ka: " << "(" << ka[0][0] << "," << ka[0][1] << "," << ka[0][2] << ")" << " Kd: " << "(" << kd[0][0] << "," << kd[0][1] << "," << kd[0][2] << ")" << " Ks: "<< "(" << ks[0][0] << "," << ks[0][1] << "," << ks[0][2] << ")" << " n: " << n[0] << endl;
    	cout << "5) Material(Object) 1 ----- Ka: " << "(" << ka[1][0] << "," << ka[1][1] << "," << ka[1][2] << ")" << " Kd: " << "(" << kd[1][0] << "," << kd[1][1] << "," << kd[1][2] << ")" << " Ks: "<< "(" << ks[1][0] << "," << ks[1][1] << "," << ks[1][2] << ")" << " n: " << n[1] << endl;
    	cout << "6) From point ----- ff: " << "(" << ff[0] << "," << ff[1] << "," << ff[2] << ")\n";
    	cout << "7) Up vector ----- up: " << "(" << up[0] << "," << up[1] << "," << up[2] << ")\n";
    	cout << "8) At point ----- at: " << "(" << at[0] << "," << at[1] << "," << at[2] << ")\n";
    	cout << "9) Viewing angle ----- alpha: " << angle << endl;
    	cout << "10) Resolution ------ N: " << N;
    	cout << "\n\n--> Change option: ";
    	string in;
    	cin >> in;
    	int option = atoi(in.c_str());
    	if(option == 1){
    		string input;
    		cout << "x: ";
    		cin >> input;
    		float x = atof(input.c_str());
    		cout << "y: ";
    		cin >> input;
    		float y = atof(input.c_str());
    		cout << "z: ";
    		cin >> input;
    		float z = atof(input.c_str());
    		X = make_point(x,y,z);
    	}
    	else if(option == 2){
    		string input;
    		cout << "r: ";
    		cin >> input;
    		float r = atof(input.c_str());
    		cout << "g: ";
    		cin >> input;
    		float g = atof(input.c_str());
    		cout << "b: ";
    		cin >> input;
    		float b = atof(input.c_str());
    		La = make_point(r,g,b);
    	}
    	else if(option == 3){
    		string input;
    		cout << "r: ";
    		cin >> input;
    		float r = atof(input.c_str());
    		cout << "g: ";
    		cin >> input;
    		float g = atof(input.c_str());
    		cout << "b: ";
    		cin >> input;
    		float b = atof(input.c_str());
    		Li = make_point(r,g,b);
    	}
    	else if(option == 4){
    		string input;
    		cout << "which coefficient? 1)Ka 2)Kd 3)Ks : ";
    		cin >> input;
    		int which = atoi(input.c_str());
    		cout << "r: ";
    		cin >> input;
    		float r = atof(input.c_str());
    		cout << "g: ";
    		cin >> input;
    		float g = atof(input.c_str());
    		cout << "b: ";
    		cin >> input;
    		float b = atof(input.c_str());
    		if(which == 1){
    			ka[0] = make_point(r,g,b);
    		}
    		else if(which == 2){
    			kd[0] = make_point(r,g,b);
    		}
    		else{
    			ks[0] = make_point(r,g,b);
    		}
    	}
    	else if(option == 5){
    		string input;
    		cout << "which coefficient? 1)Ka 2)Kd 3)Ks : ";
    		cin >> input;
    		float which = atoi(input.c_str());
    		cout << "r: ";
    		cin >> input;
    		float r = atof(input.c_str());
    		cout << "g: ";
    		cin >> input;
    		float g = atof(input.c_str());
    		cout << "b: ";
    		cin >> input;
    		float b = atof(input.c_str());
    		if(which == 1){
    			ka[1] = make_point(r,g,b);
    		}
    		else if(which == 2){
    			kd[1] = make_point(r,g,b);
    		}
    		else{
    			ks[1] = make_point(r,g,b);
    		}
    	}
    	else if(option == 6){
    		string input;
    		cout << "x: ";
    		cin >> input;
    		float x = atof(input.c_str());
    		cout << "y: ";
    		cin >> input;
    		float y = atof(input.c_str());
    		cout << "z: ";
    		cin >> input;
    		float z = atof(input.c_str());
    		ff = make_point(x,y,z);
    	}
    	else if(option == 7){
    		string input;
    		cout << "x: ";
    		cin >> input;
    		float x = atof(input.c_str());
    		cout << "y: ";
    		cin >> input;
    		float y = atof(input.c_str());
    		cout << "z: ";
    		cin >> input;
    		float z = atof(input.c_str());
    		up = make_point(x,y,z);
    	}
    	else if(option == 8){
    		string input;
    		cout << "x: ";
    		cin >> input;
    		float x = atof(input.c_str());
    		cout << "y: ";
    		cin >> input;
    		float y = atof(input.c_str());
    		cout << "z: ";
    		cin >> input;
    		float z = atof(input.c_str());
    		at = make_point(x,y,z);
    	}
    	else if(option == 9){
    		string input;
    		cout << "angle: ";
    		cin >> input;
    		float this_angle = atof(input.c_str());
    		angle = this_angle;
    	}
    	else if(option == 10){
    		string input;
    		cout << "N: ";
    		cin >> input;
    	}
    	load_data(ff,up,at,angle,X,K,La,Li,ka[0],kd[0],ks[0],ka[1],kd[1],ks[1],n[0],n[1]);
    	load_matrices();
    	get_intersect();
    	set_change = 0;
    }
    //blits the current opengl framebuffer on the screen
    glutSwapBuffers();
    //checks for opengl errors
    check();
}

void print_point(vector<float> p){
    cout << "(" << p[0] << "," << p[1] << "," << p[2] << ")" << endl;
}

void printing(){
	float max_r = 0;
	float max_g = 0;
	float max_b = 0;
	float max_r2 = 0;
	float max_g2 = 0;
	float max_b2 = 0;

	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			//cout << "int: " << intensities_r[i][j]<< " ";
			if(max_r < intensities_r[i][j])
				max_r = intensities_r[i][j];
			if(max_g < intensities_g[i][j])
				max_g = intensities_g[i][j];
			if(max_b < intensities_b[i][j])
				max_b = intensities_b[i][j];

			if(max_r2 < intensities2_r[i][j])
				max_r2 = intensities2_r[i][j];
			if(max_g2 < intensities2_g[i][j])
				max_g2 = intensities2_g[i][j];
			if(max_b2 < intensities2_b[i][j])
				max_b2 = intensities2_b[i][j];
		}
	}
	//cout << "max_r: " << max_r << "max_g:" << max_g <<"max_b: " << max_b<< endl;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			float I_r = intensities_r[i][j]/max_r;
			float I_g = intensities_g[i][j]/max_g;
			float I_b = intensities_b[i][j]/max_b;
			//cout << intensities_r[i][j] << " " << intensities_g[i][j] << " " << intensities_b[i][j] << endl;

			vector<float> this_point = make_point(I_r,I_g,I_b);
			draw_pix(i,j,this_point[0],this_point[1],this_point[2]);

			if(I_r == 0 && I_g == 0 && I_g == 0){
				float I_r2 = intensities2_r[i][j]/max_r2;
				float I_g2 = intensities2_g[i][j]/max_g2;
				float I_b2 = intensities2_b[i][j]/max_b2;
				vector<float> this_point2 = make_point(I_r2,I_g2,I_b2);
				draw_pix(i,j,this_point2[0],this_point2[1],this_point2[2]);
			}
		}
	}
}

void drawing(){
	//cout << "drawing" << endl;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			float temp_r;
			float temp_g;
			float temp_b;
			int material = 0;
			//cout << "here" << endl;
			if(intersect_matrix[i][j].empty()){
				temp_r = 0;
				temp_g = 0;
				temp_b = 0;
				continue;
			}
			vector<float> point = intersect_matrix[i][j];
			//print_point(point);
			float I_r,I_g,I_b;
			if(!isnan(point[0])){
				//cout << "i: " << i << " j: " << j << " ";
				float f_p_dis = get_magnitude(create_vector(point,ff));
				vector<float> light_vector = to_unit(create_vector(point,X));
				vector<float> view_vector = to_unit(create_vector(point,ff));
				vector<float> normal = normal_matrix[i][j];

				//get r(unit) 2((n dot l)*n) − l
	            float dot_product = get_dot_product(normal,light_vector);
	            vector<float> reflection_vector;
	            reflection_vector.push_back(2*dot_product*normal[0]-light_vector[0]);
	            reflection_vector.push_back(2*dot_product*normal[1]-light_vector[1]);
	            reflection_vector.push_back(2*dot_product*normal[2]-light_vector[2]);

	            float n_l = get_dot_product(normal,light_vector);
	            float r_v = get_dot_product(reflection_vector,view_vector);
	            //cout << "p: ";
	            //print_point(point);
	            //cout << ""

				I_r = ka[material][0]*La[0] + (Li[0]/(K + f_p_dis))*(kd[material][0]*n_l + ks[material][0]*pow(r_v,n[material]));
				I_g = ka[material][1]*La[1] + (Li[1]/(K + f_p_dis))*(kd[material][1]*n_l + ks[material][1]*pow(r_v,n[material]));
				I_b = ka[material][2]*La[2] + (Li[2]/(K + f_p_dis))*(kd[material][2]*n_l + ks[material][2]*pow(r_v,n[material]));
				//vector<float> this_point = make_point(I_r,I_g,I_b);
				//draw_pix(i,j,this_point[0],this_point[1],this_point[2]);
				//cout << I_r << " " << I_g << " " << I_b << endl;
				temp_r = I_r;
				temp_g = I_g;
				temp_b = I_b;
			}
			else{
				temp_r = 0;
				temp_g = 0;
				temp_b = 0;
			}
			intensities_r[i][j] = temp_r;
			intensities_g[i][j] = temp_g;
			intensities_b[i][j] = temp_b;
		}
	}

	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			float temp_r;
			float temp_g;
			float temp_b;
			int material = 1;
			//cout << "here" << endl;

			if(intersect_matrix2[i][j].empty()){
				temp_r = 0;
				temp_g = 0;
				temp_b = 0;
				continue;
			}
			vector<float> point = intersect_matrix2[i][j];
			float I_r,I_g,I_b;
			//print_point(point);
			if(!isnan(point[0])){
				//cout << "i: " << i << " j: " << j << " ";
				float f_p_dis = get_magnitude(create_vector(point,ff));
				vector<float> light_vector = to_unit(create_vector(point,X));
				vector<float> view_vector = to_unit(create_vector(point,ff));
				vector<float> normal = normal_matrix2[i][j];

				//get r(unit) 2((n dot l)*n) − l
	            float dot_product = get_dot_product(normal,light_vector);
	            vector<float> reflection_vector;
	            reflection_vector.push_back(2*dot_product*normal[0]-light_vector[0]);
	            reflection_vector.push_back(2*dot_product*normal[1]-light_vector[1]);
	            reflection_vector.push_back(2*dot_product*normal[2]-light_vector[2]);

	            float n_l = get_dot_product(normal,light_vector);
	            float r_v = get_dot_product(reflection_vector,view_vector);
	            //cout << "p: ";
	            //print_point(point);
	            //cout << ""

				I_r = ka[material][0]*La[0] + (Li[0]/(K + f_p_dis))*(kd[material][0]*n_l + ks[material][0]*pow(r_v,n[material]));
				I_g = ka[material][1]*La[1] + (Li[1]/(K + f_p_dis))*(kd[material][1]*n_l + ks[material][1]*pow(r_v,n[material]));
				I_b = ka[material][2]*La[2] + (Li[2]/(K + f_p_dis))*(kd[material][2]*n_l + ks[material][2]*pow(r_v,n[material]));
				//vector<float> this_point = make_point(I_r,I_g,I_b);
				//draw_pix(i,j,this_point[0],this_point[1],this_point[2]);
				temp_r = I_r;
				temp_g = I_g;
				temp_b = I_b;
			}
			else{
				temp_r = 0;
				temp_g = 0;
				temp_b = 0;
			}
			intensities2_r[i][j] = temp_r;
			intensities2_g[i][j] = temp_g;
			intensities2_b[i][j] = temp_b;
		}
	}
}

void load_data(vector<float> ff_in, vector<float> up_in, vector<float> at_in, float angle_in, vector<float> X_in, float K_in, vector<float> La_in, vector<float> Li_in, vector<float> ka_in0, vector<float> kd_in0, vector<float> ks_in0, vector<float> ka_in1, vector<float> kd_in1, vector<float> ks_in1, int n_in0, int n_in1){
	/*
	ff[0] = 0;
	ff[1] = 0;
	ff[2] = 10;
	up[0] = 0;
	up[1] = 1;
	up[2] = 0;
	at[0] = 0;
	at[1] = 0;
	at[2] = 0;

	angle = 0.3;
	*/
	ff[0] = ff_in[0];
	ff[1] = ff_in[1];
	ff[2] = ff_in[2];

	up[0] = up_in[0];
	up[1] = up_in[1];
	up[2] = up_in[2];

	at[0] = at_in[0];
	at[1] = at_in[1];
	at[2] = at_in[2];

	angle = angle_in;

	vector<float> temp1 = create_vector(ff,at);
	b3 = to_unit(temp1);
	vector<float> temp2 = get_cross_product(b3,up);
	b1 = to_unit(temp2);
	b2 = to_unit(get_cross_product(b1,b3));
	vector<float> temp3 = create_vector(at,ff);
	fx = temp3[0];
	fy = temp3[1];
	fz = temp3[2];
	
	/*
	cout << "b1: " << b1[0]<< " " << b1[1] << " " << b1[2] << endl;
	cout << "b2: " << b2[0]<< " " << b2[1] << " " << b2[2] << endl;
	cout << "b3: " << b3[0]<< " " << b3[1] << " " << b3[2] << endl;
	cout << "fx:"  << fx << " fy:" << fy << " fz:"<< fz<< endl;
	*/

	/*
	//light
	X = make_point(2,2,2);
	K = 3.4641;
	La = make_point(.5,.5,.5);
	Li = make_point(.7,.7,.7);

	//obj_material
	ka.push_back(make_point(0.1,0.2,0.1));
	kd.push_back(make_point(0.2,0.4,0.1));
	ks.push_back(make_point(0.7,0.8,0.7));
	n.push_back(5);

	ka.push_back(make_point(0.2,0.2,0.2));
	kd.push_back(make_point(0.4,0.4,0.4));
	ks.push_back(make_point(0.9,0.9,0.9));
	n.push_back(20);
	*/
	X = X_in;
	K = K_in;
	La = La_in;
	Li = Li_in;

	ka[0] = ka_in0;
	kd[0] = kd_in0;
	ks[0] = ks_in0;
	n[0] = n_in0;

	ka[1] = ka_in1;
	kd[1] = kd_in1;
	ks[1] = ks_in1;
	n[1] = n_in1;
}

void load_matrices(){
	//eye_location_matrix.clear();
	for(int i = 0; i < N; i++){
		vector<vector<float> > temp_row;
		vector<vector<float> > empty_row;
		vector<float> empty;
		for(int j = 0; j < N; j++){
			vector<float> temp_col;
			vector<float> empty_col;
			//add x, y, z
			float x_eye = ((float)i)/(N-1)-0.5;
			float y_eye = ((float)j)/(N-1)-0.5;
			float z_eye = (float)(1/(tan((double)angle)));

			vector<float> temp_point = make_point(x_eye,y_eye,z_eye);
			temp_col = eye_to_world(temp_point);
			temp_row.push_back(temp_col);
			empty_row.push_back(empty_col);
			empty.push_back(0);
		}
		world_location_matrix.push_back(temp_row);
		intersect_matrix.push_back(empty_row);
		intersect_matrix2.push_back(empty_row);
		normal_matrix.push_back(empty_row);
		normal_matrix2.push_back(empty_row);

		intensities_r.push_back(empty);
		intensities_g.push_back(empty);
		intensities_b.push_back(empty);
		intensities2_r.push_back(empty);
		intensities2_g.push_back(empty);
		intensities2_b.push_back(empty);
	}

	/*
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			cout << world_location_matrix[i][j][0] << "," << world_location_matrix[i][j][1] << "," << world_location_matrix[i][j][2] << "-";
		}
		cout << endl;
	}
	*/
}

void draw_pix(int x, int y, float R, float G, float B){
    glBegin(GL_POINTS);
    glColor3f(R,G,B);
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
    glutAddMenuEntry("--change parameters--",CHANGE);
    glutAddMenuEntry("--Quit--",QUIT);

    // attach the menu to the right button
    glutAttachMenu(GLUT_RIGHT_BUTTON);
}

void processMenuEvents(int option) {

    switch (option) {
        case QUIT :
            set_quit = 1;
            break;
        case CHANGE :
	        set_change = 1;
	        break;
    }
}


//called repeatedly when glut isn't doing anything else
void idle()
{
    //redraw the scene over and over again
    glutPostRedisplay();    
}

void get_intersect(){
	vector<float> C = make_point(0,0,0);
	vector<float> O = ff;
	vector<float> C2 = make_point(1,1,1);
	vector<float> L = create_vector(O,C);
	vector<float> L2 = create_vector(O,C2);
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			vector<float> location = world_location_matrix[i][j];
			vector<float> D = to_unit(create_vector(ff,location));

			float Tca = get_dot_product(L,D);
			if(Tca < 0)
				continue;
			float d = sqrt(get_dot_product(L,L)-pow(Tca,2));
			float d_sqr = get_dot_product(L,L)-pow(Tca,2);
			//cout << "d: " << d<<endl;
			if(d_sqr < 0)
				continue;

			if(d > circle_R){
				//cout <<"here";
				continue;
			}
			float Thc = sqrt(pow(circle_R,2)-d_sqr);
			float t0 = Tca - Thc;
			float t1 = Tca + Thc;

			//cout << "t0: " << t0<< endl;
			//print_point(D);
			float x = ff[0] + t0*D[0];
			float y = ff[1] + t0*D[1];
			float z = ff[2] + t0*D[2];

			vector<float> intersect = make_point(x,y,z);
			//print_point(intersect);
			vector<float> normal = create_vector(C,intersect);
			normal = to_unit(normal);

			intersect_matrix[i][j] = intersect;
			normal_matrix[i][j] = normal;
		}
	}

	//cout << "here" << endl;
	for(int i = 0; i < N; i++){
		for(int j = 0; j < N; j++){
			vector<float> location = world_location_matrix[i][j];
			vector<float> D = to_unit(create_vector(ff,location));

			float Tca = get_dot_product(L2,D);
			if(Tca < 0)
				continue;
			float d = sqrt(get_dot_product(L2,L2)-pow(Tca,2));
			float d_sqr = get_dot_product(L2,L2)-pow(Tca,2);
			//cout << "d: " << d<<endl;
			if(d_sqr < 0)
				continue;

			if(d > circle_R2){
				//cout <<"here";
				continue;
			}
			float Thc = sqrt(pow(circle_R2,2)-d_sqr);
			float t0 = Tca - Thc;
			float t1 = Tca + Thc;

			float x = ff[0] + t0*D[0];
			float y = ff[1] + t0*D[1];
			float z = ff[2] + t0*D[2];
			vector<float> intersect = make_point(x,y,z);
			//print_point(intersect);
			vector<float> normal = create_vector(C2,intersect);
			normal = to_unit(normal);
			//print_point(normal);
			intersect_matrix2[i][j] = intersect;
			normal_matrix2[i][j] = normal;
		}
	}

}

float abs_val(float value){
    if(value < 0)
        return value*(-1);
    else
        return value;
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


vector<float> scale_vector(vector<float> v, float scal){
	vector<float> temp;
	temp.push_back(scal*v[0]);
	temp.push_back(scal*v[1]);
	temp.push_back(scal*v[2]);
	return temp;
}

float get_magnitude(vector<float> v){
    float sum = 0;
    for(int i = 0; i < v.size(); i++)
        sum += pow(v[i],2);
    return sqrt(sum);
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

vector<float> eye_to_world(vector<float> point){
	float new_x = b1[0]*point[0] + b2[0]*point[1] + b3[0]*point[2];
	float new_y = b1[1]*point[0] + b2[1]*point[1] + b3[1]*point[2];
	float new_z = b1[2]*point[0] + b2[2]*point[1] + b3[2]*point[2];
	vector<float> new_point = make_point(new_x+fx,new_y+fy,new_z+fz);

	return new_point;
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


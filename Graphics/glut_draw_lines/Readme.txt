Menu: right click on the interface
1) choose which polyhedron to operate on
2) set scaling factor
3) set translation vector
4) set rotation vector (rotation direction according to right hand rule) and rotation angle
5) projected view on XY plane
6) projected view on XZ plane
7) projected view on YZ plane
8) reset all points

Terminal:
1) Takes filename
2) Takes user inputs for the chosen operations (from menu)
3) Show vertex coordinates of the chosen polyhedron (before and after each transformation)

Note:
The program assumes all inputs are in valid format (numbers).
The rotation axis will appear and stay on the screen after it is specified
The rotation handles all special cases (cases for slopes equaling 0: delta_x, delta_y, delta_z; 6 cases in total excluding the case where the line becomes a point).
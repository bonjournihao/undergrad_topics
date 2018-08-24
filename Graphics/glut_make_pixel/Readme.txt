—- “Make” for makefile
-— ./transform2d for execution
-- The program loads 3 polygons from 3 files instead of 1 file
(polygon0.data  polygon1.data  polygon2.data)
-- Right mouse click for menu display
-- After clicking on the menu, return to terminal for further instructions
-- Right now, the clipping function does clipping on 1 polygon even when polygons overlap. But if you wanted the 3 polygons to be clipped all together, I could call the clipping function 3 times for each polygons to change the scene

NOTE: only deals with polygons whose inner angles are less than 180 degrees
NOTE: the order of vertices needs to be clockwise
NOTE: assumes user enters valid(number) inputs

Menu:
1) show VERTICES 	(print vertices in the window)
2) DDA 			(draw lines using DDA algorithm)
3) BRES 		(draw lines using Bresenham algorithm)
4) Rasterize 		(rasterize polygons)

5) --Choose polygon 0/1/2--

6) Clip 				(terminal)
7) Set viewpoint 			(terminal)
8) Set scaling factor			(terminal) 
9) Set translation vector 		(terminal)
10) Set rotation angle (in degrees) 	(terminal)
11) --Quit-- (exit and save points)
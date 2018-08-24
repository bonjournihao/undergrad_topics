Notes:
--"make" for makefile
--"./Curves" for execution
--Default: click on the screen to add points to the first curve
--The program assumes that the inputs are valid numerical values

Menu: right click on the interface
1) Choose a curve    
---Use terminal for inputs

2) Add points        
---Click any where on the screen to add a point

3) Connect points 	 
---Disconnect or connect the line segments; connected by default

4) Delete points     
---Click on a point to delete
---If a point was not sucessfully clicked, you will be notified in the terminal

5) Insert points     
---Click on a leading point to insert a point
---If a point was not sucessfully clicked, you will be notified in the terminal

6) Move points       
---Click on a point and drag
---Result will only show after you finish the dragging
---If a point was not sucessfully clicked, you will be notified in the terminal

7) --Bazier--
---Draw the current curve using Bazier

8) --B-spline-- 
---Draw the current curve using B-spline

9) ----B-spline-changes----
---Make Bspline parameter changes to the current curve: order/knots positions
---Use terminal for inputs
---if the current line is not drawn using B-spline, you will be notified in the terminal

10) Add a new curve
---Add a new curve to the scene
---By default, you will be in "Add points" mode

11) Quit
---Exit the program
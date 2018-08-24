import java.awt.Point;
import java.util.ArrayList;
import java.util.List;


public class Astar implements AIModule
{
	static double getHeuristic(final TerrainMap map, final Point pt1, final Point pt2) {
	    double h1 = map.getTile(pt1);
        double h2 = map.getTile(pt2);
        double h_diff = Math.abs(h2-h1);
        double dis = Math.sqrt(Math.pow(2, pt2.y-pt1.y) + Math.pow(2, pt2.x-pt1.x));

        return 10*h_diff/dis;
    }
    static ArrayList<Cell> open;
    static List<Cell> closed;
    static int startI, startJ;
    static int endI, endJ;
    static Cell startNode;
    static Point endPoint;


    public List<Point> createPath(final TerrainMap map){
        open = new ArrayList();
        closed = new ArrayList();
        endPoint = map.getEndPoint();

        final List<Point> path = new ArrayList<>();
        startI = map.getStartPoint().x;
        startJ = map.getStartPoint().y;
        endI = map.getEndPoint().x;
        endJ = map.getEndPoint().y;

        startNode = new Cell(startI,startJ);

        Astar(map);
        System.out.print("completed Astar");

        path.add(new Point(closed.get(0).i, closed.get(0).j));
        Cell parentNode = closed.get(0).parent;
        while(parentNode != null){
            path.add(0,new Point(parentNode.i, parentNode.j));
            Cell temp;
            temp = parentNode.parent;
            parentNode = temp;
        }
        return path;
    }

    static class Cell{

        double heuristicCost = 0;
        double cost = 0;
        double finalCost = 0; //G+H
        int i, j;
        Cell parent;

        Cell(int i, int j){
            this.i = i;
            this.j = j;
        }
    }

    static void checkAndUpdateCost(final TerrainMap map, Cell current, Cell t, double costSoFar){
        //if t is null, skip
        if(t == null)
            return;

        Point temp = new Point(t.i, t.j);
        t.heuristicCost = getHeuristic(map, temp, endPoint);
        double t_final_cost = t.heuristicCost+costSoFar;


        //check if t in open
        boolean inOpen = false;
        int k = 0;
        //System.out.format("t.i: %d,  t.j: %d, t_final_cost: %f \n", t.i, t.j, t_final_cost);
        for(; k < open.size(); k++){
            //System.out.format("k.i: %d,  k.j: %d \n", open.get(k).i, open.get(k).j);
            if(open.get(k).i == t.i && open.get(k).j == t.j)
            {
                //System.out.format("in open\n");
                inOpen = true;
                break;
            }
        }

        //if t not in openList or we have a cheaper path to t
        if(!inOpen || (inOpen && t_final_cost<open.get(k).finalCost)){
            //System.out.format("t.i: %d,  t.j: %d, t_final_cost: %f \n", t.i, t.j, t_final_cost);
            t.finalCost = t_final_cost;
            t.parent = current;
            if(!inOpen)
            {
                open.add(t);
                //System.out.format("t.i: %d,  t.j: %d, t_final_cost: %f \n", t.i, t.j, t_final_cost);
            }
            else{
                open.get(k).finalCost = t_final_cost;
                open.get(k).parent = current;
            }
        }
    }

    public static boolean inClosed(Cell a){
        for(int k = 0; k < closed.size(); k++){
            if (closed.get(k).i == a.i && closed.get(k).j == a.j)
                return true;
        }

        return false;
    }


    public static void Astar(final TerrainMap map){
        //add the start location to open list.
        open.add(startNode);

        while(true){
            //list empty
            if(open.isEmpty())
                break;

            //get node with min cost
            int k = 0;
            double min = open.get(0).finalCost;
            int min_i = 0;
            for(;k < open.size();k++){
                if(open.get(k).finalCost < min){
                    min = open.get(k).finalCost;
                    min_i = k;
                }
            }

            Cell current = new Cell(0,0);
            current.i = open.get(min_i).i;
            current.j = open.get(min_i).j;
            current.finalCost = open.get(min_i).finalCost;
            current.parent = open.get(min_i).parent;
            open.remove(min_i);


            //add current to close list
            closed.add(0,current);


            //found end node
            if(current.i == endI && current.j == endJ){
                System.out.format("current.i: %d,  current.j: %d, final_cost: %f \n", current.i, current.j, current.finalCost);
                return;
            }

            Cell t;
            Point temp1;
            Point temp2;
            if(current.i-1>=0){
                t = new Cell(0,0);
                t.i = current.i-1;
                t.j = current.j;
                if(!inClosed(t)){
                    temp1 = new Point();
                    temp2 = new Point();
                    temp1.x = current.i;
                    temp1.y = current.j;
                    temp2.x = t.i;
                    temp2.y = t.j;
                    //System.out.format("%d, %d\n",temp2.x, temp2.y);
                    t.cost = map.getCost(temp1, temp2);
                    checkAndUpdateCost(map, current, t, current.finalCost+t.cost);
                }

                if(current.j-1>=0){
                    t = new Cell(0,0);
                    t.i = current.i-1;
                    t.j = current.j-1;
                    if(!inClosed(t)){
                        temp1 = new Point();
                        temp2 = new Point();
                        temp1.x = current.i;
                        temp1.y = current.j;
                        temp2.x = t.i;
                        temp2.y = t.j;
                        t.cost = map.getCost(temp1, temp2);
                        checkAndUpdateCost(map, current, t, current.finalCost+t.cost);
                    }
                }

                if(current.j+1<map.getWidth()){
                    t = new Cell(0,0);
                    t.i = current.i-1;
                    t.j = current.j+1;
                    if(!inClosed(t)){
                        temp1 = new Point();
                        temp2 = new Point();
                        temp1.x = current.i;
                        temp1.y = current.j;
                        temp2.x = t.i;
                        temp2.y = t.j;
                        t.cost = map.getCost(temp1, temp2);
                        checkAndUpdateCost(map, current, t, current.finalCost+t.cost);
                    }
                }
            }

            if(current.j-1>=0){
                t = new Cell(0,0);
                t.i = current.i;
                t.j = current.j-1;
                if(!inClosed(t)){
                    temp1 = new Point();
                    temp2 = new Point();
                    temp1.x = current.i;
                    temp1.y = current.j;
                    temp2.x = t.i;
                    temp2.y = t.j;
                    t.cost = map.getCost(temp1, temp2);
                    checkAndUpdateCost(map, current, t, current.finalCost+t.cost);
                }
            }

            if(current.j+1<map.getWidth()){
                t = new Cell(0,0);
                t.i = current.i;
                t.j = current.j+1;
                if(!inClosed(t)){
                    temp1 = new Point();
                    temp2 = new Point();
                    temp1.x = current.i;
                    temp1.y = current.j;
                    temp2.x = t.i;
                    temp2.y = t.j;
                    t.cost = map.getCost(temp1, temp2);
                    checkAndUpdateCost(map, current, t, current.finalCost+t.cost);
                }
            }

            if(current.i+1<map.getHeight()){
                t = new Cell(0,0);
                t.i = current.i+1;
                t.j = current.j;
                if(!inClosed(t)){
                    temp1 = new Point();
                    temp2 = new Point();
                    temp1.x = current.i;
                    temp1.y = current.j;
                    temp2.x = t.i;
                    temp2.y = t.j;
                    t.cost = map.getCost(temp1, temp2);
                    checkAndUpdateCost(map, current, t, current.finalCost+t.cost);
                }

                if(current.j-1>=0){
                    t = new Cell(0,0);
                    t.i = current.i+1;
                    t.j = current.j-1;
                    if(!inClosed(t)){
                        temp1 = new Point();
                        temp2 = new Point();
                        temp1.x = current.i;
                        temp1.y = current.j;
                        temp2.x = t.i;
                        temp2.y = t.j;
                        t.cost = map.getCost(temp1, temp2);
                        checkAndUpdateCost(map, current, t, current.finalCost+t.cost);
                    }
                }

                if(current.j+1<map.getWidth()){
                    t = new Cell(0,0);
                    t.i = current.i+1;
                    t.j = current.j+1;
                    if(!inClosed(t)){
                        temp1 = new Point();
                        temp2 = new Point();
                        temp1.x = current.i;
                        temp1.y = current.j;
                        temp2.x = t.i;
                        temp2.y = t.j;
                        t.cost = map.getCost(temp1, temp2);
                        checkAndUpdateCost(map, current, t, current.finalCost+t.cost);
                    }
                }
            }
        }
    }
}
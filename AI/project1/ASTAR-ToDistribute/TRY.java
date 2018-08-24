import java.awt.Point;
import java.util.*;

public class TRY implements AIModule
{

  public List<Point> createPath(final TerrainMap map)
	{
		final ArrayList<Point> path = new ArrayList<Point>();
		Point CurrentPoint = map.getStartPoint();

		final HashMap<Point, Point> preceed = new HashMap<Point, Point>();
		final HashMap<Point, Double> distance = new HashMap<Point, Double>();
    final HashMap<Point, Double> heu = new HashMap<Point, Double>();

		preceed.put(map.getStartPoint(), null);
		distance.put(map.getStartPoint(), 0.0);
    heu.put(map.getStartPoint(), 0.5*Integer.MAX_VALUE);

    PriorityQueue<Point> Q = new PriorityQueue<Point>
    (
    		new Comparator<Point>()
    		{
                public int compare(Point n1, Point n2)
                {
                  return heu.get(n1) > heu.get(n2) ? 1 : -1;
                }
    		}

    );
//System.out.println("HERE0");
		Q.add(map.getStartPoint());
//System.out.println("HERE");
		while (!Q.isEmpty())
		{
			CurrentPoint = Q.poll();

			if ( CurrentPoint.equals(map.getEndPoint()))
					{break;}

			Point[] next = map.getNeighbors(CurrentPoint);

			for (int i=0; i < next.length; i++)
			{
				double tentative_g = distance.get(CurrentPoint) + map.getCost(CurrentPoint, next[i]);

				if (!distance.containsKey(next[i]))
        {
          distance.put(next[i], tentative_g);
          preceed.put(next[i], CurrentPoint);
          heu.put(next[i],tentative_g+getHeuristic(map,CurrentPoint,next[i]));
          Q.add(next[i]);
        }
        else if(tentative_g < distance.get(next[i]))
        {
          distance.put(next[i], tentative_g);
          preceed.put(next[i], CurrentPoint);
          heu.put(next[i],tentative_g+getHeuristic(map,CurrentPoint,next[i]));
          Q.add(next[i]);
        }
      }
    }

    CurrentPoint = map.getEndPoint();
    path.add(CurrentPoint);
  //  System.out.println("here from#:"+from.size());
    while (!CurrentPoint.equals(map.getStartPoint()))
    {
      CurrentPoint = preceed.get(CurrentPoint);
      path.add(0, CurrentPoint);
      //System.out.println("here path#:"+path.size());
    }
  //  System.out.println("hereend");
    return path;
  }



  public  static double getHeuristic(final TerrainMap map, final Point pt1,final Point pt2)
  {
    //double gn = map.getCost(pt1,pt2);
    double hn = 0;
    double htdif = map.getTile(map.getEndPoint()) - map.getTile(pt2);
    int dis = ashdistance(map,pt2);
    double tmp = htdif;
    double remaining_steps;

    if (htdif==0){
      hn = dis;
    }
    else{ //htdif <> 06
      if(Math.abs(htdif) > dis){ // |htdif| > dis
        hn = dis*Math.pow(2,tmp/dis);
      }
      else{ //|htdif| < dis
        remaining_steps = dis - Math.abs(htdif);
        if (htdif > 0)
          hn = remaining_steps + Math.abs(htdif)*Math.pow(2,1);
        else
          hn = remaining_steps + Math.abs(htdif)*Math.pow(2,-1);
      }
    }
    return hn;
  }

  public static int ashdistance(TerrainMap map,Point pt)
  {
    int x2 = map.getEndPoint().x;
    int y2 = map.getEndPoint().y;
    int x1 = pt.x;
    int y1 = pt.y;
    int dis1 = Math.max(Math.abs(x1-x2),Math.abs(y1-y2));
    int dis2 = Math.abs(x1-x2)+Math.abs(y1-y2);
    return dis1;
  }
}

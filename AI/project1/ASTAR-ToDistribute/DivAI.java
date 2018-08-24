import java.awt.Point;
import java.util.*;

public class DivAI implements AIModule
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
				double tentative_g = distance.get(CurrentPoint) + map.getCost1(CurrentPoint, next[i]);

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
   /*if (htdif==0)
    {hn = 0.5*dis;}
    else if (htdif>0)
    {
      hn = 0.5*(dis-1);
    }
    else if (htdif<0)
    {
      double avg = htdif/dis;
      double tmp = htdif;
      for(int i  = dis;i>0;i--)
      {
        hn+=tmp/(avg+1);
        tmp-=avg;
      }

    //  htdif = Math.sqrt(Math.pow(map.getEndPoint().x-pt2.x,2)+Math.pow(map.getEndPoint().y-pt2.y,2));
  }*/

     //hn = 0.5*Math.sqrt(Math.pow(map.getEndPoint().x-pt2.x,2)+Math.pow(map.getEndPoint().y-pt2.y,2));

  //  double fn = gn+hn;

  //if(dis==1){return map.getTile(pt2)/(map.getTile(map.getEndPoint())+1);}

    /*
    if (htdif == 0)
      hn = 0.5*dis;
    else{
      int k = 0;
      double cost_sum = 0;
      double h_0 = map.getTile(pt2);
      double delta_h = htdif/dis;
      double h_t = map.getTile(map.getEndPoint());
      for(;k != dis; k++){
        cost_sum += (h_0+k*delta_h)/(h_0+(k+1)*delta_h+1);
      }
      hn = cost_sum;
    }
  

    if(dis==1){return map.getTile(pt2)/(map.getTile(map.getEndPoint())+1);}
    */
    return 0.5*(dis-2)+(1/(map.getTile(pt2)+1))+(map.getTile(map.getEndPoint())/2);

    //return hn;
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

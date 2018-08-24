import java.awt.Point;
import java.util.*;

public class AStarExp_999200555_912143385 implements AIModule
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
    if (htdif==0){
      if(map.getTile(pt2) != 0){
        hn = dis;
      }
      else{
        hn = 2 + (dis-2) + 0.5;
      }
    }
    else if(dis==1)
    {hn = Math.pow(2,htdif);}
    else
    {
      if(Math.abs(htdif)>dis){hn = dis*Math.pow(2,htdif/dis);}
      else
      {
        double remain = dis-Math.abs(htdif);
        if (remain<0) {remain=0;}
        if(htdif>0)
        {
          hn = remain+Math.abs(htdif)*2;
        }
        else
        {
          hn = remain+Math.abs(htdif)*0.5;
        }
      }
    }
    /*else if (htdif<0)
    {
      double tmp = htdif;
      hn = dis*Math.pow(2,tmp/dis);
    }
    else
    {

      double tmp = htdif;
      hn = dis*Math.pow(2,tmp/dis);
      if(dis<htdif){hn = 2*htdif;}
    //  htdif = Math.sqrt(Math.pow(map.getEndPoint().x-pt2.x,2)+Math.pow(map.getEndPoint().y-pt2.y,2));
    }
*/
     //hn = 0.5*Math.sqrt(Math.pow(map.getEndPoint().x-pt2.x,2)+Math.pow(map.getEndPoint().y-pt2.y,2));

  //  double fn = gn+hn;
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

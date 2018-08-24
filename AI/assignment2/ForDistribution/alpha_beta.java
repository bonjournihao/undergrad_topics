import java.util.*;
import java.awt.Point;
import java.lang.Comparable;

public class alpha_beta extends AIModule
{
    private int player;
    public class move_value {
        public int move;
        public int value = -1;

        public move_value(int m, int v) {
            move = m;
            value = v;
        }
        /*
        @Override
        public int compareTo(move_value temp) {
            if(this.value <= temp.value)
                return -1;
            else
                return 1;
        }
        */
    }

    class moveComparator implements Comparator<move_value> {
        @Override
        public int compare(move_value o1, move_value o2) {
            if (o1.value > o2.value) {
                return 1;
            }
            if (o1.value < o2.value) {
                return -1;
            }
            return 0;
        }
    }

    private static int[][] evaluationTable = {{3, 4, 5, 7, 5, 4, 3},
            {4, 6, 8, 10, 8, 6, 4},
            {5, 8, 11, 13, 11, 8, 5},
            {5, 8, 11, 13, 11, 8, 5},
            {4, 6, 8, 10, 8, 6, 4},
            {3, 4, 5, 7, 5, 4, 3}};

    private int[] sort_moves(final GameStateModule state, final int[] moves, int parent){
        int[] temp_moves = new int[moves.length];

        List<move_value> temp_list = new ArrayList<>();

        for(int i = 0; i < moves.length; i++){
            if (state.canMakeMove(moves[i])) {
                state.makeMove(moves[i]);
                //move_value temp = new move_value(moves[i], parent+computeScore(state));
                move_value temp = new move_value(moves[i], parent+computeScore2(state));
                temp_list.add(temp);
                state.unMakeMove();
            }
        }

        moveComparator comparator= new moveComparator();
        Collections.sort(temp_list, comparator);

        //Arrays.sort(temp_list);
        for(int i = 0; i < temp_list.size(); i++)
            temp_moves[i] = temp_list.get(i).move;

        return temp_moves;
    }


    private int[] reverse_moves(final int[] moves){
        int[] temp_moves = new int[moves.length];
        temp_moves[3] = moves[3];
        int temp;

        int i;
        for(i = 0; i < moves.length / 2; i++)
        {
            temp = moves[i];
            temp_moves[i] = moves[moves.length - i - 1];
            temp_moves[moves.length - i - 1] = temp;
        }
        return temp_moves;
    }

    private int[] generateMoves_min(final GameStateModule state, int parent)
    {
        int i;
        int[] temp_moves;
        int[] moves = new int[state.getWidth()];

        for(i = 0; i < state.getWidth(); ++i)
        {
            if(state.canMakeMove(i))
            {
                moves[i] = i; // play column i
            }
            else
            {
                moves[i] = -1; //move invalid
            }
        }

        temp_moves = sort_moves(state, moves, parent);
        return temp_moves;
    }//generateMoves

     private int[] generateMoves_max(final GameStateModule state, int parent)
    {
        int i;
        int[] moves = new int[state.getWidth()];
        int[] temp_moves;

        for(i = 0; i < state.getWidth(); ++i)
        {
            if(state.canMakeMove(i))
            {
                moves[i] = i;
            }
            else
            {
                moves[i] = -1;
            }
        }

        //descending sorting for max player
        temp_moves = sort_moves(state, moves, parent);
        temp_moves = reverse_moves(temp_moves);
        
        return temp_moves;
    }

    public void getNextMove(final GameStateModule state)
    {
        int max, prevMax = Integer.MIN_VALUE;
        int tempCol = -1, col = -1;
        int a = Integer.MIN_VALUE, b = Integer.MAX_VALUE;

        //set player
        player = state.getCoins() % 2 + 1;

        int score = initFirstStateScore( state );
        int[] moves = generateMoves_max(state, 0);
        for(int move : moves)
        {
            if(state.canMakeMove(move))//if  move valid
            {
                if(terminate == true)
                {
                    System.out.print("alpha beta terminate\n");
                    return;
                }

                state.makeMove(move);
                max = MinVal(state, 1, score, a, b);

                if(max > prevMax)
                {
                    prevMax = max;
                    tempCol = move;
                }//update better move

                state.unMakeMove();
            }
        }
        col = tempCol;
        chosenMove = col; //set move
    }

    private int MaxVal(final GameStateModule state, int depth, int parentScore, int a, int b)
    {
        int util = Integer.MIN_VALUE;
        int[] moves;
        if(state.isGameOver()) //terminal test
        {
            int winner = state.getWinner();

            if(winner == 0)//if draw
                return 1;
            else
                return Integer.MIN_VALUE + 1;
        }

        if( depth == 10) //max at depth
        {
            util = parentScore + computeScore( state );
            //util = parentScore + computeScore2( state );
            return util;
        }

        moves = generateMoves_max(state, parentScore);
        //int score = parentScore + computeScore( state );
        int score = parentScore + computeScore2( state );

        for(int move : moves)
        {
            if(terminate == true)
            {
                System.out.print("alpha beta terminate max\n");
                System.out.print( depth + "\n");
                break;
            }

            if(state.canMakeMove(move))//if the move is valid
            {
                state.makeMove(move);
                util = Math.max(util, MinVal(state, depth + 1, score, a, b));
                state.unMakeMove();

                if(util >= b)
                    return util;

                if (a < util)
                    a = util;
            }
        }
        return util;
    }

    private int MinVal(final GameStateModule state, int depth, int parentScore, int a, int b)
    {
        int util = Integer.MAX_VALUE;
        int[] moves;
        if(state.isGameOver()) //terminal test
        {
            int winner = state.getWinner();

            if(winner == 0)//if draw
                return 1;
            else
                return Integer.MAX_VALUE;
        }

        if(depth == 9) //min at max depth
        {
            //util = parentScore + computeScore( state );
            util = parentScore + computeScore2( state );
            return util;
        }

        moves = generateMoves_min(state, parentScore);
        //int score = parentScore + computeScore( state );
        int score = parentScore + computeScore2( state );

        for(int move : moves)
        {
            if(terminate == true)
            {
                System.out.print("alpha beta terminate min\n");
                break;
            }

            if(state.canMakeMove(move))//if the move is valid try playing it
            {
                state.makeMove(move);
                util = Math.min(util, MaxVal(state, depth + 1, score, a, b));
                state.unMakeMove();

                if(util <= a)
                    return util;

                if (b > util)
                    b = util;
            }
        }
        return util;
    }//MaxVal

    private int initFirstStateScore( final GameStateModule state ){
        //return computeScore(state);
        return computeScore2(state);
    }

    private int computeScore( final GameStateModule state ){
        int width = state.getWidth(), height = state.getHeight();
        int multiplier = 1;
        int score = 100;
        for(int i = 0 ; i < width; i++){
            for(int j = 0; j < height; j++){
                if(state.getAt(i,j) == player || state.getAt(i,j) == state.getCoins() % 2 + 1){
                    if (state.getAt(i,j) == state.getCoins() % 2 + 1)
                        multiplier = -1;
                    //corner
                    if(((i ==0)&&(j==0))||((i==0)&&(j==height-1))||((i==width-1)&&(j==0))||((i==width-1)&&(j==height-1)))
                        score += 1*multiplier;
                    //edge
                    else if(i == 0 || i == width - 1 || j == 0 || j == height - 1)
                        score += 2*multiplier;
                    //center
                    else 
                        score += 7*multiplier;
                }
            }
        }
        if (score < 0)
            score = 0;
        return score;
    }
    private int computeScore2( final GameStateModule state ){
        int score = 100;
        for (int i = 0; i < 6; i++){
            for (int j = 0; j < 5; j++){
                if(state.getAt(i,j) == player)
                    score += evaluationTable[j][i];
                else if(state.getAt(i,j) == player % 2 + 1)
                    score -= evaluationTable[j][i];
            }
        }
        if (score < 0)
            return 0;
        return score;
    }
}


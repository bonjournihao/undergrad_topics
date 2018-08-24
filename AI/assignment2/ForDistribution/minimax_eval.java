import java.util.*;
import java.awt.Point;

public class minimax_eval extends AIModule
{
    private int maxPly = 2; //maximum number of moves to play ahead

    private int prevMoveX, prevMoveY;
    private int player;

    //Hold points value
    //We can experiment with these values
    //1 coin of ourPlayer with 3 blank spot  +x
    //2 coin of ourPlayer with 2 blank spot  +x
    //3 coin of ourPlayer with 1 blank spot  +x
    //4 coin of ourPlayer with 0 blank spot  +x
    //Contains the negative value as well
    private static final int[] points = { 0, 10, 100, 500, 100000000 };
    private int[][] pointsCoin = {
        {  3,  4,  5,  7,  7,  5,  4,  3 },
        {  4,  6,  8, 10, 10,  8,  6,  4 },
        {  5,  8, 11, 13, 13, 11,  8,  5 },
        {  7, 10, 13, 16, 16, 13, 10,  7 },
        {  7, 10, 13, 16, 16, 13, 10,  7 },
        {  5,  8, 11, 13, 13, 11,  8,  5 },
        {  4,  6,  8, 10, 10,  8,  6,  4 },
        {  3,  4,  5,  7,  7,  5,  4,  3 } };

    //generates the moves that can be performed from the current state
    //as of right now it generates them in left to right order
    //returns an array of the columns that can be played in
    //each entry contains either a column number to play in that column
    //or a -1 to indicate that the move is invalid
    private int[] generateMoves(final GameStateModule state)
    {
        int i;
        int[] moves = new int[state.getWidth()]; //maybe this should be Integer instead of int?

        for(i = 0; i < state.getWidth(); ++i)
        {
            if(state.canMakeMove(i))
            {
                moves[i] = i; //can play column i
            }
            else
            {
                moves[i] = -1; //mark move as invalid
            }
        }

        return moves;
    }//generateMoves

    //start reading here
    public void getNextMove(final GameStateModule state) //think of this as minimax or alpah beta
    {
        int[] moves = generateMoves(state);//which column to play in
        int max, colToPlay, oldMax;
        max = oldMax = Integer.MIN_VALUE;
        colToPlay = -1;
        int tmpColToPlay = -1;

        //set player
        player = state.getCoins() % 2 + 1;

        int score = initFirstStateScore( state );
        /* System.out.println( "score: " + score ); */
        /* int s2 = cS( state ); */
        /* System.out.println(); */
        /* System.out.println(score); */
        /* System.out.println(); */
        /* System.out.println(s2); */

        //while( !terminate )
        //{
            max = oldMax = Integer.MIN_VALUE;
            for(int move : moves)
            {
                if(move != -1)//if the move is valid try playing it
                {
                    if(terminate == true)
                    {
                        System.out.print("terminate\n");
                        /* System.out.print( maxPly + " "); */
                        maxPly--;
                        return;
                    }

                    prevMoveX = move;
                    prevMoveY = state.getHeightAt( move );

                    state.makeMove(move);
                    max = MinVal(state, 1);

                    /* System.out.print( move + " " ); */
                    /* System.out.println(); */
                    /* System.out.println( "max: " + max + " old Max: " + oldMax ); */

                    if(max > oldMax)//if the new move was better than the old move that is the one we want to play
                    {
                        oldMax = max;
                        tmpColToPlay = move;
                    }//if the new move was better than the old move that is the one we want to play

                    state.unMakeMove();
                }//if the move is valid play it
            }
            maxPly++;
            colToPlay = tmpColToPlay;
            chosenMove = colToPlay; //set our move
        //}
    }

    private int MaxVal(final GameStateModule state, int curPly)
    {
        int util = Integer.MIN_VALUE;
        int[] moves;
        if(state.isGameOver()) //terminal test of the psuedo code
        {
            int winner = state.getWinner();

            if(winner == 0)//if the winner is 0 than there was a draw and return no reinforcement
                return 1;
            else
                return Integer.MIN_VALUE + 1;
        }

        //if our time has run out we need to return a utility function
        //right now I'm not sure if it would be better to calculate the utility function each time we reach a state or
        // only if we are told to terminate
        //right now we are calculating only if told to return
        if(curPly == maxPly || curPly == 8) //if our time has run out we need to return a utility function
        {
            util = computeScore( state );
            return util;
        }

        moves = generateMoves(state);
        int oldPrevMoveX, oldPrevMoveY;

        for(int move : moves)
        {
            if(terminate == true)
            {
                System.out.print("222terminate max\n");
                System.out.print( curPly + "\n");
                break;
            }//if we have run out of time


            if(move != -1)//if the move is valid try playing it
            {
                //save state
                oldPrevMoveX = prevMoveX;
                oldPrevMoveY = prevMoveY;

                prevMoveX = move;
                prevMoveY = state.getHeightAt( move );

                state.makeMove(move);

                util = Math.max(util, MinVal(state, curPly + 1));
                /* System.out.println(util + " " + curPly + " " + move); */
                state.unMakeMove();

                //revert back
                prevMoveX = oldPrevMoveX;
                prevMoveY = oldPrevMoveY;
            }//if the move is valid play it
        }//

        return util;
    }//MaxVal

    private int MinVal(final GameStateModule state, int curPly)
    {
        int util = Integer.MAX_VALUE;
        int[] moves;
        if(state.isGameOver()) //terminal test of the psuedo code
        {
            int winner = state.getWinner();

            if(winner == 0)//if the winner is 0 than there was a draw and return no reinforcement
                return 1;
            else
                return Integer.MAX_VALUE;
        }

        //if our time has run out we need to return a utility function
        //right now I'm not sure if it would be better to calculate the utility function each time we reach a state or
        // only if we are told to terminate
        //right now we are calculating only if told to return
        if(curPly == maxPly || curPly == 7) //if our time has run out we need to return a utility function
        {
            util = computeScore( state );
            return util;
        }

        moves = generateMoves(state);
        int oldPrevMoveX, oldPrevMoveY;

        for(int move : moves)
        {
            if(terminate == true)
            {
                System.out.print("222terminate min\n");
                //System.out.print( curPly + "\n");
                break;
            }//if we have run out of time

            if(move != -1)//if the move is valid try playing it
            {
                //save state
                oldPrevMoveX = prevMoveX;
                oldPrevMoveY = prevMoveY;

                prevMoveX = move;
                prevMoveY = state.getHeightAt( move );

                state.makeMove(move);

                util = Math.min(util, MaxVal(state, curPly + 1));

                state.unMakeMove();

                //revert back
                prevMoveX = oldPrevMoveX;
                prevMoveY = oldPrevMoveY;
            }//if the move is valid try playing it
        }//for each move play it

        return util;
    }//MaxVal

    
    private int initFirstStateScore( final GameStateModule state ){
        return computeScore(state);
    }

    private int computeScore( final GameStateModule state ){
        int width = state.getWidth(), height = state.getHeight();
        int score = 0;
        for(int i = 0 ; i < width; i++){
            for(int j = 0; j < height; j++){
                if(state.getAt(i,j) != 0){
                    //corner
                    if(((i ==0)&&(j==0))||((i==0)&&(j==height-1))||((i==width-1)&&(j==0))||((i==width-1)&&(j==height-1)))
                        score += 1;
                    //edge
                    else if(i == 0 || i == width - 1 || j == 0 || j == height - 1)
                        score += 2;
                    //center
                    else 
                        score += 7;
                }
            }
        }
        return score;
    }

}//minimax_WorldEnderH4G1


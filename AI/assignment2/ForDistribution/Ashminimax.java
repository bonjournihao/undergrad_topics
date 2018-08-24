/**
 * Created by Ash on 2017/2/26.
 */

import java.util.*;
import java.awt.Point;
import java.util.Random;
public class Ashminimax extends AIModule
{
    private int depth = 6;
    private final Random r = new Random(System.currentTimeMillis());

    public void getNextMove(final GameStateModule state)
    {
        chosenMove= ChooseMove(state,depth).column;
    }

    private mymove ChooseMove(final GameStateModule state, int depth)
    {
        int player = state.getActivePlayer();
        mymove bestmove = new mymove(-Integer.MAX_VALUE,0);
        mymove currentmove;

        for (int move = 0;move<state.getWidth();move++)
        {
            if(state.canMakeMove(move))
            {
                GameStateModule copy = state.copy();
                copy.makeMove(move);

                //Terminal Test
                if(copy.isGameOver()) //I terminate the game, so i will win with this move or making a draw move
                {
                        int winner = copy.getWinner();
                        if(winner==0)//draw
                        {
                            currentmove = new mymove(1, move);
                        }
                        else
                        {
                            currentmove = new mymove(Integer.MAX_VALUE, move);
                        }


                }
               /* else if(player ==copy.getActivePlayer())//is it my turn? MAX:MIN, should not be here for two player
                {
                    System.out.println("Same player after making a move");
                    currentmove = ChooseMove(copy,depth);
                    currentmove.column = move;
                }*/
                else if(depth>0)//it's not my turn, contrast minmax, decrease depth
                {
                    currentmove = ChooseMove(copy,depth-1);
                    currentmove.value = -1*currentmove.value; //contrast payoff for opponent
                    currentmove.column = move;
                }
                else//depth exhausted, using estimation
                {
                    currentmove = new mymove(getscore(copy),move);
                }

                if(currentmove.value>bestmove.value)
                {
                    bestmove = currentmove;
                }
                if(terminate)
                {
                    System.out.println("Not enough Time");
                    return bestmove;
                }
            }

        }

        return bestmove;

    }

    private int getscore(final GameStateModule state)
    {
        return r.nextInt(10);
    }


    public class mymove
    {
        public mymove(int value, int column)
        {
            this.value = value;
            this.column = column;
        }
        private int value;
        private int column;
    }
}


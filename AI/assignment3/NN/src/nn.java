/**
 * Created by xiezhou on 3/13/17.
 */

import java.util.Random;
import java.util.*;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.nio.charset.*;
import java.nio.file.*;

public class nn {
    public static void main(String[] args) {
        //pre-process pictures, resize to 55x50, store in new files
        nn ANN = new nn();
        ANN.Read_Data();
        ANN.reshape();
        //ANN.resize();
    }

    public int size = 960*16;
    public double [] weight  = new double [size];
    public double [][] Female = new double [55][size];
    public double [][] Male = new double[218][size];
    public double [][] Test = new double [40][size];
    public Integer [] Test_Lable  = {1,1,1,1,1,
            1,1,1,0,1,
            1,1,1,1,1,
            1,1,1,0,1,
            1,1,1,1,1,
            1,1,1,1,1,
            0,0,0,0,0,
            0,1,1,1,1};

    public double [][][] image_Female = new double [55][120][128];
    public double [][][] image_Male = new double[218][120][128];
    public double [][][] image_Test = new double [40][120][128];
    public int ssize = 55*50;
    public double [][] sFemale = new double [55][ssize];
    public double [][] sMale = new double[218][ssize];
    public double [][] sTest = new double [40][ssize];

    public void reshape(){
        for(int i = 0; i < 55; i++){
            for(int j = 0; j<120;j++){
                for(int k = 0; k<128;k++){
                    image_Female[i][j][k] = Female[i][j*128+k];
                }
            }
        }
        for(int i = 0; i < 218; i++){
            for(int j = 0; j<120;j++){
                for(int k = 0; k<128;k++){
                    image_Male[i][j][k] = Male[i][j*128+k];
                }
            }
        }
        for(int i = 0; i < 40; i++){
            for(int j = 0; j<120;j++){
                for(int k = 0; k<128;k++){
                    image_Test[i][j][k] = Test[i][j*128+k];
                }
            }
        }
    }

    public void resize(){
        for(int i = 0; i < 55; i++){

        }

        for(int i = 0; i < 218; i++){

        }

        for(int i = 0; i < 40; i++){

        }
    }

    static String readFile(String path, Charset encoding)
            throws IOException
    {
        byte[] encoded = Files.readAllBytes(Paths.get(path));
        return new String(encoded, encoding);
    }

    public void Read_Data()
    {
        String path = System.getProperty("user.dir");
        String[] folder = {"Female","Male","Test"};


        String tmppath = path+"\\"+folder[0];
        File[] Femalefiles =  Arrays.copyOfRange(new File(tmppath).listFiles(), 0, 55);
        tmppath = path+"\\"+folder[1];
        File[] Malefiles =  Arrays.copyOfRange(new File(tmppath).listFiles(), 0, 218);
        tmppath = path+"\\"+folder[2];
        File[] Testfiles =  Arrays.copyOfRange(new File(tmppath).listFiles(), 0, 40);
        String tmp = "";
        for (int i = 0; i<55;i++)
        {
            try {
                tmp = readFile(Femalefiles[i].toString(), StandardCharsets.UTF_8);
            } catch (IOException e ){System.out.println("No Such File");}
            tmp = tmp.replace("\n"," ");
            String [] list = tmp.split(" ");
            for (int j = 0;j<size;j++)
            {
                Female[i][j] = Double.parseDouble(list[j]);
            }
        }
        for (int i = 0; i<218;i++)
        {
            try {
                tmp = readFile(Malefiles[i].toString(), StandardCharsets.UTF_8);
            } catch (IOException e ){System.out.println("No Such File");}
            tmp = tmp.replace("\n"," ");
            String [] list = tmp.split(" ");
            for (int j = 0;j<size;j++)
            {
                Male[i][j] = Double.parseDouble(list[j]);
            }
        }
        for (int i = 0; i<40;i++)
        {
            try {
                tmp = readFile(Malefiles[i].toString(), StandardCharsets.UTF_8);
            } catch (IOException e ){System.out.println("No Such File");}
            tmp = tmp.replace("\n"," ");
            String [] list = tmp.split(" ");
            for (int j = 0;j<size;j++)
            {
                Test[i][j] = Double.parseDouble(list[j]);
            }
        }

    }
}

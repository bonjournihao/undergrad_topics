/**
 * Created by Ash on 2017/3/14.
 */
import java.util.Random;
import java.util.*;
import java.io.BufferedReader;
import java.io.File;
import java.io.IOException;
import java.nio.charset.*;
import java.nio.file.*;

public class p3 {
    public static void main(String[] args)
    {
        p3 ANN = new p3();

        ANN.Read_Data();
        ANN.Shuffle();
        //ANN.Ten_Fold();
        //ANN.Training();
        //ANN.Testing();

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

    public void Shuffle()
    {
        //double[][] a= {{1,1,1,1},{2,2,2,2},{3,3,3,3},{4,4,4,4}};
        List<double[]> tmp = new ArrayList<double[]>();
        tmp.addAll(Arrays.asList(Female));
        Collections.shuffle(tmp);
        double[][] tmp2 = tmp.toArray(new double[55][size]);
        for(int i  = 0;i <55;i++)
        {
            Female[i] = tmp2[i];
        }

        List<double[]> tmp3 = new ArrayList<double[]>();
        tmp3.addAll(Arrays.asList(Male));
        Collections.shuffle(tmp3);
        double[][] tmp4 = tmp3.toArray(new double[218][size]);
        for(int i  = 0;i <218;i++)
        {
            Male[i] = tmp4[i];
        }
    }

    public void Training(double[][] M)
    {

    }

    public int Testing(double[] M)
    {
        return 0;
    }

    /*
    public class single_input(){
        double[] pixels;
        int gender = 0; //male = 0, female = 1
        single_input(double[] pix, int gen)
        {
            pixels = Arrays.copyOf(pix, pix.length);
            gender = gen;
        }
    }
    */


    //TODO: add 1 more entry at the end of pixels(Female & Male); male = 0, female = 1
    public double[][] fold_Shuffle()
    {
        double [][] toFold = new double [273][size+1];

        List<double[]> tmp = new ArrayList<>();
        tmp.addAll(Arrays.asList(Female));
        tmp.addAll(55,Arrays.asList(Male));

        Collections.shuffle(tmp);

        double[][] tmp2 = tmp.toArray(new double[273][size+1]);

        for(int i  = 0;i <55;i++)
        {
            toFold[i] = tmp2[i];
        }
        return toFold;
    }

    float getMean(List<Float> list)
    {
        float sum = 0;
        for(float a : list)
            sum += a;
        return sum/list.size();
    }
    float getSd(List<Float> list)
    {
        float mean = getMean(list);
        float temp = 0;
        for(float a :list)
            temp += (a - mean)*(a - mean);
        return (float)Math.sqrt(temp/list.size());
    }

    public void five_fold(){
        List<Float> train_std_dev = new ArrayList<>();
        List<Float> train_mean = new ArrayList<>();
        List<Float> test_std_dev = new ArrayList<>();
        List<Float> test_mean = new ArrayList<>();

        //run 10 times
        for(int i = 0; i < 10; i++){
            List<Float> train_accuracy = new ArrayList<>();
            List<Float> test_accuracy = new ArrayList<>();

            //run 5 fold
            for(int j = 0; j < 5; j++) {

                int train_count = 0;
                int test_count = 0;

                //shuffle input images
                double[][] test_data = fold_Shuffle();

                //1:4
                int cut_point = (int) Math.floor(test_data.length / 5);
                double[][] A = new double[cut_point][size + 1];
                double[][] B = new double[test_data.length - cut_point][size + 1];

                for (int k = 0; k < test_data.length; k++) {
                    if (k < cut_point)
                        A[k] = test_data[k];
                    else
                        B[k] = test_data[k];
                }
                //TODO: train-> set of images;
                //TODO: test-> one image each time, output  0 -> male, 1 -> female
                Training(B);

                //get accuracies for :4 and :1
                for (int k = 0; k < B.length; k++) {
                    int prediction = Testing(B[k]);
                    if (prediction == B[k][size + 1])
                        train_count += 1;
                }
                for (int k = 0; k < A.length; k++) {
                    int prediction = Testing(A[k]);
                    if (prediction == A[k][size + 1])
                        test_count += 1;
                }

                train_accuracy.add((float)train_count/B.length);
                test_accuracy.add((float)test_count/A.length);
            }
            train_std_dev.add(getSd(train_accuracy));
            train_mean.add(getMean(train_accuracy));
            test_std_dev.add(getSd(test_accuracy));
            test_mean.add(getMean(test_accuracy));
        }
    }


}

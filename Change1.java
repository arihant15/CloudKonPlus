import java.io.File;
import java.io.FileNotFoundException;
import java.io.PrintWriter;
import java.util.ArrayList;
import java.util.Scanner;

// this is to alter  the file with and append the at the end with 50000

public class Change1 {

        public static void main(String[] args) throws FileNotFoundException {
                //

                System.out.println("the input file is" + args[0]);
                String portAdd1 = args[1];

                File file = new File(args[0]);
                if (!file.isFile()) {
                        System.out.println("inscorrect file path");
                } else {
                        Scanner scan = new Scanner(file);
                        ArrayList<String> alr = new ArrayList<String>();

                        while (scan.hasNext()) {
                                alr.add(scan.nextLine());

                        }

                        PrintWriter writer = new PrintWriter("neighbor.conf");
  for (String string : alr) {
                         writer.println(string + " " + portAdd1);

                        //System.out.println(string + " " + portAdd);
                        }
                        writer.close();
                }

        }

}



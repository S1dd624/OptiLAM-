    // 04-03-2023 - 06:25pm (Saturday)  Sushil Maurya

// Pre Deepnest Hex code for slice tagging/////////////////
// Developed by Juan Chowdhury/////////////////////////////
// Corrected & Enhanced functions by Sushil Maurya/////////
// PhD scholar, Dept of ME, IITB///////////////////////////
// For Course Project on OptiLAM///////////////////////////
// Course Code ME637///////////////////////////////////////
// Course Title: Manufacturing Automation//////////////////
// Course Instructor: Prof K P Karunakaran/////////////////
// Course Mentor: Mr. Sudhanshu Dubey//////////////////////

// All descriptive and informative comments are given by sushil maurya which are to be used to understand the flow and working of code.

#include <fstream>
#include <string>
#include <sstream>
#include <conio.h>
#include <iostream>
#include <cmath>
#include <cstring>
#include <windows.h>
#include <vector>
using namespace std;

string decToHexa(int n)
{
    char hexaDeciNum[2]; // char array to store hexadecimal number
    int i = 0;           // counter for hexadecimal number array
    while (n != 0)
    {
        int temp = 0;  // temporary variable to store remainder
        temp = n % 16; // storing remainder in temp variable.
        if (temp < 10)
            hexaDeciNum[i] = temp + 48;
        else
            hexaDeciNum[i] = temp + 55;
        i++;
        n = n / 16;
    }
    string partialHexCode = "";
    if (i == 2)
    {
        partialHexCode.push_back(hexaDeciNum[1]);
        partialHexCode.push_back(hexaDeciNum[0]);
    }
    else if (i == 1)
    {
        partialHexCode = "0";
        partialHexCode.push_back(hexaDeciNum[0]);
    }
    else if (i == 0)
    {
        partialHexCode = "00";
    }
    return partialHexCode;
}

string convertRGBtoHex(int R, int G, int B)
{
    if ((R >= 0 && R <= 255) && (G >= 0 && G <= 255) && (B >= 0 && B <= 255))
    {
        string hexCode = "#";
        hexCode += decToHexa(R);
        hexCode += decToHexa(G);
        hexCode += decToHexa(B);

        return hexCode;
    }
    else
    {
        return "-1";
    }
}

int NG_Code()
{
    int n_words = 0,   // stores total number of words in the whole test1.svg file
        n_slices = -1; // stores total number of slices in the test1.svg file

    string input_path = "C:\\OptiLAM\\output\\test1.svg";
    string key = "transform";
    vector<string> data;

    ifstream input_file(input_path.c_str());
    while (input_file.eof() == 0)
    {
        string temp;
        input_file >> temp;
        n_words++;
        data.push_back(temp);

        // when temp.find(key) is not empty, we increase n_slices value by one
        if (temp.find(key) != string::npos)
            n_slices++;

        // if(n_words<20)
        //	cout<<"\n"<<temp;
    }
    // cout<<"\n\n"<<n_words<<"\n\n"<<n_slices;
    input_file.close();

    string output_path = "C:\\OptiLAM\\output\\test1.nc";
    ofstream output_file(output_path.c_str());

    int k = 0;
    int R = k % 256;
    int G = (k / 256) % 256;
    int B = (k / 256 / 256) % 256;
    string code = convertRGBtoHex(R, G, B);

    output_file << "%00001";
    output_file << "\nN10 G21";

    int i = 12, // 12 is the position of word 'X' which is just after 'translate' word
        N = 20,
        sub_p = 1;

    float Z = 78,  // initial Z height
          X,
          Y,
          X_prev = 0;

    while (i < n_words)
    {
        // Modified by Sushil to work with new svg transform property (25-02-2023)
        // this if condition will be execuited if the 'key' i.e., 'transform' is found in the 'data' vector
        if (data[i].find(key) != string::npos)
        {
            int pos_bracket_in_transform = data[i].find("("); // change by sushil

            // cout << "\n\nPosition of '(' in transfrom is: " << pos_bracket_in_transform;
            // cout << "\nX is: " << data[i].substr(pos_bracket_in_transform + 1);
            // cout << "\nY is: " << data[i+1].substr(0, data[i+1].length() - 1);

            // Changed by sushil to work with new svg format
            stringstream sx(data[i].substr(pos_bracket_in_transform + 1));
            sx >> X;
            stringstream sy(data[i + 1].substr(0, data[i + 1].length() - 1));
            sy >> Y;
            // cout<<"\n"<<X<<"\n"<<Y<<"\n";

            if (X != X_prev)
            {
                output_file << "\nN" << N << " G91";
                N += 10;
                output_file << "\nN" << N << " A" << X - X_prev << " B" << (X - X_prev) + 0.6; //increment in B roller
                N += 10;
                output_file << "\nN" << N << " G90";
                N += 10;
            }
            output_file << "\nN" << N << " Y" << Y -175 << " C0";
            N += 10;
            //	output_file<<"\nN"<<N<<" M03 G01 F2000"; //M03 Laser on
            //	N+=10;
            output_file << "\nN" << N << " Z" << Z;
            //	output_file<<" F500";
            N += 10;
            output_file << "\nN" << N << " G04 X30	 " ; //<<sub_p;
            N += 10;
            sub_p++;
            Z += 25.10;
            output_file << "\nN" << N << " Z" << Z;
            // output_file<<" F500";
            N += 10;
            Z -= 25;
            X_prev = X;
        }
        i++;
    }
    /*float z = 15.0;
    int i=0,j,m,g=0,size1,size2,slices,q,t,GM_C=0,string_tot,y_i=0;
    string put[200000];
    string s1,firstname,fullname,code,slicenum;
//	cout<<"\nEnter the nested file name: ";
//	cin>>firstname;
    firstname="E:\\Rateesh\\Outputs\\test1";                                        //Change NEsted SVG filename here
    fullname=firstname+".svg";
//	cout<<"\n"<<fullname;
    ifstream yfile;
    yfile.open(fullname.c_str());
    slicenum="transform";
    int mter[10000];


    while(yfile.eof()==0)
    {
        yfile>>put[i];	//words
        //cout<<"\n"<<put[i];
        size_t found=put[i].find(slicenum);
        if(found!= string::npos)	{g=g+1;} //n_key

        i++;  //n_words
    }
    yfile.close();
    cout<<"\n"<<g<<"\t"<<i<<"\n";

    ofstream Gfile;
    fullname="E:\\Rateesh\\Outputs\\test1.nc";		                         //Change NC filename here
    Gfile.open(fullname.c_str());
    //cout<<"\n\n\n\n\n***********NC Code generation complete***************"<<"\n\n\nPlease find the NC file with name:"<<fullname;
    //cout<<"\n"<<i;
    slices=g-1;  //n_slices
    string_tot=i;  //n_words
    //cout<<"\nnumber of strings"<<string_tot;
    //cout<<"\nNumber of slices:"<<slices;

    g=0;
    i=0;

    int R,G,B;
    char tran[50],rot[20],xtran[20],ytran[20],y[8];
    string str2="translate"; //str1
    string str3="rotate"; //str2
    string str4="d=\"M"; //str3
    string str5="z\""; //str4
    string str6="L"; //str5
    string Xcor_sub[10][800], Ycor_sub[10][800];
    //g is slice number

    while(g<=slices)
    {
        //cout<<"\n\n"<<g;
        ///////////////////////////////HEX CODE////////////////////////////////////////////
        R = g % 256;
        G = (g/256) % 256;
        B = (g/256/256) % 256;
        code=convertRGBtoHex(R, G, B);
        //	cout<<"\n\nSlice No: "<<g<<"\t RGB Code: "<<code;
        ///////////////////////////////HEX CODE////////////////////////////////////////////
        i=0;
        while(i<string_tot)
        {


            double found=put[i].find(code);		//find hex code for slice
            if(found!= string::npos)
            {
            ///////////////////////Galvo XY///////////////////////////////////
                int jter=i;
                int iter=i;

                while(jter<string_tot)
                {

                    double found1=put[jter].find(str5); //z\"
                    //cout<<"\n"<<jter+1<<" "<<put[jter+1];
                    if(found1!= string ::npos)
                    {

                        int kter=jter+1;
                        mter[g]=kter-iter;
                        //cout<<"\n"<<kter<<"\t"<<iter;
                        int n_num=0;
                        while(iter<kter)
                        {

                            double found2=put[iter+1].find(str4); //d=\"M
                            //cout<<"\n"<<put[iter+1];

                            if(found2!= string::npos)
                            {
                                //cout<<"\nhere is";
                            //	cout<<"\nXcor:"<<put[iter+2]<<"\nYcor:"<<put[iter+3];
                                Xcor_sub[g][1]=put[iter+2];
                                Ycor_sub[g][1]=put[iter+3];
                                n_num++;
                            }

                            else
                            {
                                double found3=put[iter+1].find(str6); //L
                                if(found3!= string ::npos)
                                {
                                //	cout<<"\nXcor:"<<put[iter-1]<<"\nYcor:"<<put[iter];
                                    Xcor_sub[g][n_num+1]=put[iter+2];
                                    Ycor_sub[g][n_num+1]=put[iter+3];
                                    n_num++;
                                }
                                goto pass7;
                            }
                            pass7:
                            //	cout<<"\t"<<n_num;

                            iter++;

                        }
                        goto pass6;
                    }
                    jter++;

                }


            pass6:

                int I=i;
                j=i-3;

                while(j<I)
                {

            //////////////////A co-ordinate///////////////////////////

                    size_t found=put[j-1].find(str2); //translate
                    //cout<<"\n"<<put[j-1];
                    if(found!=string::npos)
                    {
                        //cout<<"\n"<<g;
                        strcpy(tran,put[j].c_str());
                        size1=put[j].length();
                        for(m=0;m<size1;m++)
                        {
                            if(tran[m]=='(')
                            {
                                for(int u=0;u<7;u++)
                                {
                                    xtran[u]=tran[m+u+1];
                                }
                            }
                        }

            //////////////////Y co-ordinate///////////////////////////
                        size2=put[j+1].length();
                        strcpy(tran,put[j+1].c_str());
                        m=0;
                        q=0;t=0;
                        while(tran[q]!=')')
                        {
                                ytran[q]=tran[q];
                                if(t<7)
                                {
                                    y[t]=ytran[q];
                                }
                                t++;
                            q++;
                        }
            //////////////////Rotation///////////////////////////////

                        strcpy(tran,put[j+2].c_str());
                        m=0;
                        int q=0;
                        while(tran[m+1] != ')' )
                        {
                            m=q+7;
                            rot[q]=tran[m];
                            q++;
                        }
                        stringstream s(y);
                        s >> y_i;
                        if(g==0)
                        {
                            Gfile<<"%O0001";
                            Gfile<<"\nN10 G21 G90";
                            //Gfile<<"\nN20 A"<<xtran<<" B"<<xtran<<" Y"<<y_i-175;
                            Gfile<<"\nN20 A"<<xtran<<" B"<<xtran<<" Y"<<y;             //A,B paper roller movement  Y is table movement
                            Gfile<<"\nN30 M03 G01 F500.0 C"<<rot;                       // S100 is spindle speed of table and C is rotation of table
                            Gfile<<"\nN35 Z 15";
                            Gfile<<"\nN40 P000"<<g+1;                          // P series program leads to subprogram for g+1 slice, g starts from 0
                            Gfile<<"\nN50 Z -0.1";                                    // z is table height movement
                        //	Gfile<<"\nN60 Z0.1";
                        //	Gfile<<"\nN70
                        //	Gfile<<"\nN80 M02";
                        }
                        else if(g<slices-1)
                        {
                            GM_C=(g-1)*70+70;
                            //Gfile<<"\nN"<<GM_C+10<<" A"<<xtran<<" B"<<xtran<<" Y"<<y_i-175;
                            Gfile<<"\nN"<<GM_C+10<<" A"<<xtran<<" B"<<xtran<<" Y"<<y;	//A,B paper roller movement  Y is table movement
                            Gfile<<"\nN"<<GM_C+20<<" M03 G01 F500.0 C"<<rot;			// S100 is spindle speed of table and C is rotation of table
                            Gfile<<"\nN"<<GM_C+30<<" Z "<<z;                                //raise table height add value for tensioning 0.1+0.xxxx
                            Gfile<<"\nN"<<GM_C+40<<" P000"<<g+1; // P series program leads to subprogram for g+1 slice, g starts from 0
                            Gfile<<"\nN"<<GM_C+50<<" Z "<<z-15.1;             //lower table height to allow motion of rollers for next slice
                        //	Gfile<<"\nN"
                        //	Gfile<<"\nN"<<GM_C+60<<"  M02";
                        }
                        else if(g==slices-1)
                        {
                            //Gfile<<"\nN"<<GM_C+60<<" Z -0.1";
                            //Gfile<<"\nN"<<GM_C+70<<" A"<<xtran<<" B"<<xtran<<" Y"<<y_i-175;
                            Gfile<<"\nN"<<GM_C+70<<" A"<<xtran<<" B"<<xtran<<" Y"<<y;
                            Gfile<<"\nN"<<GM_C+80<<" M03 G01 F500.0 C"<<rot;
                            Gfile<<"\nN"<<GM_C+90<<" M04";
                            Gfile<<"\nN"<<GM_C+100<<" Z "<<z;
                            Gfile<<"\nN"<<GM_C+110<<" P000"<<g+1;
                            Gfile<<"\nN"<<GM_C+50<<" Z "<<z-15.1;
                            Gfile<<"\nN"<<GM_C+120<<" M02";
                            Gfile<<"\nN"<<GM_C+130<<" G99 G63 G89";
                            //cout<<"\n end of file";
                            //Gfile<<"\nN"<<GM_C+150<<" M30";                  //Laser Trigger On
                        }

                        goto pass5;
                    }
                    j++;
                }
                goto pass5;
            }
            i++;
        }
        pass5:
        //cout<<"test";
        g++;
        //cout<<"\nRUN now";
        z-=0.1;
    }
    int count=0;
    for(g=0;g<slices-1;g++)
    {

            /////////////Initial Data of Subprogram///////////////////////////////
            Gfile<<"\n\n%P000"<<g+1;
            Gfile<<"\nN10 G00 G21 G91";
            Gfile<<"\nN20 M07 M08 M011";

        int mount=mter[g+1]/3;

        for(count=0;count<mount;count++)
        {

            //cout<<"\n"<<count<<"\tXcor:"<<Xcor_sub[g+1][count+1]<<"\tYcor:"<<Ycor_sub[g+1][count+1];
            Gfile<<"\nN"<<count+3<<"0"<<" X"<<Xcor_sub[g+1][count+1]<<" Y"<<Ycor_sub[g+1][count+1];	///might require get()
        }
            Gfile<<"\nN"<<count+3<<"0 "<<"M03";///might require get()
    }
    */
    cout << "\n\n***********NC Code generation complete***************";
    // Gfile.close();
    Sleep(2000);

    return 0;
}

////////////////////////////////////////////////////////////////////
////////////////////                       /////////////////////////
////////////////////     MAIN FUNCTION     /////////////////////////
////////////////////                       /////////////////////////
////////////////////////////////////////////////////////////////////

// depiction of what the paper parameters look like by sushil
//
//                    x(infinitly long) & pap_wid -->
//                  --------------------------------------------------------------------------
//                  |                                                                        |
//                  |             fl_wid -->                                                 |
//          ^       |      ^     ----------                                                  |
//          |       |      |     | item-1 |                                                  |
//          |       |   fl_hei   ----------                                                  |
//     y & pap_hei  |                                                       Paper Depiction  |
//                  --------------------------------------------------------------------------

int main()
{
    float x_disp = 0, // fixed distance to be moved in x direction for all element in particular column
        y_disp = 0;   // fixed displacement between every pair of item in y direction

    long i,     // -----DON'T KNOW-----
        j,      // -----DON'T KNOW-----
        k,      // -----DON'T KNOW-----
        m,      // -----DON'T KNOW-----
        g = 0,  // Current slice layer number
        s_size; // Stores the total number of SVG Files in longInteger Format
    // Loop_no; // UNUSED

    int R = 0, // Decimal color value for R
        G = 0, // Decimal color value for G
        B = 0; // Decimal color value for B

    vector<string> put; // vector used to store all the 'words' from the string formatted and inputted sliced svg for nesting usage

    string s1,     // local variable: Used to Postfix the layer-count to the SVG-filename in loop
        firstname, // local variable: Constant prefix string added to all sliced files
        fullname,  // local variable: Full name of the layer SVG in the loop after adding the firstname and the layer count
        code,      // stores the Hex code of RGB value
        pathstyle; // used for storing the path traced by the lines/polygons of the sliced SVG

    string wid, // string format of width taken from <svg width="000mm"> </svg> tag i.e., wid = 000
        hei,    // string format of height taken from <svg height="111mm"> </svg> tag i.e., hei = 111
        xtran,  // space to be left in addition to the marginX in the direction of height
        ytran;  // space to be left in addition to the marginY in the direction of width

    char cwid[15], // Stores the width from the <svg> tag of the sliced file in the form of a character array
        chei[15];  // Stores the height from the <svg> tag of the sliced file in the form of a character array
    // cord[20];   // UNUSED

    string xcor, // -----DON'T KNOW-----
        ycor,    // -----DON'T KNOW-----
        g_s,     // -----DON'T KNOW-----
        size;    // Stores the total number of SVG Files in String Format

    float fl_xtran = 0, // actual position of current slice in direction of paper width (x_displacement)
        fl_ytran = 0,   // actual position of current slice in direction of paper height (y_displacement)
        fl_wid,         // actual width of current slice
        fl_hei,         // actual height of current slice
        pap_hei = 350,  // total Paper height
        pap_wid;        // total paper widht( available for printing at a single time )

    // wid = 128 len =  225

    firstname = "icesl_script_slice_";

    ofstream myfile;                               // variable used to open and store the OUTPUT of the NESTING CODE
    myfile.open("C:\\OptiLAM\\output\\test1.svg"); // Nested SVG filename here

    myfile << "<svg xmlns=\"http://www.w3.org/2000/svg\" "; // adding the svg element tag header in the newly created "test1.svg" file.

    // Gets total number of slices that are to be nested from Slices.txt file
    ifstream range; // -----DON'T KNOW-----
    range.open("C:\\OptiLAM\\bin\\param\\Slices.txt");
    range >> size;
    stringstream ss(size); // -----DON'T KNOW-----
    ss >> s_size;

    int x = 0;        // -----DON'T KNOW-----
    int countx = 0;   // Stores the column number of the nesting
    int county = 0;   // It the variable slice number in the direction of height used to avoid paper-height-out-of-bound condition
    int marginx = 10; // Margin to be left in the direction of width i.e., pap_hei
    int marginy = 10; // Margin to be left in the direction of length i.e., pap_wid

    // added by sushil for inverted column nesting arrangement
    int previous_column_remaining_space = 0; // previous_column_remaining_space stores the vertical space left unused after placing all items in previous column. It is used to know the position from where the next inverted column's 1st item has to be placed.
    int switch_to_next_column = 0;           // 0 means false and 1 means true to switch columns

    // added by sushil for checking if nesting is possible or not
    int is_nesting_possible = 1;

    // added by sushil to check whether the size of paper for nested svg can accomodate only one item in each column. i.e., can there be only one row in nested svg output
    int is_only_one_row_possible = 0;

    // depiction of what the paper parameters look like by sushil
    //
    //                    x(infinitly long) & pap_wid -->
    //                  --------------------------------------------------------------------------
    //                  |                                                                        |
    //                  |             fl_wid -->                                                 |
    //                  |      ^     ----------                                                  |
    //                  |      |     | item-1 |                                                  |
    //                  |   fl_hei   ----------                                                  |      -----
    //                  |------------------------------------------------------------------------|        ^
    //                  |------------------------------------------------------------------------|        |
    //                  |------------------------------------------------------------------------|        |     padding_mulitplier
    //                  |------------------------------------------------------------------------|        v
    //                  |                                           fl_wid -->                   |      -----
    //                  |                                    ^     ----------                    |
    //                  |                                    |     | item-1 |                    |
    //                  |                                 fl_hei   ----------                    |      -----
    //                  |------------------------------------------------------------------------|        ^
    //                  |------------------------------------------------------------------------|        |
    //                  |------------------------------------------------------------------------|        |     padding_multiplier
    //                  |------------------------------------------------------------------------|        v
    //                  |             fl_wid -->                                                 |      -----
    //                  |      ^     ----------                                                  |
    //                  |      |     | item-1 |                                                  |
    //                  |   fl_hei   ----------                                                  |
    //                  |                                                                        |
    //                  |                                                                        |
    //                  |                                                       Paper Depiction  |
    //                  --------------------------------------------------------------------------
    // added by sushil to define the vertical padding space between different items in consecutive rows
    // when multiplier is set to 1, space of 1*fl_hei is left between different rows
    float padding_multiplier = 0;            

    // Loop for iterating through all sliced SVG files and performing some functions on them to create a single nested .svg file
    while (g < s_size)
    {
        cout << "\nFile No" << g;

        ostringstream strg; // -----DON'T KNOW-----

        strg << g;
        s1 = strg.str();
        fullname = "C:\\OptiLAM\\bin\\slices\\icesl_script_slices_SVG\\" + firstname + s1 + ".svg";
        cout << "\n"
             << fullname;

        ifstream yfile;               // Sushil - yfile is an input file buffer stream
        yfile.open(fullname.c_str()); // File Loading

        // Sushil - What is the need to always do put.clear(); ?
        put.clear(); // Sushil - clearing the content of the put vector

        while (yfile.eof() == 0) // Sushil - Checks: if the end-of-file has been reached
        {
            string temp;
            yfile >> temp;
            /*
                Sushil - We know that yfile is an buffer stream. When you extract data from the buffer using the ">>" operator, it removes the extracted data from the buffer and updates the file position indicator to point to the next piece of data in the buffer. This way, the next time you extract data from the buffer using the ">>" operator, you'll get the next piece of data, not the same one you got before.
                It moves the buffer stream word by word, or until it encounters a white space character (such as a space, tab, or newline). So, if the file contains a line like "hello world", the first yfile >> temp operation would extract "hello" from the buffer into the temp string, and the next yfile >> temp operation would extract "world".
            */
            put.push_back(temp); // Sushil - pushing all the words from the current file into the "put (string vector)"
            j = j + 1;           // This is just for maintaining the count of total number of words with black spaces
        }                        // loading find into string array
        yfile.close();

        k = j;
        // Sushil(28-01-2023) - Below color code is correct & works like this{#010000 -> #FF0000, #010100 -> #FF0100, #010200 -> #FF0200} and so on. Hence, visually they appear to be of same color but actually they are different colors
        R = g % 256;
        G = (g / 256) % 256;
        B = (g / 256 / 256) % 256;

        // Sushil - Not clear what the below commented code does
        // if(R==76 && G==01)
        //{
        //	//int q=0;
        //	for(int q=0; q<put.size(); q++)
        //		cout<<"\n"<<put[q];
        // }
        // cout<<put.size();
        code = convertRGBtoHex(R, G, B);
        //	cout<<"\n"<<code;

        string str1 = "width"; // Extracting bounding box width
        for (j = 0; j <= k; j++)
        {
            size_t found = put[j].find(str1); // Sushil - Because size_t is an unsigned integer, it can only represent non-negative values, and is typically used to index elements within a container or to represent the size of objects in memory. This makes it suitable for use as a return type for functions that deal with sizes, lengths, and indices, as it eliminates the possibility of negative values.
            if (found != string::npos)        // Sushil - string::npos is returned by .find(string) function when the match isn't found.
            {
                // Sushil - The below code stores each number in the form of string character in the "cwid" string array until the unit of width i.e., "mm" is reached.
                m = 0;
                while (m != sizeof(put[j]))
                {
                    if (put[j][m + 7] == 'm')
                    {
                        goto pass1;
                    }
                    cwid[m] = put[j][m + 7];
                    m = m + 1;
                }
            }
        }
    pass1:
        stringstream scw;         // Stores the width from "character array called cwid" into the string stream called "scw"
        scw << cwid;              // reads data from the cwid into the scw string stream
        wid = scw.str();          // converts the stringstream into a string called "wid"
        stringstream geek11(wid); // -----DON'T KNOW-----
        geek11 >> fl_wid;         // -----DON'T KNOW-----

        string str2 = "height"; // Extracting bounding box height
        for (j = 0; j <= k; j++)
        {
            size_t found = put[j].find(str2);
            if (found != string::npos)
            {
                m = 0;
                while (m != sizeof(put[j]))
                {
                    if (put[j][m + 8] == 'm')
                    {
                        goto pass2;
                    }
                    chei[m] = put[j][m + 8];
                    m = m + 1;
                }
            }
        }
    pass2:
        stringstream sch;         // Stores the height from "character array called chei" into the string stream called "sch"
        sch << chei;              // reads data from the chei into the sch string stream
        hei = sch.str();          // converts the stringstream into a string called "hei"
        stringstream geek12(hei); // -----DON'T KNOW-----
        geek12 >> fl_hei;         // -----DON'T KNOW-----

        //		cout<<"\nBoundig Box Size: "<<fl_wid<<"mm X"<<fl_hei<<"mm";
        //		cout<<"\nPage Size: "<<(s_size-index)*fl_wid<<"mm X"<<fl_hei<<"mm";

        // added by sushil to Check if nesting of SVGs is possible into a single file of given paper_height
        if ((pap_hei < (fl_hei + (2 * marginy))) && g == 0)
        {
            is_nesting_possible = 0;
            break;
        }
        // added by sushil: below 'else if' will break when this program achieves capability to nest objects according to their own size rather than current method of using the bounding-box
        else if ((pap_hei < ((fl_hei + (padding_multiplier * fl_hei) + fl_hei) + (2 * marginy))) && (g == 0))
        {
            is_only_one_row_possible = 1;
        }

        // pap_wid = (s_size - index) * fl_wid;                              // paper width is calculated here
        pap_wid = ((s_size / (int)(pap_hei / fl_hei)) + 2) * fl_wid + marginx; // -----DON'T KNOW-----
        if (g == 0)
        {
            string startx, starty;
            for (j = 0; j <= k; j++)
            {
                // this finds the example: viewBox="0.85 -177.6 224.95 128.9" string and navigates to ' " ' this string and moves to next character
                if (put[j].find("viewBox") != string::npos)
                {
                    // viewBox="0.85 -177.6 224.95 128.9"
                    startx = put[j].substr(put[j].find('"') + 1); // stores 0.85
                    starty = put[j + 1];                          // stores -177.6
                    break;
                }
            }
            // cout<<"\n"<<startx<<"\n"<<starty;
            myfile << "width=\"" << pap_wid << "mm\" ";
            myfile << "height=\"" << pap_hei << "mm\" ";
            myfile << "viewBox=\"" << startx << " " << starty << " " << pap_wid << " " << pap_hei << "\">"; //<g transform=\"translate ( "<<cent_x<<" "<<cent_y<<" ) \">";
        }

        // added by sushil for nesting when single row condition is true
        if ((is_only_one_row_possible == 1) && (g == 0))
        {
            countx = 0;
            x_disp = 0;

            fl_xtran = (countx * (fl_wid + x_disp) + marginx); // x transformation for the next column

            county = 0;
            y_disp = 0;
            fl_ytran = marginy;

            countx = countx + 2;
        }
        else if ((is_only_one_row_possible == 1) && (g != 0))
        {
            x_disp = 0;
            fl_xtran = (countx * (fl_wid + x_disp) + marginx); // x transformation for the next column
            countx = countx + 2;

            county = 0;
            y_disp = 0;
            fl_ytran = marginy;
        }

        else if (g == 0)
        {
            county = 0;
            countx = 0;
            x_disp = 0;
            // Changed by sushil to include padding_multiplier
            y_disp = 2 *fl_hei;
            fl_xtran = countx * fl_wid + marginx + x_disp; // x transformation for 1st slice
            fl_ytran = county * fl_hei + marginy + y_disp; // y transformation for 1st slice
        }
        else
        {
            county = county + 1;
            // if(fl_ytran>(pap_hei-fl_hei))                          //update slice number in y keeping x same
            // if ((county * (fl_ytran + fl_hei)) > pap_hei) // condition to avoid slice placement beyond sheet perimeter
            // if((county*(fl_ytran))>pap_hei-fl_hei)
            if (((countx % 2 == 0) && ((fl_ytran + fl_hei + (padding_multiplier * fl_hei) + fl_hei + (padding_multiplier * fl_hei) + marginy) > pap_hei)) || (switch_to_next_column == 1)) // condition by sushil maurya
            {
                // if(switch_to_next_column == 1) {
                //     cout << "\n\nForced in new column\n";
                // }
                x_disp = 20;

                // by sushil to invert column orientation
                // previous_column_remaining_space stores the vertical space left unused after placing all items in previous column
                // it is used to know the position from where the next inverted column's 1st item has to be placed.
                previous_column_remaining_space = pap_hei - (fl_ytran + fl_hei);

                countx = countx + 1;                             // This is to displace the slice in next column
                fl_xtran = countx * (fl_wid + x_disp) + marginx; // x transformation for the next column

                county = 0;
                if (countx % 2 == 0) // Check if column no is odd or even
                {
                    y_disp = 2 * fl_hei;                              // displacement between objects in even columns
                    fl_ytran = (county) * (fl_hei) + marginy + y_disp; // for even, y transformation will have offset
                }
                else // odd column
                {
                    // used when columns were not to be inverted
                    y_disp = 0; // displacement between objects in odd columns
                    // fl_ytran = (county) * (fl_hei) + marginy + y_disp; // for odd, y transformation will have no offset

                    // new condition on 04-03-2023 by sushil maurya
                    // Checking if new odd column can be started below previously left space by old column
                    if (previous_column_remaining_space - marginy >= (fl_hei + (padding_multiplier * fl_hei)))
                    {
                        // new fl_ytran is previous fl_ytran + (2 * fl_hei)
                        fl_ytran = fl_ytran + (fl_hei + (padding_multiplier * fl_hei));
                    }
                    else // If remaining space was less than 2 * fl_hei then do this
                    {
                        // new fl_ytran is previous fl_ytran - (2 * fl_hei)
                        fl_ytran = fl_ytran - (fl_hei + (padding_multiplier * fl_hei));
                    }
                }

                // Sushil - resetting switch to 0 to allow staying in same coulmn till the paper width is exceeded
                switch_to_next_column = 0;
            }
            else // if slice does not exceed paper perimeter
            {
                x_disp = 20;
                fl_xtran = countx * (fl_wid + x_disp) + marginx; // X transformation for 2nd slice

                if (countx % 2 == 0)
                {
                    // Changed condition by sushil
                    y_disp = (padding_multiplier * fl_hei) + fl_hei + (padding_multiplier * fl_hei);
                    fl_ytran = (county + 2) * (fl_hei) + marginy + (county)*y_disp;
                }
                else // for odd column sushil
                {
                    // used when the columns were not inverted
                    // y_disp = 3 * fl_hei;
                    // fl_ytran = (county) * (fl_hei) + marginy + (county)*y_disp;

                    // change by sushil to allow items other than the 1st inverted item to be placed in the nested format
                    // I took marginy - 0.1 because fl_ytran was comming out to be 9.99 when marginy was 10 and hence 1 item was not placed. So to correct this error, i took marginy - 0.1
                    if ((fl_ytran - (fl_hei + (padding_multiplier * fl_hei) + fl_hei + (padding_multiplier * fl_hei))) >= marginy - 0.1)
                    {
                        // cout << "\n\nEntering fl_ytran\n";
                        fl_ytran = fl_ytran - (fl_hei + (padding_multiplier * fl_hei) + fl_hei + (padding_multiplier * fl_hei));
                    }
                    else
                    {
                        switch_to_next_column = 1;
                        // cout << "\n\nSwitching to next column\n";
                        continue;
                    }
                }
            }
        }

        // county=county+1;

        // till here

        /*fl_xtran=countx*fl_wid+marginx;
        fl_ytran=(county)*fl_hei+marginy;
        cout<<"\n\n"<<fl_ytran;
        if(fl_ytran>(pap_hei-fl_hei))
        {
            countx=countx+1;
            fl_xtran=countx*fl_wid+marginx;
            county=0;
            fl_ytran=county*fl_hei+marginy;

        }
        county=county+1;*/

        stringstream sx, sy;
        sx << fl_xtran;
        xtran = sx.str();
        sy << fl_ytran;
        ytran = sy.str(); // float to string

        /*
            // This is the line with which the correct values of A and B is printed
            // myfile << "<g transform=\"translate ( " << xtran << " " << ytran << " ) rotate(0)\"><path ";
        */

        // Changed by sushil to allow SVG to be directly imported in the EzCad rather that the use of Boxy website as an intermediate tool for the same
        myfile << "<g transform=\"translate(" << xtran << " " << ytran << ") rotate(0)\"><path ";
        // cout<<"\n\nTransformation is: "<<xtran<<"mm X"<<ytran<<"mm\n";
        // Sleep(1000);

        long loop_no = 0, J = 0, e = 0;
        string str3 = "style";
        // Replacing color code
        for (j = J; j <= k; j++)
        {

            size_t found = put[j].find(str3);

            if (found != string::npos)
            {
                J = j;
                for (m = 12; m < 19; m++)
                {
                    put[J][m] = code[m - 12];
                }
                pathstyle = put[J];
                myfile << pathstyle << " d=\"M ";

            pass3:
                for (m = 0; m < k; m++)
                {

                    if (put[J + m + 2] == "Z")
                    {
                        if (put[J + m + 3] == "\"")
                        {

                            myfile << " z\"";
                            myfile << " class=\"active\"/></g>";
                            goto pass4;
                        }
                        else
                        {
                            loop_no = loop_no + 1;
                            //	cout<<"\nLoop No:"<<loop_no;
                            myfile << " z\"";
                            myfile << " class=\"active\"/><path ";
                            myfile << pathstyle << " d=\"M ";
                            J = J + m + 2;
                        }
                        goto pass3;
                    }
                    long pos = put[J + m + 2].find(",");
                    xcor = put[J + m + 2].substr(0, pos);
                    ycor = put[J + m + 2].substr(pos + 1);
                    // cout<<"\n"<<xcor<<":"<<ycor;
                    if (m == 0)
                    {
                        //	cout<<"\n"<<xcor<<","<<"\t"<<ycor;
                        myfile << xcor << " " << ycor;
                    }
                    else
                    {

                        myfile << " L " << xcor << " " << ycor;
                    }
                }
            }
        }
    pass4:;
        yfile.close();
        g = g + 1;
        x = x + 1;
    }
    if (is_nesting_possible == 0)
    {
        // Calling the clrscr() function
        system("cls");
        cout << "\n!!! -----------  Nesting is Not Possible  ---------- !!!\n\n";
        cout << "*******    Item Size exceeds paper dimensions    *******\n\n";
        getch();
    }
    else
    {
        myfile << "</svg>";
        myfile.close();
        cout << "\n\n*********************Nesting Complete******************\n\n";
        NG_Code();
    }
}

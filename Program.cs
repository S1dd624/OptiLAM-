using OpenCvSharp;
using System;
using System.IO;
using System.Linq;
using static System.Net.WebRequestMethods;

class ExternalEvaluator
{
    int canvasHeight;
    int canvasLength;
    float kerf_mm;
    float kerf;
    float scale;
    float pixel_size;
    string bmpDirectoryPath;

    public ExternalEvaluator(string bmpPath)
    {
        InitializeConfig(bmpPath);
    }

    private void InitializeConfig(string bmpPath)
    {
        kerf_mm = 10.0f;
        scale = 1.15f;
        pixel_size = 4.0f;
        canvasHeight = 400 / (int)pixel_size;
        canvasLength = 400 / (int)pixel_size;
        kerf = kerf_mm / pixel_size;
        bmpDirectoryPath = @"C:\\Users\\iitsi\\Nesting_GA\\Nesting_GA\\Files\\final_generator\\output_bmps";

        Console.WriteLine("Using BMP Path: " + bmpDirectoryPath);
    }

    public double EvaluateChromosomes(string chromosome)
    {
        int scaledTotA = 0;
        double fitness = 0.0f;
        int overlapS = -1;

        if (string.IsNullOrWhiteSpace(chromosome)) return -1;

        Mat canvasS = new Mat(canvasHeight, canvasLength, MatType.CV_8UC1, new Scalar(0));
        int[] X, Y, R;
        ProcessChromosome(chromosome, out X, out Y, out R);

        for (int i = 0; i < X.Length; i++)
        {
            Mat S_temp = new Mat(canvasHeight, canvasLength, MatType.CV_8UC1, new Scalar(0));
            string bmpFilePath = Path.Combine(bmpDirectoryPath, (i + 1).ToString("D3") + "_00_" + R[i].ToString("D3") + ".bmp");

            if (!System.IO.File.Exists(bmpFilePath))
            {
                Console.WriteLine($"ERROR: Missing BMP: {bmpFilePath}");
                continue;
            }

            Mat bmp = LoadBMP(bmpFilePath);
            if (bmp.Empty())
            {
                Console.WriteLine($"ERROR: Failed to load BMP: {bmpFilePath}");
                continue;
            }

            int min_edge = Math.Min(bmp.Rows, bmp.Cols);
            scale = 1 + (kerf / min_edge);

            PlaceBMPOnCanvas(S_temp, bmp, X[i], Y[i], scale);
            scaledTotA += Cv2.CountNonZero(bmp);
            Cv2.BitwiseOr(canvasS, S_temp, canvasS);
        }

        overlapS = scaledTotA - Cv2.CountNonZero(canvasS);

        if (overlapS > 0)
        {
            fitness = -overlapS;
        }
        else
        {
            int l2 = canvasS.Cols;
            int l1 = 0;
            for (int col = 0; col < l2; col++)
            {
                if (Cv2.CountNonZero(new Mat(canvasS, new Rect(col, 0, 1, canvasS.Rows))) > 0)
                {
                    l1 = col;
                    break;
                }
            }
            for (int col = l2 - 1; col >= l1; col--)
            {
                if (Cv2.CountNonZero(new Mat(canvasS, new Rect(col, 0, 1, canvasS.Rows))) > 0)
                {
                    l2 = col;
                    break;
                }
            }
            fitness = l1 + (canvasS.Cols - l2);
        }

        return fitness;
    }

    private void ProcessChromosome(string chromosome, out int[] X, out int[] Y, out int[] R)
    {
        var values = chromosome.Split(',').Select(int.Parse).ToArray();
        int numGenes = values.Length / 3;
        X = new int[numGenes];
        Y = new int[numGenes];
        R = new int[numGenes];
        for (int i = 0; i < numGenes; i++)
        {
            X[i] = values[i * 3];
            Y[i] = values[i * 3 + 1];
            R[i] = values[i * 3 + 2];
        }
    }

    private Mat LoadBMP(string bmpFilePath)
    {
        Mat bmpImage = Cv2.ImRead(bmpFilePath, ImreadModes.Grayscale);
        Cv2.BitwiseNot(bmpImage, bmpImage);
        return bmpImage;
    }

    private void PlaceBMPOnCanvas(Mat canvas, Mat bmp, int x, int y, float scale)
    {
        Rect bmpRect = new Rect(x, y, bmp.Width, bmp.Height);
        Rect canvasRect = new Rect(0, 0, canvas.Width, canvas.Height);
        Rect validROI = bmpRect & canvasRect;

        if (validROI.Width > 0 && validROI.Height > 0)
        {
            Mat canvasROI = new Mat(canvas, validROI);
            Mat bmpROI = new Mat(bmp, new Rect(validROI.X - x, validROI.Y - y, validROI.Width, validROI.Height));
            Cv2.BitwiseOr(canvasROI, bmpROI, canvasROI);
        }
    }
}

class Program
{
    static void Main(string[] args)
    {
        try
        {
            if (args.Length < 2)
            {
                Console.WriteLine("Usage: FitnessEvaluator.exe <C:\\Users\\iitsi\\Nesting_GA\\Nesting_GA\\external_dependencies\\chromosomes.csv> <output_bmps>");
                return;
            }

            string csvPath = args[0];
            string bmpFolder = args[1];

            ExternalEvaluator evaluator = new ExternalEvaluator(bmpFolder);

            using (var reader = new StreamReader(csvPath))
            {
                var lines = reader.ReadToEnd().Split('\n');
                for (int i = 1; i < lines.Length; i++)  // Skip header
                {
                    var line = lines[i].Trim();
                    if (string.IsNullOrEmpty(line)) continue;

                    var values = line.Split(',');
                    var chromosomeValues = values.Skip(1).ToArray();  // Skip ID column
                    string chromosomeStr = string.Join(",", chromosomeValues);

                    double fitness = evaluator.EvaluateChromosomes(chromosomeStr);
                    Console.WriteLine(fitness);
                }
            }
        }
        catch (Exception ex)
        {
            Console.WriteLine("ERROR: " + ex.Message);
            Console.WriteLine(ex.StackTrace);
        }
    }
}


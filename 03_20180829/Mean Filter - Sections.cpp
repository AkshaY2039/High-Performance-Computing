#include <iostream>
#include <fstream>
#include <string>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

using namespace std;

int main ()
{
	int a[384][512], b[384][512], c[384][512];
	float af[382][510], bf[382][510], cf[382][510];
	float h[3][3];

	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			h[i][j] = 1/9;
		}
	}

	double start = omp_get_wtime ();
	#pragma omp parallel sections
	{
		#pragma omp section
		{
			string linea;
			int tempa;
			ifstream myfile ("red.txt");
			if (myfile.is_open ())
			{
				for (int i = 0; i < 384; i++)
				{
					for (int j = 0; j < 512; j++)
					{
						if (getline (myfile, linea))
						{
							tempa = atoi (linea.c_str ());
							a[i][j] = tempa;
						}				
					}
				}
				myfile.close ();
				
				float sum = 0.0;
				for (int i = 1; i < 383; i++)
				{
					for (int j = 1; j < 511; j++)
					{
						for (int k = -1; k <= 1; k++)
						{
							for (int l = -1; l <= 1; l++)
							{
								sum = sum + a[i+k][j+l];
							}
						}
						af[i-1][j-1] = sum/9;
						sum = 0.0;
					}
				}

				ofstream myfile ("red_output.txt");
				for (int i = 0; i < 382; i++)
				{
					for (int j = 0; j < 512; j++)
					{
						if (myfile.is_open ())
						{
							myfile << af[i][j] << "\n";
						}
					}
				}
				myfile.close ();
			}
		}

		#pragma omp section
		{
			string lineb;
			int tempb;
			ifstream g ("green.txt");
			if (g.is_open ())
			{
				for (int i = 0; i < 384; i++)
				{
					for (int j = 0; j < 512; j++)
					{
						if (getline (g, lineb))
						{
							tempb = atoi (lineb.c_str ());
							b[i][j] = tempb;
						}				
					}
				}
				g.close ();
				
				float sumb = 0.0;
				for (int i = 1; i < 383; i++)
				{
					for (int j = 1; j < 511; j++)
					{
						for (int k =- 1; k <= 1; k++)
						{
							for (int l =- 1; l <= 1; l++)
							{
								sumb = sumb + b[i+k][j+l];
							}
						}
						bf[i-1][j-1] = sumb/9;
						sumb = 0.0;
					}
				}

				ofstream g ("green_output.txt");
				for (int i = 0; i < 382; i++)
				{
					for (int j = 0; j < 512; j++)
					{
						if (g.is_open ())
						{
							g << bf[i][j] << "\n";
						}
					}
				}
				g.close();
			}
		}

		#pragma omp section
		{
			string linec;
			int tempc;
			ifstream bl ("blue.txt");
			if (bl.is_open ())
			{
				for (int i = 0; i < 384; i++)
				{
					for (int j = 0; j < 512; j++)
					{
						if (getline (bl, linec))
						{
							tempc = atoi (linec.c_str ());
							c[i][j] = tempc;
						}				
					}
				}
				bl.close ();
				
				float sumc = 0.0;
				for (int i = 1; i < 383; i++)
				{
					for (int j = 1; j < 511; j++)
					{
						for (int k = -1; k <= 1; k++)
						{
							for (int l = -1;l <= 1; l++)
							{
								sumc = sumc + b[i+k][j+l];
							}
						}
						cf[i-1][j-1] = sumc/9;
						sumc = 0.0;
					}
				}

				ofstream bl ("blue_output.txt");
				for (int i = 0; i < 382; i++)
				{
					for (int j = 0; j < 512; j++)
					{
						if (bl.is_open ())
						{
							bl << cf[i][j] << "\n";
						}
					}
				}
				bl.close ();
			}
		}
	}
	double end=omp_get_wtime ();
	double time_elapsed = end - start;
	cout << time_elapsed << endl;
		
	return 0;
}
// ***********************************************************************
// Example of creating pthreads
// Each thread is created to run the printHello function
// ***********************************************************************

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <iostream>
#include <string>
#include <fstream>
#include <cstring>
#include <iomanip>
#include <vector>
#include <algorithm>
using namespace std;

// Global constants
const int MAX = 5;  // Maximum number of threads allowed
int N = 0;
int M = 0;
vector<string> load;
std::vector<int> countW;
int wordCount = 0;
vector<string> visited;
std::vector<int> help;

/*
void *printHello(void *arg)
{
	int tid = (intptr_t)arg;

	printf("Hello world from thread %d\n", tid);

	pthread_exit(NULL);
}

void clean()
{
	cout << "work";
	for (int i = 0; i < load.size(); i++)
	{
		string temp = load[i];
		if (temp == "")
		{
		cout << i;
			for (int j = i; j < load.size()-1; j++)
			{
				if ((j + 1) == load.size() - 1)
				{
					load[j] == "";
				}

				else
				{
					load[j] = load[j + 1];
				}

				load[load.size() - 1] = "";
			}
		}
	}
}
*/
void calculateSKEWED()
{ 
	//N = 15; 
	//M = 3;
	int num = N;
	//cout << N;
	//cout << 1;
	/**/
	int flag = 0;
	for (int i = 0; i < M; i++)
	{
		help.push_back(1);
		//help[i-1] = 1;
		//k += (N / M);
	}
	num -= M;

	for(int k=0; k < num; k++)
	{

		for (int u = 0; u < M; u++)
		{
			if (num <= 2)
			{
				break;
			}
			help[u] += u + 1;
			num -= (u + 1);
			flag = u;
			k = 0;
		}
		if (num <= 2)
		{
			break;
		}
	}
		
	
	//cout << flag << num << endl;
	if (num > 0)
	{
		if (num == 2 && flag == 0)
		{
			help[flag] += 1;
			help[flag+1] += 1;
		}
		if (num == 2 && flag == 1)
		{
			help[flag + 1] += 2;
		}
		if (num == 1 && flag == 0)
		{
			help[flag] += 1;
		}
		if (num == 1 && flag == 1)
		{
			help[flag + 1] += 1;
		}
		if (num == 2 && flag == 2)
		{
			help[flag + 2] += 2;
		}
		if (num == 1 && flag == 2)
		{
			help[flag + 2] += 1;
		}
	}
	

	/* 
	for (int i = 0; i < M; i++)
	{
		cout << help[i] << endl;
		//k += (N / M);
	}
	*/
	
}
void COUNT(string filename)
{
	ifstream fin;
	fin.open(filename.c_str());
	string line;
	
	while (getline(fin, line))
	{
			N++;
			//cout << line << endl;
	}
}

void printMAP(int c)
{
	ofstream fout;
	string filename = "map-out-";
	filename += std::to_string(c) + ".txt";
	fout.open(filename.c_str());
	/*
	for (int h = 0; h < load.size(); h++)
	{
		//cout << load[h];
		//cout << endl;
	}
	*/
	for (int f = 1; f < visited.size(); f++)
	{
		if (visited[f] != " ")
		{
			fout << visited[f] << " " << countW[f];
			fout << endl;
		}
		
	}
	visited.clear();
	countW.clear();
}

int search(string word)
{
	for (int v = 0; v < visited.size(); v++)
	{
		if (word == visited[v])
		{
			return 1;
		}
	}
	return 0;
}


void processVEC()
{
	int i = 0;
	int sea = 0;
	sort(load.begin(), load.end());
	for (int h = 0; h < load.size(); h++)
	{
			string temp = load[h];
			sea = search(temp);
			i = 0;
			if (sea == 0)
			{
				for (int o = 0; o < load.size(); o++)
				{
					if (load[o] == temp)
					{
						i++;
					}

				}
				visited.push_back(temp);
				countW.push_back(i);
			}
	}
	load.clear();
}

void loadVec(std::string filename)//function to separtate words individually
{
	//cout << filename << endl;
	ifstream fin;
	fin.open(filename.c_str());
	string s;
	int j = 0;
	int c = 0;

	while (getline(fin, s))
	{
		//cout << s << endl;
		j = 0;
		c = 0;
		//N++;
		for (int h = 0; h < s.size() ; h++)
		{
			c++;
			if ((s.at(h) == ' '))
			{
				load.push_back(s.substr(j, c - 1));
				c = 0;
				j = h + 1;
			}
			if ((h + 1) == s.size() )
			{
				load.push_back(s.substr(j, c));
			}
		}
	}
	fin.close();
	processVEC();
	//cout << "work";
	//clean();
}

void split(string filename, string split)
{
	//cout << split;
	ifstream fin;
	ofstream fout;
	fin.open(filename.c_str());
	string file;
	string line;
	int j = 0;
	int k = 1;
	int f = 0;
	int c = 1;
	if (!((N / M) % 2 == 0))
	{
		j = 1;
	}
	//cout << N << endl;
	//cout << M;
	if (split == "equal")
	{
		
		while (c <= M)
		{
			file = "split-";
			file += std::to_string(c) + ".txt";
			fout.open(file.c_str());
			for (int i = 0; i < (N/M)+k; i++)
			{
				getline(fin, line);
				if (j == 1 && (i+1) == (N/M))
				{
					k = 0;
				}
				
				//cout << file << endl;
				//
				fout << line << endl;

			}
			fout.close();
			loadVec(file);
			printMAP(c);
			c++;
		}

	}

	if (split == "skewed")
	{
		calculateSKEWED();
		for (int i = 0; i < help.size(); i++)
		{
			file = "split-";
			file += std::to_string(c) + ".txt";
			fout.open(file.c_str());
			for (int h = 0; h < help[i]; h++)
			{
				getline(fin, line);
				fout << line << endl;
			}
			fout.close();
			loadVec(file);
			printMAP(c);
			c++;
		}
	}
	fin.close();
}

/**/




void process(string filename)
{
	ifstream fin;
	ofstream fout;
	fin.open(filename.c_str());
	std::string str;
	std::string line = " ";
	fout.open("output.txt");
	int count = 0;
	int co = 0;

	while (getline(fin, str)) // read a line at a time
	{
		//N++;
		//cout << "1." << str;
		//cout << str.length() << endl;
		count = 0;
		co = 0;

		while (str.length() == 0)//moves to next line if the line is empty ie has a space.
		{
			getline(fin, str);
		}

		for (int k = 0; k < str.size(); k++)
		{
			if (str.at(k) == ' ')
			{
				count++;
			}
			//cout << "Count:" << count << endl;
			if ((count) == str.size())
			{
				//cout << str << "1";
				getline(fin, str);
				//cout << count << endl;
				k = 0;
				count = 0;
			}
			//
		}
		for (int i = 0; i < str.size(); i++)//to terminate all other special characters except alpabets
		{
			if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')))
			{
				//str.erase(i, 1);
				str[i] = ' ';
				//i--;
			}
		}
		//cout << str << endl;
		//stringstream line(str);


		for (int j = 0; j < str.length(); ++j)//convert to lower case.
		{
			str[j] = tolower(str[j]);
		}

		string red;
		stringstream line2(str);
		while (line2 >> str)
		{
			str += " ";
			red += str;
			co++;
			//N++;
		}
		string blue= red.substr(0, red.size() - 1);
		fout << blue;
		if (co != 0)
		{
			fout << endl;
		}

	}
	fin.close();
	fout.close();
	string outfile = "output.txt";
	//cout << 1;
	COUNT(outfile);
	//loadVec(outfile);
}




int main(int argc, char * argv[])
{
	
	
	//int i, rs;
	/**/
	if (argc < 2)
	{
		exit(EXIT_FAILURE);
	}
	string num = argv[2];
	stringstream geek(num);
	geek >> M;
	pthread_t threads[M];
	
	//cout << argv[3];
	process(argv[1]);
	split("output.txt", argv[3]);
	//print();
	/*
	for (i = 0; i < MAX; i++)
	{
		cout<<("In main: creating thread %d\n", i);
		rs = pthread_create(&threads[i], NULL, printHello, (void*)(intptr_t)i);
		if (rs)
		{
			cout<<("Error creating thread %d\n", i);
			return 1;
		}
	}

	pthread_exit(NULL);
	return 0
	*/
}
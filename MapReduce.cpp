//============================================================================
// Name        : main.cpp
// Author      : Sujjanaraj Muruganandam
// Description : CSE 330 Project2 Map Reduce
//============================================================================

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
#include <semaphore.h>
#include <bits/stdc++.h>
using namespace std;

// Global variables
int N = 0;
int M = 0;
int R = 0;
int masterREDUCE = 1;
int Rcount = 1;
int Mcount = 1;
vector<string> load;
std::vector<int> countW;
int wordCount = 0;
vector<string> visited;
std::vector<int> help;
int fly;
ofstream outfile;
int seeker = 1;
pthread_mutex_t lock_it;

//Function to calculate the map-outs for the skewed function.
void calculateSKEWED()
{

	int num = N;
	int flag = 0;
	for (int i = 0; i < M; i++)
	{
		help.push_back(1);
	}
	num -= M;

	for (int k = 0; k < num; k++)
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
	if (num > 0)
	{
		if (num == 2 && flag == 0)
		{
			help[flag] += 1;
			help[flag + 1] += 1;
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

}
//Function to count the number of words.
void COUNT(string filename)
{
	ifstream fin;
	fin.open(filename.c_str());
	string line;

	while (getline(fin, line))
	{
		N++;
	}
}

//Function to search and return 1 if word found and 0 if its not.
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

//Function to store words according to their count in two separate vectors. One holds the words and the other holds the count.
void processVEC(int c)
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

	string filename = "map-out-";
	filename += std::to_string(seeker) + ".txt";
	outfile.open(filename.c_str());
	for (int f = 0; f < visited.size(); f++)
	{
		if (visited[f] != " ")
		{
			outfile << visited[f] << " " << countW[f];
			outfile << endl;
		}

	}
	seeker++;
	visited.clear();
	countW.clear();
	outfile.close();

}

// Function to seperate words by " " and store them in load function.
void loadVec(std::string filename, int i)
{

	ifstream fin;
	fin.open(filename.c_str());
	string s;
	int j = 0;
	int c = 0;
	int z = 0;
	while (getline(fin, s))
	{
		j = 0;
		c = 0;
		for (int h = 0; h < s.size(); h++)
		{
			c++;
			if ((s.at(h) == ' '))
			{
				load.push_back(s.substr(j, c - 1));
				c = 0;
				j = h + 1;
			}
			if ((h + 1) == s.size())
			{
				load.push_back(s.substr(j, c));
			}
		}
	}
	fin.close();
	visited.clear();
	countW.clear();
	processVEC(i);
}

//Function to create map-outs by calling other functions.
void *printMAP(void *arg)
{
	int c = (intptr_t)arg;
	string file = "split-";
	file += std::to_string(c) + ".txt";
	pthread_mutex_lock(&lock_it);
	loadVec(file, c);
	pthread_mutex_unlock(&lock_it);
	visited.clear();
	countW.clear();
	pthread_exit(NULL);
}

//Function to find a word in a file and perform some actions for the purpose of creating the reduce-outs.
void find(string word, string numW, int check)
{
	int num = 0;
	stringstream geek(numW);
	geek >> num;

	for (int i = 0; i < visited.size(); i++)
	{
		if (visited[i] == word)
		{
			if (check != 0)
			{
				countW[i] += num;
			}
			else
				countW.insert(countW.begin() + i, num);
		}
	}
}

//Function to load the vectors with value from either the map-outs or reduce-outs according to the trigger value.
void loadReduce(int y, int trigger)
{
	ifstream fin;
	string file;
	int fire = 0;//Variable to see if the trigger is for masterREDUCE or reduce and assign the value accordingly.
	if (trigger == 1)
	{
		file = "map-out-";
		file += std::to_string(Mcount) + ".txt";
		fire = Mcount;
	}
	if (trigger == 2)
	{
		file = "reduce-out-";
		file += std::to_string(masterREDUCE) + ".txt";
		fire = masterREDUCE;
	}

	fin.open(file.c_str());
	string s;
	while (fin)
	{
		getline(fin, s);
		for (int h = 0; h < s.size(); h++)
		{
			if ((s.at(h) == ' '))
			{
				visited.push_back(s.substr(0, h));
				int temp = 0;
				string num = s.substr(h, (s.size() - h));
				stringstream geek3(num);
				geek3 >> temp;
				countW.push_back(temp);
				break;
			}
		}

	}
	fin.close();
	for (int g = fire + 1; g <= y; g++)
	{
		if (trigger == 1)
		{
			file = "map-out-";
			file += std::to_string(g) + ".txt";
		}
		if (trigger == 2)
		{
			file = "reduce-out-";
			file += std::to_string(g) + ".txt";
		}
		fin.open(file.c_str());
		while (getline(fin, s))
		{
			for (int h = 0; h < s.size(); h++)
			{
				if ((s.at(h) == ' '))
				{
					int check = search(s.substr(0, h));
					if (check == 1)
					{
						find(s.substr(0, h), s.substr(h, (s.size() - h)), check);
					}
					else
					{
						visited.push_back(s.substr(0, h));
						sort(visited.begin(), visited.end());
						find(s.substr(0, h), s.substr(h, (s.size() - h)), check);
					}
				}
			}
		}
		fin.close();
	}
}

//Function to print all the rreduced words to the respective reduce-outs.
void printReduce(int y)
{
	ofstream fout;
	string file = "reduce-out-";
	file += std::to_string(Rcount) + ".txt";
	fout.open(file.c_str());
	for (int j = 0; j < visited.size(); j++)
	{
		fout << visited[j] << " " << countW[j];
		fout << endl;
	}
	visited.clear();
	countW.clear();
	Rcount++;
	Mcount += y;
}

//Function to reduce the map-outs to the correspoint reduce-outs by the 'R' value.
void *reduceMAP(void *arg)
{
	int y = (intptr_t)arg;
	pthread_mutex_lock(&lock_it);
	loadReduce(y, 1);
	printReduce(y);
	pthread_mutex_unlock(&lock_it);
	visited.clear();
	countW.clear();
	pthread_exit(NULL);
}

//Function to split the files according to the input value M.
void split(string filename, string split)
{
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
	if (split == "equal")
	{

		while (c <= M)
		{
			file = "split-";
			file += std::to_string(c) + ".txt";
			fout.open(file.c_str());
			for (int i = 0; i < (N / M) + k; i++)
			{
				getline(fin, line);
				if (j == 1 && (i + 1) == (N / M))
				{
					k = 0;
				}

				fout << line << endl;

			}
			fout.close();
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
			c++;
		}
	}
	fin.close();
}

//Function to process the files by removing everthing except alpahbets.
void process(string filename)
{
	ifstream fin;
	ofstream fout;
	fin.open(filename.c_str());
	std::string str;
	std::string line = " ";
	fout.open("processed.txt");
	int count = 0;
	int co = 0;

	while (getline(fin, str)) // read a line at a time
	{

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

			if ((count) == str.size())
			{

				getline(fin, str);

				k = 0;
				count = 0;
			}
		}
		for (int i = 0; i < str.size(); i++)//to terminate all other special characters except alpabets
		{
			if (!((str[i] >= 'a' && str[i] <= 'z') || (str[i] >= 'A' && str[i] <= 'Z')))
			{

				str[i] = ' ';

			}
		}


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
		}
		string blue = red.substr(0, red.size() - 1);
		fout << blue;
		if (co != 0)
		{
			fout << endl;
		}

	}
	fin.close();
	fout.close();
	string outfile = "processed.txt";
	COUNT(outfile);

}

//Function to compute and output all the words in reduce files as final word count.
void *reduceFINAL(void *)
{
	pthread_mutex_lock(&lock_it);
	loadReduce(R, 2);

	for (int j = 0; j < visited.size(); j++)
	{
		cout << visited[j] << " " << countW[j];
		cout << endl;
	}
	visited.clear();
	countW.clear();
	pthread_mutex_unlock(&lock_it);
	pthread_exit(NULL);
}

//Main function
int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		exit(EXIT_FAILURE);
	}
	//To store R in int
	string num2 = argv[4];
	stringstream geek2(num2);
	geek2 >> R;

	//To store M in int
	string num = argv[2];
	stringstream geek(num);
	geek >> M;

	pthread_t threads[M];//Create threads of M map-outs
	pthread_t reduce[R];//Create threads of R reduce-outs
	pthread_t master[1];//Create master thread to print all the reduce files
	pthread_attr_t attr;
	string file;
	int rs, rs2, rs3;
	void *status;
	long t;
	int i;
	int k = 0;
	int h = 1;

	//Call function process and split to process and split the files.
	process(argv[1]);
	split("processed.txt", argv[3]);

	//Create threads for map-outs and reduce-outs.
	for (i = 1; i <= M; i++)
	{
		k++;
		rs = pthread_create(&threads[i], NULL, printMAP, (void*)(intptr_t)i);
		if ((M / R) == k)
		{
			k = 0;
			rs2 = pthread_create(&reduce[h], NULL, reduceMAP, (void*)(intptr_t)i);
			h++;
		}
		if (rs)
		{
			return 1;
		}
	}
	//Master thread that stdout all the reduce files in a final word count.
	for (int y = 0; y < 1; y++)
		rs3 = pthread_create(&master[y], NULL, reduceFINAL, NULL);

	pthread_exit(NULL);
	return 0;
}

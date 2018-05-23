#include <iostream>
#include "elevator.h"
#include <cstdio>
#include <cmath>
#include <algorithm>
#include <queue>
#include <string>
#include <fstream>
//#include "run.cpp"
using namespace std;
using namespace myelevator;
void chang(int argc, char *argv[])
{
	for(int i = 0;i < argc;i++)
	{
		int l = strlen(argv[i]);
		for(int j = 0;j < l;j++)
		{
			if(argv[i][j] > 'A' && argv[i][j] < 'Z')
				argv[i][j] = argv[i][j] - 'A' + 'a';
		}
	}
	return;
}
int main(int argc, char *argv[])
{
	chang(argc, argv);
	for(int i = 1;i < argc;i++)
	{
		if(strcmp(argv[i], "run") == 0)
		{
            bool flag = false;
            if(argv[i+1][0] == '-')
            {
                i++;
                if(argv[i][1] == 'v')
                    flag = true;
                if(argv[i][1] == 'd')
                    flag = false;
            }
            string film = argv[++i];
            ifstream Film;
            Film.open(film, ios::in);
            if(!Film)
            {
                Film.close();
                cout << "The film does not exist" << endl;
                break;
            }
            if(argc > i+1 && argv[i+1][0] == '-')
            {
                if (argv[i + 1][1] == 'c')
                {
                    if(argc - i < 5)
                    {
                        cout << "The films is too few" << endl;
                        break;
                    }
                    if(argc - i > 5)
                    {
                        cout << "The films is too more" << endl;
                        break;
                    }
                    string out1 = argv[i+2];
                    string out2 = argv[i+3];
                    string out3 = argv[i+4];
                    if(flag)
                        Run::global_run_main(film, out1, out2, out3);
                    else
                        Run::distribution_run_main(film, out1, out2, out3);
                    break;
                }
            }
			else
			{
				Film.close();
                if(flag)
                    Run::global_run_main(film);
                else
				    Run::distribution_run_main(film);
			}
			break;
		}
		if(strcmp(argv[i], "view") == 0)
		{
			string film = argv[i+1];
			ifstream Film;
			Film.open(film, ios::in);
			if(!Film)
			{
				Film.close();
				cout << "The film does not exist" << endl;
			}
			else
			{
				string temp;
				getline(Film, temp);
                int l = temp.size();
                for(int i = 0;i < l;i++)
                    if(temp[i] == '\r')
                        temp[i] = '\n';
                cout << temp << endl;
				Film.close();
			}
			break;
		}
        if(strcmp(argv[i], "-help") == 0)
        {
            cout << "       \"run\": To running the elevator." << endl
                 << "       \"view\": To seeing the documents in the Terminal" << endl
                 << "       \"-C\":To customizing the location of output file" << endl
                 << "       \"-D\":Using the Global-DFS-Algorithm" << endl
                 << "       \"-V\":Using the Distribution-DFS-Algorithm" << endl;
            break;
        }
		cout << argv[i] << ": " << "Command not found. Enter \"-help\" to get the help." << endl;
		break;
	}
	return 0;
}
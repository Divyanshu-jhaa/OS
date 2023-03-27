#include <iostream>
#include <fstream>
using namespace std;

class OS
{
private:
    char M[100][4];
    char IR[4];
    char R[4];
    int IC;
    int SI;
    bool C;
    char buffer[40];

public:
    void init();
    void LOAD();
    void Execute();
    void MOS();

    fstream infile;
    fstream outfile;
};

void OS::init()
{

    for (int i = 0; i < 100; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            M[i][j] = ' ';
        }
    }

    IR[0] = {' '};
    R[0] = {' '};
    C = false;
}

void OS::MOS()
{

    if (SI == 1)
    {
        for (int i = 0; i <= 39; i++)
            buffer[i] = '\0';

        infile.getline(buffer, 40);

        int k = 0;
        int i = IR[2] - 48;
        i = i * 10;

        for (int l = 0; l < 10; ++l)
        {
            for (int j = 0; j < 4; ++j)
            {
                M[i][j] = buffer[k];
                k++;
            }
            if (k == 40)
            {
                break;
            }
            i++;
        }

        for (int i = 0; i < 100; i++)
        {
            cout << "M[" << i << "]\t";
            for (int j = 0; j < 4; j++)
            {
                cout << M[i][j];
            }
            cout << endl;
        }
    }
    else if (SI == 2)
    {
        for (int i = 0; i <= 39; i++)
            buffer[i] = '\0';

        int k = 0;
        int i = IR[2] - 48;
        i = i * 10;

        for (int l = 0; l < 10; ++l)
        {
            for (int j = 0; j < 4; ++j)
            {
                buffer[k] = M[i][j];
                if (buffer[k] != NULL)
                {
                    outfile << buffer[k];
                }

                k++;
            }
            if (k == 40)
            {
                break;
            }
            i++;
        }
        for (int i = 0; i < 100; i++)
        {
            cout << "M[" << i << "]\t";
            for (int j = 0; j < 4; j++)
            {
                cout << M[i][j];
            }
            cout << endl;
        }

        outfile << "\n";
    }
    else if (SI == 3)
    {

        outfile << "\n";
        outfile << "\n";
    }
}

void OS::Execute()
{
    while (true)
    {
        for (int i = 0; i < 4; i++)
        {
            IR[i] = M[IC][i];
        }
        cout << endl;
        IC++;

        if (IR[0] == 'G' && IR[1] == 'D')
        {
            SI = 1;
            MOS();
        }
        else if (IR[0] == 'P' && IR[1] == 'D')
        {
            SI = 2;
            MOS();
        }
        else if (IR[0] == 'H')
        {
            SI = 3;
            MOS();
            break;
        }
        else if (IR[0] == 'L' && IR[1] == 'R')
        {
            int i = IR[2] - 48;
            i = i * 10 + (IR[3] - 48);

            for (int j = 0; j <= 3; j++)
                R[j] = M[i][j];

            // for(int j=0;j<=3;j++)
            //  cout<<R[j];

            cout << endl;
        }
        else if (IR[0] == 'S' && IR[1] == 'R')
        {
            int i = IR[2] - 48;
            i = i * 10 + (IR[3] - 48);

            for (int j = 0; j <= 3; j++)
                M[i][j] = R[j];

            cout << endl;
        }
        else if (IR[0] == 'C' && IR[1] == 'R')
        {
            int i = IR[2] - 48;
            i = i * 10 + (IR[3] - 48);
            int count = 0;

            for (int j = 0; j <= 3; j++)
                if (M[i][j] == R[j])
                    count++;

            if (count == 4)
                C = true;
            else
                C = false;
        }
        else if (IR[0] == 'B' && IR[1] == 'T')
        {
            if (C == true)
            {
                int i = IR[2] - 48;
                i = i * 10 + (IR[3] - 48);

                IC = i;
            }
        }
    }
}

void OS::LOAD()
{

    cout << "Reading Data..." << endl;
    int x = 0;
    do
    {
        for (int i = 0; i <= 39; i++)
            buffer[i] = '\0';

        infile.getline(buffer, 40);

        if (buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'J')
        {
            init();
        }
        else if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'T' && buffer[3] == 'A')
        {
            IC = 0;
            Execute();
        }
        else if (buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D')
        {
            x = 0;
            continue;
        }
        else
        {
            int k = 0;

            for (; x < 100; ++x)
            {
                for (int j = 0; j < 4; ++j)
                {
                    M[x][j] = buffer[k];
                    if (buffer[k] == 'H')
                    {
                        for (j = 1; j < 4; j++)
                        {
                            M[x][j] = '0';
                        }
                    };
                    k++;
                }

                if (k == 40 || buffer[k] == '\0' || buffer[k] == '\n')
                {
                    break;
                }
            }
        }

    } while (!infile.eof());
}

int main()
{
    OS os;

    os.infile.open("input.txt", ios::in);
    os.outfile.open("output.txt", ios::out);

    if (!os.infile)
    {
        cout << "Failure" << endl;
    }
    else
    {
        cout << "File Exist" << endl;
    }

    os.LOAD();

    return 0;
}

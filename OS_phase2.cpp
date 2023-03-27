#include <iostream>
#include <fstream>
#include <bits/stdc++.h>
using namespace std;

class OS
{
private:
    char M[300][4];
    char IR[4];
    char R[4];
    int IC;
    int SI;
    bool C;
    char buffer[40];
    int PTR;
    int RA;
    int VA;
    int PI;
    struct PCB
    {
        int jobId;
        int TTC;
        int LLC;
        int TTL;
        int TLL;
    };
    struct PCB pcb;
    unordered_map<int, string> errors = {
        {0, "No Error"},
        {1, "Out of Data"},
        {2, "Line Limit Exceeded"},
        {3, "Time Limit Exceeded"},
        {4, "Operation Code Error"},
        {5, "Operand Error"},
        {6, "Invalid Page Fault"}
        };

public:
    void init();
    void LOAD();
    void Execute();
    void MOS();
    int allocate();
    int AddressMap(int VA);
    void terminate(int code);
    void print_memory();

    fstream infile;
    fstream outfile;
};

void OS::print_memory()
{
    cout << "\nMEMORY: \n";
    for (int i = 0; i < 300; i++)
    {
        cout << "M[" << i << "]\t";
        for (int j = 0; j < 4; j++)
        {
            cout << M[i][j];
        }
        cout << "\n";
    }
}

void OS::terminate(int Code){                    // print in file cause of termination in case of error
    cout<<"\n"<< errors[Code]<<endl;
    outfile<<"\nProgram Terminated abnormally"<<endl;
    outfile<<errors[Code]<<"\n\n";
}

void OS::init()
{
    for (int i = 0; i < 300; i++)
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

int OS::allocate()
{
    return (rand() % 30);
}

int OS::AddressMap(int VA)
{
    int pte = PTR + VA / 10;
    string temp = "";
    if (M[pte][0] != '0')
    {
        cout << "Page Fault" << endl;
        return -1;
    }
    else
    {
        for (int i = 0; i < 4; i++)
        {

            temp += M[pte][i];
        }
        return ((stoi(temp) * 10) + (VA % 10));
    }
}

void OS::Execute()
{
    while (true)
    {
        RA = AddressMap(IC);
        for (int i = 0; i < 4; i++)
        {
            IR[i] = M[RA][i];
        }
        IC++;
        int add = IR[2] - 48;
        add = (add * 10) + (IR[3] - 48);

        if (IR[0] == 'L' && IR[1] == 'R')
        {
            int ra = AddressMap(add);
            for (int i = 0; i < 4; i++)
            {
                R[i] = M[ra][i];
            }
            cout << "Contents of the register" << endl;
            for (int j = 0; j < 4; j++)
            {
                cout << R[j] << endl;
            }
        }
        else if (IR[0] == 'S' && IR[1] == 'R')
        {
            int ra = AddressMap(add);
            if (ra != -1)
            {
                for (int i = 0; i < 4; i++)
                    M[ra][i] = R[i];
            }
            else
            {
                int frame = allocate();
                int i = PTR;
                while (M[i][0] != '#')
                {
                    i++;
                }
                int temp = frame / 10;
                M[i][0] = '0';
                M[i][1] = '0';
                M[i][2] = temp + 48;
                M[i][3] = frame % 10 + 48;
                frame = frame * 10;
                for (int i = 0; i < 4; i++)
                    M[frame][i] = R[i];
            }
        }
        else if (IR[0] == 'C' && IR[1] == 'R')
        {
            //
        }
        else if (IR[0] == 'B' && IR[1] == 'T')
        {
            //
        }
        else if (IR[0] == 'G' && IR[1] == 'D')
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
    }
}

void OS::MOS()
{
    if (SI == 1)
    {
        int frame_no = allocate(), tcount = PTR, k = 0;
        while (M[tcount][0] != '#')
            tcount++;
        M[tcount][0] = '0';
        M[tcount][1] = '0';
        M[tcount][2] = (frame_no / 10) + '0';
        M[tcount][3] = (frame_no % 10) + '0';

        for (int i = 0; i <= 39; i++)
            buffer[i] = ' ';
        infile.getline(buffer, 40);

        for (int i = frame_no * 10; k <= 39; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                M[i][j] = buffer[k];
                k++;
            }

            if (k == 40 || buffer[k] == ' ' || buffer[k] == '\n')
                break;
        }
    }
    else if (SI == 2)
    {
        int add = IR[2] - 48;
        add = (add * 10);
        int ra = AddressMap(add);
        if (ra != -1)
        {
            string out;
            for (int i = 0; i < 10; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    out += M[ra][j];
                }
                ra += 1;
            }
            outfile << out << "\n";
        }
    }
    else if (SI == 3)
    {
        outfile << "\n"
                << "\n";
    }
}

void OS::LOAD()
{
    cout << "Reading Data..." << endl;
    do
    {
        for (int i = 0; i <= 39; i++)
            buffer[i] = ' ';
        infile.getline(buffer, 40);
        if (buffer[0] == '$' && buffer[1] == 'A' && buffer[2] == 'M' && buffer[3] == 'J')
        {
            init();
            pcb.jobId = 0;
            pcb.TLL = 0;
            pcb.TTC = 0;
            pcb.LLC = 0;
            pcb.TTL = 0;
            for (int i = 4, j = 3; i < 8; i++, j--)
            {
                pcb.jobId += ((buffer[i] - 48) * pow(10, j));
            }
            for (int i = 8, j = 3; i < 12; i++, j--)
            {
                pcb.TTL += ((buffer[i] - 48) * pow(10, j));
            }
            for (int i = 12, j = 3; i < 16; i++, j--)
            {
                pcb.TLL += (buffer[i] - 48) * pow(10, j);
            }
            PTR = allocate() * 10;
            for (int i = PTR; i < PTR + 10; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    M[i][j] = '#';
                }
            }
            cout << "Job ID: " << pcb.jobId << "\n";
            cout << "TTL: " << pcb.TTL << "\n";
            cout << "TLL: " << pcb.TLL << "\n";
            cout << "PTR: " << PTR << "\n";
        }
        else if (buffer[0] == '$' && buffer[1] == 'D' && buffer[2] == 'T' && buffer[3] == 'A')
        {
            IC = 0;
            Execute();
        }
        else if (buffer[0] == '$' && buffer[1] == 'E' && buffer[2] == 'N' && buffer[3] == 'D')
        {
            continue;
        }
        else
        {
            int frame_no = allocate(), tcount = PTR, k = 0;
            while (M[tcount][0] != '#')
                tcount++;
            M[tcount][0] = '0';
            M[tcount][1] = '0';
            M[tcount][2] = (frame_no / 10) + '0';
            M[tcount][3] = (frame_no % 10) + '0';

            for (int i = frame_no * 10; k <= 39; i++)
            {
                for (int j = 0; j < 4; j++)
                {
                    M[i][j] = buffer[k];
                    if (buffer[k] == 'H')
                    {
                        for (j = 1; j < 4; j++)
                        {
                            M[i][j] = '0';
                        }
                    }
                    k++;
                }

                if (k == 40 || buffer[k] == ' ' || buffer[k] == '\n')
                    break;
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
    os.print_memory();
    return 0;
}
#include <iostream>
#include <chrono>
#include <ctime>
#include <ratio>
#include <ncurses.h>
#include <cstdlib>
#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include <cmath>
#include <fstream>

using namespace std;
using namespace chrono;

int height, length;
int **board;
int comingshape[4][4];
int randshape = 0, randshape2 = 2; // buuuug
int iEra, jEra;
double spf; // second per frame
double timer = 0;
int score = 0;
int totalPlayers; // for file
string name;
int level;

struct Player
{
    string name;
    int score;
    double time;
    int level;
};

bool Kbhit()
{
    struct termios oldt, newt;
    int ch;
    int oldf;

    // Save the current terminal attributes
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Set the terminal to non-canonical mode (disable line buffering)
    newt.c_lflag &= ~(ICANON | ECHO);

    // Apply the new terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Set the file descriptor for stdin to non-blocking
    oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
    fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

    // Attempt to read a character from stdin
    ch = getchar();

    // Restore the original terminal settings and file descriptor flags
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
    fcntl(STDIN_FILENO, F_SETFL, oldf);

    // Check if a character was read
    if (ch != EOF)
    {
        ungetc(ch, stdin);
        return true; // A key was pressed
    }

    return false; // No key was pressed
}
char MyGetch()
{
    char ch;
    struct termios oldt, newt;

    // Get the current terminal settings
    tcgetattr(STDIN_FILENO, &oldt);
    newt = oldt;

    // Set the terminal to non-canonical mode and turn off echo
    newt.c_lflag &= ~(ICANON | ECHO);

    // Apply the new settings
    tcsetattr(STDIN_FILENO, TCSANOW, &newt);

    // Read a single character
    ch = getchar();

    // Restore the original terminal settings
    tcsetattr(STDIN_FILENO, TCSANOW, &oldt);

    return ch;
}
void LeftMove()
{
    bool leftmove = true;
    for (int i = 0; i < height; i++)
    {
        if (board[i][0] == 1)
            leftmove = false;
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 1; j < length; j++)
        {
            if (board[i][j] == 1)
            {
                if (board[i][j - 1] > 20)
                    leftmove = false;
            }
        }
    }
    if (leftmove == true)
    {
        for (int j = 1; j < length; j++)
        {
            for (int i = 0; i < height; i++)
            {
                if (board[i][j] == 1)
                {
                    board[i][j - 1] = 1;
                    board[i][j] = 0;
                }
            }
        }
        jEra--;
    }
}
void RightMove()
{
    bool rightmove = true;
    for (int i = 0; i < height; i++)
    {
        if (board[i][length - 1] == 1)
            rightmove = false;
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = length - 2; j >= 0; j--)
        {
            if (board[i][j] == 1)
            {
                if (board[i][j + 1] > 20)
                    rightmove = false;
            }
        }
    }
    if (rightmove == true)
    {
        for (int j = length - 2; j >= 0; j--)
        {
            for (int i = 0; i < height; i++)
            {
                if (board[i][j] == 1)
                {
                    board[i][j + 1] = 1;
                    board[i][j] = 0;
                }
            }
        }
        jEra++;
    }
}
void DownMove(bool &downmove)
{
    for (int j = 0; j < length; j++) 
    {
        if (board[height - 1][j] == 1)
            downmove = false;
    }
    for (int j = 0; j < length; j++)
    {
        for (int i = 0; i < height - 1; i++)
        {
            if (board[i][j] == 1)
            {
                if (board[i + 1][j] > 20)
                {
                    downmove = false;
                }
            }
        }
    }
    if (downmove == true)
    {
        for (int j = 0; j < length; j++)
        {
            for (int i = height - 2; i >= 0; i--)
            {
                if (board[i][j] == 1)
                {
                    board[i + 1][j] = 1;
                    board[i][j] = 0;
                }
            }
        }
        iEra++;
    }

    if (downmove == false)
    {
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < length; j++)
            {
                if (board[i][j] == 1)
                    board[i][j] = 20 + randshape;
            }
        }
    }
}
void PrintBoard()
{
    system("clear");
    cout << "Next Shape:" << endl
         << endl;
    for (int i = 0; i < 4; i++)
    {
        for (int j = 0; j < 4; j++)
        {
            if (comingshape[i][j] == 1)
            {
                switch (randshape2)
                {
                case 1:
                    cout << "\x1b[41m "
                            " \x1b[0m";
                    break;
                case 2:
                    cout << "\x1b[42m "
                            " \x1b[0m";
                    break;
                case 3:
                    cout << "\x1b[43m "
                            " \x1b[0m";
                    break;
                case 4:
                    cout << "\x1b[44m "
                            " \x1b[0m";
                    break;
                case 5:
                    cout << "\x1b[45m "
                            " \x1b[0m";
                    break;
                case 6:
                    cout << "\x1b[46m "
                            " \x1b[0m";
                    break;
                case 7:
                    cout << "\x1b[47m "
                            " \x1b[0m";
                    break;
                }
            }
            else
                cout << "  ";
        }
        cout << endl;
    }
    cout << "\u256D";
    for (int k = 0; k < length; k++)
    {
        cout << "\u2500\u2500";
    }
    cout << "\u256E";
    cout << endl;
    for (int i = 1; i < height; i++)
    {
        cout << "\u2502";
        for (int j = 0; j < length; j++)
        {
            if (board[i][j] > 20)
            {
                switch (board[i][j])
                {
                case 21:
                    cout << "\x1b[41m "
                            " \x1b[0m";
                    break;
                case 22:
                    cout << "\x1b[42m "
                            " \x1b[0m";
                    break;
                case 23:
                    cout << "\x1b[43m "
                            " \x1b[0m";
                    break;
                case 24:
                    cout << "\x1b[44m "
                            " \x1b[0m";
                    break;
                case 25:
                    cout << "\x1b[45m "
                            " \x1b[0m";
                    break;
                case 26:
                    cout << "\x1b[46m "
                            " \x1b[0m";
                    break;
                case 27:
                    cout << "\x1b[47m "
                            " \x1b[0m";
                    break;
                }
            }
            else if (board[i][j] == 1)
            {
                switch (randshape)
                {
                case 1:
                    cout << "\x1b[41m "
                            " \x1b[0m";
                    break;
                case 2:
                    cout << "\x1b[42m "
                            " \x1b[0m";
                    break;
                case 3:
                    cout << "\x1b[43m "
                            " \x1b[0m";
                    break;
                case 4:
                    cout << "\x1b[44m "
                            " \x1b[0m";
                    break;
                case 5:
                    cout << "\x1b[45m "
                            " \x1b[0m";
                    break;
                case 6:
                    cout << "\x1b[46m "
                            " \x1b[0m";
                    break;
                case 7:
                    cout << "\x1b[47m "
                            " \x1b[0m";
                    break;
                }
            }
            else if (board[i][j] == 11)
                cout << "\x1b[48;5;243m "
                        " \x1b[0m";
            else
                cout << "  ";
        }
        cout << "\u2502" << endl;
    }

    cout << "\u2570";
    for (int k = 0; k < length; k++)
    {
        cout << "\u2500\u2500";
    }
    cout << "\u256F";
    cout << endl;
    cout << endl;
    cout << "Time: " << static_cast<int>(timer) << "   "
         << "Score: " << score << endl;
}
void FullLine()
{
    int countLine = 0;
    for (int i = 0; i < height; i++)
    {
        bool fullline = true;
        for (int j = 0; j < length; j++)
        {
            if (board[i][j] < 20)
                fullline = false;
        }
        if (fullline == true)
        {
            countLine++;
            for (int j = 0; j < length; j++)
            {
                board[i][j] = 0;
                usleep(30 * 1000);
                PrintBoard();
            }
            for (int k = i - 1; k >= 0; k--)
            {
                for (int j = 0; j < length; j++)
                {
                    board[k + 1][j] = board[k][j];
                    board[k][j] = 0;
                }
            }
        }
    }
    if (countLine > 0)
        score = score + (1 + (countLine - 1) * 0.5) * (level * length * length);
}
void ShapeGenerator(bool &endgame)
{
    randshape = randshape2;
    int jcenter = (length / 2) - 1;
    for (int j = 0; j < length; j++)
    {
        if (board[1][j] > 20)
            endgame = true;
    }

    if (endgame == false)
    {
        switch (randshape)
        {
        case 1: // squere
            board[0][jcenter] = 1;
            board[0][jcenter + 1] = 1;
            board[1][jcenter] = 1;
            board[1][jcenter + 1] = 1;

            break;
        case 2: // L shape
            iEra = 0;
            jEra = jcenter + 1;
            board[0][jcenter] = 1;
            board[1][jcenter] = 1;
            board[1][jcenter + 1] = 1;
            board[1][jcenter + 2] = 1;
            break;
        case 3: // L reverse
            iEra = 0;
            jEra = jcenter + 1;
            board[0][jcenter + 2] = 1;
            board[1][jcenter] = 1;
            board[1][jcenter + 1] = 1;
            board[1][jcenter + 2] = 1;
            break;
        case 4: // Straight shape
            iEra = 0;
            jEra = jcenter;
            board[1][jcenter - 1] = 1;
            board[1][jcenter] = 1;
            board[1][jcenter + 1] = 1;
            board[1][jcenter + 2] = 1;
            break;
        case 5: // Z shape
            iEra = 1;
            jEra = jcenter + 1;
            board[0][jcenter] = 1;
            board[0][jcenter + 1] = 1;
            board[1][jcenter + 1] = 1;
            board[1][jcenter + 2] = 1;
            break;
        case 6: // Z reverse
            iEra = 1;
            jEra = jcenter + 1;
            board[0][jcenter + 2] = 1;
            board[0][jcenter + 1] = 1;
            board[1][jcenter + 1] = 1;
            board[1][jcenter] = 1;
            break;
        case 7: // midle finger shape
            iEra = 1;
            jEra = jcenter + 1;
            board[0][jcenter + 1] = 1;
            board[1][jcenter] = 1;
            board[1][jcenter + 1] = 1;
            board[1][jcenter + 2] = 1;
            break;
        }
        srand(static_cast<unsigned int>(time(nullptr)));
        randshape2 = rand() % 6;
        randshape2 += rand() % 2 + 1;
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                comingshape[i][j] = 0;
            }
        }
        switch (randshape2)
        {
        case 1: // squere
            comingshape[0][1] = 1;
            comingshape[0][1 + 1] = 1;
            comingshape[1][1] = 1;
            comingshape[1][1 + 1] = 1;

            break;
        case 2: // L shape
            comingshape[0][1] = 1;
            comingshape[1][1] = 1;
            comingshape[1][1 + 1] = 1;
            comingshape[1][1 + 2] = 1;
            break;
        case 3: // L reverse
            comingshape[0][1 + 2] = 1;
            comingshape[1][1] = 1;
            comingshape[1][1 + 1] = 1;
            comingshape[1][1 + 2] = 1;
            break;
        case 4: // Straight shape
            comingshape[1][1 - 1] = 1;
            comingshape[1][1] = 1;
            comingshape[1][1 + 1] = 1;
            comingshape[1][1 + 2] = 1;
            break;
        case 5: // Z shape
            comingshape[0][1] = 1;
            comingshape[0][1 + 1] = 1;
            comingshape[1][1 + 1] = 1;
            comingshape[1][1 + 2] = 1;
            break;
        case 6: // Z reverse
            comingshape[0][1 + 2] = 1;
            comingshape[0][1 + 1] = 1;
            comingshape[1][1 + 1] = 1;
            comingshape[1][1] = 1;
            break;
        case 7: // midle finger shape
            comingshape[0][1 + 1] = 1;
            comingshape[1][1] = 1;
            comingshape[1][1 + 1] = 1;
            comingshape[1][1 + 2] = 1;
            break;
        }
    }
}
void Rotation()
{
    int copyBoard[height][length];
    bool rotation = true;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < length; j++)
        {
            copyBoard[i][j] = board[i][j];
            if (board[i][j] == 1)
            {
                if (iEra - (jEra - j) > height - 1 || iEra - (jEra - j) < 1 || jEra + (iEra - i) > length - 1 || jEra + (iEra - i) < 0)
                    rotation = false;
                else if (board[iEra - (jEra - j)][jEra + (iEra - i)] > 20)
                {
                    rotation = false;
                }
            }
        }
    }
    if (rotation == true)
    {
        cout << "\nrotation true test\n";
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < length; j++)
            {
                if (board[i][j] == 1)
                {
                    copyBoard[iEra - (jEra - j)][jEra + (iEra - i)] = 3;
                }
            }
        }
        for (int i = 0; i < height; i++)
        {
            for (int j = 0; j < length; j++)
            {
                if (copyBoard[i][j] == 1)
                    copyBoard[i][j] = 0;
                if (copyBoard[i][j] == 3)
                {
                    copyBoard[i][j] = 1;
                }
                board[i][j] = copyBoard[i][j];
            }
        }
    }
}
void ShadowShape()
{
    int copyBoard[height][length];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < length; j++)
        {
            if (board[i][j] == 11)
                board[i][j] = 0;

            if (board[i][j] == 1)
                copyBoard[i][j] = 11;
            else
                copyBoard[i][j] = board[i][j];
        }
    }
    bool shadowmove = true;
    while (shadowmove == true)
    {
        for (int j = 0; j < length; j++)
        {
            if (copyBoard[height - 1][j] == 11 || board[1][j] > 20)
                shadowmove = false;
        }
        for (int j = 0; j < length; j++)
        {
            for (int i = 0; i < height - 1; i++)
            {
                if (copyBoard[i][j] == 11)
                {
                    if (copyBoard[i + 1][j] > 20)
                    {
                        shadowmove = false;
                    }
                }
            }
        }
        if (shadowmove == true)
        {
            for (int j = 0; j < length; j++)
            {
                for (int i = height - 2; i >= 0; i--)
                {
                    if (copyBoard[i][j] == 11)
                    {
                        copyBoard[i + 1][j] = 11;
                        copyBoard[i][j] = 0;
                    }
                }
            }
        }
    }
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < length; j++)
        {
            if (copyBoard[i][j] == 11 && board[i][j] == 0)
                board[i][j] = 11;
        }
    }
}
// file for leaderboard ______________________________________________________________________________________________________________
void BubbleSort(Player *&leaderBoard)
{
    for (int i = 0; i < totalPlayers; i++)
    {
        for (int j = 0; j < totalPlayers - 1; j++)
        {
            if (leaderBoard[j].score < leaderBoard[j + 1].score)
            {
                swap(leaderBoard[j], leaderBoard[j + 1]);
            }
            else if (leaderBoard[j].score == leaderBoard[j + 1].score && leaderBoard[j].time > leaderBoard[j + 1].time)
            {
                swap(leaderBoard[j], leaderBoard[j + 1]);
            }
        }
    }
}
void ReadFromFile(Player *&leaderBoard)
{
    int i = 0;
    string playerName;
    int playerScore, level;
    double time;
    ifstream file("leaderboard.txt");
    if (file.is_open())
    {
        while (file >> playerName >> playerScore >> time >> level)
        {
            leaderBoard[i].name = playerName;
            leaderBoard[i].score = playerScore;
            leaderBoard[i].time = time;
            leaderBoard[i].level = level;
            i++;
        }
        file.close();
    }
}
void WriteToFile(Player *&leaderBoard)
{
    ofstream file("leaderboard.txt");
    for (int i = 0; i < totalPlayers; ++i)
    {
        file << leaderBoard[i].name << " " << leaderBoard[i].score << " " << leaderBoard[i].time << ' ' << leaderBoard[i].level << endl;
    }
    file.close();
}
// read from file and count the amount of lines(players)
void PlayerCount()
{
    string playerName, playerScore, time, level;
    ifstream file("leaderboard.txt", ios::in);
    if (file.is_open())
    {
        while (file >> playerName >> playerScore >> time >> level)
        {
            totalPlayers++;
        }
        file.close();
    }
}
// in this function if any same name with same board size is available , it will give me it's address if not will give me -1
int SamePlayerAddress()
{
    if (totalPlayers != 0)
    {
        Player *temp = new Player[totalPlayers]; //????????
        ReadFromFile(temp);
        for (int i = 0; i < totalPlayers; i++)
        {
            if (name == temp[i].name && level == temp[i].level)
                return i;
        }
        return -1;
    }
    return -1;
}
void AppendToLeaderboard(Player *&leaderBoard)
{
    PlayerCount();
    int address = SamePlayerAddress();
    Player *temp = new Player[totalPlayers];
    ReadFromFile(temp);
    if (address >= 0)
    {
        bool isBetter = false;
        if (temp[address].score < score)
        {
            leaderBoard = new Player[totalPlayers];
            ReadFromFile(leaderBoard);
            leaderBoard[address].score = score;
            leaderBoard[address].time = timer;
            isBetter = true;
        }
        if (isBetter)
            BubbleSort(leaderBoard);
        else
        {
            leaderBoard = new Player[totalPlayers];
            ReadFromFile(leaderBoard);
        }
    }
    else
    {
        totalPlayers++;
        leaderBoard = new Player[totalPlayers];
        ReadFromFile(leaderBoard);
        leaderBoard[totalPlayers - 1].name = name;
        leaderBoard[totalPlayers - 1].score = score;
        leaderBoard[totalPlayers - 1].time = timer;
        leaderBoard[totalPlayers - 1].level = level;
        BubbleSort(leaderBoard);
    }
}
void PrintFromFile()
{
    int i = 1;
    string playerName;
    int playerScore;
    double time;
    char inpDif;
    bool cycle = true;
    cout << "1:Easy       2:Medium       3:Hard\n";
    while (cycle)
    {
        cin >> inpDif;
        system("clear");
        cout << "1:Easy       2:Medium       3:Hard\n";
        for (int i = 0; i < 40; i++)
            cout << "\u2550";
        cout << endl;
        cout << "          "
             << " name "
             << " score "
             << "  time " << endl;
        ifstream file("leaderboard.txt", ios::in);
        while (file >> playerName >> playerScore >> time >> level)
        {
            if (inpDif - '0' == level)
            {
                cout << i << "  - " << setw(10) << playerName << setw(6)
                     << playerScore << setw(10) << time << endl;
                i++;
            }
        }
        file.close();
        i = 1;
        for (int i = 0; i < 40; i++)
            cout << "\u2550";
        cout << endl;
        cout << "Press e to Exit\n";
        if (inpDif == 'e')
        {
            cycle = false;
        }
    }
    system("clear");
}
//____________________________________________________________________________________________________________________________________
void printGameOver()
{
    cout << "\x1b[31m ██████╗  █████╗ ███╗   ███╗███████╗ ██████╗ ██╗   ██╗███████╗██████╗ \x1b[0m\n";
    usleep(300000);
    cout << "\x1b[32m██╔════╝ ██╔══██╗████╗ ████║██╔════╝██╔═══██╗██║   ██║██╔════╝██╔══██╗\x1b[0m\n";
    usleep(300000);
    cout << "\x1b[33m██║  ███╗███████║██╔████╔██║█████╗  ██║   ██║██║   ██║█████╗  ██████╔╝\x1b[0m\n";
    usleep(300000);
    cout << "\x1b[34m██║   ██║██╔══██║██║╚██╔╝██║██╔══╝  ██║   ██║╚██╗ ██╔╝██╔══╝  ██╔══██╗\x1b[0m\n";
    usleep(300000);
    cout << "\x1b[35m╚██████╔╝██║  ██║██║ ╚═╝ ██║███████╗╚██████╔╝ ╚████╔╝ ███████╗██║  ██║\x1b[0m\n";
    usleep(300000);
    cout << "\x1b[36m ╚═════╝ ╚═╝  ╚═╝╚═╝     ╚═╝╚══════╝ ╚═════╝   ╚═══╝  ╚══════╝╚═╝  ╚═╝\x1b[0m\n";
    usleep(2000000);
}
void HowToPlay()
{
    system("clear");
    cout << "w: Rotation\nd: Move shape to right\ns: Move shape down and gain 1 score for each use\n";
    cout << "a: Move shape to left\nspace: move shape to it's shadow\np: Pause the game\n";
    cout << "If you fullfill the line it will clear aand give you scores; The more lines you full together more score you get\n";
    cout << "If any of first line of board fill with a shape that has been constant you loseeeeee\n";
    cout << "Watch out!! \nAll menu in game work with numbers\n";
    system("read -n 1 -s -p \"Press any key to continue...\"");
    cout << endl;
}
void PlayGame()
{
    bool y = true;
    char diffculty;
    while (y)
    {
        cout << "What level of difficulty you want to play: \n1:Easy \n2:Medium \n3:Hard\n";
        cin >> diffculty;
        switch (diffculty)
        {
        case '1':
            spf = 1.5;
            y = false;
            break;
        case '2':
            spf = 1;
            y = false;
            break;
        case '3':
            spf = 0.5;
            y = false;
            break;
        default:
            break;
        }
    }
    level = diffculty - '0';
    cout << "Enter your name: \n";
    cin >> name;
    cout << "Enter the Height of board:\n";
    cin >> height;
    cout << "Enter the Length of board:\n";
    cin >> length;
    height = height + 1;
    board = new int *[height];
    for (int i = 0; i < height; i++)
    {
        board[i] = new int[length];
        for (int j = 0; j < length; j++)
        {
            board[i][j] = 0;
        }
    }
    srand(static_cast<unsigned int>(time(nullptr))); // ساختن کامینگ شیپ
    randshape2 = rand() % 6;
    randshape2 += rand() % 2 + 1;
    bool endgame = false;
    PrintBoard();
    while (endgame == false)
    {
        spf = (2 - (0.5 * level)) * pow(0.85, score / (5 * length * length));
        ShapeGenerator(endgame);
        ShadowShape();
        PrintBoard();
        bool downmove = true;
        time_point<system_clock> start, end;
        double lag = 0;
        bool pause = false, restart = false;
        while (downmove == true && endgame == false)
        {
            start = system_clock::now();
            if (lag >= spf && pause == false)
            {
                DownMove(downmove);
                if (downmove == false)
                    FullLine();
                system("clear");
                PrintBoard();
                lag = 0;
            }
            double pauseTime;
            if (Kbhit())
            {
                char value = MyGetch();
                if (value == 'a')
                {
                    LeftMove();
                    ShadowShape();
                }
                else if (value == 'd')
                {
                    RightMove();
                    ShadowShape();
                }
                else if (value == 's')
                {
                    DownMove(downmove);
                    score++;
                    if (downmove == false)
                        FullLine();
                }
                else if (value == 'w')
                {
                    Rotation();
                    ShadowShape();
                }
                else if (value == ' ')
                {
                    while (downmove == true)
                    {
                        DownMove(downmove);
                        usleep(10 * 1000);
                        PrintBoard();
                        score++;
                    }
                    FullLine();
                }
                else if (value == 'p')
                {
                    pause = true;
                    int userInp;
                    pauseTime = timer;
                    cout << "\n 1- Resume\n 2- Restart\n 3- Exit\n";
                    cin >> userInp;
                    switch (userInp)
                    {
                    case 1:
                        break;
                    case 2:
                        restart = true;
                        for (int i = 0; i < height; i++)
                        {
                            for (int j = 0; j < length; j++)
                                board[i][j] = 0;
                        }
                        downmove = false;
                        timer = 0;
                        score = 0;
                        break;
                    case 3:
                        endgame = true;
                        break;
                    default:
                        break;
                    }
                }
                PrintBoard();
            }
            end = system_clock::now();
            duration<double> elapsed_seconds = end - start;
            timer += elapsed_seconds.count();
            lag += elapsed_seconds.count();
            if (pause == true)
            {
                timer = pauseTime;
                lag = 0;
                pause = false;
            }
            if (restart)
            {
                timer = 0;
                score = 0;
                lag = 0;
                restart = false;
            }
        }
    }
    cout << "End Game" << endl
         << endl;
}
void DisplayMenu()
{
    bool exit = false;
    while (!exit)
    {
        system("clear");
        cout << "\n\x1b[31m████████╗███████╗████████╗██████╗ ██╗███████╗\x1b[0m\n";
        usleep(30000);
        cout << "\x1b[32m╚══██╔══╝██╔════╝╚══██╔══╝██╔══██╗██║██╔════╝\x1b[0m\n";
        usleep(30000);
        cout << "\x1b[33m   ██║   █████╗     ██║   ██████╔╝██║███████╗\x1b[0m\n";
        usleep(30000);
        cout << "\x1b[34m   ██║   ██╔══╝     ██║   ██╔══██╗██║╚════██║\x1b[0m\n";
        usleep(30000);
        cout << "\x1b[35m   ██║   ███████╗   ██║   ██║  ██║██║███████║\x1b[0m\n";
        usleep(30000);
        cout << "\x1b[36m   ╚═╝   ╚══════╝   ╚═╝   ╚═╝  ╚═╝╚═╝╚══════╝\x1b[0m\n";
        cout << "1- New Game \n2- How to Play\n3- Leaderboard\n4- Exit\n○ Enter Menu orders :\n";
        char userInput;
        cin >> userInput;
        switch (userInput)
        {
        case '1':
        {
            PlayGame();
            printGameOver();
            for (int i = 0; i < height; i++)
            {
                delete[] board[i];
            }
            delete[] board;
            Player *leaderBoard;
            AppendToLeaderboard(leaderBoard);
            WriteToFile(leaderBoard);
            delete[] leaderBoard;
            totalPlayers = 0;
            score = 0;
            timer = 0;
            break;
        }
        case '2':
            HowToPlay();
            break;
        case '3':
            PrintFromFile();
            break;
        case '4':
            exit = true;
            break;
        default:
            break;
        }
    }
}

// *************************************************************************************
// *************************************************************************************
// *************************************************************************************
int main()
{
    DisplayMenu();
    return 0;
}
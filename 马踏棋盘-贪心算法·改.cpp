#include <iostream>
#include <iomanip>
#include <windows.h>
using namespace std;
const int chessBoardSize = 8;
const int xd[8] = { -2, -1, 1, 2, 2, 1, -1, -2 };//x跳跃方向
const int yd[8] = { 1, 2, 2, 1, -1, -2, -2, -1 };//y跳跃方向

class Horse//马类
{
public:
    Horse() :x(0), y(0) {}//构造函数
    Horse(int a, int b) :x(a),y(b) {}//构造函数
    void HorseTour();//马踏棋盘功能
private:
    int x;//x坐标
    int y;//y坐标
    
    void Jump(int i);//跳跃
    int SelectDirection(int weight[chessBoardSize][chessBoardSize], int nextStatus[chessBoardSize][chessBoardSize][8]);//选择最小的出口权值方位
};

class ChessBoard//棋盘类
{
private:
    friend class Horse;

    int step[chessBoardSize][chessBoardSize];//马走过的路线
    int weight[chessBoardSize][chessBoardSize];//权值

    ChessBoard();
    void OutChessboard();//输出棋盘上马走过的路线
    void InitWeight();//初始化棋盘上的权值
    void JumpSetWeight(int x, int y);//马跳跃后修改权值
    void RecallSetWeight(int x, int y);//回溯后修改权值
};

class SqStack//顺序栈
{
public:
    void InitStack(int n);//顺序栈初始化
    int Push(Horse e);//入栈
    Horse Pop();//出栈
    int IsEmpty();//判断栈是否为空
    Horse GetTop();//取栈顶元素
private:
    Horse* base;//栈底
    Horse* top;//栈顶
    int stackSize;//栈的大小
};

/*--------------------------栈成员函数--------------------------*/

void SqStack::InitStack(int n)//顺序栈初始化
{
    base = new Horse[n * n];
    if (!base) exit(OVERFLOW);
    top = base;
    stackSize = n * n;
}

int SqStack::Push(Horse e)//入栈
{
    if (top - base == stackSize) return 0;
    else
    {
        *this->top++ = e;//将e放入栈顶，栈顶指针上移
        return 1;
    }
}

Horse SqStack::Pop()//出栈
{
    Horse e;
    if (top == base);
    else
    {
        e = *--this->top;//栈顶指针下移并将栈顶元素赋值给e
        return e;
    }
}

Horse SqStack::GetTop()//取栈顶元素
{
    if (top != base)
    {
        return *(top - 1);
    }
}

int SqStack::IsEmpty()//判断栈是否为空
{
    if (top == base) return 0;
    else return 1;
}

/*----------------------------马类成员函数----------------------------*/

void Horse::Jump(int i)//马的跳跃
{
    x = x + xd[i];
    y = y + yd[i];
}

int Horse::SelectDirection(int weight[chessBoardSize][chessBoardSize], int nextStatus[chessBoardSize][chessBoardSize][8])
{
    int minDirection = 8;//初始化方向，如果没有出口，返回8代表无出口
    int minWT = 9;//较小的权值，初始化为9
    int xb, yb;
    for (int i = 0; i < 8; i++)
    {
        if (nextStatus[x][y][i] == 0)
        {
            xb = x + xd[i];
            yb = y + yd[i];
            if (xb >= 0 && xb < chessBoardSize && yb >= 0 && yb < chessBoardSize && weight[xb][yb] != 9 && weight[xb][yb] < minWT)//每次选择出口权值较小的的方向
            {
                minWT = weight[xb][yb];
                minDirection = i;
            }
        }
    }
    return minDirection;
}

void Horse::HorseTour()//马踏棋盘
{
    int noNum = 1;//定义解法个数
    int stepNum = 1;
    SqStack tour;//用来储存每步坐标的栈
    ChessBoard board;//马所走的棋盘
    board.InitWeight();
    int nextStatus[chessBoardSize][chessBoardSize][8];//定义马是否走过此方向并初始化
    for (int i = 0; i < chessBoardSize; i++)
    {
        for (int j = 0; j < chessBoardSize; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                nextStatus[i][j][k] = 0;
            }
        }
    }
    board.JumpSetWeight(this->x, this->y);
    board.step[x][y] = stepNum;
    tour.InitStack(chessBoardSize);
    tour.Push(*this);//入栈
    while (tour.IsEmpty())//当栈不为空时进行
    {
        int direction;
        *this = tour.GetTop();//取栈顶元素
        direction = this->SelectDirection(board.weight, nextStatus);
        if (stepNum == chessBoardSize * chessBoardSize)//当马踏遍所有格子的时候输出棋盘
        {
            cout << "NO." << noNum << endl;
            board.OutChessboard();
            cout << endl;
            if (noNum == 100)
            {
                break;
            }
            noNum++;
        }
        if (direction == 8)//假如方向为8，说明没有可选的方向，回溯
        {
            for (int i = 0; i < 8; i++)
            {
                nextStatus[x][y][i] = 0;
            }
            board.RecallSetWeight(x, y);
            stepNum--;//步数减一
            tour.Pop();//出栈
        }
        else//当不超过棋盘并且当前格子的状态为没走过时可进行跳跃
        {
            nextStatus[x][y][direction] = 1;
            Jump(direction);
            stepNum++;//步数加一
            board.step[x][y] = stepNum;
            board.JumpSetWeight(x, y);
            tour.Push(*this);//新坐标入栈

        }
    }
}

/*---------------------------棋盘类成员函数---------------------------*/

ChessBoard::ChessBoard()
{
    for (int i = 0; i < chessBoardSize; i++)
    {
        for (int j = 0; j < chessBoardSize; j++)
        {
            step[i][j] = 0;
            weight[i][j] = 0;
        }
    }
}

void ChessBoard::OutChessboard()//输出棋盘
{
    for (int i = 0; i < chessBoardSize; i++)
    {
        for (int j = 0; j < chessBoardSize; j++)
        {
            cout << setw(4) << step[i][j];
        }
        cout << endl;
    }
}

void ChessBoard::InitWeight()
{
    for (int i = 0; i < chessBoardSize; i++)//计算全部格子的周围可达点的数量
    {
        for (int j = 0; j < chessBoardSize; j++)
        {
            for (int k = 0; k < 8; k++)
            {
                int x = i + xd[k];
                int y = j + yd[k];
                if (x >= 0 && x < chessBoardSize && y >= 0 && y < chessBoardSize)
                {
                    weight[i][j]++;
                }
            }
        }
    }
}

void ChessBoard::JumpSetWeight(int x, int y)
{
    for (int i = 0; i < 8; i++)
    {
        int xb = x + xd[i];
        int yb = y + yd[i];
        if (xb >= 0 && xb < chessBoardSize && yb >= 0 && yb < chessBoardSize && weight[xb][yb] != 9)//周围可行点权值减一
        {
            weight[xb][yb]--;
        }
    }
    weight[x][y] = 9;//权值变为9为不可通过位置
}

void ChessBoard::RecallSetWeight(int x, int y)
{
    weight[x][y] = 0;//将回溯点初始化为0
    for (int i = 0; i < chessBoardSize; i++)//重新计算回溯点权值和周围可达点的权值
    {
        int xb = x + xd[i];
        int yb = y + yd[i];
        if (xb >= 0 && xb < chessBoardSize && yb >= 0 && yb < chessBoardSize && weight[xb][yb] != 9)
        {
            weight[x][y]++;
            weight[xb][yb]++;
        }
    }
}

/*------------------------------主函数------------------------------*/
int main()
{
    int x, y;
    int con = 1;
    while (1)
    {
        if (con)
        {
            cout << "输入马的初始坐标(x,y)：";
            cin >> x >> y;
            if (x < 0 || y < 0 || x >= chessBoardSize || y >= chessBoardSize)
            {
                cout << "输入错误，请重新输入！" << endl;
                Sleep(1500);//延迟
                system("cls");//清屏
            }
            else
            {
                Horse Ha(x, y);
                Ha.HorseTour();
                cout << "是否输入其它坐标（1:YES 0:NO）:";
                cin >> con;
                Sleep(1500);//延迟
                system("cls");//清屏
            }
        }
        else 
        {
            return 0;
        }
    }
    
}

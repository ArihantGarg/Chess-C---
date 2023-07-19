#include<bits/stdc++.h>
using namespace std;

set<pair<string,string>> pos; /* Set of possible moves */

string board[8][8];

void setUpChessBoard()
{
    for(int i=2;i<6;i++)
        for(int j=0;j<8;j++)
            board[i][j]="empty";

    for(int i=0;i<8;i++)
    {
        board[1][i]="bPawn";
        board[6][i]="wPawn";
    }

    board[0][0]=board[0][7]="bRook";
    board[7][0]=board[7][7]="wRook";
    board[0][1]=board[0][6]="bKnight";
    board[7][1]=board[7][6]="wKnight";
    board[0][2]=board[0][5]="bBishop";
    board[7][2]=board[7][5]="wBishop";
    
    board[0][3]="bQueen";
    board[7][3]="wQueen";
    board[0][4]="bKing";
    board[7][4]="wKing";
}

void displayBoard() // Improve piece display
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
            cout<<board[i][j]<<"  ";
        cout<<"\n";
    }

    cout<<"\n\n";
}

bool checkKing(bool type) /*Later when deciding result, for now checks if king is present*/
{
    string kingType = (type ? "b" : "w");

    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            if(board[i][j]==kingType+"King")
                return 1; 

    return 0;
}





bool checkBetween(pair<int,int> prev,pair<int,int> next)
{
    if(prev.first == next.first)
    {
        for(int i=min(prev.second,next.second)+1;i<max(prev.second,next.second);i++)
            if(board[prev.first][i]!="empty")
                return 1;
    }
    else if(prev.second == next.second)
    {
        for(int i=min(prev.first,next.first)+1;i<max(prev.first,next.first);i++)
            if(board[i][prev.second]!="empty")
                return 1;
    }
    else if( abs(prev.first-next.first)==abs(prev.second-next.second))
    {
        if(prev.first>next.first)
            swap(prev,next);

        int slope=1;
        if(prev.second>next.second) 
            slope=-1;

        for(int i=1;i<abs(prev.first-next.first);i++)
            if(board[prev.first+i][prev.second+slope*i]!="empty")
                return 1;
    }
    else
        return 1;

    return 0;
}

bool checkHorizontal(pair<int,int> prev,pair<int,int> next)
{
    if( prev.first!=next.first )
        return 0;

    if(checkBetween(prev,next))
        return 0;

    if(board[prev.first][prev.second]=="wQueen" || board[prev.first][prev.second]=="bQueen")
        return 1;

    if( ( board[prev.first][prev.second]=="wKing" || board[prev.first][prev.second]=="bKing" ) &&  (abs(prev.second-next.second)==1) )
        return 1;

    if(board[prev.first][prev.second]=="wRook" || board[prev.first][prev.second]=="bRook")
        return 1;

    return 0;
}

bool checkVertical(pair<int,int> prev,pair<int,int> next)
{
    if( prev.second!=next.second )
        return 0;

    if(checkBetween(prev,next))
        return 0;

    if(board[prev.first][prev.second]=="wQueen" || board[prev.first][prev.second]=="bQueen")
        return 1;

    if( ( board[prev.first][prev.second]=="wKing" || board[prev.first][prev.second]=="bKing" ) &&  (abs(prev.first-next.first)==1) )
        return 1;

    if(board[prev.first][prev.second]=="wRook" || board[prev.first][prev.second]=="bRook")
        return 1;

    if(board[next.first][next.second]=="empty" && ( ( board[prev.first][prev.second]=="wPawn" && (prev.first-next.first==1 || (prev.first==6 && prev.first-next.first==2)) ) || ( board[prev.first][prev.second]=="bPawn" && (next.first-prev.first==1 || (prev.first==1 && next.first-prev.first==2))) ) )
        return 1;

    return 0;
}

bool checkDiagonal(pair<int,int> prev,pair<int,int> next)
{
    if( abs(prev.first-next.first) != abs(prev.second-next.second))
        return 0;

    if(checkBetween(prev,next))
        return 0;

    if(board[prev.first][prev.second]=="wQueen" || board[prev.first][prev.second]=="bQueen")
        return 1;

    if( ( board[prev.first][prev.second]=="wKing" || board[prev.first][prev.second]=="bKing" ) &&  (abs(prev.first-next.first)==1) )
        return 1;

    if(board[prev.first][prev.second]=="wBishop" || board[prev.first][prev.second]=="bBishop")
        return 1;

    if( (board[prev.first][prev.second]=="wPawn" && prev.first-next.first==1 && board[next.first][next.second][0]=='b') || (board[prev.first][prev.second]=="bPawn" && next.first-prev.first==1 && board[next.first][next.second][0]=='w') )
        return 1;

    return 0;
}

bool checkKnight(pair<int,int> prev,pair<int,int> next)
{
    if(abs(prev.first-next.first)+abs(prev.second-next.second)!=3 || abs(prev.first-next.first)==0 || abs(prev.second-next.second)==0)
        return 0;

    if(board[prev.first][prev.second]=="wKnight" || board[prev.first][prev.second]=="bKnight")
        return 1;

    return 0;
}

string findString(pair<int,int> prev)
{
    int x=prev.first;
    int y=prev.second;

    string s="a1";

    s[1]='8'-x;
    s[0]='a'+y;

    return s;
}

void addPossibleMove(pair<int,int> prev,pair<int,int> next)
{
    string s1,s2;

    s1=findString(prev);
    s2=findString(next);

    pos.insert({s1,s2});
    //cout<<s1<<" , "<<s2<<"\n";
}

void possibleMoves3(bool currentMove,pair<int,int> prev,pair<int,int> next) // King not in check after move,castle move
{
    if(checkHorizontal(prev,next)||checkVertical(prev,next)||checkDiagonal(prev,next)||checkKnight(prev,next))
        addPossibleMove(prev,next);
}

void possibleMoves2(char type,bool currentMove,pair<int,int> prev)
{
    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j][0]!=type)
            {
                possibleMoves3(currentMove,prev,{i,j});
            }
        }
    }
}

void possibleMoves(bool currentMove)
{
    char type = (currentMove ? 'b' : 'w');

    for(int i=0;i<8;i++)
    {
        for(int j=0;j<8;j++)
        {
            if(board[i][j][0]==type)
            {
                possibleMoves2(type,currentMove,{i,j});
            }
        }
    }
}

bool checkValidMove(string s1,string s2)
{
    return (bool)(pos.count({s1,s2}));
}

void makeMove(string s1,string s2,bool currentMove) // En passant edge case, Castle edge case
{
    pair<int,int> prev,next;

    prev={'8'-s1[1],s1[0]-'a'};
    next={'8'-s2[1],s2[0]-'a'};

    board[next.first][next.second]=board[prev.first][prev.second];
    board[prev.first][prev.second]="empty";
}

bool gameEnd(bool currentMove)
{
    if(checkKing(currentMove)==0)
        return 1;

    possibleMoves(currentMove);

    return (pos.size() ? 0 : 1 );
}

void gameEndMessage(bool currentMove)
{
    if(!gameEnd(currentMove))
    {
        cout<<"Error, game ended";
        return;
    }
    
    if(checkKing(0) && checkKing(1) && pos.size()>0)
        cout<<"Error, neither king present";
    else if(checkKing(0) && checkKing(1) && pos.size()==0)
        cout<<"Draw by stalemate";
    else if(checkKing(0))
        cout<<"White won";
    else if(checkKing(1))
        cout<<"Black won";
    else
        cout<<"ERROR";
}

int main()
{
    bool currentMove=0; /* 0 for white, 1 for black*/
    setUpChessBoard();

    while(!gameEnd(currentMove))
    {
        displayBoard();
        string s1,s2;
        cin>>s1>>s2;
        
        while(!checkValidMove(s1,s2))
        {
            cout<<"Invalid Move\n";
            cin>>s1>>s2;
        }

        makeMove(s1,s2,currentMove);

        currentMove=!currentMove;
        pos.clear();
    }

    displayBoard();
    gameEndMessage(currentMove);

    return 0;
}
#include<bits/stdc++.h>
using namespace std;

#include<io.h>
#include<fcntl.h>

#define cout wcout

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
        {
            if(board[i][j]=="empty")
                cout<<" ";
            else if(board[i][j]=="bPawn")
                wcout<<L"\u2659";
            else if(board[i][j]=="bRook")
                cout<<L"\u2656";
            else if(board[i][j]=="bKnight")
                cout<<L"\u2658";
            else if(board[i][j]=="bBishop")
                cout<<L"\u2657";
            else if(board[i][j]=="bQueen")
                cout<<L"\u2655";
            else if(board[i][j]=="bKing")
                cout<<L"\u2654";
            else if(board[i][j]=="wPawn")
                cout<<L"\u265F";
            else if(board[i][j]=="wRook")
                cout<<L"\u265C";
            else if(board[i][j]=="wKnight")
                cout<<L"\u265E";
            else if(board[i][j]=="wBishop")
                cout<<L"\u265D";
            else if(board[i][j]=="wQueen")
                cout<<L"\u265B";
            else if(board[i][j]=="wKing")
                cout<<L"\u265A";

            cout<<" ";
        }
        cout<<"\n";
    }

    cout<<"\n\n";
}

void makeMove(string s1,string s2,bool currentMove, string presentBoard[][8]) // En passant edge case, Castle edge case, promotion is auto queen
{
    pair<int,int> prev,next;

    prev={'8'-s1[1],s1[0]-'a'};
    next={'8'-s2[1],s2[0]-'a'};

    presentBoard[next.first][next.second]=presentBoard[prev.first][prev.second];
    presentBoard[prev.first][prev.second]="empty";

    //Promotion (Auto Queen)

    if(presentBoard[next.first][next.second]=="bPawn" && next.first==7)
        presentBoard[next.first][next.second]="bQueen";
        
    if(presentBoard[next.first][next.second]=="wPawn" && next.first==0)
        presentBoard[next.first][next.second]="wQueen";
}

bool checkBetween(pair<int,int> prev,pair<int,int> next, string presentBoard[][8])
{
    if(prev.first == next.first)
    {
        for(int i=min(prev.second,next.second)+1;i<max(prev.second,next.second);i++)
            if(presentBoard[prev.first][i]!="empty")
                return 1;
    }
    else if(prev.second == next.second)
    {
        for(int i=min(prev.first,next.first)+1;i<max(prev.first,next.first);i++)
            if(presentBoard[i][prev.second]!="empty")
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
            if(presentBoard[prev.first+i][prev.second+slope*i]!="empty")
                return 1;
    }
    else
        return 1;

    return 0;
}

bool checkHorizontal(pair<int,int> prev,pair<int,int> next, string presentBoard[][8])
{
    if( prev.first!=next.first )
        return 0;

    if(checkBetween(prev,next,presentBoard))
        return 0;

    if(presentBoard[prev.first][prev.second]=="wQueen" || presentBoard[prev.first][prev.second]=="bQueen")
        return 1;

    if( ( presentBoard[prev.first][prev.second]=="wKing" || presentBoard[prev.first][prev.second]=="bKing" ) &&  (abs(prev.second-next.second)==1) )
        return 1;

    if(presentBoard[prev.first][prev.second]=="wRook" || presentBoard[prev.first][prev.second]=="bRook")
        return 1;

    return 0;
}

bool checkVertical(pair<int,int> prev,pair<int,int> next, string presentBoard[][8])
{
    if( prev.second!=next.second )
        return 0;

    if(checkBetween(prev,next,presentBoard))
        return 0;

    if(presentBoard[prev.first][prev.second]=="wQueen" || presentBoard[prev.first][prev.second]=="bQueen")
        return 1;

    if( ( presentBoard[prev.first][prev.second]=="wKing" || presentBoard[prev.first][prev.second]=="bKing" ) &&  (abs(prev.first-next.first)==1) )
        return 1;

    if(presentBoard[prev.first][prev.second]=="wRook" || presentBoard[prev.first][prev.second]=="bRook")
        return 1;

    if(presentBoard[next.first][next.second]=="empty" && ( ( presentBoard[prev.first][prev.second]=="wPawn" && (prev.first-next.first==1 || (prev.first==6 && prev.first-next.first==2)) ) || ( presentBoard[prev.first][prev.second]=="bPawn" && (next.first-prev.first==1 || (prev.first==1 && next.first-prev.first==2))) ) )
        return 1;

    return 0;
}

bool checkDiagonal(pair<int,int> prev,pair<int,int> next, string presentBoard[][8])
{
    if( abs(prev.first-next.first) != abs(prev.second-next.second))
        return 0;

    if(checkBetween(prev,next,presentBoard))
        return 0;

    if(presentBoard[prev.first][prev.second]=="wQueen" || presentBoard[prev.first][prev.second]=="bQueen")
        return 1;

    if( ( presentBoard[prev.first][prev.second]=="wKing" || presentBoard[prev.first][prev.second]=="bKing" ) &&  (abs(prev.first-next.first)==1) )
        return 1;

    if(presentBoard[prev.first][prev.second]=="wBishop" || presentBoard[prev.first][prev.second]=="bBishop")
        return 1;

    if( (presentBoard[prev.first][prev.second]=="wPawn" && prev.first-next.first==1 && presentBoard[next.first][next.second][0]=='b') || (presentBoard[prev.first][prev.second]=="bPawn" && next.first-prev.first==1 && presentBoard[next.first][next.second][0]=='w') )
        return 1;

    return 0;
}

bool checkKnight(pair<int,int> prev,pair<int,int> next, string presentBoard[][8])
{
    if(abs(prev.first-next.first)+abs(prev.second-next.second)!=3 || abs(prev.first-next.first)==0 || abs(prev.second-next.second)==0)
        return 0;

    if(presentBoard[prev.first][prev.second]=="wKnight" || presentBoard[prev.first][prev.second]=="bKnight")
        return 1;

    return 0;
}

bool checkKing(bool type,string presentBoard [][8])
{
    pair<int,int> loc={-1,-1};

    string kingType = (type ? "b" : "w");

    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            if(presentBoard[i][j]==kingType+"King")
                loc={i,j};

    //

    string otherType = (type ? "w" : "b");
    char otType = (type ? 'w' : 'b');

    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
        {
            if(presentBoard[i][j][0]==otType)
            {
                if(checkHorizontal({i,j},loc,presentBoard)||checkVertical({i,j},loc,presentBoard)||checkDiagonal({i,j},loc,presentBoard)||checkKnight({i,j},loc,presentBoard))
                    return 1;
            }
        }

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

bool checkDiscoveredCheck(bool currentMove , pair<int,int> prev , pair<int,int> next )
{
    string currBoard[8][8];

    for(int i=0;i<8;i++)
        for(int j=0;j<8;j++)
            currBoard[i][j]=board[i][j];

    string s1,s2;

    s1=findString(prev);
    s2=findString(next);

    makeMove(s1,s2, currentMove , currBoard );

    if(checkKing(currentMove,currBoard))
        return 1;

    return 0;
}

void possibleMoves3(bool currentMove,pair<int,int> prev,pair<int,int> next) // En passant,castle move
{
    if(checkHorizontal(prev,next,board)||checkVertical(prev,next,board)||checkDiagonal(prev,next,board)||checkKnight(prev,next,board))
        if(checkDiscoveredCheck( currentMove , prev , next) == 0)
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

bool gameEnd(bool currentMove)
{
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
    
    if(checkKing(0,board) && checkKing(1,board))
        cout<<"Error, Both kings in check";
    else if(checkKing(0,board)==0 && checkKing(1,board)==0 && pos.size()==0)
        cout<<"Draw by stalemate";
    else if(checkKing(0,board))
        cout<<"White won";
    else if(checkKing(1,board))
        cout<<"Black won";
    else
        cout<<"ERROR";
}

int main()
{
    _setmode(_fileno(stdout), _O_U16TEXT);
    bool currentMove=0; /* 0 for white, 1 for black */
    setUpChessBoard();

    while(!gameEnd(currentMove))
    {
        cout<<"\n";
        displayBoard();
        string s1,s2;
        cin>>s1>>s2;

        if(checkKing(currentMove,board))
            cout<<"\nYou are in check\n";

        // string s;
        // cin>>s;
        // s1=s.substr(0,2);
        // s2=s.substr(2,2);
        
        while(!checkValidMove(s1,s2))
        {
            cout<<"Invalid Move\n";
            cin>>s1>>s2;
        }

        makeMove(s1,s2,currentMove,board);

        currentMove=!currentMove;
        pos.clear();
    }

    displayBoard();
    gameEndMessage(currentMove);

    return 0;
}
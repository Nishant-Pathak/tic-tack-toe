#include <iostream>
#include <vector>
#include <cstdlib>
#include <climits>
#include <ctime>
using namespace std;

void BotPlayRand(vector <vector <char> > &m) {
    int i(rand()%3), j(rand()%3);
//   cout << "Playing at" << i << ", " << j << endl;
    m[i][j] = 'O';
    return;
}
void display(vector <vector <char> > &m) {
   cout << "GAME STATE:" << endl;
   for(int i=0; i < m.size(); i++) {
      for(int j=0; j < m[i].size(); j++)
         cout << m[i][j] << " ";
      cout << endl;
   }
   return;
}
bool validMove(vector <vector <char> > &m , int &i, int&j) {
    return (i < m.size())     &&
           (j < m[i].size())  &&
           (m[i][j] == '*');
}

bool WinState (vector <vector <char> > &m , char ch = 'O') {
    for(int i=0;i < m.size();i++) {
        if(ch == m[i][0] && m[i][0] == m[i][1] && m[i][1] == m[i][2]) return true;
        if(ch == m[0][i] && m[0][i] == m[1][i] && m[1][i] == m[2][i]) return true;
    }
    if(ch == m[0][0] && m[0][0] == m[1][1] && m[1][1] == m[2][2]) return true;
    if(ch == m[2][0] && m[2][0] == m[1][1] && m[1][1] == m[0][2]) return true;
    return false;
}

bool DoneGame(vector <vector <char> > &m ) {
    for(int i=0;i < m.size();i++)
        for(int j=0;j < m[i].size();j++) if(m[i][j] == '*') return false;
    return true;
}

bool LooseState(vector <vector <char> > &m ) {
    return WinState(m, 'X');
}

int rWeight(char c1, char c2, char c3)
{
	char row[3] = {c1, c2, c3};
	int numX = 0;
	int numO = 0;
	
	for (int i = 0; i < 3; ++i)
	{	
		if (row[i] == 'X') numX++;
		if (row[i] == 'O') numO++;
	}

	if (numX == numO) return 0;
	else if (numO == 3) return 100;  // favourable win
	else if (numO == 2) return 10;
	else if (numO == 1) return 1;
	else if (numX == 3) return -100;  // human win
	else if (numX == 2) return -10;
	else if (numX == 1) return -1;
	else return 0;	
}
int Evaluate(vector <vector <char> > &m) {
return rWeight(m[0][0], m[0][1], m[0][2]) + rWeight(m[1][0], m[1][1], m[1][2])
	+ rWeight(m[2][0], m[2][1], m[2][2]) + rWeight(m[0][0], m[1][0], m[2][0])
	+ rWeight(m[0][1], m[1][1], m[2][1])+ rWeight(m[0][2], m[1][2], m[2][2])
	+ rWeight(m[0][0], m[1][1], m[2][2])+ rWeight(m[0][2], m[1][1], m[2][0]);
//    if(LooseState(m)) return -100;//
//    if(WinState(m)) return 100;/
//    if(DoneGame(m))return 0;
//    cout << "Not done yet" << endl;
//    return 0;
}

bool GoalState(vector <vector <char> > &m, char ch='O') {
    return WinState(m,ch) || LooseState(m) || DoneGame(m);
}

int print;
int minimax(vector < vector<char> >node, int level, bool IsBot, int &ii, int &jj) {
   // cout << "**********" << endl;
  //  display(node);
   // cout << "**********" << endl;
    if(GoalState(node)) return Evaluate(node);   // return the heuristic value
    int bestval, tempval, lf(10 - level), iii , jjj;
    if(IsBot) {
        bestval = INT_MIN;
        for(int i = 0; i<node.size(); i++) {
            for(int j = 0; j<node[i].size(); j++) {
                if(node[i][j] == '*') {
                    node[i][j] = 'O';
                    tempval = minimax(node, level + 1, false, ii , jj);
                    if(tempval > bestval) {
                        bestval = tempval;
                        iii = i;
                        jjj = j;
                    }
  //                  if(level == print && false) {
 //                       display(node);
 //                       cout << "val :" << tempval << endl;
  //                      }
                    node[i][j] = '*';
                }
           }
        }
    ii = iii;
    jj = jjj;
    } else {
    // supposed human
        bestval = INT_MAX;
        for(int i = 0; i < node.size(); i++) {
            for(int j = 0; j<node[i].size(); j++) {
                if(node[i][j] == '*') {
                    node[i][j] = 'X';
                    tempval = minimax(node, level + 1, true, ii, jj);
                    if(tempval < bestval ) {
                        bestval = tempval;
                        ii = i;
                        jj = j;
                    }
                    node[i][j] = '*';
                }
            }
        }
    }
    return bestval;
}

void PlayBot(vector <vector <char> > &m, int level) {
    int playi(0), playj(0);
    int bestval = INT_MIN, tempval;
    print = level;
    bestval = minimax(m, level, true, playi, playj);
//    int val = minimax(m, level, true); // true for bot player
/*
    for(int i = 0; i < m.size(); i++) {
        for(int j = 0; j<m[i].size(); j++) {
            if(m[i][j] == '*') {
                m[i][j] = 'X';
                tempval = -minimax(m, level + 1, true);
                cout << tempval << " " << i << " " << j << endl;
                if(tempval > bestval) {
                    bestval = tempval;
                    playi = i;
                    playj = j;
                }
                m[i][j] = '*';        // restore the board
            }
        }
    }*/
    cout << "Bestval:" << bestval
         << " Bestmove: (" << playi << ", " << playj << ")" << endl;
    m[playi][playj] = 'O';
}

int main(){
    srand(time(0));
    bool BotStart(false);
    string yesno;
    int TotalMoves(0),i(0),j(0);
    vector <vector <char> > m(3, vector <char> (3,'*'));
REYESNO:
    cout << "Do you want first turn ? yes:true no:false" << endl;
    cin >> yesno;
    if(yesno == "yes") BotStart = false;
    else if(yesno == "no") BotStart = true;
    else {
         cout << "Invlid input type again" << endl;
         goto REYESNO;
    }
    cout << "You can play your turn by entering cordinates as : 1 2 starting 0 0 for top left corner" << endl;
    if(BotStart) {
        cout << "Bot starting" << endl;
        BotPlayRand(m);
        TotalMoves = 1;
    }
    while(TotalMoves < 9) {
PLAY:
        display(m);
        cout << "Your move X. Enter cordinates:";
        cin >> i >> j;
        if(!validMove(m,i,j)) goto PLAY;
        else m[i][j] = 'X';
        TotalMoves ++;
        if(GoalState(m)) goto Done;
        PlayBot(m, TotalMoves);
        if(GoalState(m)) goto Done;
        TotalMoves++;
        cout << "total moves :" << TotalMoves << endl;
    }
Done:
    display(m);
    if(LooseState(m)) cout << "Bot Loose :(" << endl;
    else if(WinState(m)) cout << "Bot Win:" << endl;
    else cout << "Draw" << endl;
    return 0;

}

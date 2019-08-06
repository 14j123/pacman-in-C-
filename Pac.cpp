#include <iostream>
#include <stdio.h>
#include <windows.h>
#include <string>
#include <vector>

using namespace std;
int lives = 3;
char tmp_map[18][32];

char map[18][32] = {
	"+#############################+",
	"|                  #######    |",
	"|                             |",
	"|## ########### ##   #########|",
	"|   |                         |",
	"| | |### |  |           |     |",
	"| |      |  | |###  |   |  |  |",
	"| | #####|  | |      ## |     |",
	"| |           |###  |      |  |",
	"| |##### ###         ##       |",
	"|          ######  ####### ###|",
	"|                             |",
	"|# ### ####      ###   #######|",
	"|                             |",
	"|           #                 |",
	"|                             |",
	"|                             |",
	"+#############################+"
	};

void ShowMap()
{
	for(int i = 0; i < 18; i++) {
		printf("%s\n",map[i] );
	}
}

void gotoxy( short x, short y )
{
    HANDLE hStdout = GetStdHandle(STD_OUTPUT_HANDLE) ;
    COORD position = { x, y } ;

    SetConsoleCursorPosition( hStdout, position ) ;
}

class entity {
public:
	entity( int x, int y ){
		this ->x = x;
		this ->y = y;
	}

	void move_x( int p ){
		if( map[y][x+p] == ' ' ) x += p;
	}

	void move_y( int p ){
		if( map[y+p][x] == ' ' ) y += p;
	}

	void move( int p, int q ){
		x += p;
		y += q;
	}

	int get_x(){ return x; }
	int get_y(){ return y; }

	void draw( char p ){
		map[x][y] = p;
		gotoxy( x, y ); printf( "%c", p );
	}


	int x;
	int y;
};

struct walk {
	short walk_count;
	short x;
	short y;
	short back;
};

struct target {
	short x;
	short y;
};

vector<target> walk_queue;

vector<walk> BFSArray;

void AddArray( int x, int y, int wc , int back ){
	if( tmp_map[y][x] == ' ' || tmp_map[y][x] == '.' ){
		tmp_map[y][x] = '#';
		walk tmp;
		tmp.x = x;
		tmp.y = y;
		tmp.walk_count = wc;
		tmp.back = back;
		BFSArray.push_back( tmp );
	}
}

void FindPath( int sx, int sy, int x, int y ){
	memcpy( tmp_map, map, sizeof(map) );
	BFSArray.clear();
	walk tmp;
	tmp.x = sx;
	tmp.y = sy;
	tmp.walk_count = 0;
	tmp.back = -1;
	BFSArray.push_back( tmp );

	int i = 0;
	while( i < BFSArray.size() ){
		if( BFSArray[i].x == x && BFSArray[i].y == y ){
			walk_queue.clear();
			target tmp2;
			while( BFSArray[i].walk_count != 0 ){
				tmp2.x = BFSArray[i].x;
				tmp2.y = BFSArray[i].y;
				walk_queue.push_back( tmp2 );

				i = BFSArray[i].back;
			}

			break;
		}

		AddArray( BFSArray[i].x+1, BFSArray[i].y, BFSArray[i].walk_count+1, i );
		AddArray( BFSArray[i].x-1, BFSArray[i].y, BFSArray[i].walk_count+1, i );
		AddArray( BFSArray[i].x, BFSArray[i].y+1, BFSArray[i].walk_count+1, i );
		AddArray( BFSArray[i].x, BFSArray[i].y-1, BFSArray[i].walk_count+1, i );

		i++;
	}

	BFSArray.clear();
}

void classic()
{
	
	system("cls");
	printf("Instruction:\n1. Arrow Keys to move your hero\n2. Eat the dots produced by the Eater to gain poins\n3. Don't get caught by the Eater\n\n");
	printf("H -> Hard\nN -> Normal\nE -> Easy\n\nInput : ");

	char diffi;

	cin >> diffi;
    int pts = 0;
    
	int speedmod = 3;
	if( diffi == 'N' ){
		speedmod = 2;
	}else if( diffi == 'H' ){
		speedmod = 1;
	}
	
    for(int i=3;i>0;i--){
    bool running = true;
	int x = 15; // hero x
    int y = 16; // hero y
	int old_x;
	int old_y;

	int ex = 1;
	int ey = 1;

	system("cls");
    ShowMap();

	gotoxy( x, y ); cout << "H";

	int frame = 0;

	FindPath( ex,ey,x,y );

	while( running ){
		gotoxy( x, y ); cout << " ";

		old_x = x;
		old_y = y;

		if ( GetAsyncKeyState( VK_UP ) ){
			if( map[y-1][x] == '.' ){ y--; pts++; } else
			if( map[y-1][x] == ' ' ) y--;
		}
		if ( GetAsyncKeyState( VK_DOWN ) ){
			if( map[y+1][x] == '.' ){ y++; pts++; } else
			if( map[y+1][x] == ' ' ) y++;
		}
		if ( GetAsyncKeyState( VK_LEFT ) ){
			if( map[y][x-1] == '.' ){ x--; pts++; } else
			if( map[y][x-1] == ' ' ) x--;
		}
		if ( GetAsyncKeyState( VK_RIGHT ) ){
			if( map[y][x+1] == '.' ){ x++; pts++; } else
			if( map[y][x+1] == ' ' ) x++;
		}

		if( old_x != x || old_y != y ){
			FindPath( ex,ey,x,y );
		}

		gotoxy( x,y ); cout << "H";

		map[ey][ex] = '.';
		gotoxy( ex, ey ); cout << ".";

		if( frame%speedmod == 0 && walk_queue.size() != 0 ){
			ex = walk_queue.back().x;
			ey = walk_queue.back().y;
			walk_queue.pop_back();
		}

		gotoxy( ex, ey ); cout << "E";

        if( ex == x && ey == y ){
			break;
		}
		
		gotoxy( 32, 18 );
		gotoxy( 32, 1 ); cout << pts;
		Sleep( 100 );
		frame++;
		
	cout << "\tlives remaining: " << i << endl;
		
	}
    } 
	system("cls");
	cout<<"***************************GAME OVER***************************"<<endl;
	cout<<"\t\t\tYour score: "<<pts<<endl;
	system("pause");
}


void survival()
{
	bool running = true;
	int x = 15; // hero x
	int y = 16; // hero y
	int old_x;
	int old_y;

	int ex = 1;
	int ey = 1;


	int pts = 0;
	int speedmod=3;
	system("cls");
    ShowMap();

	gotoxy( x, y ); cout << "H";

	int frame = 0;

	FindPath( ex,ey,x,y );

	while( running ){
		gotoxy( x, y ); cout << " ";

		old_x = x;
		old_y = y;

		if ( GetAsyncKeyState( VK_UP ) ){
			if( map[y-1][x] == '.' ){ y--; pts++; } else
			if( map[y-1][x] == ' ' ) y--;
		}
		if ( GetAsyncKeyState( VK_DOWN ) ){
			if( map[y+1][x] == '.' ){ y++; pts++; } else
			if( map[y+1][x] == ' ' ) y++;
		}
		if ( GetAsyncKeyState( VK_LEFT ) ){
			if( map[y][x-1] == '.' ){ x--; pts++; } else
			if( map[y][x-1] == ' ' ) x--;
		}
		if ( GetAsyncKeyState( VK_RIGHT ) ){
			if( map[y][x+1] == '.' ){ x++; pts++; } else
			if( map[y][x+1] == ' ' ) x++;
		}

		if( old_x != x || old_y != y ){
			FindPath( ex,ey,x,y );
		}

		gotoxy( x,y ); cout << "H";

		map[ey][ex] = '.';
		gotoxy( ex, ey ); cout << ".";

		if( frame%speedmod == 0 && walk_queue.size() != 0 ){
			ex = walk_queue.back().x;
			ey = walk_queue.back().y;
			walk_queue.pop_back();
		}

		gotoxy( ex, ey ); cout << "E";

		if( ex == x && ey == y ){
			break;
		}
		


		gotoxy( 32, 18 );
		gotoxy( 32, 1 ); cout << pts;
		Sleep( 100 );
		frame++;
		
	if (pts>50)
	{
		speedmod=2;
	}if(pts>100)
	{
		speedmod=1;
	}	
		
	}

	system("cls");
	cout<<"***************************GAME OVER***************************"<<endl;
	cout<<"\t\t\tYour score: "<<pts<<endl<<endl;
	system("pause");
}
void menu()
{
     int ss;
	char cont;
	do{
    system("cls");
    cout<<"***********************************************"<<endl;
	cout<<"***********************************************"<<endl;     
    cout << "       #############		       "<< endl;       
    cout << "     #######  ########		       "<< endl;         
    cout << "    ###################            "<<endl;           
    cout << "   ################                 "<<endl;
	cout << "  ##############				    "<<endl;
    cout << " #############		 PACMAN.... "<<endl;
    cout << "  ##############		           "<<endl;
    cout << "   ##################			   "<<endl;
    cout << "     ##################	       "<<endl;
    cout << "       ##############			   "<<endl;   
    cout<<"\n";
    cout<<"##Please select your mode:"<<endl;
	cout<<"1. Classic mode:"<<endl;
	cout<<"2. Survival mode"<<endl; 
	cout<<"3. How to play"<<endl;
	cout<<"***********************************************"<<endl;
	cout<<"***********************************************"<<endl;
    
    cin>> ss;
	switch(ss)
	{
              case 1:
                   do{
		           classic();
		           system("cls");
		           cout<<"Do you wish to play again?"<<endl;
		           cout<<"Enter Y/N: ";
		           cin>>cont;
                   }while(cont=='Y');
		           cout<<"To return to main menu, enter 0. To quit, enter 9. ";
                   cout<<"Your choice: "; 
                   cin>>ss;
                   if (ss==9)
                   {
                             system("cls");
                             cout<<"Exiting..."<<endl;
                             cout<<"Bye-bye..."<<endl;
                   }
                   break;
              case 2:
                   do{
                   survival();
		           system("cls");
		           cout<<"Do you wish to play again?"<<endl;
		           cout<<"Enter Y/N: ";
		           cin>>cont;
                   }while(cont=='Y');
                   cout<<"To return to main menu, enter 0. To quit, enter 9. ";
                   cout<<"Your choice: "; 
                   cin>>ss;
                   if (ss==9)
                   {
                             system("cls");
                             cout<<"Exiting..."<<endl;
                             cout<<"Bye-bye..."<<endl;
                   }
                   break;
              case 3:
                   system("cls");
                   cout<<"--------------------------------------------------------------"<<endl;
                   cout<<" HOW TO PLAY: "<<endl;
                   cout<<"--------------------------------------------------------------"<<endl<<endl;
                   cout<<"Classic Mode:"<<endl;
                   cout<<"1. Choose your map type."<<endl;
                   cout<<"2. Choose your level of difficulty."<<endl;
                   cout<<"3. Play the game by using the LEFT, RIGHT, UP, DOWN keys"<<endl;
                   cout<<"   to move the Hero through the map. Eat the '.'s to score points."<<endl;
                   cout<<"   Avoid getting eaten by the Eater."<<endl<<endl;
                   cout<<"--------------------------------------------------------------"<<endl<<endl;
                   cout<<"Survival Mode: "<<endl;
                   cout<<"1. Choose your map type."<<endl;
                   cout<<"2. Play the game by using the LEFT, RIGHT, UP, DOWN keys"<<endl;
                   cout<<"   to move the Hero through the map. Eat the '.'s to score points."<<endl;
                   cout<<"   Avoid getting eaten by the Eater."<<endl;
                   cout<<"3. The Eater will increase in speed as you score more points."<<endl<<endl;
                   cout<<"--------------------------------------------------------------"<<endl<<endl;
                   cout<<"Enter 0 to return to menu."<<endl;
                   cin>>ss;
              case 4:
                   system("cls");
                   cout<<"Exiting..."<<endl;
                   cout<<"Bye-bye..."<<endl;
    }
    }while(ss==0);
}

int main()
{
	system("COLOR FC");
	system("COLOR 0E");
    
    menu();
    
	system("pause");
	return 0;
}

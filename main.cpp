//
//  main.cpp
//  LinkUp
//
//  Created by Cong on 13-4-14.
//  Copyright (c) 2013年 liuchyin. All rights reserved.
//

#include <iostream>
#include <set>
#include <queue>
using namespace std;






struct point{
	point(int x, int y, int turn, int distance, int forward, point* parent):
        x(x), y(y), turn(turn), distance(distance), forward(forward), parent(parent){}
	int x;
	int y;
	int turn;
    int distance;
    int forward;
    point *parent;
};

class LinkUp{
private:
    int** matrix;
    int N;
    queue<point*> BFS;
    queue<point*> close;
    point* root;
public:
    LinkUp(int N) : N(N), root(NULL){
        int *temp = new int[N * N];
        matrix = new int*[N];
        for(int i = 0; i != N; ++i)
            matrix[i] = temp + i * N;
        
        memset(temp, 0, N * N * sizeof(int));
        
        for(int i = 0; i != N * N / 3; ++i){
            int item = rand() % 5 + 1;
            int pos1;
            int pos2;
            do{
                pos1 = rand() % (N * N);
            }while(matrix[pos1 / N][pos1 % N] != 0);
            
            matrix[pos1 / N][pos1 % N] = item;
            do{
                pos2 = rand() % (N * N);
            }while(matrix[pos2 / N][pos2 % N] != 0);
            matrix[pos2 / N][pos2 % N] = item;
        }
    }
    
    LinkUp(int *_matrix, int N) : N(N), root(NULL){
        int *temp = new int[N * N];
        matrix = new int*[N];
        for(int i = 0; i != N; ++i)
            matrix[i] = temp + i * N;
        for(int i = 0; i != N; ++i){
            for(int j = 0; j != N; ++j)
                matrix[i][j] = _matrix[i * N + j];
        }
    }
    
    bool findPath(int x1, int y1, int x2, int y2){
        root = new point(x1, y1, 0, 0, 0, NULL);
        if(x1 < 0 || x2 < 0 || y1 < 0 || y2 < 0 || x1 > N || x2 > N || y1 > N || y2 > N){
            cout<<"invalid input"<<endl;
            return false;
        }
        
        if(matrix[x1][y1] == 0 || matrix[x2][y2] == 0){
            cout<<"invalid input"<<endl;
            return false;
        }
        
        if(matrix[x1][y1] != matrix[x2][y2]){
            cout<<"inequality item"<<endl;
            return false;
        }
        
        if(expand(x1, y1, x2, y2) ||
           expandOneTurn(x2, y2) ||
           expandTwoTurn(x2, y2))
        {
            matrix[x1][y1] = 0;
            matrix[x2][y2] = 0;
            release();
            return true;
        }
        return false;
    }
    
    
    bool expandTwoTurn(int x2, int y2){
        while (!BFS.empty()) {
            point* front = BFS.front();
            int x = front->x;
            int y = front->y;
            if(front->forward == 1){
                int y1 = y + 1;
                while(y1 < N && matrix[x][y1] == 0)
                    ++y1;
                if(x == x2 && y1 == y2)
                    return true;
                y1  = y - 1;
                while(y1 >= 0 && matrix[x][y1] == 0)
                    ++y1;
                if(x == x2 && y1 == y2)
                    return true;
            }else{
                int x1 = x + 1;
                while(x1 < N && matrix[x1][y] == 0)
                    ++x1;
                if(x1 == x2 && y == y2){
                    return true;
                }
                x1 = x - 1;
                while(x1 >= 0 && matrix[x1][y] == 0)
                    --x1;
                if(x1 == x2 && y == y2){
                    return true;
                }
            }
            close.push(BFS.front());
            BFS.pop();
        }
        return false;
    }
    //x轴扩展 为1， y轴扩展为0
    bool expandOneTurn(int x2, int y2){
        while(BFS.front()->turn == 0){
            point* front = BFS.front();
            int x = front->x;
            int y = front->y;

            if (front->forward == 1) {
                int y1 = y + 1;
                while(y1 < N && matrix[x][y1] == 0){
                    BFS.push(new point(x, y1, 1, front->distance + y1 - y, 0, front));
                    ++y1;
                }
                if(x == x2 && y1 == y2){
                    return true;
                }
                
                y1 = y - 1;
                while(y1 >= 0 && matrix[x][y1] == 0){
                    BFS.push(new point(x, y1, 1, front->distance + y - y1, 0, front));
                    --y1;
                }
                if(x == x2 && y1 == y2){
                    return true;
                }
            }else{
                int x1 = x + 1;
                while(x1 < N && matrix[x1][y] == 0){
                    BFS.push(new point(x1, y, 1, front->distance + x1 - x, 1, front));
                    ++x1;
                }
                if(x1 == x2 && y == y2){
                    return true;
                }
                x1 = x - 1;
                while(x1 >= 0 && matrix[x1][y] == 0){
                    BFS.push(new point(x1, y, 1, front->distance + x - x1, 1, front));
                    --x1;
                }
                if(x1 == x2 && y == y2){
                    return true;
                }
            }
            close.push(front);
            BFS.pop();
        }
        return false;
    }
    
    //x轴扩展 为1， y轴扩展为0
    bool expand(int x1, int y1, int x2, int y2){
        int x = x1 + 1;
        while(x < N && matrix[x][y1] == 0){
            BFS.push(new point(x, y1, 0, x - x1, 1, root));
            x++;
        }
        if(x == x2 && y1 == y2)
            return true;
        x = x1 - 1;
        while(x >= 0 && matrix[x][y1] == 0){
            BFS.push(new point(x, y1, 0, x1 - x, 1, root));
            x--;
        }
        if(x == x2 && y1 == y2)
            return true;
        int y = y1 + 1;
        while(y < N && matrix[x1][y] == 0){
            BFS.push(new point(x1, y, 0, y1 - y, 0, root));
            y++;
        }
        if(x1 == x2 && y == y2)
            return true;
        y = y1 - 1;
        while(y >= 0 && matrix[x1][y] == 0){
            BFS.push(new point(x1, y, 0, y - y1, 0, root));
            --y;
        }
        if(x1 == x2 && y == y2)
            return true;
        return false;
    }
    
    void print(){
        for(int i = 0; i != N; ++i){
            for(int j = 0; j != N; ++j)
                cout<<matrix[i][j]<<" ";
            cout<<endl;
        }
    }
    void release(){
        delete root;
        root = NULL;
        while(!BFS.empty()){
            delete BFS.front();
            BFS.pop();
        }
        
        while(!close.empty()){
            delete close.front();
            close.pop();
        }
    }
};
/*

*/

int main()
{
    /*int data[] = {0, 0, 0, 0, 0, 0,
        0, 0, 0, 4, 0, 0,
        0, 0, 0, 0, 3, 5,
        5, 3, 3, 3, 4, 0,
        0, 3, 5, 3, 3, 3,
        0, 0, 3, 3, 0, 5,};
 
    LinkUp linkup(data, 6);*/
    LinkUp linkup(6);
    linkup.print();

    int x1, y1, x2, y2;
    while(true){
        cin>>x1>>y1>>x2>>y2;
        linkup.findPath(x1, y1, x2, y2);
        linkup.print();
        
    }
	return 0;
}

#pragma once
#include "string"
#include "vector"
#include "unordered_map"
#include "queue"
#include "cstdlib"
#include "ctime"
#include <iostream>
#include "algorithm"
#include "stack"
using namespace std;

class Solution
{
public:
    // Definition for singly-linked list.
    void maze(vector<vector<int>> &map)
    {
        //U unvisited, 0 visited
        for (int i = 0; i < map.size(); ++i)
        {
            for (int j = 0; j < map[0].size(); ++j)
            {
                map[i][j] = 1;
            }
        }
        _maze(map, 0, 0);
    }

    void showMaze(vector<vector<int>> &map)
    {

        for (int i = 0; i < map.size(); ++i)
        {
            for (int j = 0; j < map[0].size(); ++j)
            {
                cout << map[i][j];
            }
            cout << endl;
        }
    }
    //Use DFS
    void _maze(vector<vector<int>> &map, int i, int j)
    {
        int direct[][2] = {{0, 1}, {0, -1}, {-1, 0}, {1, 0}};
        int visitOrder[] = {0, 1, 2, 3};
        //out of boundary
        if (i < 0 || j < 0 || i >= map.size() || j >= map[0].size())
            return;
#ifdef DEBUG
        cout << "(" << i << ", " << j << ")" << endl;
#endif
        //visited, go back to the coming direction, return
        if (map[i][j] == 0)
            return;

        //some neightbors are visited in addition to the coming direction, return
        //this is to avoid circles in maze
        if (countVisitedNeighbor(map, i, j) > 1)
            return;

        map[i][j] = 0; // visited

        //shuffle the visitOrder
        shuffle(visitOrder, 4);

        for (int k = 0; k < 4; ++k)
        {
            int ni = i + direct[visitOrder[k]][0];
            int nj = j + direct[visitOrder[k]][1];
            _maze(map, ni, nj);
        }
    }

    int countVisitedNeighbor(vector<vector<int>> &map, int i, int j)
    {
        int direct[][2] = {{1, 0}, {-1, 0}, {0, 1}, {0, -1}};
        int count = 0;
        for (int k = 0; k < 4; ++k)
        {
            int ni = i + direct[k][0];
            int nj = j + direct[k][1];
            //out of boundary
            if (ni < 0 || nj < 0 || ni >= map.size() || nj >= map[0].size())
                continue;
            if (map[ni][nj] == 0)
                count++; //visited
        }
        return count;
    }
    void shuffle(int a[], int n)
    {
        for (int i = 0; i < n; ++i)
        {
            swap(a[i], a[rand() % n]);
        }
    }
    void swap(int &a, int &b)
    {
        int c = a;
        a = b;
        b = c;
    }
};

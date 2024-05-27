#pragma once
#include "point.h"
#include <deque>
#include<fstream>
#include <string>
#include <iostream>

class Read_exception{};
struct Pixel {
public:
	Pixel() {};
	Pixel(const Pixel& p) {
		first = p.first;
		second = p.second;
		third = p.third;
	}
	unsigned char first, second, third;

};

const int HEADER_SIZE = 14;

class Maze_solver
{
private:
	int width, height, padding;
	long long** maze_map;
	bool** logic_map;
	bool** marked;
	Pixel** image;
	Point start, finish;

	//1 символ - 1 байт, иначе всё ломается, поэтому используем unsigned char
	unsigned char DIB[124];
	unsigned char header[HEADER_SIZE];


	char* file_path;
	void read();
	void create_maze();
	void find_way();
public:
	Maze_solver();
	Maze_solver(char* afile_path);
	~Maze_solver();
	void solve();
	void save();

};


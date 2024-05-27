#include "maze_solver.h"

Maze_solver::Maze_solver(char* afile_path)
{
	file_path = afile_path;
	read();


}

Maze_solver::~Maze_solver()
{

	//for (int x = 0; x < height; x++){
	//	delete[] logic_map[x];
	//	delete[] maze_map[x];
	//}
	//delete[] logic_map;
	//delete[] maze_map;

}


//используем волновой алгоритм. 
//для записи
void Maze_solver::solve()
{
	//maze_map = new long long* [height];
	//logic_map = new bool* [height];
	//for (int x = 0; x < height; x++) {
	//	maze_map[x] = new long long[width];
	//	logic_map[x] = new bool[width];

	//}
	deque<Point> to_mark;
	Point* next =start.neighboors();
	maze_map[start.y][start.x] = 0;
	for (int i = 0; i < 4; i++) {
		to_mark.push_back(*next);
		next++;
	}
	marked[start.y][start.x] = 1;
	long long iter = 0;
	while (true) {
		
		if (to_mark.empty()) {
			break;
		}
		Point cur = to_mark.front();


		bool pos_bds = (cur.x >= 0 && cur.y >= 0), im_bds=(cur.x <width && cur.y <height);
		if (pos_bds && im_bds && logic_map[cur.y][cur.x] && !marked[cur.y][cur.x]) {

			maze_map[cur.y][cur.x] = maze_map[cur.fy][cur.fx] + 1;
			marked[cur.y][cur.x] = 1;
			if (cur.y == finish.y && cur.x == finish.x ) {
				break;
			}

			Point* next = cur.neighboors();
			for (int i = 0; i < 4; i++) {
				to_mark.push_back(*next);
				next++;
			}
		}
		to_mark.pop_front();
		iter++;

	}
	std::cout << iter << std::endl;
	find_way();
}

void Maze_solver::save()
{	
	//ofstream debug_path;
	//debug_path.open("debug_wave.txt");

	//for (int y = 0; y < height; y++) {
	//	for (int x = 0; x < width; x++) {
	//		debug_path << maze_map[y][x] << ' ';
	//	}
	//	debug_path << std::endl;
	//}
	//debug_path.close();
	std::cout << "finish val =" << maze_map[finish.y][finish.x] << std::endl;
	std::cout << "start val =" << maze_map[start.y][start.x] << std::endl;




	ofstream saving;
	saving.open("output.bmp", std::ios::binary);
	for (int i = 0; i < 14; i++) {
		saving << header[i];
	}
	for (int i = 0; i < 40; i++) {
		saving << DIB[i];
	}
	for (int y = 0; y < height; y++) {
		for (int x = 0; x < width; x++) {
			saving << image[y][x].first << image[y][x].second << image[y][x].third;
		}
	}
	saving.close();

}


void Maze_solver::read()
{
	int startx = 0, starty = 0, finishx = 0, finishy = 0;
	std::ifstream input_image;
	//std::ofstream debug,log_debug;
	input_image.open(file_path, std::ios::binary);
	if (!input_image.is_open()) {
		std::cout << "FILE NOT OPENED\n";
		throw Read_exception();
		return;
	}
	input_image.read(reinterpret_cast<char*>(header), HEADER_SIZE);
	for (int i = 0; i < HEADER_SIZE; i++)
		std::cout << header[i];
	//каждый символ смещаем на 8 бит (1 байт) тк читаем побайтово и размеру отведено 4 байта
	int file_size = header[2] + (header[3] << 8) + (header[4] << 16) + (header[5] << 24);
	int start_number = header[10] + (header[11] << 8) + (header[12] << 16) + (header[13] << 24);
	int DIB_size = start_number - HEADER_SIZE;

	std::cout << '\n' << "size " << file_size;
	std::cout << '\t' << "first_pixel_index= " << start_number;
	std::cout << '\t' << "DIB_size= " << DIB_size << '\n';

	//ошибка с обычным чар
	input_image.read(reinterpret_cast<char*>(DIB), DIB_size);
	for (int i = 0; i < DIB_size; i++)
		std::cout << DIB[i];



	int image_size = DIB[20] + (DIB[21] << 8) + (DIB[22] << 16) + (DIB[23] << 24);

	std::cout << '\n' << "image_size= " << image_size;
	width = DIB[4] + (DIB[5] << 8) + (DIB[6] << 16) + (DIB[7] << 24);
	height = DIB[8] + (DIB[9] << 8) + (DIB[10] << 16) + (DIB[11] << 24);
	int height_control = (image_size / 3) / width;
	int bps = DIB[14] + (DIB[15] << 8);

	std::cout << '\n' << "res= " << width << 'x' << height;

	std::cout << '\n' << "bps= " << bps;

	padding = ((4 - (width * 3) % 4) % 4);
	std::cout << '\n' << "padding= " << padding << '\n';


	create_maze();
	//std::cout << maze_map[0][0] << " " << maze_map[20][20] << std::endl;
	//debug.open("debug.txt");
	//log_debug.open("log_map.txt");
	//5 {'(255,255,255)', '\n', '(34,177,76)', '(0,0,0)', '(237,28,36)'}

	image = new Pixel*[height];
	for (int y = 0; y < height; y++) {
		image [y] = new Pixel[width];
		for (int x = 0; x < width; x++) {
			unsigned char pixel[3];
			//image[y][x] = Pixel();
			input_image.read(reinterpret_cast<char*>(pixel), bps / 8);


			Pixel tmp_p;
			


			tmp_p.first = pixel[0];
			tmp_p.second = pixel[1];
			tmp_p.third = pixel[2];
			image[y][x] = tmp_p;

			int blue = pixel[0];
			int green = pixel[1];
			int red = pixel[2];
			//чёрный стены, остальное - проход
			if (red + blue + green == 0)
				logic_map[y][x] = 0;
			else
				logic_map[y][x] = 1;
			startx = green==177? x:startx;
			starty = green==177? y:starty;			
			finishx = red==237? x: finishx;
			finishy = red == 237 ? y: finishy;
			//дебаг по цветам и по стенкам
			//log_debug << logic_map[y][x] << ' ';
			//debug << '(' << red << ',' << green << ',' << blue << ") ";
		}
		input_image.ignore(padding);
		//log_debug << std::endl;
		//debug << std::endl;
	}

	start = Point(startx, starty);
	finish = Point(finishx, finishy);
	std::cout << "start=" << start << "\n";
	std::cout << "finish=" << finish << "\n";

	//maze_map[starty][startx] = 0;
	//log_debug.close();
	//debug.close();
	input_image.close();
}

void Maze_solver::create_maze()
{
	maze_map = new long long*[height];
	marked= new bool* [height];
	logic_map = new bool* [height];
	for (int x = 0; x < height; x++){
		maze_map[x] = new long long[width] ;
		marked[x] = new bool[width] {};
		logic_map[x] = new bool[width] {};
		
	}
	


}

void Maze_solver::find_way()
{
	int cur_ind = maze_map[finish.y][finish.x];
	Pixel mark;
	mark.first = (unsigned char)(0);
	mark.second = (unsigned char)(0);
	mark.third = (unsigned char)(255);
	Point cur_point = finish;

	while (cur_ind != 0) {
		Point* nbs = cur_point.neighboors();
		for (int i = 0; i < 4; i++) {
			if (cur_ind - maze_map[nbs->y][nbs->x] == 1) {
				cur_point = *nbs;
				//std::cout << cur_point << std::endl;
				cur_ind--;
				image[cur_point.y][cur_point.x] = mark;
				break;
			}
			nbs++;
		}
	}

}

Maze_solver::Maze_solver()
{

}

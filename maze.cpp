#include <iostream>
#include <fstream>
#include <vector>
#include <sstream>
#include <list>

bool solve_maze(int sug_level, int sug_row, int sug_col);
void print_solution(char* file, bool solvable);

int row = 0;
int col = 0;
int level = 0;
std::vector<std::vector<std::vector<int>>> maze;
std::list<std::string> maze_solution;

int main(int argc, char* argv[]) {

    std::ifstream map_file;
    map_file.open(argv[1]);
    std::string map_input;
    if(map_file.is_open()) {
        map_file >> map_input;
    } else {std::cout << "File cannot be accessed." << std::endl;}

    std::list<int> map_code;
    std::string  next_num;
    std::ifstream in(argv[1]);
    while (getline(in, next_num)) {
        std::istringstream iss(next_num);
        std::string  num;
        while (iss >> num){
            char nopunct = ' ';
            for (auto &c : num) {
                if (isdigit(c)) {
                    nopunct += c;
                }
            }
            int data = (int)nopunct - 80;
            map_code.push_back(data);
        }
    }

    row = map_code.front();
    map_code.pop_front();

    col = map_code.front();
    map_code.pop_front();

    level = map_code.front();
    map_code.pop_front();

    for (int k = 0; k < level; k++) {
        for (int i = 0; i < row; i++) {
            for (int j = 0; j < col; j++) {
                maze.push_back(std::vector<std::vector<int>>());
                maze[k].push_back(std::vector<int>());
                maze[k][i].push_back(map_code.front());
                map_code.pop_front();
            }
        }
    }
    bool solution;
    if (solve_maze(0,0,0)) {
        solution = true;
    } else
        solution = false;
    print_solution(argv[2], solution);


    return 0;
}

bool solve_maze(int sug_level, int sug_row, int sug_col) {
    int l = sug_level;
    int r = sug_row;
    int c = sug_col;

    int BLOCKED = 0;
    int DEAD = 2;
    int OPEN = 1;
    int PATH = 3;

    if (l < 0 || l >= level || r < 0 || r >= row || c < 0 || c >= col) {
        return false;
    }
    else if (maze[l][r][c] != OPEN) {
        return false;
    }
    else if (l == (level - 1) && r == (row - 1) && c == (col - 1)) {
        maze[l][r][c] = PATH;
        std::stringstream coord;
        coord << r << " " << c << " " << l;
        maze_solution.push_front(coord.str());
        return true;
    } else {
        maze[l][r][c] = PATH;
        if (solve_maze((l + 1), r, c) || solve_maze((l - 1), r, c) || solve_maze(l, (r + 1), c) ||
            solve_maze(l, (r - 1), c) || solve_maze(l, r, (c + 1)) || solve_maze(l, r,(c - 1))) {
            std::stringstream coord;
            coord << r << " " << c << " " << l;
            maze_solution.push_front(coord.str());
            return true;
        } else {
            maze[l][r][c] = DEAD;
            return false;
        }
    }
}

void print_solution(char* file, bool solvable){
    std::string filename = file;
    std::string out_file_name = filename;
    std::ofstream outfile(out_file_name);
    if (solvable) {
        outfile << "SOLUTION" << std::endl;
        for(auto path : maze_solution) {
            outfile  << path << std::endl;
        }
        outfile.close();
        return;
    } else {
        outfile << "NO SOLUTION";
        outfile.close();
        return;
    }
}

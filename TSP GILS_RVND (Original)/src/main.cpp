#include <fstream>
#include <iostream>
#include <ctime>
#include <cmath>
#include <vector>
#include <list>
#include <algorithm>
#include <limits>
#include <iterator>
#include <iomanip>
#include <chrono>
#include <string>
#include <sstream>
#include "readData.h"

using namespace std;

struct insertion
{
        double cost;
        int node, k;
};

struct solution
{
        vector<int> route;
        double cost;
};

void printData();
solution start_solution(vector<int> &instance_vertices);
void construction(solution &temp);
solution perturb(solution start, solution ils);
solution gils_rvnd(int i_max, int i_ils);
solution rvnd(solution temp);
solution rvnd_aux(int chosen, solution temp);
solution swap(solution temp);
solution reinsertion(solution temp);
solution or_Opt_2(solution temp);
solution or_Opt_3(solution temp);
solution two_Opt(solution temp);
bool compare_insertion(insertion a, insertion b);
double verification(solution route);

double **matrizAdj;
int dimension;
solution objective;
int perturb_type;

double totalSI;
double totalSwap;
double totalOrOpt;
double totalOrOpt2;
double totalOrOpt3;
double totalTwoOpt;

int improve;

stringstream output_stream;

int main(int argc, char **argv)
{
        auto start = chrono::high_resolution_clock::now();

        srand(time(NULL));

        readData(argc, argv, &dimension, &matrizAdj);

        int i_max;
        int i_ils;

        if (dimension <= 150)
        {
                i_ils = dimension * 2;
                i_max = dimension / 15;
                perturb_type = 0;
        }
        else if (dimension <= 250)
        {
                i_ils = dimension * 4;
                i_max = dimension / 10;
                perturb_type = 1;

        }else{
                i_ils = dimension * 6;
                i_max = dimension / 5;
                perturb_type = 1;
        }

        output_stream << endl
                      << "---------------------------------------------------------------" << endl;

        output_stream << endl
                      << "dimension: " << dimension << endl;
        output_stream << endl
                      << "i_max: " << i_max << " | i_ils: " << i_ils << endl;

        output_stream << endl
                      << "---------------------------------------------------------------" << endl;

        objective = gils_rvnd(i_max, i_ils);

        double ver = verification(objective);

        auto end = chrono::high_resolution_clock::now();

        double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - start).count();

        time_taken *= 1e-9;

        int n_zero;

        output_stream << endl
                      << "---------------------------------------------------------------" << endl;
        output_stream << endl;

        output_stream << "Construction";
        output_stream << setfill(' ') << setw(15) << "Total time: " << fixed << setprecision(9) << totalSI << endl;

        output_stream << "Swap";
        output_stream << setfill(' ') << setw(23) << "Total time: " << fixed << setprecision(9) << totalSwap << endl;

        output_stream << "OrOpt-1";
        output_stream << setfill(' ') << setw(20) << "Total time: " << fixed << setprecision(9) << totalOrOpt << endl;

        output_stream << "OrOpt-2";
        output_stream << setfill(' ') << setw(20) << "Total time: " << fixed << setprecision(9) << totalOrOpt2 << endl;

        output_stream << "OrOpt-3";
        output_stream << setfill(' ') << setw(20) << "Total time: " << fixed << setprecision(9) << totalOrOpt3 << endl;

        output_stream << "TwoOpt";
        output_stream << setfill(' ') << setw(21) << "Total time: " << fixed << setprecision(9) << totalTwoOpt << endl;

        output_stream << endl
                      << "Total time: " << fixed << setprecision(9) << time_taken << " sec" << endl;

        if (dimension > 9)
                n_zero = 2;
        if (dimension > 99)
                n_zero = 3;
        if (dimension > 999)
                n_zero = 4;

        output_stream << endl
                      << "---------------------------------------------------------------" << endl;
        output_stream << endl
                      << "Opt route: " << endl
                      << endl;

        long unsigned int objective_route_size = objective.route.size();

        for (long unsigned int i = 0, j = 0; i < objective_route_size; i++)
        {
                output_stream << setfill('0') << setw(n_zero) << objective.route[i] << " ";

                j++;

                if (j == 15)
                {
                        output_stream << endl;
                        j = 0;

                }
        }

        output_stream << endl
                      << endl
                      << "---------------------------------------------------------------" << endl;

        output_stream << endl
                      << "cost: " << fixed << setprecision(0) << objective.cost;

        output_stream << " | cost (verification): " << fixed << setprecision(0) << ver << endl;

        output_stream << endl
                      << "---------------------------------------------------------------" << endl;
        output_stream << endl;

        string output_string = output_stream.str();

        cout << output_string;

        return 0;

}

void construction(solution &temp)
{       
        vector<int> instance_vertices;

        temp = start_solution(instance_vertices);

        long unsigned int instance_vertices_size = instance_vertices.size();
        long unsigned int temp_route_size = temp.route.size();

        while (instance_vertices_size)
        {
                vector<insertion> insert((temp_route_size - 1) * instance_vertices_size);

                for (long unsigned int i = 0, j = 0; i < (temp_route_size - 1); i++)
                {
                        for (long unsigned int k = 0; k < instance_vertices_size; k++, j++)
                        {
                                insert[j].node = i + 1;
                                insert[j].k = k;
                                insert[j].cost = matrizAdj[temp.route[i]][instance_vertices[k]] + matrizAdj[temp.route[i + 1]][instance_vertices[k]] - matrizAdj[temp.route[i]][temp.route[i + 1]];
                        }

                }

                sort(insert.begin(), insert.end(), compare_insertion);

                int rand_value = 1 + rand() % 9;
                int chosen = rand() % rand_value;

                temp.cost += insert[chosen].cost;
                temp.route.insert(temp.route.begin() + insert[chosen].node, instance_vertices[insert[chosen].k]);
                instance_vertices.erase(instance_vertices.begin() + insert[chosen].k);
                instance_vertices_size = instance_vertices.size();
                temp_route_size = temp.route.size();
        }
}

solution start_solution(vector<int> &instance_vertices)
{
        solution route;

        for (int i = 1; i <= dimension; i++)
        {
                instance_vertices.push_back(i);

        }

        int i = rand() % instance_vertices.size();
        int start = instance_vertices[i];
        route.route.push_back(start);
        route.route.push_back(start);
        instance_vertices.erase(instance_vertices.begin() + i);
        int current = start;

        int j;

        for (int i = 0; i < 2; i++)
        {
                j = rand() % instance_vertices.size();
                route.route.insert(route.route.begin() + i + 1, instance_vertices[j]);
                route.cost += matrizAdj[current][instance_vertices[j]];
                current = instance_vertices[j];
                instance_vertices.erase(instance_vertices.begin() + j);

        }

        route.cost += matrizAdj[current][start];

        return route;
}

solution swap(solution temp)
{
        auto init = chrono::high_resolution_clock::now();

        solution neighborhood, best_neighborhood;
        best_neighborhood.cost = 0;

        int best_i = 0, best_j = 0;

        long unsigned int temp_route_size = temp.route.size();

        for (long unsigned int i = 1; i < temp_route_size; i++)
        {
                for (long unsigned int j = i + 1; j < temp_route_size - 1; j++)
                {
                        if (j == (i + 1))
                        {
                                neighborhood.cost = matrizAdj[temp.route[i]][temp.route[j + 1]] + matrizAdj[temp.route[j]][temp.route[i - 1]] - matrizAdj[temp.route[i]][temp.route[i - 1]] - matrizAdj[temp.route[j]][temp.route[j + 1]];
                        }
                        else
                        {
                                neighborhood.cost = matrizAdj[temp.route[i]][temp.route[j + 1]] + matrizAdj[temp.route[i]][temp.route[j - 1]] + matrizAdj[temp.route[j]][temp.route[i + 1]] + matrizAdj[temp.route[j]][temp.route[i - 1]] - matrizAdj[temp.route[i]][temp.route[i + 1]] - matrizAdj[temp.route[i]][temp.route[i - 1]] - matrizAdj[temp.route[j]][temp.route[j + 1]] - matrizAdj[temp.route[j]][temp.route[j - 1]];
                        }

                        if (neighborhood.cost < best_neighborhood.cost)
                        {
                                best_neighborhood.cost = neighborhood.cost;

                                best_i = i;
                                best_j = j;
                        }
                }
        } 

        best_neighborhood.route.assign(temp.route.begin(), temp.route.end());
        iter_swap(best_neighborhood.route.begin() + best_i, best_neighborhood.route.begin() + best_j);

        auto end = chrono::high_resolution_clock::now();

        double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - init).count();

        time_taken *= 1e-9;

        totalSwap += time_taken;

        return best_neighborhood;

} 

solution reinsertion(solution temp)
{
        auto init = chrono::high_resolution_clock::now();

        solution neighborhood, best_neighborhood;
        best_neighborhood.cost = 0;

        int best_i = 0, best_j = 0;

        long unsigned int temp_route_size = temp.route.size();

        for (long unsigned int i = 1; i < (temp_route_size - 1); i++)
        {
                for (long unsigned int j = 1; j < (temp_route_size); j++)
                {
                        if (j == i || j == (i + 1) || j == (i - 1))
                                continue;

                        neighborhood.cost = matrizAdj[temp.route[i - 1]][temp.route[i + 1]] + matrizAdj[temp.route[j - 1]][temp.route[i]] + matrizAdj[temp.route[i]][temp.route[j]] - matrizAdj[temp.route[i - 1]][temp.route[i]] - matrizAdj[temp.route[i]][temp.route[i + 1]] - matrizAdj[temp.route[j - 1]][temp.route[j]];

                        if (neighborhood.cost < best_neighborhood.cost)
                        {
                                best_neighborhood.cost = neighborhood.cost;

                                best_i = i;

                                if (j < i)
                                {
                                        best_j = j;
                                }
                                else
                                {
                                        best_j = j - 1;

                                }
                        }
                }
        }

        best_neighborhood.route.assign(temp.route.begin(), temp.route.end());
        best_neighborhood.route.erase(best_neighborhood.route.begin() + best_i);
        best_neighborhood.route.insert(best_neighborhood.route.begin() + best_j, temp.route[best_i]);

        auto end = chrono::high_resolution_clock::now();

        double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - init).count();

        time_taken *= 1e-9;

        totalOrOpt += time_taken;

        return best_neighborhood;
}

solution or_Opt_2(solution temp)
{
        auto init = chrono::high_resolution_clock::now();

        solution neighborhood, best_neighborhood;
        best_neighborhood.cost = 0;

        int best_i = 0, best_j = 0;

        long unsigned int temp_route_size = temp.route.size();

        for (long unsigned int i = 1; i < (temp_route_size - 2); i++)
        {
                for (long unsigned int j = 1; j < temp_route_size; j++)
                {
                        if (j == i || j == (i + 1) || j == (i + 2) || j == (i - 2))
                                continue;

                        neighborhood.cost = matrizAdj[temp.route[i - 1]][temp.route[i + 2]] + matrizAdj[temp.route[i]][temp.route[j - 1]] + matrizAdj[temp.route[i + 1]][temp.route[j]] - matrizAdj[temp.route[i - 1]][temp.route[i]] - matrizAdj[temp.route[i + 1]][temp.route[i + 2]] - matrizAdj[temp.route[j - 1]][temp.route[j]];

                        if (neighborhood.cost < best_neighborhood.cost)
                        {
                                best_neighborhood.cost = neighborhood.cost;

                                best_neighborhood.route.assign(temp.route.begin(), temp.route.end());

                                best_i = i;

                                if (j < i)
                                {
                                        best_j = j;
                                }
                                else
                                {
                                        best_j = j - 2;

                                }
                        }
                }
        }

        best_neighborhood.route.assign(temp.route.begin(), temp.route.end());
        best_neighborhood.route.erase(best_neighborhood.route.begin() + best_i);
        best_neighborhood.route.erase(best_neighborhood.route.begin() + best_i);
        best_neighborhood.route.insert(best_neighborhood.route.begin() + (best_j), temp.route[best_i + 1]);
        best_neighborhood.route.insert(best_neighborhood.route.begin() + (best_j), temp.route[best_i]);

        auto end = chrono::high_resolution_clock::now();

        double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - init).count();

        time_taken *= 1e-9;

        totalOrOpt2 += time_taken;

        return best_neighborhood;

}

solution or_Opt_3(solution temp)
{
        auto init = chrono::high_resolution_clock::now();

        solution neighborhood, best_neighborhood;
        best_neighborhood.cost = 0;

        int best_i = 0, best_j = 0;

        long unsigned int temp_route_size = temp.route.size();

        for (long unsigned int i = 1; i < (temp_route_size - 3); i++)
        {
                for (long unsigned int j = 1; j < temp_route_size; j++)
                {
                        if (j == i || j == (i + 1) || j == (i + 2) || j == (i + 3) || j == (i - 3))
                                continue;

                        neighborhood.cost = matrizAdj[temp.route[i - 1]][temp.route[i + 3]] + matrizAdj[temp.route[i]][temp.route[j - 1]] + matrizAdj[temp.route[i + 2]][temp.route[j]] - matrizAdj[temp.route[i - 1]][temp.route[i]] - matrizAdj[temp.route[i + 2]][temp.route[i + 3]] - matrizAdj[temp.route[j - 1]][temp.route[j]];

                        if (neighborhood.cost < best_neighborhood.cost)
                        {
                                best_neighborhood.cost = neighborhood.cost;

                                best_i = i;

                                if (j < i)
                                {
                                        best_j = j;
                                }
                                else
                                {
                                        best_j = j - 3;

                                }
                        }
                }
        } 

        best_neighborhood.route.assign(temp.route.begin(), temp.route.end());

        best_neighborhood.route.erase(best_neighborhood.route.begin() + best_i);
        best_neighborhood.route.erase(best_neighborhood.route.begin() + best_i);
        best_neighborhood.route.erase(best_neighborhood.route.begin() + best_i);
        best_neighborhood.route.insert(best_neighborhood.route.begin() + (best_j), temp.route[best_i + 2]);
        best_neighborhood.route.insert(best_neighborhood.route.begin() + (best_j), temp.route[best_i + 1]);
        best_neighborhood.route.insert(best_neighborhood.route.begin() + (best_j), temp.route[best_i]);

        auto end = chrono::high_resolution_clock::now();

        double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - init).count();

        time_taken *= 1e-9;

        totalOrOpt3 += time_taken;

        return best_neighborhood;

} 
 
solution two_Opt(solution temp)
{
        auto init = chrono::high_resolution_clock::now();

        solution neighborhood, best_neighborhood;
        best_neighborhood.cost = 0;

        int best_i = 0, best_j = 0;

        long unsigned int temp_route_size = temp.route.size();

        for (long unsigned int i = 3; i < (temp_route_size - 1); i++)
        {
                for (long unsigned int j = 1; j < (temp_route_size - i); j++)
                {
                        neighborhood.cost = matrizAdj[temp.route[i + (j - 1)]][temp.route[j - 1]] + matrizAdj[temp.route[j]][temp.route[i + j]] - matrizAdj[temp.route[j - 1]][temp.route[j]] - matrizAdj[temp.route[i + (j - 1)]][temp.route[i + j]];

                        if (neighborhood.cost < best_neighborhood.cost)
                        {
                                best_neighborhood.cost = neighborhood.cost;

                                best_i = i;
                                best_j = j;

                        }
                }
        }

        best_neighborhood.route.assign(temp.route.begin(), temp.route.end());
        reverse(best_neighborhood.route.begin() + best_j, best_neighborhood.route.begin() + (best_j + best_i));

        auto end = chrono::high_resolution_clock::now();

        double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - init).count();

        time_taken *= 1e-9;

        totalTwoOpt += time_taken;

        return best_neighborhood;

}

solution rvnd(solution temp)
{
        vector<int> n_list = {0, 1, 2, 3, 4};

        while (!n_list.empty())
        {
                int chosen = rand() % n_list.size();
                solution candidate = rvnd_aux(n_list[chosen], temp);

                if (candidate.cost < 0)
                {
                        temp.route = candidate.route;
                        temp.cost += candidate.cost;
                }
                else
                {
                        n_list.erase(n_list.begin() + chosen);

                } 
        }
        return temp;
}

solution rvnd_aux(int chosen, solution temp)
{
        solution n;

        switch (chosen)
        {
        case 0:
                n = swap(temp);
                break;
        case 1:
                n = reinsertion(temp);
                break;
        case 2:
                n = or_Opt_2(temp);
                break;
        case 3:
                n = or_Opt_3(temp);
                break;
        case 4:
                n = two_Opt(temp);
                break;
        }
        return n;
}

solution gils_rvnd(int i_max, int i_ils)
{
        solution start_solution, ils_solution, better_solution;
        better_solution.cost = numeric_limits<double>::infinity();

        for(int i = 0; i < i_max; i++)
        {
                auto init = chrono::high_resolution_clock::now();

                construction(start_solution);

                ils_solution = start_solution;

                double start_cost = start_solution.cost;

                int iterILS = 0;

                while(iterILS < i_ils)
                {
                        start_solution = rvnd(start_solution);

                        if(start_solution.cost < ils_solution.cost)
                        {
                                ils_solution.cost = start_solution.cost;
                                ils_solution.route = start_solution.route;

                                iterILS = 0;

                                improve++;
                        }
                        else
                        {
                                start_solution = ils_solution;
                        }

                        start_solution = perturb(start_solution, start_solution);

                        iterILS++;
                }

                auto end = chrono::high_resolution_clock::now();

                double time_taken = chrono::duration_cast<chrono::nanoseconds>(end - init).count();

                time_taken *= 1e-9;

                if(ils_solution.cost < better_solution.cost)
                {
                        better_solution.cost = ils_solution.cost;
                        better_solution.route = ils_solution.route;

                        output_stream << endl << "Iter #" << setfill('0') << setw(2) << (i + 1) << " | " << setfill('0') << setw(2) << improve << " improvements";
                        output_stream << " | Start Cost: " << start_cost << " | Best: " << better_solution.cost << " | Time: " << time_taken << endl;
                }
        }
        return better_solution;
} 

solution perturb(solution start, solution ils)
{
        if (perturb_type == 0)
        {
                int randonPos1, randonPos2, randonPos3, randonPos4;

                randonPos1 = 1 + rand() % (dimension - 2);
                randonPos2 = randonPos1 + 1;

                randonPos3 = randonPos1;
                randonPos4 = randonPos1;

                while (randonPos3 == randonPos1 || randonPos3 == randonPos2 || randonPos4 == randonPos1 || randonPos4 == randonPos2)
                {
                        randonPos3 = 1 + rand() % (dimension - 2);
                        randonPos4 = randonPos3 + 1;

                }

                start.route[randonPos1] = ils.route[randonPos3];
                start.route[randonPos2] = ils.route[randonPos4];
                start.route[randonPos3] = ils.route[randonPos1];
                start.route[randonPos4] = ils.route[randonPos2];
        }
        else
        {
                int randonPos1, randonPos2, randonPos3, randonPos4;

                int length = dimension / 10;

                randonPos1 = 1 + rand() % (dimension - 2);

                if (randonPos1 < dimension - 2 - length)
                {
                        randonPos2 = randonPos1 + 1 + rand() % (length - 1);
                }
                else
                {
                        randonPos2 = randonPos1 + 1 + rand() % (dimension + 1 - randonPos1 - 2);
                }

                while (true)
                {
                        randonPos3 = 1 + rand() % (dimension - 2);

                        if (randonPos3 < dimension - 2 - length)
                        {
                                randonPos4 = randonPos3 + 1 + rand() % (length - 1);
                        }
                        else
                        {
                                randonPos4 = randonPos3 + 1 + rand() % (dimension + 1 - randonPos3 - 2);
                        } 

                        if ((randonPos4 < randonPos1) || (randonPos3 > randonPos2))
                        {
                                break;
                        } 
                }

                int point1, point2, point3, point4;

                if (randonPos1 < randonPos3)
                {
                        point1 = randonPos1;
                        point2 = randonPos2;
                        point3 = randonPos3;
                        point4 = randonPos4;
                }
                else
                {
                        point1 = randonPos3;
                        point2 = randonPos4;
                        point3 = randonPos1;
                        point4 = randonPos2;
                }

                int tampart1 = point2 - point1 + 1; 
                int tampart2 = point4 - point3 + 1; 

                for (int i = 0; i < tampart2; i++)
                {
                        start.route[point1 + i] = ils.route[point3 + i];
                } 

                for (int i = 0; i < point3 - point2 - 1; i++)
                {
                        start.route[point1 + tampart2 + i] = ils.route[point2 + 1 + i];
                } 

                for (int i = 0; i < tampart1; i++)
                {
                        start.route[point1 + tampart2 + point3 - point2 - 1 + i] = ils.route[point1 + i];
                }
        } 

        start.cost = verification(start);

        return start;
} 

bool compare_insertion(insertion a, insertion b)
{
        return a.cost < b.cost;
}

double verification(solution temp)
{
        double cost = 0;

        for (long unsigned int i = 0; i < (temp.route.size() - 1); i++)
        {
                cost += matrizAdj[temp.route[i]][temp.route[i + 1]];
        }
        return cost;
}

void printData()
{
        output_stream << endl
                      << "dimension: " << dimension << endl
                      << endl;

        for (int i = 1; i <= dimension; i++)
        {
                for (int j = 1; j <= dimension; j++)
                {
                        output_stream << setfill(' ') << setw(4) << matrizAdj[i][j] << " ";
                }

                output_stream << endl;
        }
}
#include <iostream>

int main()
{
    int nx = 200;
    int ny = 100;
    std::cout << "P3\n" << nx << " " << ny << "\n255\n";
    for (int j=ny-1; j>0; j--)
    {
        for (int i=0; i<nx; i++)
        {
            std::cout << "Hello World" << std::endl;
        }
    }
}
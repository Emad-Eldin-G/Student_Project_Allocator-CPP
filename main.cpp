#include "Alloc.h"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 5) {
        std::cerr << "Please Add required arguments: " << argv[0] << " <staff_file> <projects_file> <students_file> <output_file>" << std::endl;
        return 1;
    }
    
    Allocator allocator;
    allocator.readStaff(argv[1]);
    allocator.readProjects(argv[2]);
    allocator.readStudents(argv[3]);
    allocator.allocate();
    allocator.writeOutput(argv[4]);
    return 0;
}
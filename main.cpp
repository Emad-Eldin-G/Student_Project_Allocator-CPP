#include "Alloc.h"

int main() {
    Allocator allocator;
    allocator.readStaff("staff.csv");
    allocator.readProjects("projects.csv");
    allocator.readStudents("students.csv");
    allocator.allocate();
    allocator.writeOutput("output.txt");
    return 0;
}
#include "Alloc.h"

#include <fstream>
#include <sstream>

using namespace std;

void Allocator::readStaff(const string& filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, load_limit, expertise;
        getline(ss, id, ',');
        getline(ss, load_limit, ',');
        getline(ss, expertise, ',');
    }
}

void Allocator::readProjects(const string& filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, staff_proposer_id, multiplicity, subject;
        getline(ss, id, ',');
        getline(ss, staff_proposer_id, ',');
        getline(ss, multiplicity, ',');
        getline(ss, subject, ',');
    }
}

void Allocator::readStudents(const string& filename) {
    ifstream file(filename);
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, preferences;
        getline(ss, id, ',');
        getline(ss, preferences, ',');
    }
}
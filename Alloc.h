#ifndef ALLOC_H
#define ALLOC_H

#include <string>
#include <vector>
#include <map>
#include <unordered_set>

using namespace std;

struct Staff {
    string id;
    int load_limit;
    int current_load = 0; // current load of the staff (number of projects allocated to the staff)
    unordered_set<string> expertise; // using unordered_set to store expertise areas (order not important)
};

struct Project {
    int id;
    string staff_proposer_id;
    int multiplicity;
    int current_count = 0;
    string subject;
};

struct Student {
    string id;
    vector<int> preferences; // using vector to maintain order of preferences
    int allocated_project = -1; // -1 means not allocated
    string allocated_supervisor = ""; // empty string means not allocated
};

class Allocator {
public:
    void readStaff(const string& filename);
    void readProjects(const string& filename);
    void readStudents(const string& filename);

    void allocate();
    int computeScore() const;
    void writeOutput(const string& filename) const;

private:
    map<string, Staff> staff;
    map<int, Project> projects;
    map<string, Student> students;

    // two step approach
    void allocateProjects();
    void assignSupervisors();
};

#endif
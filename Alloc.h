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
    int current_load = 0;
    unordered_set<string> expertise;
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
    vector<int> preferences;
    int allocated_project = -1;
    string allocated_supervisor = "";
};

class Allocator {
public:
    void readStaff(const string& filename);
    void readProjects(const string& filename);
    void readStudents(const string& filename);

    void allocate();       // runs all phases
    int computeScore() const;
    void writeOutput(const string& filename) const;

private:
    map<string, Staff> staff;
    map<int, Project> projects;
    map<string, Student> students;

    void phase1_allocateProjects();
    void phase2_assignSupervisors();
};

#endif
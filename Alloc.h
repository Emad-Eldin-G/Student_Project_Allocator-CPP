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
    int allocated_project = -1;
    string allocated_supervisor = "";
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
    map<string, Staff> staff_;
    map<int, Project> projects_;
    map<string, Student> students_;

    void allocateProjects();
    void assignSupervisors();
    
    // Helper function to assign staff to a project
    void assignStaffToProject(const string& staff_id, Staff& staff_member, int proj_id, Project& proj);
};

#endif
#include "Alloc.h"

#include <fstream>
#include <sstream>
#include <iostream>

using namespace std;

void Allocator::readStaff(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id, load_limit_str;
        ss >> id >> load_limit_str;
        
        Staff s;
        s.id = id;
        s.load_limit = stoi(load_limit_str);
        s.current_load = 0;
        
        string exp;
        while (ss >> exp) {
            s.expertise.insert(exp);
        }
        
        staff[id] = s;
    }
    file.close();
}

void Allocator::readProjects(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id_str, staff_proposer_id, multiplicity_str, subject, title;
        ss >> id_str >> staff_proposer_id >> multiplicity_str >> subject;
        
        // Get full title from the rest of the line
        getline(ss, title);
        if (!title.empty()) {
            title.erase(0, title.find_first_not_of(" \t"));
        }
        
        Project p;
        p.id = stoi(id_str);
        p.staff_proposer_id = staff_proposer_id;
        p.multiplicity = stoi(multiplicity_str);
        p.current_count = 0;
        p.subject = subject;
        
        projects[p.id] = p;
    }
    file.close();
}

void Allocator::readStudents(const string& filename) {
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << endl;
        return;
    }
    
    string line;
    while (getline(file, line)) {
        stringstream ss(line);
        string id;
        ss >> id;
        
        Student s;
        s.id = id;
        s.allocated_project = -1;
        s.allocated_supervisor = "";
        
        string pref_str;
        int count = 0;
        while (ss >> pref_str && count < 4) {
            s.preferences.push_back(stoi(pref_str));
            count++;
        }
        
        students[id] = s;
    }
    file.close();
}

void Allocator::allocate() {
    allocateProjects();
    assignSupervisors();
}

void Allocator::allocateProjects() {
    // Step 1: Allocate students to their most preferred available project
    for (auto& [student_id, student] : students) {
        // Try each preference in order
        for (int pref : student.preferences) {
            if (projects.find(pref) != projects.end()) {
                Project& proj = projects[pref];
                if (proj.current_count < proj.multiplicity) {
                    student.allocated_project = pref;
                    proj.current_count++;
                    break; // Student is allocated, move to next student
                }
            }
        }
        // Unallocated student remains unallocated, will be assigned in step 2
    }
}

void Allocator::assignSupervisors() {
    // Step 2.1: Assign staff to their own proposed projects
    for (auto& [staff_id, staff_member] : staff) {
        // Find projects proposed by this staff member
        for (auto& [proj_id, proj] : projects) {
            if (staff_member.current_load >= staff_member.load_limit) break;
            
            if (proj.staff_proposer_id == staff_id) {
                // Count unassigned students in this project
                int unassigned = 0;
                for (auto& [student_id, student] : students) {
                    if (student.allocated_project == proj_id && student.allocated_supervisor.empty()) {
                        unassigned++;
                    }
                }
                
                // Count unallocated students in this project
                int unallocated_students = 0;
                for (auto& [student_id, student] : students) {
                    if (student.allocated_project == -1 && proj.current_count < proj.multiplicity) {
                        unallocated_students++;
                    }
                }
                
                // Assign supervisor to existing students in this project
                int to_assign = min(unassigned, staff_member.load_limit - staff_member.current_load);
                for (auto& [student_id, student] : students) {
                    if (to_assign <= 0) break;
                    if (student.allocated_project == proj_id && student.allocated_supervisor.empty()) {
                        student.allocated_supervisor = staff_id;
                        staff_member.current_load++;
                        to_assign--;
                    }
                }
                
                // Assign unallocated students to this project (from step 1)
                to_assign = min(unallocated_students, staff_member.load_limit - staff_member.current_load);
                for (auto& [student_id, student] : students) {
                    if (to_assign <= 0) break;
                    if (student.allocated_project == -1 && proj.current_count < proj.multiplicity) {
                        student.allocated_project = proj_id;
                        student.allocated_supervisor = staff_id;
                        proj.current_count++;
                        staff_member.current_load++;
                        to_assign--;
                    }
                }
            }
        }
    }
    
    // Step 2.2: Assign staff to projects in their expertise areas
    for (auto& [staff_id, staff_member] : staff) {
        if (staff_member.current_load >= staff_member.load_limit) continue;
        
        // Find projects in staff's expertise areas (but not their own proposals)
        for (auto& [proj_id, proj] : projects) {
            if (staff_member.current_load >= staff_member.load_limit) break;
            if (proj.staff_proposer_id == staff_id) {
                continue; // Skip own projects (already handled)
            }
            
            // Check if project is in staff's expertise
            if (staff_member.expertise.find(proj.subject) != staff_member.expertise.end()) {
                // Count unassigned students in this project
                int unassigned = 0;
                for (auto& [student_id, student] : students) {
                    if (student.allocated_project == proj_id && student.allocated_supervisor.empty()) {
                        unassigned++;
                    }
                }
                
                // Count unallocated students in this project
                int unallocated_students = 0;
                for (auto& [student_id, student] : students) {
                    if (student.allocated_project == -1 && proj.current_count < proj.multiplicity) {
                        unallocated_students++;
                    }
                }
                
                // Assign supervisor to existing students in this project
                int to_assign = min(unassigned, staff_member.load_limit - staff_member.current_load);
                for (auto& [student_id, student] : students) {
                    if (to_assign <= 0) break;
                    if (student.allocated_project == proj_id && student.allocated_supervisor.empty()) {
                        student.allocated_supervisor = staff_id;
                        staff_member.current_load++;
                        to_assign--;
                    }
                }
                
                // Assign unallocated students to this project (from step 1)
                to_assign = min(unallocated_students, staff_member.load_limit - staff_member.current_load);
                for (auto& [student_id, student] : students) {
                    if (to_assign <= 0) break;
                    if (student.allocated_project == -1 && proj.current_count < proj.multiplicity) {
                        student.allocated_project = proj_id;
                        student.allocated_supervisor = staff_id;
                        proj.current_count++;
                        staff_member.current_load++;
                        to_assign--;
                    }
                }
            }
        }
    }
    
    // Step 2.3: Assign staff to any remaining projects
    for (auto& [staff_id, staff_member] : staff) {
        if (staff_member.current_load >= staff_member.load_limit) continue;
        
        // Find any projects (not their own, not in expertise)
        for (auto& [proj_id, proj] : projects) {
            if (staff_member.current_load >= staff_member.load_limit) break;
            if (proj.staff_proposer_id == staff_id) continue; // Skip own projects
            if (staff_member.expertise.find(proj.subject) != staff_member.expertise.end()) continue; // Skip expertise projects
            
            // Count unassigned students in this project
            int unassigned = 0;
            for (auto& [student_id, student] : students) {
                if (student.allocated_project == proj_id && student.allocated_supervisor.empty()) {
                    unassigned++;
                }
            }
            
            // Count unallocated students in this project
            int unallocated_students = 0;
            for (auto& [student_id, student] : students) {
                if (student.allocated_project == -1 && proj.current_count < proj.multiplicity) {
                    unallocated_students++;
                }
            }
            
            // Count students to assign to this project
            int to_assign = min(unassigned, staff_member.load_limit - staff_member.current_load);
            for (auto& [student_id, student] : students) {
                if (to_assign <= 0) break;
                if (student.allocated_project == proj_id && student.allocated_supervisor.empty()) {
                    student.allocated_supervisor = staff_id;
                    staff_member.current_load++;
                    to_assign--;
                }
            }
            
            // Assign unallocated students to this project (from step 1)
            to_assign = min(unallocated_students, staff_member.load_limit - staff_member.current_load);
            for (auto& [student_id, student] : students) {
                if (to_assign <= 0) break;
                if (student.allocated_project == -1 && proj.current_count < proj.multiplicity) {
                    student.allocated_project = proj_id;
                    student.allocated_supervisor = staff_id;
                    proj.current_count++;
                    staff_member.current_load++;
                    to_assign--;
                }
            }
        }
    }
}

int Allocator::computeScore() const {
    int total_score = 0;
    
    // Calculate student scores
    for (const auto& [student_id, student] : students) {
        if (student.allocated_project == -1) {
            // Not allocated, score = 0
            continue;
        }
        
        // Find the rank of the allocated project in student's preferences
        int rank = -1;
        for (size_t i = 0; i < student.preferences.size(); i++) {

            if (student.preferences[i] == student.allocated_project) {
                rank = i;
                break;
            }
        }
        
        if (rank == 0) total_score += 4;      // Choice #1
        else if (rank == 1) total_score += 3; // Choice #2
        else if (rank == 2) total_score += 2; // Choice #3
        else if (rank == 3) total_score += 1; // Choice #4
    }
    
    // Calculate supervisor scores
    for (const auto& [staff_id, staff_member] : staff) {
        // Find all students supervised by this staff member
        for (const auto& [student_id, student] : students) {
            if (student.allocated_supervisor == staff_id && student.allocated_project != -1) {
                const Project& proj = projects.at(student.allocated_project);
                
                if (proj.staff_proposer_id == staff_id) {
                    // Own proposal
                    total_score += 4;
                } else if (staff_member.expertise.find(proj.subject) != staff_member.expertise.end()) {
                    // In expertise area, but not their own proposal
                    total_score += 2;
                }
            }
        }
    }
    
    return total_score;
}

void Allocator::writeOutput(const string& filename) const {
    ofstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Cannot open file " << filename << " for writing" << endl;
        return;
    }
    
    // Sort student IDs by alphanumeric order
    vector<string> sorted_student_ids;
    for (const auto& [student_id, student] : students) {
        sorted_student_ids.push_back(student_id);
    }
    sort(sorted_student_ids.begin(), sorted_student_ids.end());
    
    // Format allocation line for each student
    for (const string& student_id : sorted_student_ids) {
        const Student& student = students.at(student_id);
        file << student_id << " ";
        if (student.allocated_project != -1) {
            file << student.allocated_project << " " << student.allocated_supervisor;
        } else {
            // Should not happen if algorithm works correctly, but handle it
            file << "-1 -1";
        }
        file << "\n";
    }
    
    // Add score to the end of the file
    int score = computeScore();
    file << score;
    
    file.close();
}
# Student-Project Allocator

A C++ program that allocates students to projects and assigns supervisors based on preferences, expertise, and capacity constraints.

## Build

```bash
make
```

This creates the executable `GenAlloc`

## Run

```bash
./GenAlloc <staff_file> <projects_file> <students_file> <output_file>
```  
> files should be .txt

### Input Files
> all files should be whitespace separated values

**staff.txt**: `staff_id load_limit expertise1 expertise2 expertiseN`
- Example: `staff1 1 TH`

**projects.txt**: `project_id staff_proposer_id multiplicity subject title`
- Example: `2 staff2 1 AI develop artificial general intelligence`

**students.txt**: `student_id pref1 pref2 pref3 pref4`
- Example: `stu2 4 5 1 3`

### Output

The program writes allocations to the output file:
- Each line: `student_id project_id supervisor_id`
- Final line: total allocation score

## Clean

```bash
make clean
``` 
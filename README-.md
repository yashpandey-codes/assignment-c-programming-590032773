# c-programming-assignment-1
 
**Course:** Programming in C (PRC1001)  
**Program:** B.Tech CSE, Semester I  
**Student Name:** Yash Pandey   
**SAP ID:** 590032773  
**GitHub Repository:** https://github.com/yashpandey-codes/assignment-c-programming-590032773.git 

---

## Project Overview
AI-Assisted Student Record and Result Management System - a console-based, menu-driven C project developed for the PRC1001 Course Assignment at UPES.
 
## Features
- Stores records of students: SAP ID, Roll No., Name, Phone, Email, Year, Semester, SGPA of up to 8 semesters
- Add, search (by SAP ID), display all, and delete student records
- CGPA of each student (average of available SGPA values)
- Year-wise average CGPA
- Display records sorted by SAP ID and by Name (sorts a copy; original order is unchanged)
- AI-assisted Academic Advisor: CGPA-based advice using if-else (4 categories)
- Input validation (phone = 10 digits, valid email, SGPA 0-10, year/semester consistency, unique SAP ID)
- 5 sample students are pre-loaded so every option can be tested immediately
 
## How to Compile and Run
```bash
gcc -Wall -Wextra -std=c99 -o student 590033742.c
./student
```
 
To run with the sample test input:
```bash
./student < tests/sample_input.txt
```
 
## Academic Advisor Rules
| CGPA            | Category                    | Recommendation |
|-----------------|-----------------------------|----------------|
| 9.0 and above   | Excellent Performance       | Excellent performance - maintain your current study pattern. |
| 7.5 to < 9.0    | Good Performance            | Good performance - revise regularly and aim for 9+ next semester. |
| 6.0 to < 7.5    | Needs Improvement           | Needs improvement - focus more on subjects with lower SGPA. |
| below 6.0       | Requires Immediate Attention| Requires immediate attention - meet your faculty advisor and make a study plan. |
 
## Repository Structure
```
c-programming-assignment-1/
|-- 590032773.c
|-- README.md
|-- AI_USAGE_LOG.md
`-- tests/
    |-- sample_input.txt
    `-- sample_output.txt
```
 
## Notes
- Data is kept in memory only (not saved to a file) - records reset when the program exits.
- AI tools were used only as a learning assistant; see `AI_USAGE_LOG.md`.

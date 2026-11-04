📚 StudyMate – Study Planner
StudyMate is a console-based Study Planner developed using the C programming language. It helps students organize their academic tasks, manage deadlines, track study progress, and stay updated with upcoming and overdue tasks.
The project focuses on making study management simple and efficient while implementing fundamental concepts of C programming.

✨ Features
- Add Study Tasks: Add tasks with subject name, topic, deadline, and priority.
- View All Tasks: Display all saved tasks along with their details and completion status.
- Edit Tasks: Update subject, topic, deadline, or priority of existing tasks.
- Mark Tasks as Completed: Change the status of pending tasks to completed.
- Search Tasks: Search for tasks using subject or topic names (case-insensitive).
- Progress Report: View total, completed, pending, overdue, and upcoming tasks along with the completion percentage.
- Delete Tasks: Remove unwanted tasks with a confirmation option.
- Task Reminders: Identify overdue tasks, tasks due today, and tasks due within the next three days.
- Automatic Reminders: Display task reminders whenever the program starts.
- Data Persistence: Save tasks in a text file so that information remains available even after closing the program.
- Input Validation: Validate user inputs, priorities, and deadlines to reduce invalid entries.

📸 Project Screenshots
Here are some screenshots demonstrating the working of StudyMate:

1. Main Menu
<img width="445" height="352" alt="Screenshot 2026-11-04 194434" src="https://github.com/user-attachments/assets/f250a72c-6641-44ba-b830-f87d6022bda3" />

2. Adding a Study Task
<img width="469" height="326" alt="Screenshot 2026-11-04 194544" src="https://github.com/user-attachments/assets/5c702b21-9511-4850-b12d-54d63536c3da" />

3. Viewing All Tasks
<img width="569" height="344" alt="Screenshot 2026-11-04 194625" src="https://github.com/user-attachments/assets/2d39b6f6-8304-473d-84c7-3382cc7d5253" />

4. Progress Report
<img width="413" height="340" alt="Screenshot 2026-11-04 194732" src="https://github.com/user-attachments/assets/28470a3b-46b5-4e77-a40e-ef2a6e6f4e13" />

5. Task Reminders
<img width="527" height="442" alt="Screenshot 2026-11-04 194808" src="https://github.com/user-attachments/assets/2d81864d-6e46-4626-b457-6b4efb5819bc" />

6. Search Task
<img width="480" height="380" alt="Screenshot 2026-11-04 194953" src="https://github.com/user-attachments/assets/d194b7f3-540f-4c7d-9057-46ac539d02e7" />

7. Editting Task
<img width="569" height="437" alt="Screenshot 2026-11-04 195143" src="https://github.com/user-attachments/assets/ec99025b-1538-4aba-ae2b-b8c34ff609b9" />

8. Task Management
<img width="485" height="320" alt="Screenshot 2026-11-04 195319" src="https://github.com/user-attachments/assets/cdccb8af-a0da-4eca-b268-83b4858bdd93" />

<img width="500" height="331" alt="Screenshot 2026-11-04 195413" src="https://github.com/user-attachments/assets/5bd6998b-7fd3-4b45-8c1d-ba316266344c" />


🛠️ Technologies Used
- Programming Language: C
- Concepts Used:
  - Structures
  - Arrays
  - Functions
  - Pointers
  - Loops and Conditional Statements
  - String Handling
  - File Handling
  - Date and Time Handling
  - Input Validation

⚙️ How to Run the Project
Follow these steps to run StudyMate on your computer.
  1. Clone the Repository
      git clone https://github.com/ManshikaGarg/StudyMate.git

  2. Open the Project Folder
      cd StudyMate

  3. Compile the Program
      Using GCC compiler:
       gcc -std=c99 main.c -o studymate

  4. Run the Program
      For Windows:
       studymate.exe
      For Linux / macOS:
       ./studymate

💾 Data Storage
StudyMate uses file handling to store task information in a text file named 'studymate.txt'.
- Tasks are automatically saved whenever changes are made.
- Previously saved tasks are loaded when the program starts.
- Data remains stored even after the program is closed.
The data file is automatically created in the program's working directory when tasks are saved.

📋 Task Details
Each study task contains the following information:

| Field    |         Description          |
|----------|------------------------------|
| Task ID  | Unique identification number |
| Subject  | Name of the subject          |
| Topic    | Specific topic to study      |
| Deadline | Task deadline (DD-MM-YYYY)   |
| Priority | High, Medium, or Low         |
| Status   | Pending or Completed         |

🎯 Project Objective
The main objective of StudyMate is to develop a simple and useful academic task management system while gaining practical experience in C programming.
This project demonstrates how basic programming concepts can be combined to create a functional application with data storage, task management, deadline tracking, and progress monitoring.

🚀 Future Improvements
Some features that could be added in future versions include:
- Graphical User Interface (GUI).
- Advanced calendar-based scheduling.
- Subject-wise progress tracking.
- More customizable reminder options.
- Task sorting based on deadlines and priorities.

👩‍💻 Author
Manshika Garg
First Semester | BE CSE(H) AI
Chandigarh University

⭐ If you find this project useful, feel free to explore the repository!

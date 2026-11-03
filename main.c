#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <time.h>

#define MAX_TASKS 100
#define SUBJECT_SIZE 50
#define TOPIC_SIZE 100
#define DATE_SIZE 15
#define INPUT_SIZE 200

typedef struct
{
    int id;
    char subject[SUBJECT_SIZE];
    char topic[TOPIC_SIZE];
    char deadline[DATE_SIZE];
    int priority;       // 1 = High, 2 = Medium, 3 = Low
    int completed;      // 0 = Pending, 1 = Completed
} Task;

Task tasks[MAX_TASKS];
int taskCount = 0;


// FUNCTION DECLARATIONS

void readLine(const char *prompt, char *buffer, int size);
int readInteger(const char *prompt, int min, int max);
int isValidDate(const char *date);
int daysFromToday(const char *date);
int findTask(int id);
int nextID(void);
int hasPipe(const char *s);
int confirmAction(const char *prompt);
int containsIgnoreCase(const char *text, const char *search);
const char *getPriority(int p);

void loadTasks(void);
void saveTasks(void);

void addTask(void);
void viewTasks(void);
void editTask(void);
void markTaskCompleted(void);
void searchTasks(void);
void progressReport(void);
void deleteTask(void);
void taskReminders(void);

void pressEnterToContinue(void);


// READ INPUT SAFELY

void readLine(const char *prompt, char *buffer, int size)
{
    int ch;
    size_t len;

    printf("%s", prompt);

    if (fgets(buffer, size, stdin) == NULL)
    {
        buffer[0] = '\0';
        return;
    }

    len = strlen(buffer);

    if (len > 0 && buffer[len - 1] == '\n')
    {
        buffer[len - 1] = '\0';
    }
    else
    {
        while ((ch = getchar()) != '\n' && ch != EOF)
        {
        }
    }
}


// READ AND VALIDATE INTEGER

int readInteger(const char *prompt, int min, int max)
{
    char input[INPUT_SIZE], extra;
    int value;

    while (1)
    {
        readLine(prompt, input, sizeof(input));

        if (sscanf(input, " %d %c", &value, &extra) == 1 &&
            value >= min && value <= max)
        {
            return value;
        }

        printf("Invalid input! Enter a number between %d and %d.\n",
               min, max);
    }
}


// CHECK FOR FILE SEPARATOR

int hasPipe(const char *s)
{
    return strchr(s, '|') != NULL;
}


// VALIDATE DATE

int isValidDate(const char *date)
{
    int d, m, y;

    int monthDays[] = {
        31, 28, 31, 30, 31, 30,
        31, 31, 30, 31, 30, 31
    };

    if (strlen(date) != 10 ||
        date[2] != '-' || date[5] != '-')
    {
        return 0;
    }

    for (int i = 0; i < 10; i++)
    {
        if (i != 2 && i != 5 &&
            !isdigit((unsigned char)date[i]))
        {
            return 0;
        }
    }

    if (sscanf(date, "%2d-%2d-%4d", &d, &m, &y) != 3)
    {
        return 0;
    }

    if (y < 1900 || m < 1 || m > 12)
    {
        return 0;
    }

    // Leap year check

    if (y % 400 == 0 ||
        (y % 4 == 0 && y % 100 != 0))
    {
        monthDays[1] = 29;
    }

    return d >= 1 && d <= monthDays[m - 1];
}


// CALCULATE DAYS BETWEEN TODAY AND DEADLINE

// Negative = Overdue
// 0 = Due today
// 1 to 3 = Upcoming within 3 days

int daysFromToday(const char *date)
{
    int d, m, y;

    time_t now, targetTime, todayTime;

    struct tm target = {0};
    struct tm today = {0};
    struct tm *current;

    if (!isValidDate(date))
    {
        return 999999;
    }

    sscanf(date, "%2d-%2d-%4d", &d, &m, &y);

    target.tm_mday = d;
    target.tm_mon = m - 1;
    target.tm_year = y - 1900;
    target.tm_hour = 12;

    targetTime = mktime(&target);

    now = time(NULL);
    current = localtime(&now);

    if (current == NULL)
    {
        return 999999;
    }

    today.tm_mday = current->tm_mday;
    today.tm_mon = current->tm_mon;
    today.tm_year = current->tm_year;
    today.tm_hour = 12;

    todayTime = mktime(&today);

    return (int)(difftime(targetTime, todayTime) / 86400.0);
}


// FIND TASK USING ID

int findTask(int id)
{
    for (int i = 0; i < taskCount; i++)
    {
        if (tasks[i].id == id)
        {
            return i;
        }
    }

    return -1;
}


// GENERATE NEXT TASK ID

int nextID(void)
{
    int max = 0;

    for (int i = 0; i < taskCount; i++)
    {
        if (tasks[i].id > max)
        {
            max = tasks[i].id;
        }
    }

    return max + 1;
}


// GET PRIORITY NAME

const char *getPriority(int p)
{
    if (p == 1)
        return "High";

    if (p == 2)
        return "Medium";

    return "Low";
}


// LOAD TASKS FROM FILE

void loadTasks(void)
{
    FILE *fp = fopen("studymate.txt", "r");

    char line[INPUT_SIZE];

    taskCount = 0;

    if (fp == NULL)
    {
        return;
    }

    while (fgets(line, sizeof(line), fp) &&
           taskCount < MAX_TASKS)
    {
        Task t;
        char *part;

        line[strcspn(line, "\r\n")] = '\0';

        part = strtok(line, "|");
        if (!part) continue;
        t.id = atoi(part);

        part = strtok(NULL, "|");
        if (!part) continue;
        snprintf(t.subject, sizeof(t.subject), "%s", part);

        part = strtok(NULL, "|");
        if (!part) continue;
        snprintf(t.topic, sizeof(t.topic), "%s", part);

        part = strtok(NULL, "|");
        if (!part || !isValidDate(part)) continue;
        snprintf(t.deadline, sizeof(t.deadline), "%s", part);

        part = strtok(NULL, "|");
        if (!part) continue;
        t.priority = atoi(part);

        part = strtok(NULL, "|");
        if (!part) continue;
        t.completed = atoi(part);

        if (t.id > 0 &&
            t.priority >= 1 && t.priority <= 3 &&
            (t.completed == 0 || t.completed == 1))
        {
            tasks[taskCount++] = t;
        }
    }

    fclose(fp);
}


// SAVE TASKS TO FILE

void saveTasks(void)
{
    FILE *fp = fopen("studymate.txt", "w");

    if (fp == NULL)
    {
        printf("\nError: Could not save tasks!\n");
        return;
    }

    for (int i = 0; i < taskCount; i++)
    {
        fprintf(fp, "%d|%s|%s|%s|%d|%d\n",
                tasks[i].id,
                tasks[i].subject,
                tasks[i].topic,
                tasks[i].deadline,
                tasks[i].priority,
                tasks[i].completed);
    }

    fclose(fp);
}


// 1. ADD STUDY TASK

void addTask(void)
{
    Task t;
    char input[INPUT_SIZE];

    if (taskCount >= MAX_TASKS)
    {
        printf("\nTask limit reached!\n");
        return;
    }

    printf("\n========== ADD STUDY TASK ==========\n");

    do
    {
        readLine("Enter subject name: ", input, sizeof(input));

        if (!input[0] || hasPipe(input))
        {
            printf("Subject cannot be empty or contain '|'.\n");
        }

    } while (!input[0] || hasPipe(input));

    snprintf(t.subject, sizeof(t.subject), "%s", input);


    do
    {
        readLine("Enter topic name: ", input, sizeof(input));

        if (!input[0] || hasPipe(input))
        {
            printf("Topic cannot be empty or contain '|'.\n");
        }

    } while (!input[0] || hasPipe(input));

    snprintf(t.topic, sizeof(t.topic), "%s", input);


    do
    {
        readLine("Enter deadline (DD-MM-YYYY): ",
                 t.deadline, sizeof(t.deadline));

        if (!isValidDate(t.deadline))
        {
            printf("Invalid date! Use DD-MM-YYYY.\n");
        }

    } while (!isValidDate(t.deadline));


    printf("\n1. High\n");
    printf("2. Medium\n");
    printf("3. Low\n");

    t.priority = readInteger("Choose priority: ", 1, 3);

    t.id = nextID();

    t.completed = 0;

    tasks[taskCount++] = t;

    saveTasks();

    printf("\nTask added successfully! Task ID: %d\n", t.id);
}


// 2. VIEW ALL TASKS

void viewTasks(void)
{
    if (!taskCount)
    {
        printf("\nNo study tasks available!\n");
        return;
    }

    printf("\n============================== ALL STUDY TASKS ==============================\n");

    printf("%-5s %-18s %-22s %-13s %-10s %-12s\n",
           "ID", "Subject", "Topic", "Deadline",
           "Priority", "Status");

    printf("-------------------------------------------------------------------------------\n");

    for (int i = 0; i < taskCount; i++)
    {
        printf("%-5d %-18.18s %-22.22s %-13s %-10s %-12s\n",
               tasks[i].id,
               tasks[i].subject,
               tasks[i].topic,
               tasks[i].deadline,
               getPriority(tasks[i].priority),
               tasks[i].completed ? "Completed" : "Pending");
    }
}


// 3. EDIT STUDY TASK

void editTask(void)
{
    int id, index, p;

    char input[INPUT_SIZE];

    if (!taskCount)
    {
        printf("\nNo tasks available to edit!\n");
        return;
    }

    id = readInteger("\nEnter Task ID to edit: ", 1, 1000000);

    index = findTask(id);

    if (index == -1)
    {
        printf("\nTask not found!\n");
        return;
    }

    printf("Current subject: %s\n", tasks[index].subject);

    readLine("New subject (ENTER to keep): ",
             input, sizeof(input));

    if (input[0] && !hasPipe(input))
    {
        snprintf(tasks[index].subject,
                 sizeof(tasks[index].subject), "%s", input);
    }
    else if (hasPipe(input))
    {
        printf("Subject unchanged: '|' is not allowed.\n");
    }


    printf("Current topic: %s\n", tasks[index].topic);

    readLine("New topic (ENTER to keep): ",
             input, sizeof(input));

    if (input[0] && !hasPipe(input))
    {
        snprintf(tasks[index].topic,
                 sizeof(tasks[index].topic), "%s", input);
    }
    else if (hasPipe(input))
    {
        printf("Topic unchanged: '|' is not allowed.\n");
    }


    printf("Current deadline: %s\n", tasks[index].deadline);

    while (1)
    {
        readLine("New deadline DD-MM-YYYY (ENTER to keep): ",
                 input, sizeof(input));

        if (!input[0])
        {
            break;
        }

        if (isValidDate(input))
        {
            snprintf(tasks[index].deadline,
                     sizeof(tasks[index].deadline), "%s", input);
            break;
        }

        printf("Invalid date! Try again.\n");
    }


    printf("Current priority: %s\n",
           getPriority(tasks[index].priority));

    printf("1. High  2. Medium  3. Low  0. Keep current\n");

    p = readInteger("New priority: ", 0, 3);

    if (p != 0)
    {
        tasks[index].priority = p;
    }

    saveTasks();

    printf("\nTask updated successfully!\n");
}


// CONFIRMATION FUNCTION

int confirmAction(const char *prompt)
{
    char input[20];

    while (1)
    {
        readLine(prompt, input, sizeof(input));

        if (strlen(input) == 1)
        {
            char c = (char)toupper((unsigned char)input[0]);

            if (c == 'Y')
                return 1;

            if (c == 'N')
                return 0;
        }

        printf("Invalid choice! Enter Y or N.\n");
    }
}


// 4. MARK TASK AS COMPLETED

void markTaskCompleted(void)
{
    int id, index;

    if (!taskCount)
    {
        printf("\nNo tasks available!\n");
        return;
    }

    id = readInteger("\nEnter Task ID: ", 1, 1000000);

    index = findTask(id);

    if (index == -1)
    {
        printf("\nTask not found!\n");
        return;
    }

    if (tasks[index].completed)
    {
        printf("\nThis task is already completed!\n");
        return;
    }

    printf("\nTask ID      : %d\n", tasks[index].id);

    printf("Subject Name : %s\n", tasks[index].subject);

    if (confirmAction(
        "Mark this task as completed? (Y/N): "))
    {
        tasks[index].completed = 1;

        saveTasks();

        printf("\nTask marked as completed successfully!\n");
    }
    else
    {
        printf("\nOperation cancelled! Task is still pending.\n");
    }
}


// SEARCH IGNORING UPPERCASE AND LOWERCASE

int containsIgnoreCase(const char *text, const char *search)
{
    size_t n = strlen(search);

    if (n == 0)
        return 1;

    for (; *text; text++)
    {
        size_t i = 0;

        while (i < n && text[i] &&
               tolower((unsigned char)text[i]) ==
               tolower((unsigned char)search[i]))
        {
            i++;
        }

        if (i == n)
            return 1;
    }

    return 0;
}


// 5. SEARCH TASKS

void searchTasks(void)
{
    char search[INPUT_SIZE];

    int found = 0;

    if (!taskCount)
    {
        printf("\nNo tasks available!\n");
        return;
    }

    readLine("\nEnter subject or topic to search: ",
             search, sizeof(search));

    if (!search[0])
    {
        printf("\nSearch text cannot be empty!\n");
        return;
    }

    for (int i = 0; i < taskCount; i++)
    {
        if (containsIgnoreCase(tasks[i].subject, search) ||
            containsIgnoreCase(tasks[i].topic, search))
        {
            printf("\nID: %d\n", tasks[i].id);

            printf("Subject: %s\n", tasks[i].subject);

            printf("Topic: %s\n", tasks[i].topic);

            printf("Deadline: %s\n", tasks[i].deadline);

            printf("Priority: %s\n",
                   getPriority(tasks[i].priority));

            printf("Status: %s\n",
                   tasks[i].completed ? "Completed" : "Pending");

            found = 1;
        }
    }

    if (!found)
    {
        printf("\nNo matching tasks found!\n");
    }
}


// 6. PROGRESS REPORT

void progressReport(void)
{
    int completed = 0;
    int pending = 0;
    int overdue = 0;
    int upcoming = 0;

    for (int i = 0; i < taskCount; i++)
    {
        if (tasks[i].completed)
        {
            completed++;
        }
        else
        {
            int d = daysFromToday(tasks[i].deadline);

            pending++;

            if (d < 0)
            {
                overdue++;
            }
            else if (d <= 3)
            {
                upcoming++;
            }
        }
    }

    printf("\n========== STUDY PROGRESS REPORT ==========\n");

    printf("Total Tasks       : %d\n", taskCount);

    printf("Completed Tasks   : %d\n", completed);

    printf("Pending Tasks     : %d\n", pending);

    printf("Overdue Tasks     : %d\n", overdue);

    printf("Due within 3 days : %d\n", upcoming);

    printf("Completion Rate   : %.2f%%\n",
           taskCount ? completed * 100.0 / taskCount : 0.0);
}


// 7. DELETE TASK

void deleteTask(void)
{
    int id, index;

    if (!taskCount)
    {
        printf("\nNo tasks available to delete!\n");
        return;
    }

    id = readInteger("\nEnter Task ID to delete: ",
                     1, 1000000);

    index = findTask(id);

    if (index == -1)
    {
        printf("\nTask not found!\n");
        return;
    }

    printf("\nTask ID      : %d\n", tasks[index].id);

    printf("Subject Name : %s\n", tasks[index].subject);

    if (confirmAction(
        "Are you sure you want to delete this task? (Y/N): "))
    {
        for (int i = index; i < taskCount - 1; i++)
        {
            tasks[i] = tasks[i + 1];
        }

        taskCount--;

        saveTasks();

        printf("\nTask deleted successfully!\n");
    }
    else
    {
        printf("\nTask deletion cancelled!\n");
    }
}


// 8. TASK REMINDERS

void taskReminders(void)
{
    int overdue = 0;
    int today = 0;
    int upcoming = 0;

    int d;

    printf("\n==================== TASK REMINDERS ====================\n");

    if (!taskCount)
    {
        printf("No tasks yet. Add a task to get reminders!\n");
        return;
    }


    // OVERDUE TASKS

    printf("\n[ OVERDUE TASKS ]\n");

    for (int i = 0; i < taskCount; i++)
    {
        d = daysFromToday(tasks[i].deadline);

        if (!tasks[i].completed && d < 0)
        {
            printf("ID: %d | %s - %s | Deadline: %s\n",
                   tasks[i].id,
                   tasks[i].subject,
                   tasks[i].topic,
                   tasks[i].deadline);

            overdue++;
        }
    }

    if (!overdue)
    {
        printf("No overdue tasks. Great!\n");
    }


    // TASKS DUE TODAY

    printf("\n[ DUE TODAY ]\n");

    for (int i = 0; i < taskCount; i++)
    {
        if (!tasks[i].completed &&
            daysFromToday(tasks[i].deadline) == 0)
        {
            printf("ID: %d | %s - %s | Deadline: %s\n",
                   tasks[i].id,
                   tasks[i].subject,
                   tasks[i].topic,
                   tasks[i].deadline);

            today++;
        }
    }

    if (!today)
    {
        printf("No tasks due today.\n");
    }


    // UPCOMING TASKS WITHIN THREE DAYS

    printf("\n[ UPCOMING - NEXT 3 DAYS ]\n");

    for (int i = 0; i < taskCount; i++)
    {
        d = daysFromToday(tasks[i].deadline);

        if (!tasks[i].completed && d >= 1 && d <= 3)
        {
            printf("ID: %d | %s - %s | Deadline: %s (in %d day%s)\n",
                   tasks[i].id,
                   tasks[i].subject,
                   tasks[i].topic,
                   tasks[i].deadline,
                   d,
                   d == 1 ? "" : "s");

            upcoming++;
        }
    }

    if (!upcoming)
    {
        printf("No upcoming tasks in the next 3 days.\n");
    }


    // REMINDER SUMMARY

    printf("\nReminder Summary:\n");

    printf("Overdue Tasks  : %d\n", overdue);

    printf("Due Today      : %d\n", today);

    printf("Upcoming Tasks : %d\n", upcoming);
}


// PRESS ENTER TO CONTINUE

void pressEnterToContinue(void)
{
    char input[INPUT_SIZE];

    readLine("\nPress ENTER to continue...",
             input, sizeof(input));
}


// MAIN FUNCTION

int main(void)
{
    int choice;

    time_t now;

    struct tm *today;

    char date[DATE_SIZE] = "Unavailable";

    loadTasks();

    now = time(NULL);

    today = localtime(&now);

    if (today)
    {
        strftime(date, sizeof(date), "%d-%m-%Y", today);
    }


    printf("\n========================================\n");

    printf("       STUDYMATE - STUDY PLANNER        \n");

    printf("========================================\n");

    printf("Today's date: %s\n", date);


    // AUTOMATIC REMINDERS WHEN PROGRAM STARTS

    taskReminders();


    do
    {
        printf("\n=============== MAIN MENU ===============\n");

        printf("1. Add Study Task\n");

        printf("2. View All Tasks\n");

        printf("3. Edit Study Task\n");

        printf("4. Mark Task as Completed\n");

        printf("5. Search Tasks\n");

        printf("6. View Progress Report\n");

        printf("7. Delete Task\n");

        printf("8. View Task Reminders\n");

        printf("9. Exit\n");


        choice = readInteger("Enter your choice: ", 1, 9);


        switch (choice)
        {
            case 1:
                addTask();
                pressEnterToContinue();
                break;

            case 2:
                viewTasks();
                pressEnterToContinue();
                break;

            case 3:
                editTask();
                pressEnterToContinue();
                break;

            case 4:
                markTaskCompleted();
                pressEnterToContinue();
                break;

            case 5:
                searchTasks();
                pressEnterToContinue();
                break;

            case 6:
                progressReport();
                pressEnterToContinue();
                break;

            case 7:
                deleteTask();
                pressEnterToContinue();
                break;

            case 8:
                taskReminders();
                pressEnterToContinue();
                break;

            case 9:
                saveTasks();

                printf("\nThank you for using StudyMate!\n");

                printf("Keep learning and stay productive!\n");

                break;
        }

    } while (choice != 9);


    return 0;
}
// Saqqaf Al-Yazidi

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

#define MAX_USERS 1000
#define MAX_NAME_LENGTH 510
#define MAX_PHONE_LENGTH 15
#define MAX_ADDRESS_LENGTH 100
#define MAX_EMAIL_LENGTH 50
#define MAX_COURSES 100
#define MAX_LINE 256
#define MAX_COURSE_INFO 1024
#define MAX_CONTENT 65536
#define MAX_LINE_LENGTH 1024
#define MAX_FILE_CONTENT 2048
#define MAX_PASSWORD_LENGTH 15
#define MAX_USERNAME_LENGTH 7

typedef struct {
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    char role[MAX_NAME_LENGTH];
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    char email[MAX_EMAIL_LENGTH];
} User;

typedef struct {
    char studentID[MAX_NAME_LENGTH];
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char phoneNumber[MAX_PHONE_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    char coursesEnrolled[MAX_COURSES][MAX_NAME_LENGTH];
    int numCoursesEnrolled;
} Student;


typedef struct {
    char courseID[10];
    int marks;
    char attendance[20];
} Course;

typedef struct {
    float aThreshold;
    float bThreshold;
    float cThreshold;
    float dThreshold;
} GradingSystem;

void displayMainMenu();
void displayMenu(char *role, char *username);
int login(char *username, char *password, char *role, char *firstName, char *lastName, char *phone, char *address, char *email);
void viewPersonalDetails(char *username);
void updatePersonalDetails(char *username);
void viewEnrolledCourses(char* username);
void viewMarks(char* username);
void viewAttendance(char* username);
void registerUser();
void updateUserPassword(const char *username);
char defineGradingSystem(float cgpa, GradingSystem gs);
void generateStudentReport(const char *studentID, GradingSystem gs);
void viewAllCourses();
void setGradingThresholds();
void updateCourseInformation(char *courseID, char *newCourseName);
void enrollStudentToCourse(const char *courseID, const char *studentUsername);
void assignLecturerToCourse(char *courseID, char *lecturerID);
void viewEnrolledCourses(char *username);
void viewAttendance(char *username);
void viewStudentProfile(char *username);
void manageStudentAttendance(const char *courseID, const char *studentID, const char *date, const char *newAttendance, bool modifyExisting);
void updateCourseMarks(char *courseID, char *studentUsername, float newMark);
bool checkAttendanceExists(const char *courseID, const char *studentID, const char *date);
void assignCourseMarks(char *courseID, char *studentUsername, float newMark);
bool isStudentEnrolledInCourse(const char *studentUsername, const char *courseID);
void updateStudentsFile(const char *studentID, const char *courseID);
// Login and Menu

int main() {
    int choice;
    char username[MAX_NAME_LENGTH];
    char password[MAX_NAME_LENGTH];
    char role[MAX_NAME_LENGTH];
    char firstName[MAX_NAME_LENGTH];
    char lastName[MAX_NAME_LENGTH];
    char phone[MAX_PHONE_LENGTH];
    char address[MAX_ADDRESS_LENGTH];
    char email[MAX_EMAIL_LENGTH];
    int loggedIn = 0;

    do {
        FILE *userFile = fopen("users.txt", "r");
        if (userFile == NULL) {
            printf("Error: Unable to open users file.\n");
            return 1;
        }

        displayMainMenu();
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                printf("Enter username: ");
                scanf("%s", username);
                printf("Enter password: ");
                scanf("%s", password);

                loggedIn = login(username, password, role, firstName, lastName, phone, address, email);
                if (loggedIn) {
                    printf("Login successful! Welcome, %s %s (%s).\n", firstName, lastName, role);
                    displayMenu(role, username);
                } else {
                    printf("Invalid username or password. Please try again.\n");
                }
                break;
            case 2:
                printf("Quitting application.\n");
                break;
            default:
                printf("Invalid choice. Please try again.\n");
        }

        fclose(userFile); // Close the file after each iteration
    } while (choice != 2 && !loggedIn);

    return 0;
}

void displayMainMenu() {
    printf("\nMain Menu:\n");
    printf("1. Login\n");
    printf("2. Quit Application\n");
    printf("Enter your choice: ");
}

void displayMenu(char *role, char *username) {
    int choice;
    do {
        if (strcmp(role, "Student") == 0) {
            printf("\n%s Menu:\n", role);
            printf("1. View and update personal details\n");
            printf("2. View enrolled courses\n");
            printf("3. View marks\n");
            printf("4. View attendance\n");
            printf("0. Logout\n");
        } else if (strcmp(role, "Programme-Admin") == 0) {
            printf("\n%s Menu:\n", role);
            printf("1. View student profiles\n");
            printf("2. View course information\n");
            printf("3. Update specific course information\n");
            printf("4. Enroll students into specific course\n");
            printf("5. Assign lecturer with specific course\n");
            printf("0. Logout\n");
        } else if (strcmp(role, "Lecturer") == 0) {
            printf("\n%s Menu:\n", role);
            printf("1. Access and update student attendance\n");
            printf("2. Assign course marks\n");
            printf("3. Update existing course marks\n");
            printf("0. Logout\n");
        } else if (strcmp(role, "System-Admin") == 0) {
            printf("\n%s Menu:\n", role);
            printf("1. Register a user\n");
            printf("2. Modify user's password\n");
            printf("3. Define grading system\n");
            printf("4. Generate report on student performance\n");
            printf("0. Logout\n");
        }

        printf("Enter your choice: ");
        scanf("%d", &choice);

        // Handling choices based on role
        if (strcmp(role, "Student") == 0) {
            switch (choice) {
                case 1:
                    viewPersonalDetails(username);
                    break;
                case 2:
                    viewEnrolledCourses(username);
                    break;
                case 3:
                    viewMarks(username);
                    break;
                case 4:
                    viewAttendance(username);
                    break;
                case 0:
                    main();
                    break;
            }
        } else if (strcmp(role, "Programme-Admin") == 0) {
            char courseID[50], newCourseName[100], lecturerID[50], studentUsername[50];
            switch (choice) {
                case 1:
                    printf("Enter student username to view profile: ");
                    scanf("%s", studentUsername);
                    viewStudentProfile(studentUsername);
                    break;
                case 2:
                    viewAllCourses();
                    break;
                case 3:
                    printf("Enter course ID: ");
                    scanf("%s", courseID);
                    printf("Enter new course name: ");
                    scanf(" %[^\n]%*c", newCourseName); 
                    updateCourseInformation(courseID, newCourseName);
                    break;
                case 4:
                    printf("Enter course ID to enroll a student: ");
                    scanf("%s", courseID);
                    printf("Enter student username: ");
                    scanf("%s", studentUsername);
                    enrollStudentToCourse(courseID, studentUsername);
                    updateStudentsFile(studentUsername, courseID);
                    break;
                case 5:
                    printf("Enter course ID to assign a lecturer: ");
                    scanf("%s", courseID);
                    printf("Enter lecturer ID: ");
                    scanf("%s", lecturerID);
                    assignLecturerToCourse(courseID, lecturerID);
                    break;
                case 0:
                    main();
                    break;
            }
        } else if (strcmp(role, "Lecturer") == 0) {
            char courseID[50], studentUsername[50], date[20], newAttendance[20];
            bool modifyExisting;
            int action;
            float newMark;
            switch (choice) {
                case 1: // Manage Student Attendance
                    printf("Enter course ID: ");
                    scanf("%s", courseID);
                    printf("Enter student username: ");
                    scanf("%s", studentUsername);
                    printf("Enter date (YYYY-MM-DD): ");
                    scanf("%s", date);
                    printf("Enter new attendance status (Present/Absent): ");
                    scanf("%s", newAttendance);
                    printf("Modify existing record? (0 for No, 1 for Yes): ");
                    scanf("%d", &action);
                    modifyExisting = action != 0;   
                    manageStudentAttendance(courseID, studentUsername, date, newAttendance, modifyExisting);
                    break;
                case 2:
                    printf("Enter course ID: ");
                    scanf("%s", courseID);
                    printf("Enter student username: ");
                    scanf("%s", studentUsername);
                    printf("Enter new mark: ");
                    scanf("%f", &newMark);
                    assignCourseMarks(courseID, studentUsername, newMark);
                    break;
                case 3: // Update Course Marks
                    printf("Enter course ID: ");
                    scanf("%s", courseID);
                    printf("Enter student username: ");
                    scanf("%s", studentUsername);
                    printf("Enter new mark: ");
                    scanf("%f", &newMark);
                    updateCourseMarks(courseID, studentUsername, newMark);
                    break;
                case 0:
                    main();
                    break;
            }
        } else if (strcmp(role, "System-Admin") == 0) {
            char studentID[10];
            GradingSystem gs;  // Declaration inside a block
            switch (choice) {
                case 1:
                    registerUser();
                    break;
                case 2:
                    printf("Enter username: ");
                    scanf("%s", username);
                    updateUserPassword(username);
                    break;
                case 3:
                    setGradingThresholds(&gs);
                    break;
                case 4:
                    printf("Enter Student ID: ");
                    scanf("%s", studentID);
                    generateStudentReport(studentID, gs);
                    break;
                case 0:
                    main();
                    break;
            }
        }
        
    } while (choice != 0);
}

int login(char *username, char *password, char *role, char *firstName, char *lastName, char *phone, char *address, char *email) {
    FILE *userFile = fopen("users.txt", "r");
    if (userFile == NULL) {
        printf("Error: Unable to open users file.\n");
        return 0;
    }

    char line[200];
    while (fgets(line, sizeof(line), userFile)) {
        char id[MAX_NAME_LENGTH], pass[MAX_NAME_LENGTH], userRole[MAX_NAME_LENGTH], first[MAX_NAME_LENGTH], last[MAX_NAME_LENGTH];
        sscanf(line, "%s %s %s %s %s", id, pass, userRole, first, last);
        if (strcmp(id, username) == 0 && strcmp(pass, password) == 0) {
            strcpy(role, userRole);
            strcpy(firstName, first);
            strcpy(lastName, last);
            fclose(userFile);
            return 1;
        }
    }

    fclose(userFile);
    return 0;
}


// STUDENT FUNCTIONS

void viewPersonalDetails(char *username) {
    FILE *studentsFile = fopen("students.txt", "r");
    if (studentsFile == NULL) {
        printf("Error: Unable to open students file.\n");
        return;
    }

    Student student;
    char line[200];
    while (fgets(line, sizeof(line), studentsFile)) {
        sscanf(line, "%s %s %s %s %s %s --- %[^\n]", student.studentID, student.firstName, student.lastName, student.phoneNumber, student.address, student.email, line);
        
        char *token = strtok(line, ",");
        student.numCoursesEnrolled = 0;
        while (token != NULL && student.numCoursesEnrolled < MAX_COURSES) {
            strcpy(student.coursesEnrolled[student.numCoursesEnrolled], token);
            token = strtok(NULL, ",");
            student.numCoursesEnrolled++;
        }

        if (strcmp(student.studentID, username) == 0) {
            printf("Personal Details:\n");
            printf("Student ID: %s\n", student.studentID);
            printf("First Name: %s\n", student.firstName);
            printf("Last Name: %s\n", student.lastName);
            printf("Phone Number: %s\n", student.phoneNumber);
            printf("Address: %s\n", student.address);
            printf("Email: %s\n", student.email);
            fclose(studentsFile);
            updatePersonalDetails(username); 
            return;
        }
    }

    fclose(studentsFile);
    printf("Personal details not found.\n");
}

int isValidPhoneNumber(const char* phoneNumber) {
    int len = strlen(phoneNumber);
    if (len < 7 || len > 10) return 0; // Check length is within 7 to 10 digits

    while (*phoneNumber) {
        if (*phoneNumber < '0' || *phoneNumber > '9') return 0; // Each char must be a digit
        phoneNumber++;
    }

    return 1; // The phone number is valid
}

int isValidEmail(const char* email) {
    int atSign = 0, dot = 0;
    const char* atPosition = NULL;
    while (*email) {
        if (*email == '@') {
            atSign = 1;
            atPosition = email; // Keep track of '@' position
        }
        if (*email == '.' && atPosition && email > atPosition) dot = 1; // Ensure '.' appears after '@'
        email++;
    }
    return atSign && dot; // The email is valid if it contains '@' followed by '.'
}

void updatePersonalDetails(char *username) {
    FILE *studentsFile = fopen("students.txt", "r");
    if (studentsFile == NULL) {
        printf("Error: Unable to open students file.\n");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error: Unable to open temporary file.\n");
        fclose(studentsFile);
        return;
    }

    Student student;
    int found = 0;
    char line[200];

    while (fgets(line, sizeof(line), studentsFile)) {
        char courses[200]; // Assuming all courses joined will not exceed 200 characters
        sscanf(line, "%s %s %s %s %s %s --- %[^\n]", student.studentID, student.firstName, student.lastName, student.phoneNumber, student.address, student.email, courses);
        
        if (strcmp(student.studentID, username) == 0) {
            found = 1;

            int choice;
            do {
                printf("\nUpdate Personal Details for %s:\n", username);
                printf("1. Update Phone Number\n");
                printf("2. Update Address\n");
                printf("3. Update Email\n");
                printf("0. Done\n");
                printf("Enter your choice: ");
                scanf("%d", &choice);

                char newInput[MAX_EMAIL_LENGTH]; // For address and email
                switch (choice) {
                    case 1:
                        printf("Enter new phone number (7-10 digits): ");
                        scanf("%s", newInput);
                        if (isValidPhoneNumber(newInput)) {
                            strcpy(student.phoneNumber, newInput);
                            printf("Phone number updated successfully.\n");
                        } else {
                            printf("Invalid phone number. Must be 7 to 10 digits.\n");
                        }
                        break;
                    case 2:
                        printf("Enter new address: ");
                        scanf(" %[^\n]s", newInput); // Read the entire line, including spaces
                        strcpy(student.address, newInput);
                        printf("Address updated successfully.\n");
                        break;
                    case 3:
                        printf("Enter new email: ");
                        scanf("%s", newInput);
                        if (isValidEmail(newInput)) {
                            strcpy(student.email, newInput);
                            printf("Email updated successfully.\n");
                        } else {
                            printf("Invalid email format.\n");
                        }
                        break;
                    case 0:
                        printf("Exiting update menu.\n");
                        break;
                    default:
                        printf("Invalid choice. Please try again.\n");
                        break;
                }
            } while (choice != 0);
        }

        // Write the updated (or not) record back to the temporary file
        fprintf(tempFile, "%s %s %s %s %s %s --- %s\n", student.studentID, student.firstName, student.lastName, student.phoneNumber, student.address, student.email, courses);
    }

    fclose(studentsFile);
    fclose(tempFile);

    // Replace the old file with the updated one
    remove("students.txt");
    rename("temp.txt", "students.txt");

    if (!found) {
        printf("Personal details not found for %s.\n", username);
    } else {
        printf("Personal details updated successfully.\n");
    }
}

void viewEnrolledCourses(char* username) {
    FILE *file = fopen("students.txt", "r");
    if (!file) {
        printf("Error opening students file.\n");
        return;
    }
    
    Student student;
    char line[MAX_LINE_LENGTH];
    int found = 0;
    
    printf("Courses enrolled by %s:\n", username);
    while (fgets(line, sizeof(line), file)) {
        // Parse the line for student details up to '---'
        sscanf(line, "%s %s %s %s %s %s --- %[^\n]", student.studentID, student.firstName, student.lastName, student.phoneNumber, student.address, student.email, line);
        
        if (strcmp(student.studentID, username) == 0) {
            char* token = strtok(line, ",");
            student.numCoursesEnrolled = 0;
            while (token != NULL && student.numCoursesEnrolled < MAX_COURSES) {
                strcpy(student.coursesEnrolled[student.numCoursesEnrolled], token);
                printf("- %s\n", student.coursesEnrolled[student.numCoursesEnrolled]);
                token = strtok(NULL, ",");
                student.numCoursesEnrolled++;
            }
            found = 1;
            break; // Break out of the loop once the matching user is found and courses are printed
        }
    }
    
    if (!found) {
        printf("No courses found for the given student.\n");
    }
    
    fclose(file);
}

void viewMarks(char* studentID) {
    FILE* file = fopen("marks.txt", "r");
    if (!file) {
        printf("Error opening marks file.\n");
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    int found = 0;
    float totalMarks = 0;
    int courseCount = 0;
    
    printf("Marks for %s:\n", studentID);
    
    while (fgets(line, sizeof(line), file)) {
        char studentIDFromFile[50], courseID[50];
        float mark;
        sscanf(line, "%s %s %f", studentIDFromFile, courseID, &mark);
        
        if (strcmp(studentIDFromFile, studentID) == 0) {
            printf("Course ID: %s, Mark: %.2f\n", courseID, mark);
            totalMarks += mark; // Accumulate total marks
            found = 1;
            courseCount++; // Increment the count of courses
        }
    }
    
    if (!found) {
        printf("No marks found for the given student.\n");
    } else {
        // Assuming the total marks are out of 100 per course, adjust this calculation as needed
        float averageMark = totalMarks / courseCount;
        // Convert average marks to CGPA here directly, adjust the conversion logic as needed
        float averageCGPA = (averageMark / 100) * 4.0; // Example conversion
        printf("Overall CGPA for %s: %.2f\n", studentID, averageCGPA);
    }
    
    fclose(file);
}

void viewAttendance(char* username) {
    FILE* file = fopen("attendance.txt", "r");
    if (!file) {
        printf("Error opening attendance file.\n");
        return;
    }
    
    char line[MAX_LINE_LENGTH];
    int found = 0;
    printf("Attendance record for %s:\n", username);
    
    while (fgets(line, sizeof(line), file)) {
        // Parse the line into course ID, student username from file, attendance date, and status
        char courseID[50], studentID[50], date[20], status[50]; // Adjusted the status array size to accommodate various statuses
        sscanf(line, "%s %s %s %[^\n]", courseID, studentID, date, status);
        
        // Check if the line corresponds to the student we're interested in
        if (strcmp(studentID, username) == 0) {
            printf("Course ID: %s, Date: %s, Status: %s\n", courseID, date, status);
            found = 1;
        }
    }
    
    if (!found) {
        printf("No attendance records found for the given student.\n");
    }
    
    fclose(file);
}

// SYSTEM ADMIN FUNCTIONS

int containsNoNumbers(const char *str) {
    while (*str) {
        if (isdigit((unsigned char)*str)) return 0; // Check for digits
        str++;
    }
    return 1;
}

bool isValidUsername(const char *username) {
    size_t len = strlen(username);
    if (len > MAX_USERNAME_LENGTH - 1) // Check length
        return false;

    // Check if the first two characters are ST, LR, PA, or S and the rest are digits
    if ((strncmp(username, "ST", 2) == 0 || strncmp(username, "LR", 2) == 0 || strncmp(username, "PA", 2) == 0 || strncmp(username, "S", 1) == 0) &&
        (strspn(username + 2, "0123456789") == len - 2))
        return true;

    return false;
}

int isValidRole(const char *role) {
    return strcmp(role, "Student") == 0 || strcmp(role, "Lecturer") == 0 || strcmp(role, "Programme-Admin") == 0;
}

int isValidPassword(const char *password) {
    int len = strlen(password);
    return len >= 7 && len <= 14; // Check length
}

void registerUser() {
    User newUser;

    printf("Enter username: ");
    scanf(" %49[^\n]", newUser.username);

    // Validate the username format
    if (!isValidUsername(newUser.username)) {
        printf("Invalid username format. Username must start with ST, LR, PA, or S followed by up to 6 digits.\n");
        return;
    }

    do {
        printf("Enter password (7-14 characters): ");
        scanf(" %49[^\n]", newUser.password);
    } while (!isValidPassword(newUser.password));

    do {
        printf("Enter role (Student/Programme-Admin/Lecturer): ");
        scanf(" %49[^\n]", newUser.role);
    } while (!isValidRole(newUser.role));

    do {
        printf("Enter first name (no numbers): ");
        scanf(" %49[^\n]", newUser.firstName);
    } while (!containsNoNumbers(newUser.firstName));

    do {
        printf("Enter last name (no numbers): ");
        scanf(" %49[^\n]", newUser.lastName);
    } while (!containsNoNumbers(newUser.lastName));

    do {
        printf("Enter phone (7-10 numbers): ");
        scanf(" %14[^\n]", newUser.phone);
    } while (!isValidPhoneNumber(newUser.phone));

    do {
        printf("Enter email (must include '@' and '.'): ");
        scanf(" %49[^\n]", newUser.email);
    } while (!isValidEmail(newUser.email));

        printf("Enter address: ");
        scanf(" %49[^\n]", newUser.address);


    FILE *userFile = fopen("users.txt", "a");
    if (userFile == NULL) {
        printf("Error: Unable to open users file.\n");
        return;
    }

    fprintf(userFile, "%s %s %s %s %s\n", newUser.username, newUser.password, newUser.role, newUser.firstName, newUser.lastName);
    fclose(userFile);

    if (strcmp(newUser.role, "Student") == 0) {
        FILE *studentFile = fopen("students.txt", "a");
        if (studentFile == NULL) {
            printf("Error: Unable to open students file.\n");
            return;
        }

        fprintf(studentFile, "%s %s %s %s %s %s ---\n", newUser.username, newUser.firstName, newUser.lastName, newUser.phone, newUser.address, newUser.email);
        fclose(studentFile);
    } else if (strcmp(newUser.role, "Lecturer") == 0) {
        FILE *lecturerFile = fopen("lecturers.txt", "a");
        if (lecturerFile == NULL) {
            printf("Error: Unable to open lecturers file.\n");
            return;
        }

        fprintf(lecturerFile, "%s %s %s %s %s %s\n", newUser.username, newUser.firstName, newUser.lastName, newUser.phone, newUser.address, newUser.email);
        fclose(lecturerFile);
    } else if (strcmp(newUser.role, "Programme-Admin") == 0) {
        FILE *adminFile = fopen("admins.txt", "a");
        if (adminFile == NULL) {
            printf("Error: Unable to open admins file.\n");
            return;
        }

        fprintf(adminFile, "%s %s %s %s %s %s\n", newUser.username, newUser.firstName, newUser.lastName, newUser.phone, newUser.address, newUser.email);
        fclose(adminFile);
    }

    printf("User registered successfully.\n");
}

void updateUserPassword(const char *username) {
    FILE *userFile = fopen("users.txt", "r");
    if (userFile == NULL) {
        printf("Error: Unable to open users file.\n");
        return;
    }

    FILE *tempFile = fopen("temp.txt", "w");
    if (tempFile == NULL) {
        printf("Error: Unable to open temporary file.\n");
        fclose(userFile);
        return;
    }

    User user;
    int found = 0;
    char line[200];

    // Loop to read each user entry
    while (fgets(line, sizeof(line), userFile)) {
        sscanf(line, "%s %s %s %s %s", user.username, user.password, user.role, user.firstName, user.lastName);

        if (strcmp(user.username, username) == 0) {
            found = 1;

            char newPassword[MAX_PASSWORD_LENGTH];
            printf("Enter new password for %s (7 to 14 characters): ", username);
            scanf("%14s", newPassword);

            if (isValidPassword(newPassword)) {
                strcpy(user.password, newPassword);
                printf("Password updated successfully for %s.\n", username);
            } else {
                printf("Invalid password length.\n");
            }
        }

        fprintf(tempFile, "%s %s %s %s %s\n", user.username, user.password, user.role, user.firstName, user.lastName);
    }

    fclose(userFile);
    fclose(tempFile);


    // File renaming
    if (rename("temp.txt", "users.txt") == 0) {
        printf("File renamed successfully.\n");
    } else {
        printf("Error renaming file. Make sure the file is not in use by another process and you have sufficient permissions.\n");
    }

    if (!found) {
        printf("User %s not found.\n", username);
    }
}

void generateStudentReport(const char *studentID, GradingSystem gs) {
    FILE *marksFile = fopen("marks.txt", "r");
    FILE *attendanceFile = fopen("attendance.txt", "r");
    if (marksFile == NULL || attendanceFile == NULL) {
        printf("Error opening file(s)\n");
        if (marksFile != NULL) fclose(marksFile);
        if (attendanceFile != NULL) fclose(attendanceFile);
        return;
    }

    Course courses[MAX_COURSES];
    int numCourses = 0;
    float totalMarks = 0.0;
    bool studentFound = false;

    char course[30], student[30], status[40], date[20];
    float mark;

    // Read marks file
    while (fscanf(marksFile, "%s %s %f", student, course, &mark) == 3) {
        if (strcmp(student, studentID) == 0) {
            bool courseFound = false;
            // Check if the course already exists in the array
            for (int i = 0; i < numCourses; i++) {
                if (strcmp(courses[i].courseID, course) == 0) {
                    courseFound = true;
                    break;
                }
            }
            if (!courseFound) {
                strcpy(courses[numCourses].courseID, course);
                strcpy(courses[numCourses].attendance, "N/A");
                numCourses++;
            }
            // Store the marks
            for (int i = 0; i < numCourses; i++) {
                if (strcmp(courses[i].courseID, course) == 0) {
                    courses[i].marks = (int)mark; // Assuming the marks are integers
                    break;
                }
            }
            studentFound = true;
        }
    }

    // Read attendance file
    rewind(attendanceFile); // Rewind to the beginning of the file
    while (fscanf(attendanceFile, "%s %s %s %s", course, student, date, status) == 4) {
        if (strcmp(student, studentID) == 0) {
            // Update attendance status for the corresponding course
            for (int i = 0; i < numCourses; i++) {
                if (strcmp(courses[i].courseID, course) == 0) {
                    strcpy(courses[i].attendance, status);
                    break;
                }
            }
        }
    }

    fclose(marksFile);
    fclose(attendanceFile);

    if (!studentFound) {
        printf("Student not found.\n");
    } else {
        printf("Student Performance Report:\n");
        printf("Student ID: %s\n", studentID);
        for (int i = 0; i < numCourses; i++) {
            printf("%s:\n", courses[i].courseID);
            printf("Marks: %d\n", courses[i].marks);
            printf("Attendance: %s\n", courses[i].attendance);
            totalMarks += courses[i].marks;
        }

        float cgpa = (numCourses > 0) ? (totalMarks / numCourses) / 25.0 : 0.0;

        printf("CGPA: %.2f\n", cgpa);
        printf("Letter Grade: %c\n", defineGradingSystem(cgpa, gs));
    }
}

char defineGradingSystem(float cgpa, GradingSystem gs) {
    if (cgpa >= gs.aThreshold)
        return 'A';
    else if (cgpa >= gs.bThreshold)
        return 'B';
    else if (cgpa >= gs.cThreshold)
        return 'C';
    else if (cgpa >= gs.dThreshold)
        return 'D';
    else
        return 'F';
}

void setGradingThresholds(GradingSystem *gs) {
    printf("Enter the minimum CGPA for an A grade: ");
    scanf("%f", &gs->aThreshold);
    printf("Enter the minimum CGPA for a B grade: ");
    scanf("%f", &gs->bThreshold);
    printf("Enter the minimum CGPA for a C grade: ");
    scanf("%f", &gs->cThreshold);
    printf("Enter the minimum CGPA for a D grade: ");
    scanf("%f", &gs->dThreshold);
}

/// LECTURER FUNCTIONS 

void manageStudentAttendance(const char *courseID, const char *studentID, const char *date, const char *newAttendance, bool modifyExisting) {
    // Check if the record exists when modifying
    if (modifyExisting && !checkAttendanceExists(courseID, studentID, date)) {
        printf("No existing attendance record found to modify.\n");
        return;
    }

    FILE *file = fopen("attendance.txt", "r");
    if (!file) {
        printf("Unable to open attendance file.\n");
        return;
    }

    FILE *tempFile = fopen("temp_attendance.txt", "w");
    char line[256];
    int updated = 0;

    while (fgets(line, sizeof(line), file)) {
        char currentCourseID[50], currentStudentID[50], currentDate[20], currentAttendance[20];
        sscanf(line, "%s %s %s %s", currentCourseID, currentStudentID, currentDate, currentAttendance);

        // When modifying, check for the specific record to update
        if (modifyExisting && strcmp(currentCourseID, courseID) == 0 && strcmp(currentStudentID, studentID) == 0 && strcmp(currentDate, date) == 0) {
            strcpy(currentAttendance, newAttendance);
            updated = 1;
        }
        fprintf(tempFile, "%s %s %s %s\n", currentCourseID, currentStudentID, currentDate, currentAttendance);
    }

    // If adding a new record and it hasn't been updated (meaning it wasn't found, so it's new)
    if (!modifyExisting && !updated) {
        fprintf(tempFile, "%s %s %s %s\n", courseID, studentID, date, newAttendance);
        updated = 1; // Mark as updated for message purposes
    }

    fclose(file);
    fclose(tempFile);

    // Replace old attendance file with new
    remove("attendance.txt");
    rename("temp_attendance.txt", "attendance.txt");

    if (updated) {
        printf("Attendance record processed successfully.\n");
    } else {
        printf("Failed to process the attendance record.\n");
    }
}

bool checkAttendanceExists(const char *courseID, const char *studentID, const char *date) {
    FILE *file = fopen("attendance.txt", "r");
    if (!file) return false;

    char line[256];
    while (fgets(line, sizeof(line), file)) {
        char currentCourseID[50], currentStudentID[50], currentDate[20];
        sscanf(line, "%s %s %s", currentCourseID, currentStudentID, currentDate);

        if (strcmp(currentCourseID, courseID) == 0 && strcmp(currentStudentID, studentID) == 0 && strcmp(currentDate, date) == 0) {
            fclose(file);
            return true; // Record exists
        }
    }

    fclose(file);
    return false; // No record found
}

bool isStudentEnrolledInCourse(const char *studentUsername, const char *courseID) {
    FILE *studentsFile = fopen("students.txt", "r");
    if (!studentsFile) {
        printf("Unable to open students file.\n");
        return false;
    }

    char line[1024];
    while (fgets(line, sizeof(line), studentsFile)) {
        char currentUsername[50], courses[1024];
        // Assuming the format STUDENTID FIRSTNAME LASTNAME PHONENUMBER ADDRESS --- COURSES
        sscanf(line, "%*s %*s %*s %*s %*s %*s --- %[^\n]", courses);
        if (strstr(line, studentUsername) && strstr(courses, courseID)) {
            fclose(studentsFile);
            return true;
        }
    }

    fclose(studentsFile);
    return false;
}

void updateCourseMarks(char *courseID, char *studentUsername, float newMark) {
    FILE *file = fopen("marks.txt", "r");
    if (!file) {
        printf("Unable to open marks file.\n");
        return;
    }

    FILE *tempFile = fopen("temp_marks.txt", "w");
    char line[256];
    int updated = 0;

    while (fgets(line, sizeof(line), file)) {
        char currentCourseID[50], currentUsername[50];
        float mark;
        sscanf(line, "%s %s %f", currentUsername, currentCourseID, &mark);

        if (strcmp(currentCourseID, courseID) == 0 && strcmp(currentUsername, studentUsername) == 0) {
            mark = newMark; // Update mark
            updated = 1;
        }
        fprintf(tempFile, "%s %s %f\n", currentUsername, currentCourseID, mark);
    }

    fclose(file);
    fclose(tempFile);

    remove("marks.txt");
    rename("temp_marks.txt", "marks.txt");

    if (updated) {
        printf("Marks updated successfully.\n");
    } else {
        printf("No matching record found to update.\n");
    }
}

void assignCourseMarks(char *courseID, char *studentUsername, float newMark) {
    if (!isStudentEnrolledInCourse(studentUsername, courseID)) {
        printf("Student is not enrolled in the specified course.\n");
        return;
    }

    FILE *file = fopen("marks.txt", "r+");
    if (!file) {
        printf("Unable to open marks file.\n");
        return;
    }

    FILE *tempFile = fopen("temp_marks.txt", "w");
    char line[256];
    bool found = false;

    while (fgets(line, sizeof(line), file)) {
        char currentCourseID[50], currentUsername[50];
        float mark;
        sscanf(line, "%s %s %f", currentUsername, currentCourseID, &mark);

        if (strcmp(currentCourseID, courseID) == 0 && strcmp(currentUsername, studentUsername) == 0) {
            mark = newMark; // Update mark if it already exists
            found = true;
        }
        fprintf(tempFile, "%s %s %f\n", currentUsername, currentCourseID, mark);
    }

    if (!found) {
        // Add new mark if it doesn't exist
        fprintf(tempFile, "%s %s %f\n", studentUsername, courseID, newMark);
    }

    fclose(file);
    fclose(tempFile);

    remove("marks.txt");
    rename("temp_marks.txt", "marks.txt");

    if (found) {
        printf("Marks updated successfully.\n");
    } else {
        printf("New mark added successfully.\n");
    }
}

/// PROGRAMME ADMIN FUNCTIONS

void viewStudentProfile(char *username) {
    FILE *file = fopen("students.txt", "r");
    char line[256];
    int found = 0;

    if (!file) {
        printf("Unable to open students.txt\n");
        return;
    }

    while (fgets(line, sizeof(line), file)) {
        if (strncmp(line, username, strlen(username)) == 0) {
            printf("Profile Found:\n%s", line);
            found = 1;
            break;
        }
    }

    fclose(file);

    if (!found) {
        printf("Profile for %s not found.\n", username);
    }
}

void viewAllCourses() {
    FILE *file = fopen("courses.txt", "r");
    char line[256];

    if (!file) {
        printf("Unable to open courses.txt\n");
        return;
    }

    printf("All Courses:\n");
    while (fgets(line, sizeof(line), file)) {
        printf("%s", line);
    }

    fclose(file);
}

int isValidCourseName(const char *newCourseName) {
    int length = strlen(newCourseName);
    int spaceCount = 0;

    // Check if the length is within limit and contains only letters and spaces
    for (int i = 0; i < length; i++) {
        if (!isalpha(newCourseName[i]) && newCourseName[i] != ' ')
            return 0;
        if (newCourseName[i] == ' ')
            spaceCount++;
    }

    // Ensure there is exactly one space (indicating two words)
    if (spaceCount != 1)
        return 0;

    return 1;
}

void updateCourseInformation(char *courseID, char *newCourseName) {
    FILE *file = fopen("courses.txt", "r");
    if (!file) {
        printf("Unable to open courses.txt\n");
        return;
    }

    char line[MAX_LINE];
    char content[MAX_CONTENT] = "";
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        char tempLine[MAX_LINE];
        strcpy(tempLine, line); // Copy line to tempLine because strtok modifies the string

        char *token = strtok(tempLine, " ");
        if (strcmp(token, courseID) == 0) {
            // CourseID matched, now rebuild the line with new information
            if (!isValidCourseName(newCourseName)) {
                printf("Invalid course name format. Course name should contain only letters and consist of two words separated by a space.\n");
                fclose(file);
                return;
            }
            
            strcat(content, courseID); // Add CourseID
            strcat(content, " ");
            strcat(content, newCourseName); // Add new CourseName

            // Copy the student usernames without modification
            token = strtok(NULL, " "); // Skip old CourseName
            token = strtok(NULL, " "); // Skip old LecturerID
            while ((token = strtok(NULL, " \n")) != NULL) {
                strcat(content, " ");
                strcat(content, token);
            }

            strcat(content, "\n");
            found = 1;
        } else {
            strcat(content, line); // Line is unchanged, just copy it
        }
    }

    fclose(file);

    if (!found) {
        printf("Course ID %s not found.\n", courseID);
        return;
    }

    // Write the updated content back to courses.txt
    file = fopen("courses.txt", "w");
    if (!file) {
        printf("Unable to open courses.txt for writing.\n");
        return;
    }

    fputs(content, file);
    fclose(file);

    printf("Course information for %s updated successfully.\n", courseID);
}

void enrollStudentToCourse(const char *courseID, const char *studentUsername) {
    FILE *file = fopen("courses.txt", "r");
    if (!file) {
        printf("Unable to open courses.txt for reading.\n");
        return;
    }

    char line[MAX_LINE], newContent[MAX_COURSE_INFO] = "";
    int found = 0, alreadyEnrolled = 0;

    while (fgets(line, sizeof(line), file)) {
        char *newline = strchr(line, '\n');
        if (newline) *newline = '\0'; // Remove the newline character for easier manipulation

        if (strncmp(line, courseID, strlen(courseID)) == 0 && line[strlen(courseID)] == ' ') {
            found = 1;

            if (strstr(line, studentUsername)) {
                alreadyEnrolled = 1;
                break; // Stop processing if the student is already enrolled
            } else {
                char *lrPtr = strstr(line, "LR");
                if (lrPtr) {
                    char *studentsPtr = strchr(lrPtr, ' ');
                    if (studentsPtr && *(studentsPtr + 1) != '\0') { 
                        // If there are already enrolled students, append with a comma
                        snprintf(studentsPtr + strlen(studentsPtr), MAX_LINE - strlen(studentsPtr), ",%s", studentUsername);
                    } else {
                        // If there are no students enrolled yet (or no space found after LR identifier), append the student ID directly
                        snprintf(line + strlen(line), MAX_LINE - strlen(line), " %s", studentUsername);
                    }
                }
            }
        }

        // Reconstruct the file content with the modified (or unmodified) line
        strcat(newContent, line);
        strcat(newContent, "\n");
    }

    fclose(file);

    // Handle the outcome of the enrollment process
    if (alreadyEnrolled) {
        printf("Student %s is already enrolled in course %s.\n", studentUsername, courseID);
    } else if (found) {
        file = fopen("courses.txt", "w");
        if (!file) {
            printf("Unable to open courses.txt for writing.\n");
            return;
        }
        fputs(newContent, file);
        fclose(file);
        printf("Student %s enrolled in course %s successfully.\n", studentUsername, courseID);
    } else {
        printf("Course ID %s not found.\n", courseID);
    }
}

int isCourseValid(const char *courseID) {
    FILE *coursesFile = fopen("courses.txt", "r");
    if (!coursesFile) {
        printf("Unable to open courses.txt\n");
        return 0; // Indicates failure
    }

    char courseLine[MAX_LINE];
    int courseIDLength = strlen(courseID);
    while (fgets(courseLine, sizeof(courseLine), coursesFile)) {
        // Ensure the course ID matches and is followed by a space
        if (strncmp(courseLine, courseID, courseIDLength) == 0 && courseLine[courseIDLength] == ' ') {
            fclose(coursesFile);
            return 1; // Course is valid
        }
    }

    fclose(coursesFile);
    return 0; // Course not found, hence not valid
}

void updateStudentsFile(const char *studentID, const char *courseID) {
    if (!isCourseValid(courseID)) {
        printf("Course ID %s not found in courses.txt. Cannot enroll student.\n", courseID);
        return;
    }

    FILE *file = fopen("students.txt", "r");
    if (!file) {
        printf("Unable to open students.txt\n");
        return;
    }

    char content[MAX_CONTENT] = {0}, line[MAX_LINE];
    int found = 0, courseAlreadyEnrolled = 0;

    while (fgets(line, sizeof(line), file)) {
        char *newline = strchr(line, '\n');
        if (newline) *newline = '\0'; // Remove the newline character

        if (strstr(line, studentID) == line) { // Ensure line starts with studentID
            found = 1;
            char *coursesPart = strstr(line, "---") + 4;

            if (strstr(coursesPart, courseID)) {
                courseAlreadyEnrolled = 1;
                printf("Student %s is already enrolled in course %s.\n", studentID, courseID);
                break; // Stop processing further
            }

            // Append new course ID correctly
            if (strlen(coursesPart) > 0) {
                // If there are existing courses, append with a comma separator
                snprintf(line + strlen(line), MAX_LINE - strlen(line), ",%s", courseID);
            } else {
                // If there are no courses listed, append without a comma
                snprintf(line + strlen(line), MAX_LINE - strlen(line), "%s", courseID);
            }
        }

        strcat(content, line); // Add the processed line to content
        strcat(content, "\n"); // Re-add the newline character
    }

    fclose(file);

    if (!courseAlreadyEnrolled && found) {
        file = fopen("students.txt", "w");
        if (!file) {
            printf("Unable to reopen students.txt for writing.\n");
            return;
        }
        fputs(content, file);
        fclose(file);
        printf("Student %s successfully enrolled in course %s.\n", studentID, courseID);
    } else if (!found) {
        printf("Student ID %s not found in students.txt.\n", studentID);
    }
}

void assignLecturerToCourse(char *courseID, char *newLecturerID) {
    FILE *file = fopen("courses.txt", "r");
    if (!file) {
        printf("Unable to open courses.txt\n");
        return;
    }

    char line[MAX_LINE], newContent[MAX_CONTENT] = "";
    int found = 0;

    while (fgets(line, sizeof(line), file)) {
        // Make a copy of the line for safe manipulation
        char lineCopy[MAX_LINE];
        strcpy(lineCopy, line);
        
        char *lineToken = strtok(lineCopy, " ");
        // Check if the first token (courseID) matches
        if (lineToken && strcmp(lineToken, courseID) == 0) {
            found = 1;
            // Reconstruct the line with the new lecturer ID
            strcat(newContent, courseID); // Add courseID
            for (int i = 0; i < 2; ++i) { // Append course name parts
                lineToken = strtok(NULL, " ");
                if (lineToken) {
                    strcat(newContent, " ");
                    strcat(newContent, lineToken);
                }
            }

            // Append the new lecturer ID
            strcat(newContent, " ");
            strcat(newContent, newLecturerID);

            // Skip over any existing lecturer ID in the original line
            lineToken = strtok(NULL, " ");
            if (lineToken && lineToken[0] == 'L') {
                // There was an existing lecturer ID; move to the next part (students)
                lineToken = strtok(NULL, "");
            } else {
                // No lecturer ID found; the token now points to the students (if any)
                // No action needed as lineToken is already advanced
            }

            // Append the rest of the line (student IDs, if any)
            if (lineToken) {
                strcat(newContent, " ");
                strcat(newContent, lineToken);
            } else {
                strcat(newContent, "\n"); // Ensure the line ends with a newline character
            }
        } else {
            // Line does not correspond to the course being updated; append it unchanged
            strcat(newContent, line);
        }
    }

    fclose(file);

    // Only rewrite the file if the course was found
    if (found) {
        file = fopen("courses.txt", "w");
        if (!file) {
            printf("Unable to open courses.txt for writing.\n");
            return;
        }
        fputs(newContent, file);
        fclose(file);
        printf("Lecturer %s assigned to course %s successfully.\n", newLecturerID, courseID);
    } else {
        printf("Course ID %s not found.\n", courseID);
    }
}

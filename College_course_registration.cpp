
#include <iostream>
#include <vector>
#include <string>
#include <algorithm>
#include <limits>
using namespace std;

class Student {
public:
    int id;
    string name;
    vector<string> registeredCourses;

    Student(int studentId, string studentName)
        : id(studentId), name(studentName) {}
};

class Course {
public:
    string code;
    string title;
    int capacity;
    vector<int> enrolledStudents;

    Course(string c, string t, int cap)
        : code(c), title(t), capacity(cap) {}
};

class CollegeRegistration {
private:
    vector<Student> students;
    vector<Course> courses;

    int findStudent(int id) const {
        for (int i = 0; i < (int)students.size(); i++) {
            if (students[i].id == id)
                return i;
        }
        return -1;
    }

    int findCourse(const string& code) const {
        for (int i = 0; i < (int)courses.size(); i++) {
            if (courses[i].code == code)
                return i;
        }
        return -1;
    }

public:
    CollegeRegistration() {
        // Sample courses
        courses.emplace_back("CS101", "Programming in C++", 3);
        courses.emplace_back("CS102", "Data Structures", 3);
        courses.emplace_back("CS103", "Database Management", 3);
        courses.emplace_back("CS104", "Computer Networks", 3);
        courses.emplace_back("CS105", "Operating Systems", 3);
    }

    void addStudent() {
        int id;
        string name;

        cout << "Enter student ID: ";
        if (!(cin >> id)) {
            cout << "Invalid ID!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        if (findStudent(id) != -1) {
            cout << "Student ID already exists!\n";
            return;
        }

        cin.ignore(numeric_limits<streamsize>::max(), '\n');
        cout << "Enter student name: ";
        getline(cin, name);

        if (name.empty()) {
            cout << "Name cannot be empty!\n";
            return;
        }

        students.emplace_back(id, name);
        cout << "Student added successfully!\n";
    }

    void displayStudents() const {
        if (students.empty()) {
            cout << "No students registered in the system yet.\n";
            return;
        }

        cout << "\n--- STUDENT LIST ---\n";
        for (const auto& s : students) {
            cout << "ID: " << s.id
                 << " | Name: " << s.name
                 << " | Courses: " << s.registeredCourses.size()
                 << '\n';
        }
    }

    void displayCourses() const {
        cout << "\n--- AVAILABLE COURSES ---\n";

        for (const auto& c : courses) {
            cout << "Code: " << c.code
                 << " | Course: " << c.title
                 << " | Seats: "
                 << c.capacity - (int)c.enrolledStudents.size()
                 << "/" << c.capacity << '\n';
        }
    }

    void registerCourse() {
        int studentId;
        string courseCode;

        cout << "Enter student ID: ";
        if (!(cin >> studentId)) {
            cout << "Invalid ID!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        int si = findStudent(studentId);

        if (si == -1) {
            cout << "Student not found. Add the student first.\n";
            return;
        }

        cout << "Enter course code: ";
        cin >> courseCode;

        int ci = findCourse(courseCode);

        if (ci == -1) {
            cout << "Course not found!\n";
            return;
        }

        auto& course = courses[ci];
        auto& student = students[si];

        if (find(student.registeredCourses.begin(),
                 student.registeredCourses.end(), courseCode)
                 != student.registeredCourses.end()) {
            cout << "Student is already registered for this course.\n";
            return;
        }

        if ((int)course.enrolledStudents.size() >= course.capacity) {
            cout << "Course is full! No seats available.\n";
            return;
        }

        student.registeredCourses.push_back(courseCode);
        course.enrolledStudents.push_back(studentId);

        cout << "Course registration successful!\n";
    }

    void viewStudentCourses() const {
        int id;
        cout << "Enter student ID: ";

        if (!(cin >> id)) {
            cout << "Invalid ID!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        int si = findStudent(id);

        if (si == -1) {
            cout << "Student not found!\n";
            return;
        }

        const auto& student = students[si];

        cout << "\nCourses registered by "
             << student.name << ":\n";

        if (student.registeredCourses.empty()) {
            cout << "No courses registered.\n";
            return;
        }

        for (const string& code : student.registeredCourses) {
            int ci = findCourse(code);
            if (ci != -1)
                cout << code << " - " << courses[ci].title << '\n';
        }
    }

    void dropCourse() {
        int studentId;
        string courseCode;

        cout << "Enter student ID: ";
        if (!(cin >> studentId)) {
            cout << "Invalid ID!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        int si = findStudent(studentId);

        if (si == -1) {
            cout << "Student not found!\n";
            return;
        }

        cout << "Enter course code to drop: ";
        cin >> courseCode;

        int ci = findCourse(courseCode);

        if (ci == -1) {
            cout << "Course not found!\n";
            return;
        }

        auto& registered = students[si].registeredCourses;

        auto studentCourse = find(
            registered.begin(), registered.end(), courseCode);

        if (studentCourse == registered.end()) {
            cout << "Student is not registered for this course.\n";
            return;
        }

        registered.erase(studentCourse);

        auto& enrolled = courses[ci].enrolledStudents;

        enrolled.erase(
            remove(enrolled.begin(), enrolled.end(), studentId),
            enrolled.end());

        cout << "Course dropped successfully!\n";
    }

    void viewCourseStudents() const {
        string courseCode;
        cout << "Enter course code: ";
        cin >> courseCode;

        int ci = findCourse(courseCode);

        if (ci == -1) {
            cout << "Course not found!\n";
            return;
        }

        const auto& course = courses[ci];

        cout << "\nStudents in " << course.title << ":\n";

        if (course.enrolledStudents.empty()) {
            cout << "No students enrolled yet.\n";
            return;
        }

        for (int id : course.enrolledStudents) {
            int si = findStudent(id);

            if (si != -1) {
                cout << "ID: " << students[si].id
                     << " | Name: " << students[si].name << '\n';
            }
        }
    }

    void searchStudent() const {
        int id;
        cout << "Enter student ID: ";

        if (!(cin >> id)) {
            cout << "Invalid ID!\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            return;
        }

        int si = findStudent(id);

        if (si == -1) {
            cout << "Student not found!\n";
            return;
        }

        cout << "Student found!\n";
        cout << "ID: " << students[si].id << '\n';
        cout << "Name: " << students[si].name << '\n';
        cout << "Registered courses: "
             << students[si].registeredCourses.size() << '\n';
    }
};

int main() {
    CollegeRegistration system;
    int choice;

    do {
        cout << "\n====================================\n";
        cout << " COLLEGE COURSE REGISTRATION SYSTEM\n";
        cout << "====================================\n";
        cout << "1. Add Student\n";
        cout << "2. Display All Students\n";
        cout << "3. Display Available Courses\n";
        cout << "4. Register for a Course\n";
        cout << "5. View Student Courses\n";
        cout << "6. Drop a Course\n";
        cout << "7. View Students in a Course\n";
        cout << "8. Search Student\n";
        cout << "0. Exit\n";
        cout << "Enter your choice: ";

        if (!(cin >> choice)) {
            cout << "Invalid input! Enter a number.\n";
            cin.clear();
            cin.ignore(numeric_limits<streamsize>::max(), '\n');
            continue;
        }

        switch (choice) {
            case 1: system.addStudent(); break;
            case 2: system.displayStudents(); break;
            case 3: system.displayCourses(); break;
            case 4: system.registerCourse(); break;
            case 5: system.viewStudentCourses(); break;
            case 6: system.dropCourse(); break;
            case 7: system.viewCourseStudents(); break;
            case 8: system.searchStudent(); break;
            case 0: cout << "Exiting registration system.\n"; break;
            default: cout << "Invalid choice! Try again.\n";
        }

    } while (choice != 0);

    return 0;
}

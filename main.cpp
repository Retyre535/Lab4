#include <iostream>
#include <chrono>
#include <random>
#include <fstream>
#include <string>
#include <functional>
#include <vector>
#include <complex>
#include "BinaryTree.h"
#include "DataTypes.h"

template<typename Func>
double measureExecutionTime(Func &&func) {
    auto start = std::chrono::high_resolution_clock::now();
    func();
    auto end = std::chrono::high_resolution_clock::now();
    return std::chrono::duration<double, std::milli>(end - start).count();
}

void demonstrateDataTypes() {
    std::cout << "\n=====================================\n";
    std::cout << "Demonstration of different types of data\n";
    std::cout << "=====================================\n";
    std::cout << "\n1. Integer tree\n";
    BinaryTree<int> intTree;
    intTree.insert(5);
    intTree.insert(3);
    intTree.insert(7);
    std::cout << "In-order: ";
    for (int value: intTree.traverseInOrder()) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << "\n2. Real number tree\n";
    BinaryTree<double> doubleTree;
    doubleTree.insert(5.5);
    doubleTree.insert(3.14);
    doubleTree.insert(7.77);
    std::cout << "In-order: ";
    for (double value: doubleTree.traverseInOrder()) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << "\n3. Complex number tree\n";
    BinaryTree<Complex> complexTree;
    complexTree.insert(Complex(1.0, 2.0));
    complexTree.insert(Complex(0.5, 1.5));
    complexTree.insert(Complex(2.0, 1.0));
    std::cout << "In-order: ";
    for (const Complex &value: complexTree.traverseInOrder()) {
        std::cout << value.real() << "+" << value.imag() << "i ";
    }
    std::cout << std::endl;
    std::cout << "\n4. Tree of strings\n";
    BinaryTree<std::string> stringTree;
    stringTree.insert("banana");
    stringTree.insert("apple");
    stringTree.insert("cherry");
    std::cout << "In-order: ";
    for (const std::string &value: stringTree.traverseInOrder()) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << "\n5. Function tree\n";
    BinaryTree<MathFunction> functionTree;
    MathFunction f1 = [](double x) { return x * x; };
    MathFunction f2 = [](double x) { return std::sin(x); };
    MathFunction f3 = [](double x) { return std::sqrt(x); };
    functionTree.insert(f1);
    functionTree.insert(f2);
    functionTree.insert(f3);
    std::cout << "Number of functions in the tree: " << functionTree.size() << std::endl;
    std::cout << "Results of calculating functions for x = 2:" << std::endl;
    double x = 2.0;
    auto functions = functionTree.traverseInOrder();
    for (size_t i = 0; i < functions.size(); i++) {
        std::cout << "Function " << (i + 1) << ": " << functions[i](x) << std::endl;
    }
    std::cout << "\n6. Student tree\n";
    BinaryTree<Student> studentTree;
    Student student1(PersonID(1234, 567890), "Ivan", "Ivanovich", "Ivanov",
                     std::time(nullptr) - 86400 * 365 * 20, "Department 2", 12345, 4.7);
    Student student2(PersonID(1234, 567891), "Petr", "Petrovich", "Petrov",
                     std::time(nullptr) - 86400 * 365 * 19, "Department 2", 12346, 4.5);
    Student student3(PersonID(1234, 567892), "Sidor", "Sidorovich", "Sidorov",
                     std::time(nullptr) - 86400 * 365 * 21, "Department 2", 12347, 4.9);
    studentTree.insert(student1);
    studentTree.insert(student2);
    studentTree.insert(student3);
    std::cout << "In-order traversal of the student tree:" << std::endl;
    for (const Student &student: studentTree.traverseInOrder()) {
        std::cout << student << std::endl;
    }
    std::cout << "\n7. Professors tree\n";
    BinaryTree<Teacher> teacherTree;
    Teacher teacher1(PersonID(5678, 123456), "Alex", "Alekseevich", "Alekseev",
                     std::time(nullptr) - 86400 * 365 * 40, "Department 1", "Professor", 15);
    Teacher teacher2(PersonID(5678, 123457), "Boris", "Borisovich", "Borisov",
                     std::time(nullptr) - 86400 * 365 * 35, "Department 1", "Associate Professor", 10);
    Teacher teacher3(PersonID(5678, 123458), "Vasiliy", "Vasilevich", "Vasilev",
                     std::time(nullptr) - 86400 * 365 * 30, "Department 1", "Assistant", 5);
    teacherTree.insert(teacher1);
    teacherTree.insert(teacher2);
    teacherTree.insert(teacher3);
    std::cout << "In-order traversal of the teacher tree:" << std::endl;
    for (const Teacher &teacher: teacherTree.traverseInOrder()) {
        std::cout << teacher << std::endl;
    }
    std::cout << "\n8. Map and where operations on students\n";
    auto mappedStudentTree = studentTree.map([](const Student &s) {
        Student newStudent = s;
        newStudent.SetAverageGrade(s.GetAverageGrade() + 0.1);
        return newStudent;
    });
    std::cout << "After applying map (increase in average score by 0.1):" << std::endl;
    for (const Student &student: mappedStudentTree.traverseInOrder()) {
        std::cout << student << std::endl;
    }
    auto filteredStudentTree = studentTree.where([](const Student &s) {
        return s.GetAverageGrade() > 4.6;
    });
    std::cout << "After applying where (students with GPA > 4.6):" << std::endl;
    for (const Student &student: filteredStudentTree.traverseInOrder()) {
        std::cout << student << std::endl;
    }
}

std::vector<int> generateRandomValues(size_t count, int min = 0, int max = 1000000) {
    std::vector<int> values(count);
    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<int> dist(min, max);
    for (size_t i = 0; i < count; ++i) {
        values[i] = dist(gen);
    }
    return values;
}

class PerformanceTest {
private:
    std::ofstream outputFile;

public:
    PerformanceTest(const std::string &filename) {
        outputFile.open(filename);
        if (!outputFile.is_open()) {
            throw std::runtime_error("Failed to open output file: " + filename);
        }
        outputFile << "operation,size,time_ms" << std::endl;
    }

    ~PerformanceTest() {
        if (outputFile.is_open()) {
            outputFile.close();
        }
    }

    void testInsert() {
        std::cout << "Testing insertion performance..." << std::endl;
        std::vector<size_t> sizes = {1000, 10000, 100000, 1000000};
        for (size_t size: sizes) {
            auto values = generateRandomValues(size);
            double time = measureExecutionTime([&]() {
                BinaryTree<int> tree;
                for (int value: values) {
                    tree.insert(value);
                }
            });
            std::cout << "Insert " << size << " elements: " << time << " ms" << std::endl;
            outputFile << "insert," << size << "," << time << std::endl;
        }
    }

    void testSearch() {
        std::cout << "Testing search performance..." << std::endl;
        std::vector<size_t> sizes = {1000, 10000, 100000, 1000000};
        for (size_t size: sizes) {
            auto values = generateRandomValues(size);
            BinaryTree<int> tree;
            for (int value: values) {
                tree.insert(value);
            }
            std::vector<int> searchValues = generateRandomValues(1000);
            double time = measureExecutionTime([&]() {
                for (int value: searchValues) {
                    tree.contains(value);
                }
            });
            std::cout << "Search in tree with " << size << " elements: " << time << " ms" << std::endl;
            outputFile << "search," << size << "," << time << std::endl;
        }
    }

    void testTraversals() {
        std::cout << "Testing traversal performance..." << std::endl;
        std::vector<size_t> sizes = {1000, 10000, 100000};
        for (size_t size: sizes) {
            auto values = generateRandomValues(size);
            BinaryTree<int> tree;
            for (int value: values) {
                tree.insert(value);
            }
            double time_klp = measureExecutionTime([&]() {
                tree.traverseKLP();
            });
            double time_kpl = measureExecutionTime([&]() {
                tree.traverseKPL();
            });
            double time_lpk = measureExecutionTime([&]() {
                tree.traverseLPK();
            });
            double time_lkp = measureExecutionTime([&]() {
                tree.traverseLKP();
            });
            std::cout << "KLP traversal with " << size << " elements: " << time_klp << " ms" << std::endl;
            std::cout << "KPL traversal with " << size << " elements: " << time_kpl << " ms" << std::endl;
            std::cout << "LPK traversal with " << size << " elements: " << time_lpk << " ms" << std::endl;
            std::cout << "LKP traversal with " << size << " elements: " << time_lkp << " ms" << std::endl;
            outputFile << "traversal_klp," << size << "," << time_klp << std::endl;
            outputFile << "traversal_kpl," << size << "," << time_kpl << std::endl;
            outputFile << "traversal_lpk," << size << "," << time_lpk << std::endl;
            outputFile << "traversal_lkp," << size << "," << time_lkp << std::endl;
        }
    }

    void testMap() {
        std::cout << "Testing map performance..." << std::endl;
        std::vector<size_t> sizes = {1000, 10000, 100000};
        for (size_t size: sizes) {
            auto values = generateRandomValues(size);
            BinaryTree<int> tree;
            for (int value: values) {
                tree.insert(value);
            }
            double time = measureExecutionTime([&]() {
                tree.map([](const int &x) { return x * 2; });
            });
            std::cout << "Map with " << size << " elements: " << time << " ms" << std::endl;
            outputFile << "map," << size << "," << time << std::endl;
        }
    }

    void testWhere() {
        std::cout << "Testing where performance..." << std::endl;
        std::vector<size_t> sizes = {1000, 10000, 100000};
        for (size_t size: sizes) {
            auto values = generateRandomValues(size);
            BinaryTree<int> tree;
            for (int value: values) {
                tree.insert(value);
            }
            double time = measureExecutionTime([&]() {
                tree.where([](const int &x) { return x % 2 == 0; });
            });
            std::cout << "Where with " << size << " elements: " << time << " ms" << std::endl;
            outputFile << "where," << size << "," << time << std::endl;
        }
    }

    void testBalancing() {
        std::cout << "Testing balancing performance..." << std::endl;
        std::vector<size_t> sizes = {1000, 10000};
        for (size_t size: sizes) {
            BinaryTree<int> tree;
            for (size_t i = 0; i < size; ++i) {
                tree.insert(i);
            }
            double time = measureExecutionTime([&]() {
                tree.balance();
            });
            std::cout << "Balance tree with " << size << " elements: " << time << " ms" << std::endl;
            outputFile << "balance," << size << "," << time << std::endl;
        }
    }

    void runAllTests() {
        testInsert();
        testSearch();
        testTraversals();
        testMap();
        testWhere();
        testBalancing();
    }
};

void demonstrateBinaryTree() {
    std::cout << "Binary Tree Demonstration" << std::endl;
    std::cout << "-------------------------" << std::endl;
    BinaryTree<int> tree;
    std::cout << "Inserting values: 5, 3, 7, 2, 4, 6, 8" << std::endl;
    tree.insert(5);
    tree.insert(3);
    tree.insert(7);
    tree.insert(2);
    tree.insert(4);
    tree.insert(6);
    tree.insert(8);
    std::cout << "\nTraversals:" << std::endl;
    std::cout << "KLP (Root-Left-Right): ";
    auto klp = tree.traverseKLP();
    for (int value: klp) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << "KPL (Root-Right-Left): ";
    auto kpl = tree.traverseKPL();
    for (int value: kpl) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << "LPK (Left-Right-Root): ";
    auto lpk = tree.traverseLPK();
    for (int value: lpk) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << "LKP (Left-Root-Right): ";
    auto lkp = tree.traverseLKP();
    for (int value: lkp) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << "PLK (Right-Left-Root): ";
    auto plk = tree.traversePLK();
    for (int value: plk) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << "PKL (Right-Root-Left): ";
    auto pkl = tree.traversePKL();
    for (int value: pkl) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << "\nMap function (multiply by 2): ";
    auto mappedTree = tree.map([](const int &x) { return x * 2; });
    auto mappedValues = mappedTree.traverseInOrder();
    for (int value: mappedValues) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << "Where function (even numbers only): ";
    auto filteredTree = tree.where([](const int &x) { return x % 2 == 0; });
    auto filteredValues = filteredTree.traverseInOrder();
    for (int value: filteredValues) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << "\nExtracting subtree with root value 3:" << std::endl;
    auto subtree = tree.extractSubtree(3);
    std::cout << "Subtree in-order traversal: ";
    auto subtreeValues = subtree.traverseInOrder();
    for (int value: subtreeValues) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << "Does the original tree contain the extracted subtree? ";
    std::cout << (tree.containsSubtree(subtree) ? "Yes" : "No") << std::endl;
    std::cout << "\nSerializing tree using KLP format: ";
    std::string serialized = tree.saveToString("KLP");
    std::cout << serialized << std::endl;
    std::cout << "Creating a new tree from the serialized string..." << std::endl;
    BinaryTree<int> newTree;
    newTree.loadFromString(serialized, "KLP");
    std::cout << "New tree in-order traversal: ";
    auto newTreeValues = newTree.traverseInOrder();
    for (int value: newTreeValues) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << "\nCreating another tree with values 10, 20, 30..." << std::endl;
    BinaryTree<int> anotherTree;
    anotherTree.insert(10);
    anotherTree.insert(20);
    anotherTree.insert(30);
    std::cout << "Merging the trees..." << std::endl;
    auto mergedTree = tree.merge(anotherTree);
    std::cout << "Merged tree in-order traversal: ";
    auto mergedValues = mergedTree.traverseInOrder();
    for (int value: mergedValues) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << "\nCreating an unbalanced tree with values 1, 2, 3, 4, 5..." << std::endl;
    BinaryTree<int> unbalancedTree;
    unbalancedTree.insert(1);
    unbalancedTree.insert(2);
    unbalancedTree.insert(3);
    unbalancedTree.insert(4);
    unbalancedTree.insert(5);
    std::cout << "Unbalanced tree pre-order traversal: ";
    auto unbalancedValues = unbalancedTree.traversePreOrder();
    for (int value: unbalancedValues) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
    std::cout << "Balancing the tree..." << std::endl;
    unbalancedTree.balance();
    std::cout << "Balanced tree pre-order traversal: ";
    auto balancedValues = unbalancedTree.traversePreOrder();
    for (int value: balancedValues) {
        std::cout << value << " ";
    }
    std::cout << std::endl;
}

void interactiveMenu() {
    enum DataType {
        INT_TYPE,
        DOUBLE_TYPE,
        COMPLEX_TYPE,
        STRING_TYPE,
        STUDENT_TYPE,
        TEACHER_TYPE
    };

    DataType currentType = INT_TYPE;

    BinaryTree<int> intTree;
    BinaryTree<double> doubleTree;
    BinaryTree<Complex> complexTree;
    BinaryTree<std::string> stringTree;
    BinaryTree<Student> studentTree;
    BinaryTree<Teacher> teacherTree;

    auto typeToString = [](DataType type) -> std::string {
        switch (type) {
            case INT_TYPE: return "Integer";
            case DOUBLE_TYPE: return "Double";
            case COMPLEX_TYPE: return "Complex";
            case STRING_TYPE: return "String";
            case STUDENT_TYPE: return "Student";
            case TEACHER_TYPE: return "Teacher";
            default: return "Unknown";
        }
    };

    while (true) {
        std::cout << "\nBinary Tree Interactive Menu - Current Type: " << typeToString(currentType) << std::endl;
        std::cout << "----------------------------" << std::endl;
        std::cout << "1. Change data type" << std::endl;
        std::cout << "2. Insert value" << std::endl;
        std::cout << "3. Remove value" << std::endl;
        std::cout << "4. Check if value exists" << std::endl;
        std::cout << "5. Display tree (all traversals)" << std::endl;
        std::cout << "6. Balance tree" << std::endl;
        std::cout << "7. Save tree to string" << std::endl;
        std::cout << "8. Load tree from string" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter your choice: ";

        int choice;
        std::cin >> choice;

        if (std::cin.fail()) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            std::cout << "Input error! Please enter a number." << std::endl;
            continue;
        }

        if (choice == 0) {
            break;
        }

        switch (choice) {
            case 1: {
                std::cout << "\nSelect data type:" << std::endl;
                std::cout << "1. Integer" << std::endl;
                std::cout << "2. Double" << std::endl;
                std::cout << "3. Complex" << std::endl;
                std::cout << "4. String" << std::endl;
                std::cout << "5. Student" << std::endl;
                std::cout << "6. Teacher" << std::endl;
                std::cout << "Enter your choice: ";

                int typeChoice;
                std::cin >> typeChoice;

                if (std::cin.fail()) {
                    std::cin.clear();
                    std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                    std::cout << "Input error! Please enter a number." << std::endl;
                    continue;
                }

                if (typeChoice >= 1 && typeChoice <= 6) {
                    currentType = static_cast<DataType>(typeChoice - 1);
                    std::cout << "Data type changed to " << typeToString(currentType) << std::endl;
                } else {
                    std::cout << "Invalid choice." << std::endl;
                }
                break;
            }
            case 2: {
                switch (currentType) {
                    case INT_TYPE: {
                        std::cout << "Enter integer value to insert: ";
                        int value;
                        std::cin >> value;
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Input error! Please enter a valid integer." << std::endl;
                        } else {
                            intTree.insert(value);
                            std::cout << "Value inserted." << std::endl;
                        }
                        break;
                    }
                    case DOUBLE_TYPE: {
                        std::cout << "Enter double value to insert: ";
                        double value;
                        std::cin >> value;
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Input error! Please enter a valid double." << std::endl;
                        } else {
                            doubleTree.insert(value);
                            std::cout << "Value inserted." << std::endl;
                        }
                        break;
                    }
                    case COMPLEX_TYPE: {
                        std::cout << "Enter complex value (real part): ";
                        double real;
                        std::cin >> real;
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Input error! Please enter a valid number." << std::endl;
                            break;
                        }

                        std::cout << "Enter complex value (imaginary part): ";
                        double imag;
                        std::cin >> imag;
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Input error! Please enter a valid number." << std::endl;
                            break;
                        }

                        complexTree.insert(Complex(real, imag));
                        std::cout << "Value inserted." << std::endl;
                        break;
                    }
                    case STRING_TYPE: {
                        std::cout << "Enter string value to insert: ";
                        std::string value;
                        std::cin.ignore();
                        std::getline(std::cin, value);
                        stringTree.insert(value);
                        std::cout << "Value inserted." << std::endl;
                        break;
                    }
                    case STUDENT_TYPE: {
                        std::cout << "Enter student details:\n";
                        std::cin.ignore();

                        std::cout << "First name: ";
                        std::string firstName;
                        std::getline(std::cin, firstName);

                        std::cout << "Middle name: ";
                        std::string middleName;
                        std::getline(std::cin, middleName);

                        std::cout << "Last name: ";
                        std::string lastName;
                        std::getline(std::cin, lastName);

                        std::cout << "Group: ";
                        std::string group;
                        std::getline(std::cin, group);

                        std::cout << "Student ID: ";
                        int studentId;
                        std::cin >> studentId;
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Input error! Please enter a valid number." << std::endl;
                            break;
                        }

                        std::cout << "Average grade: ";
                        double avgGrade;
                        std::cin >> avgGrade;
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Input error! Please enter a valid number." << std::endl;
                            break;
                        }

                        static int nextId = 1;
                        Student student(PersonID(1000 + nextId, 500000 + nextId), firstName, middleName, lastName,
                                       std::time(nullptr), group, studentId, avgGrade);
                        nextId++;

                        studentTree.insert(student);
                        std::cout << "Student added to the tree." << std::endl;
                        break;
                    }
                    case TEACHER_TYPE: {
                        std::cout << "Enter teacher details:\n";
                        std::cin.ignore();

                        std::cout << "First name: ";
                        std::string firstName;
                        std::getline(std::cin, firstName);

                        std::cout << "Middle name: ";
                        std::string middleName;
                        std::getline(std::cin, middleName);

                        std::cout << "Last name: ";
                        std::string lastName;
                        std::getline(std::cin, lastName);

                        std::cout << "Department: ";
                        std::string department;
                        std::getline(std::cin, department);

                        std::cout << "Position: ";
                        std::string position;
                        std::getline(std::cin, position);

                        std::cout << "Experience (years): ";
                        int experience;
                        std::cin >> experience;
                        if (std::cin.fail()) {
                            std::cin.clear();
                            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                            std::cout << "Input error! Please enter a valid number." << std::endl;
                            break;
                        }

                        static int nextId = 1;
                        Teacher teacher(PersonID(2000 + nextId, 100000 + nextId), firstName, middleName, lastName,
                                       std::time(nullptr), department, position, experience);
                        nextId++;

                        teacherTree.insert(teacher);
                        std::cout << "Teacher added to the tree." << std::endl;
                        break;
                    }
                }
                break;
            }
            case 3: {
                if (currentType == INT_TYPE) {
                    std::cout << "Enter integer value to remove: ";
                    int value;
                    std::cin >> value;
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Input error! Please enter a valid integer." << std::endl;
                    } else {
                        intTree.remove(value);
                        std::cout << "Value removed (if it existed)." << std::endl;
                    }
                } else if (currentType == DOUBLE_TYPE) {
                    std::cout << "Enter double value to remove: ";
                    double value;
                    std::cin >> value;
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Input error! Please enter a valid double." << std::endl;
                    } else {
                        doubleTree.remove(value);
                        std::cout << "Value removed (if it existed)." << std::endl;
                    }
                } else if (currentType == COMPLEX_TYPE) {
                    std::cout << "Remove for complex values is currently limited." << std::endl;
                } else if (currentType == STRING_TYPE) {
                    std::cout << "Enter string value to remove: ";
                    std::string value;
                    std::cin.ignore();
                    std::getline(std::cin, value);
                    stringTree.remove(value);
                    std::cout << "Value removed (if it existed)." << std::endl;
                } else {
                    std::cout << "Remove operation for this data type is limited in interactive mode." << std::endl;
                }
                break;
            }
            case 4: {
                if (currentType == INT_TYPE) {
                    std::cout << "Enter integer value to check: ";
                    int value;
                    std::cin >> value;
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Input error! Please enter a valid integer." << std::endl;
                    } else {
                        bool exists = intTree.contains(value);
                        std::cout << "Value " << (exists ? "exists" : "does not exist") << " in the tree." << std::endl;
                    }
                } else if (currentType == DOUBLE_TYPE) {
                    std::cout << "Enter double value to check: ";
                    double value;
                    std::cin >> value;
                    if (std::cin.fail()) {
                        std::cin.clear();
                        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
                        std::cout << "Input error! Please enter a valid double." << std::endl;
                    } else {
                        bool exists = doubleTree.contains(value);
                        std::cout << "Value " << (exists ? "exists" : "does not exist") << " in the tree." << std::endl;
                    }
                } else if (currentType == STRING_TYPE) {
                    std::cout << "Enter string value to check: ";
                    std::string value;
                    std::cin.ignore();
                    std::getline(std::cin, value);
                    bool exists = stringTree.contains(value);
                    std::cout << "Value " << (exists ? "exists" : "does not exist") << " in the tree." << std::endl;
                } else {
                    std::cout << "Contains check for this data type is limited in interactive mode." << std::endl;
                }
                break;
            }
            case 5: {
                switch (currentType) {
                    case INT_TYPE: {
                        if (intTree.isEmpty()) {
                            std::cout << "Tree is empty." << std::endl;
                            break;
                        }
                        std::cout << "In-order traversal: ";
                        for (const auto& value : intTree.traverseInOrder()) {
                            std::cout << value << " ";
                        }
                        std::cout << std::endl;
                        break;
                    }
                    case DOUBLE_TYPE: {
                        if (doubleTree.isEmpty()) {
                            std::cout << "Tree is empty." << std::endl;
                            break;
                        }
                        std::cout << "In-order traversal: ";
                        for (const auto& value : doubleTree.traverseInOrder()) {
                            std::cout << value << " ";
                        }
                        std::cout << std::endl;
                        break;
                    }
                    case COMPLEX_TYPE: {
                        if (complexTree.isEmpty()) {
                            std::cout << "Tree is empty." << std::endl;
                            break;
                        }
                        std::cout << "In-order traversal: ";
                        for (const auto& value : complexTree.traverseInOrder()) {
                            std::cout << value.real() << "+" << value.imag() << "i ";
                        }
                        std::cout << std::endl;
                        break;
                    }
                    case STRING_TYPE: {
                        if (stringTree.isEmpty()) {
                            std::cout << "Tree is empty." << std::endl;
                            break;
                        }
                        std::cout << "In-order traversal: ";
                        for (const auto& value : stringTree.traverseInOrder()) {
                            std::cout << "\"" << value << "\" ";
                        }
                        std::cout << std::endl;
                        break;
                    }
                    case STUDENT_TYPE: {
                        if (studentTree.isEmpty()) {
                            std::cout << "Tree is empty." << std::endl;
                            break;
                        }
                        std::cout << "In-order traversal of students:" << std::endl;
                        for (const auto& student : studentTree.traverseInOrder()) {
                            std::cout << student << std::endl;
                        }
                        break;
                    }
                    case TEACHER_TYPE: {
                        if (teacherTree.isEmpty()) {
                            std::cout << "Tree is empty." << std::endl;
                            break;
                        }
                        std::cout << "In-order traversal of teachers:" << std::endl;
                        for (const auto& teacher : teacherTree.traverseInOrder()) {
                            std::cout << teacher << std::endl;
                        }
                        break;
                    }
                }
                break;
            }
            case 6: {
                switch (currentType) {
                    case INT_TYPE:
                        intTree.balance();
                        std::cout << "Tree balanced." << std::endl;
                        break;
                    case DOUBLE_TYPE:
                        doubleTree.balance();
                        std::cout << "Tree balanced." << std::endl;
                        break;
                    default:
                        std::cout << "Balance operation is not supported for this data type." << std::endl;
                        break;
                }
                break;
            }
            case 7: {
                std::cout << "Enter traversal format (KLP, KPL, etc.): ";
                std::string format;
                std::cin >> format;

                try {
                    std::string serialized;
                    switch (currentType) {
                        case INT_TYPE:
                            serialized = intTree.saveToString(format);
                            break;
                        case DOUBLE_TYPE:
                            serialized = doubleTree.saveToString(format);
                            break;
                        default:
                            std::cout << "Serialization is not supported for this data type." << std::endl;
                            serialized = "";
                            break;
                    }

                    if (!serialized.empty()) {
                        std::cout << "Serialized tree: " << serialized << std::endl;
                    }
                } catch (const std::exception& e) {
                    std::cout << "Error during serialization: " << e.what() << std::endl;
                }
                break;
            }
            case 8: {
                std::cout << "Loading from string is supported only for basic types." << std::endl;
                if (currentType == INT_TYPE || currentType == DOUBLE_TYPE) {
                    std::cout << "Enter serialized string: ";
                    std::string serialized;
                    std::cin.ignore();
                    std::getline(std::cin, serialized);

                    std::cout << "Enter traversal format (KLP, KPL, etc.): ";
                    std::string format;
                    std::cin >> format;

                    try {
                        switch (currentType) {
                            case INT_TYPE:
                                intTree.loadFromString(serialized, format);
                                break;
                            case DOUBLE_TYPE:
                                doubleTree.loadFromString(serialized, format);
                                break;
                            default:
                                break;
                        }
                        std::cout << "Tree loaded successfully." << std::endl;
                    } catch (const std::exception &e) {
                        std::cout << "Error loading tree: " << e.what() << std::endl;
                    }
                }
                break;
            }
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    }
}

void runPerformanceTestsAndGenerateCSV() {
    std::cout << "\nRunning performance tests and generating CSV file...\n";
    try {
        PerformanceTest perf("performance_results.csv");
        perf.runAllTests();
        std::cout << "\nPerformance data has been saved to 'performance_results.csv'\n";
        std::cout << "Use the Python script to visualize the results.\n";
    } catch (const std::exception &e) {
        std::cout << "Error during performance testing: " << e.what() << std::endl;
    }
}

int main() {
    std::cout << "Binary Tree Implementation" << std::endl;
    std::cout << "=========================" << std::endl;
    int choice;
    do {
        std::cout << "\nMain Menu:" << std::endl;
        std::cout << "1. Run Demonstration" << std::endl;
        std::cout << "2. Demonstrate Various Data Types" << std::endl;
        std::cout << "3. Run Performance Tests" << std::endl;
        std::cout << "4. Interactive Menu" << std::endl;
        std::cout << "5. Generate performance_results.csv" << std::endl;
        std::cout << "0. Exit" << std::endl;
        std::cout << "Enter your choice: ";
        std::cin >> choice;
        switch (choice) {
            case 1:
                demonstrateBinaryTree();
                break;
            case 2:
                demonstrateDataTypes();
                break;
            case 3: {
                try {
                    PerformanceTest perf("performance_results.csv");
                    perf.runAllTests();
                    std::cout << "\nPerformance data has been saved to 'performance_results.csv'" << std::endl;
                    std::cout << "Use the Python script to visualize the results." << std::endl;
                } catch (const std::exception &e) {
                    std::cout << "Error during performance testing: " << e.what() << std::endl;
                }
                break;
            }
            case 4:
                interactiveMenu();
                break;
            case 5:
                runPerformanceTestsAndGenerateCSV();
                break;
            case 0:
                std::cout << "Exiting program. Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Invalid choice. Please try again." << std::endl;
        }
    } while (choice != 0);
    return 0;
}

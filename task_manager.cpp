#include <iostream>
#include <string>
#include <list>
#include <set>
#include <stdexcept>
#include <memory>
#include <algorithm>

using namespace std;

class Task {
protected:
    string name;
    string deadline;
    string status;
    int priority;

public:
    Task(const string& user_name, const string& user_deadline, int user_priority) 
        : name(user_name), deadline(user_deadline), status("Pending"), priority(user_priority) {}

    virtual void display() const {
        cout << "Task: " << name << ", Deadline: " << deadline << ", Status: " << status << ", Priority: " << priority << endl;
    }

    void updateStatus(const string& newStatus) {
        status = newStatus;
    }

    string getName() const {
        return name;
    }

    string getStatus() const {
        return status;
    }

    string getDeadline() const {
        return deadline;
    }

    int getPriority() const {
        return priority;
    }
};

class HighPriorityTask : public Task {
public:
    HighPriorityTask(const string& user_name, const string& user_deadline, int priority)
        : Task(user_name, user_deadline, priority) {}

    void display() const override {
        cout << "High Priority ";
        Task::display();
    }
};

class LowPriorityTask : public Task {
public:
    LowPriorityTask(const string& user_name, const string& user_deadline, int priority)
        : Task(user_name, user_deadline, priority) {}

    void display() const override {
        cout << "Low Priority ";
        Task::display();
    }
};

class TaskManager {
private:
    list<shared_ptr<Task>> tasks;

public:
    void addTask(const string& name, const string& deadline, int priority) {
        if (priority < 1 || priority > 5) {
            throw invalid_argument("Priority must be between 1 and 5.");
        }
        shared_ptr<Task> task;
        if (priority >= 4) {
            task = make_shared<HighPriorityTask>(name, deadline,priority);
        } else {
            task = make_shared<LowPriorityTask>(name, deadline,priority);
        }
        tasks.push_back(task);
        sortTasks();
    }

    void viewTasks() const {
        for (const auto& task : tasks) {
            task->display();
        }
    }

    void updateTaskStatus(const string& name, const string& newStatus) {
        for (auto& task : tasks) {
            if (task->getName() == name) {
                task->updateStatus(newStatus);
                return;
            }
        }
        throw runtime_error("Task not found.");
    }

    void deleteTask(const string& name) {
        auto it = find_if(tasks.begin(), tasks.end(), [&name](const shared_ptr<Task>& task) {
            return task->getName() == name;
        });

        if (it != tasks.end()) {
            tasks.erase(it);
            cout << "Task '" << name << "' deleted successfully!" << endl;
        } else {
            throw runtime_error("Task not found.");
        }
    }

private:
    void sortTasks() {
        tasks.sort([](const shared_ptr<Task>& a, const shared_ptr<Task>& b) {
            if (a->getDeadline() == b->getDeadline()) {
                return a->getPriority() > b->getPriority();
            }
            return a->getDeadline() < b->getDeadline();
        });
    }
};

int main() {
    TaskManager manager;
    int option;

    while (true) {
        cout << "\nMenu:\n";
        cout << "1. Add Task\n";
        cout << "2. View Tasks\n";
        cout << "3. Update Task Status\n";
        cout << "4. Delete Task\n";
        cout << "5. Exit\n ";
        cout << "Enter your option: ";
        cin >> option;

        if (option == 1) {
            string name, deadline, type;
            int priority;
            cout << "Enter task name: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter deadline (DD-MM-YYYY): ";
            getline(cin, deadline);
            cout << "Enter priority (1-5) or type (High/Low): ";
            if (cin >> priority) {
                try {
                    manager.addTask(name, deadline, priority);
                    cout << "Task added successfully!" << endl;
                } catch (const invalid_argument& e) {
                    cerr << "Error: " << e.what() << endl;
                }
            } else {
                cin.clear();
                cin >> type;
                try {
                    if (type == "High") {
                        manager.addTask(name, deadline, 5);
                    } else if (type == "Low") {
                        manager.addTask(name, deadline, 1);
                    } else {
                        throw invalid_argument("Invalid task type.");
                    }
                    cout << "Task added successfully!" << endl;
                } catch (const invalid_argument& e) {
                    cerr << "Error: " << e.what() << endl;
                }
            }
        } else if (option == 2) {
            cout << "Current tasks:" << endl;
            manager.viewTasks();
        } else if (option == 3) {
            string name, newStatus;
            cout << "Enter task name to update: ";
            cin.ignore();
            getline(cin, name);
            cout << "Enter new status (Pending/Completed): ";
            getline(cin, newStatus);

            try {
                manager.updateTaskStatus(name, newStatus);
                cout << "Task status updated successfully!" << endl;
            } catch (const runtime_error& e) {
                cerr << "Error: " << e.what() << endl;
            }
        } else if (option == 4) {
            string name;
            cout << "Enter task name to delete: ";
            cin.ignore();
            getline(cin, name);

            try {
                manager.deleteTask(name);
            } catch (const runtime_error& e) {
                cerr << "Error: " << e.what() << endl;
            }
        } else if (option == 5) {
            cout << "Exiting application" << endl;
            break;
        } else {
            cout << "Invalid option. Please try again." << endl;
        }
    }

    return 0;
}

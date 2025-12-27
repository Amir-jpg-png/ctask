#include <cstdlib>
#include <iostream>
#include <fstream>
#include <vector>
#include "json.hpp"
#include "Task.h"

void loadTasksFromFile(std::vector<Task>& tasks);
void print_tasks(const std::vector<Task>& tasks);
void add_task(std::vector<Task>& tasks, const std::string& task_name);
void remove_task(std::vector<Task>& tasks, unsigned short id);
void check_tasks(std::vector<Task>& tasks, unsigned short id, bool checked);
void check_all(std::vector<Task>& tasks, bool checked);
void save_tasks_to_file(const std::vector<Task>& tasks);
void print_invalid_usage_of_option(const std::string& option);
unsigned short extract_id(int argc, const char* id_to_convert, const std::string& option);
void purge_tasks(std::vector<Task>& tasks);
void print_help_message();

int main(const int argc, char* argv[]) {
    if (argc < 2)
    {
        std::cerr << "\nUsage: " << argv[0] << " 'option' <variable>" << std::endl;
        std::cerr << "Try ctask --help" << std::endl;
        return EXIT_FAILURE;
    }
    const std::string option = argv[1];
    if (option != "ls" && option != "add" && option != "rm" && option != "check" && option != "uncheck" && option != "--help" && option != "purge")
    {
        std::cerr << "\nInvalid option: " << option << std::endl;
        std::cerr << "Usage: " << argv[0] << " 'option' <variable>" << std::endl;
        std::cerr << "Try ctask --help" << std::endl;
        return EXIT_FAILURE;
    }
    if (option == "--help"){
      print_help_message();
      return EXIT_SUCCESS;
    }
    std::vector<Task> tasks;
    loadTasksFromFile(tasks);
    if (option == "ls")
    {
        std::cout << std::endl;
        print_tasks(tasks);
    }
    else if (option == "check" || option == "uncheck")
    {
        const bool checked = option == "check";
        if (argc >= 3 && std::string(argv[2]) == "all")
        {
            check_all(tasks, checked);
        } else
        {
            const unsigned short id = extract_id(argc, argv[2], option);
            check_tasks(tasks, id, checked);
        }
    }
    else if (option == "add")
    {
        std::string task_name;
        for (int i = 2; i < argc; i++)
        {
            task_name += argv[i];
            if (i + 1 < argc)
            {
                task_name += " ";
            }
        }

        add_task(tasks, task_name);
    }
    else if (option == "rm")
    {
        const unsigned short id = extract_id(argc, argv[2], option);
        remove_task(tasks, id);
    }
    else if (option == "purge")
    {
        std::cout << "Are you sure you want to remove all tasks (y/n): ";
        if (char choice = std::cin.get(); choice != 'y' && choice != 'Y')
        {
            std::cout << "Aborting purge process." << std::endl;
            return EXIT_FAILURE;
        }
        purge_tasks(tasks);
    }
    save_tasks_to_file(tasks);
}


void loadTasksFromFile(std::vector<Task>& tasks)
{
    std::ifstream f(".tasks.json");
    if (!f.is_open()) {
        std::cout << "fatal: not a ctask repository\n do you want to init it (y/n): ";
        char choice = std::cin.get();
        if (choice != 'y')
        {
            exit(0);
        }
        std::ofstream out(".tasks.json");
        if (!out.is_open()) {
            throw std::runtime_error("Failed to create .tasks.json");
        }
        out << "[]" << std::endl;
        out.close();
        return;
    }

    nlohmann::json data;
    try {
        data = nlohmann::json::parse(f);
    } catch (...) {
        std::cerr << "Failed to parse .tasks.json\n";
        return;
    }

    if (!data.is_array()) {
        std::cerr << "Corrupt .tasks.json (expected array)\n";
        return;
    }

    for (auto& item : data)
    {
        std::string name = item["name"].get<std::string>();
        bool checked = item["checked"].get<bool>();
        unsigned short id = item["id"].get<unsigned short>();

        tasks.emplace_back(name, checked, id);
    }
}

void print_tasks(const std::vector<Task>& tasks)
{
    for (auto& task : tasks)
    {
        const char* symbol = task.get_checked() ? "X" : " ";
        std::cout << "- [" << symbol << "] " << task.get_id() << " " << task.get_name() << std::endl;
    }
}

void add_task(std::vector<Task>& tasks, const std::string& task_name)
{
    tasks.emplace_back(task_name);
    std::cout << task_name << " added successfully" << std::endl;
}

void remove_task(std::vector<Task>& tasks, const unsigned short id)
{
    for (int i = 0; i < tasks.size(); i++)
    {
        if (tasks[i].get_id() == id)
        {
            Task task = tasks[i];
            tasks.erase(tasks.begin() + i);
            std::cout << task.get_name() << " removed successfully" << std::endl;
            return;
        }
    }
}

void check_tasks(std::vector<Task>& tasks, const unsigned short id, bool checked)
{
    for (auto& task : tasks)
    {
        if (task.get_id() == id)
        {
            task.set_checked(checked);
            std::cout << task.get_name() << " was" << (checked ? " checked" : " unchecked") << " successfully" << std::endl;
            return;
        }
    }
}

nlohmann::json to_json(const std::vector<Task>& tasks)
{
    nlohmann::json arr = nlohmann::json::array();

    for (const auto& task : tasks)
    {
        arr.push_back({
            {"id", task.get_id()},
            {"name", task.get_name()},
            {"checked", task.get_checked()}
        });
    }

    return arr;
}


void save_tasks_to_file(const std::vector<Task>& tasks)
{
    nlohmann::json j = to_json(tasks);

    std::ofstream f(".tasks.json");
    if (!f.is_open())
    {
        std::cerr << "Failed to open file for writing\n";
        return;
    }

    f << std::setw(2) << j << std::endl;
}


void print_invalid_usage_of_option(const std::string& option)
{
    std::cerr << "Invalid usage of option: " << option << std::endl;
    std::cerr << "\nUsage: " << "ctask 'option' <variable>" << std::endl;
    std::cerr << "Try ctask --help" << std::endl;
}

unsigned short extract_id(int argc, const char* id_to_convert, const std::string& option)
{
    if (argc < 3)
    {
        print_invalid_usage_of_option(option);
        exit(EXIT_FAILURE);
    }
    unsigned short id = 0;
    try
    {
        id = std::stoi(id_to_convert);
    } catch (...)
    {
        print_invalid_usage_of_option(option);
        exit(EXIT_FAILURE);
    }
    return id;
}

void check_all(std::vector<Task>& tasks, const bool checked)
{
    for (auto& task : tasks)
    {
        task.set_checked(checked);
    }
    std::cout << (checked ? "All tasks checked successfully.\n"
                      : "All tasks unchecked successfully.\n");

}

void purge_tasks(std::vector<Task>& tasks)
{
    tasks.clear();
    std::cout << "Tasks purged" << std::endl;
}

void print_help_message(){
        std::string help_message = R"(
ctask - Simple Command-Line Task Manager
----------------------------------------
Usage:
  ctask [command] [arguments]

Commands:
  ls
      List all tasks with their IDs and status.

  add <task_name>
      Add a new task with the given name.
      Example: ctask add "Buy groceries"

  rm <task_id>
      Remove the task with the specified ID.
      Example: ctask rm 3

  check <task_id>
      Mark the task with the given ID as completed. | Mark all tasks as completed.
      Example: ctask check 2 | ctask check all

  uncheck <task_id>
      Mark the task with the given ID as not completed. | Mark all tasks as not completed.
      Example: ctask uncheck 2 | ctask uncheck all

  --help
      Display this help message.

Notes:
  - All tasks are stored in a JSON file located in:
      .tasks.json
  - Task IDs are assigned automatically.
        )";
        std::cout << help_message << std::endl;
}

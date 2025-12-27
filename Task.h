#ifndef TODO_CLI_TASK_H
#define TODO_CLI_TASK_H
#include <string>

class Task
{
    static unsigned short highest_id;
    unsigned short id;
    std::string name;
    bool checked = false;
public:
    explicit Task(std::string name);
    Task(std::string name, bool checked, unsigned short id);
    [[nodiscard]] unsigned short get_id() const;
    [[nodiscard]] std::string get_name() const;
    [[nodiscard]] bool get_checked() const;
    void set_checked(bool checked);
};

#endif //TODO_CLI_TASK_H
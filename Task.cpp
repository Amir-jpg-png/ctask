#include "Task.h"

#include <utility>

Task::Task(std::string name) : Task(std::move(name), false, highest_id + 1)
{}

Task::Task(std::string name, const bool checked, const unsigned short id) : id(id), name(std::move(name)), checked(checked)
{
    if (id > highest_id)
        highest_id = id;
}

bool Task::get_checked() const
{
    return checked;
}

std::string Task::get_name() const
{
    return name;
}

unsigned short Task::get_id() const
{
    return id;
}

void Task::set_checked(bool checked)
{
    this->checked = checked;
}

unsigned short Task::highest_id = 0;

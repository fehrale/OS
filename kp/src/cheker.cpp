// #include <iostream>
// #include <fstream>
// #include <INIReader.h>
// #include <unordered_map>
// #include <unordered_set>
// #include <queue>
// #include <vector>
// #include <sstream>
// #include <sys/wait.h>
// #include <thread>
// #include <mutex>
// #include <string>
// #include <signal.h>
// #include <unistd.h>

// // структура для джоба
// struct Job {
//     int name;
//     std::string command;
//     std::vector<int> depends_on;
//     std::string mutex_name;
// };

// std::vector<Job> parser(const std::string& filename) {
//     INIReader reader(filename);

//     if (reader.ParseError() != 0) {
//         std::cerr << "Cannot parse INI file: " << filename << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     std::vector<Job> jobs;

//     // Список секций задаётся явно или определяется вручную (если библиотека этого не поддерживает)
//     std::vector<std::string> sections;
//     for (const auto& section : {"job1", "job2", "job3"}) { // Пример явного указания
//         if (reader.HasSection(section)) {
//             sections.push_back(section);
//         }
//     }

//     for (const auto& section : sections) {
//         Job job;
//         job.name = std::stoi(section.substr(3)); // "jobN" -> N
//         job.command = reader.Get(section, "command", "");
//         job.mutex_name = reader.Get(section, "mutex", "");

//         std::string depends_on = reader.Get(section, "depends_on", "");
//         if (!depends_on.empty()) {
//             std::istringstream ss(depends_on);
//             int dep;
//             while (ss >> dep) {
//                 job.depends_on.push_back(dep);
//             }
//         }
//         jobs.push_back(job);
//     }

//     return jobs;
// }

// // создаём дерево, которое показывает переходы от данной вершины к её родителю(ям)
// std::vector<std::unordered_set<int>> create_tree_from_up_to_down(const std::vector<Job>& jobs, std::unordered_set<int>& start_jobs) {
//     std::vector<std::unordered_set<int>> tree(jobs.size());

//     for (const auto& job : jobs) {
//         if (job.depends_on.empty()) {
//             start_jobs.insert(job.name);
//         }

//         for (const auto& dependence : job.depends_on) {
//             tree[job.name - 1].insert(dependence);
//         }
//     }
//     return tree;
// }

// // находим конечные джобы
// void find_end_jobs(const std::vector<std::unordered_set<int>>& tree, std::unordered_set<int>& end_jobs) {
//     std::vector<bool> is_top(tree.size(), true);

//     for (const auto& children : tree) {
//         for (const auto& child : children) {
//             is_top[child - 1] = false;
//         }
//     }

//     for (size_t i = 0; i < is_top.size(); ++i) {
//         if (is_top[i]) {
//             end_jobs.insert(i + 1);
//         }
//     }
// }

// // находим циклы
// bool find_cycle(std::vector<std::unordered_set<int>>& tree, int u, std::vector<int>& visited) {
//     if (visited[u] == 1) {
//         return true;
//     }
//     if (visited[u] == 2) {
//         return false;
//     }
//     visited[u] = 1;
//     for (int v : tree[u]) {
//         if (find_cycle(tree, v - 1, visited)) {
//             return true;
//         }
//     }
//     visited[u] = 2;
//     return false;
// }

// // прогоняем функцию нахождения циклов для каждой вершины дерева
// bool check_cycle(std::vector<std::unordered_set<int>>& tree, std::unordered_set<int>& starts) {
//     std::vector<int> visited(tree.size());
//     for (int start : starts) {
//         if (find_cycle(tree, start - 1, visited)) {
//             return false;
//         }
//     }
//     return true;
// }

// // обход дерева в глубину и отметка посещённых вершин
// void dfs(std::vector<std::unordered_set<int>>& tree, int u, std::vector<int>& visited) {
//     if (visited[u] == 1) {
//         return;
//     }
//     visited[u] = 1;
//     for (int v : tree[u]) {
//         dfs(tree, v - 1, visited);
//     }
// }

// /* ищет компоненты отделенные от графа
//  * @return true, если текущий компонент не связан с графом, false, если связь есть.
//  */
// bool find_components(std::vector<std::unordered_set<int>>& tree, int u, std::vector<int>& visited) {
//     if (visited[u] == 1) {
//         return false;
//     }
//     if (tree[u].empty()) {
//         return true;
//     }
//     visited[u] = 1;
//     bool res = true;
//     for (int v : tree[u]) {
//         res &= find_components(tree, v - 1, visited);
//     }
//     return res;
// }

// // проверка графа на одну компоненту связности
// bool check_connectivity(std::vector<std::unordered_set<int>>& tree, std::unordered_set<int>& starts) {
//     std::vector<int> visited(tree.size());
//     dfs(tree, *(starts.begin()) - 1, visited);
//     for (auto itr = std::next(starts.begin(), 1); itr != starts.end(); ++itr) {
//         if (find_components(tree, *itr - 1, visited)) {
//             return false;
//         }
//     }
//     return true;
// }

// void killall_jobs(std::queue<std::pair<int, pid_t>>& q) {
//     while (!q.empty()) {
//         auto [id, pid] = q.front();
//         q.pop();
//         if (pid != -1)
//             kill(pid, SIGINT);
//     }
// }

// pid_t create_job(std::string command) {
//     std::stringstream ss{command};
//     std::vector<std::string> strargs;
//     std::vector<const char*> args;
//     std::string str;
//     while (ss >> str) {
//         strargs.push_back(str);
//         args.push_back(str.c_str());
//     }

//     pid_t pid = fork();
//     if (pid == 0) {
//         execv(args[0], const_cast<char* const*>(args.data()));
//         std::cerr << "exec error" << std::endl;
//         exit(-2);
//     }

//     std::cout << "create " << pid << ": " << command << std::endl;
//     return pid;
// }

// int main() {
//     std::vector<Job> jobs = parser("test.ini"); // читаем файл

//     std::unordered_set<int> start_jobs, end_jobs;
//     std::vector<std::unordered_set<int>> tree = create_tree_from_up_to_down(jobs, start_jobs);
//     find_end_jobs(tree, end_jobs);

//     if (start_jobs.empty()) {
//         std::cerr << "Error: Отсутствуют начальные джобы" << std::endl;
//         return -1;
//     }
//     if (end_jobs.empty()) {
//         std::cerr << "Error: Отсутствуют завершающие джобы" << std::endl;
//         return -1;
//     }
//     if (!check_cycle(tree, end_jobs)) {
//         std::cerr << "Error: В графе есть циклы" << std::endl;
//         return -1;
//     }
//     if (!check_connectivity(tree, end_jobs)) {
//         std::cerr << "Error: В графе больше одной компоненты связности" << std::endl;
//         return -1;
//     }

//     std::unordered_map<std::string, bool> mutex_vals;
//     std::unordered_map<int, std::string> mutex_names;

//     for (const auto& job : jobs) {
//         std::string mutex_name = job.mutex_name;
//         if (!mutex_name.empty()) {
//             mutex_names[job.name] = mutex_name;
//             mutex_vals[mutex_name] = true;
//         }
//     }

//     std::queue<std::pair<int, pid_t>> waitq;
//     std::mutex qmtx;

//     std::thread wait_thread([&]() {
//         while (!end_jobs.empty() || !waitq.empty()) {
//             if (!waitq.empty()) {
//                 pid_t pid;
//                 int id;
//                 {
//                     std::lock_guard<std::mutex> lock(qmtx);
//                     id = waitq.front().first;
//                     pid = waitq.front().second;
//                     waitq.pop();
//                 }
//                 if (pid != -1) {
//                     int sig;
//                     waitpid(pid, &sig, 0);
//                     if (WIFSIGNALED(sig)) {
//                         std::lock_guard<std::mutex> lock(qmtx);
//                         end_jobs.clear();
//                         killall_jobs(waitq);
//                     }
//                 }
//                 {
//                     std::lock_guard<std::mutex> lock(qmtx);
//                     mutex_vals[mutex_names[id - 1]] = true;
//                 }
//                 for (std::size_t i = 0; i < tree.size(); ++i) {
//                     if (tree[i].count(id)) {
//                         tree[i].erase(id);
//                         if (tree[i].empty()) {
//                             std::lock_guard<std::mutex> lock(qmtx);
//                             start_jobs.insert(i + 1);
//                         }
//                     }
//                 }
//                 end_jobs.erase(id);
//             }
//         }
//     });

//     while (!end_jobs.empty()) {
//         std::vector<int> erase_id;
//         std::lock_guard<std::mutex> lock(qmtx);
//         for (int id : start_jobs) {
//             if ((mutex_names.count(id) && mutex_vals[mutex_names[id]]) || !mutex_names.count(id)) {
//                 std::string command = jobs[id - 1].command;
//                 if (command.front() != '.' && command.front() != '/') {
//                     int success = std::system(command.c_str());
//                     if (success == -1) {
//                         killall_jobs(waitq);
//                     }
//                     erase_id.push_back(id);
//                     waitq.push({id, -1});
//                 } else {
//                     pid_t pid = create_job(command);
//                     if (mutex_names.count(id))
//                         mutex_vals[mutex_names[id]] = false;

//                     erase_id.push_back(id);
//                     waitq.push({id, pid});
//                 }
//             }
//         }
//         for (int id : erase_id) {
//             start_jobs.erase(id);
//         }
//     }

//     killall_jobs(waitq);
//     wait_thread.join();
//     return 0;
// }





// #include <iostream>
// #include <INIReader.h>
// #include <vector>
// #include <string>
// #include <sstream>
// #include "../include/error.h"      // Подключение заголовка error.h
// #include "../include/timer_job.h"  // Подключение заголовка timer_job.h

// // структура для джоба
// struct Job {
//     int name;
//     std::string command;
//     std::vector<int> depends_on;
//     std::string mutex_name;
// };

// // Пример функции для парсинга INI файла
// std::vector<Job> parser(const std::string& filename) {
//     INIReader reader(filename);

//     if (reader.ParseError() != 0) {
//         std::cerr << "Cannot parse INI file: " << filename << std::endl;
//         exit(EXIT_FAILURE);
//     }

//     std::vector<Job> jobs;

//     // Список секций задаётся явно или определяется вручную
//     std::vector<std::string> sections;
//     for (const auto& section : {"job1", "job2", "job3", "job4", "job5", "job6"}) {
//         if (reader.HasSection(section)) {
//             sections.push_back(section);
//         }
//     }

//     for (const auto& section : sections) {
//         Job job;
//         job.name = std::stoi(section.substr(3)); // "jobN" -> N
//         job.command = reader.Get(section, "command", "");
//         job.mutex_name = reader.Get(section, "mutex", "");

//         std::string depends_on = reader.Get(section, "depends_on", "");
//         if (!depends_on.empty()) {
//             std::istringstream ss(depends_on);
//             int dep;
//             while (ss >> dep) {
//                 job.depends_on.push_back(dep);
//             }
//         }
//         jobs.push_back(job);
//     }

//     return jobs;
// }

// int main() {
//     // Шаг 1: Парсинг файла конфигурации
//     std::vector<Job> jobs = parser("new_test.ini");
//     std::cout << "Jobs parsed successfully!" << std::endl;

//     // Шаг 2: Выполнение функций из других модулей
//     simulate_error();
//     run_timer_job();

//     // Шаг 3: Выполнение задач
//     for (const auto& job : jobs) {
//         std::cout << "Executing job " << job.name << ": " << job.command << std::endl;
//         if (system(job.command.c_str()) != 0) {
//             std::cerr << "Error executing job " << job.name << ": " << job.command << std::endl;
//         }
//     }

//     std::cout << "All jobs completed successfully!" << std::endl;
//     return 0;
// }


#include <iostream>
#include <INIReader.h>
#include <vector>
#include <string>
#include <sstream>
#include <unordered_set>       // Добавлено подключение для unordered_set
#include <unordered_map> 
#include <functional>  // Добавлено подключение для std::function
#include <algorithm>
#include <stack>
#include <stdexcept>
#include <thread>
#include <mutex>

#include "../include/error.h"      // Подключение заголовка error.h
#include "../include/timer_job.h"  // Подключение заголовка timer_job.h

// структура для джоба
struct Job {
    int name;
    std::string command;
    std::vector<int> depends_on;
    std::string mutex_name;
};

// Пример функции для парсинга INI файла
std::vector<Job> parser(const std::string& filename) {
    INIReader reader(filename);

    if (reader.ParseError() != 0) {
        std::cerr << "Cannot parse INI file: " << filename << std::endl;
        exit(EXIT_FAILURE);
    }

    std::vector<Job> jobs;

    // Список секций задаётся явно или определяется вручную
    std::vector<std::string> sections;
    for (const auto& section : {"job1", "job2", "job3", "job4", "job5", "job6"}) {
        if (reader.HasSection(section)) {
            sections.push_back(section);
        }
    }

    for (const auto& section : sections) {
        Job job;
        job.name = std::stoi(section.substr(3)); // "jobN" -> N
        job.command = reader.Get(section, "command", "");
        job.mutex_name = reader.Get(section, "mutex", "");

        std::string depends_on = reader.Get(section, "depends_on", "");
        if (!depends_on.empty()) {
            std::istringstream ss(depends_on);
            int dep;
            while (ss >> dep) {
                job.depends_on.push_back(dep);
            }
        }
        jobs.push_back(job);
    }

    return jobs;
}

// Проверка на циклы в графе
bool has_cycle(const std::vector<Job>& jobs) {
    std::unordered_set<int> visited, in_recursion;
    std::unordered_map<int, std::vector<int>> adj_list;

    // Строим список смежности
    for (const auto& job : jobs) {
        adj_list[job.name] = job.depends_on;
    }

    std::function<bool(int)> dfs = [&](int job_name) {
        if (in_recursion.count(job_name)) {
            return true;  // Цикл найден
        }
        if (visited.count(job_name)) {
            return false; // Уже посещена, нет цикла
        }

        visited.insert(job_name);
        in_recursion.insert(job_name);

        for (int dep : adj_list[job_name]) {
            if (dfs(dep)) {
                return true;
            }
        }

        in_recursion.erase(job_name);
        return false;
    };

    // Проходим по всем джобам
    for (const auto& job : jobs) {
        if (dfs(job.name)) {
            return true;  // Если хотя бы один цикл найден
        }
    }

    return false;
}

// Проверка на наличие стартовых и завершающих джоб
bool validate_start_and_end_jobs(const std::vector<Job>& jobs) {
    std::unordered_set<int> dependent_jobs;
    for (const auto& job : jobs) {
        for (int dep : job.depends_on) {
            dependent_jobs.insert(dep);
        }
    }

    bool has_start = false, has_end = false;
    for (const auto& job : jobs) {
        if (job.depends_on.empty()) {
            has_start = true;  // Стартовая джоба
        }
        if (dependent_jobs.find(job.name) == dependent_jobs.end()) {
            has_end = true;    // Завершающая джоба
        }
    }

    return has_start && has_end;
}

// Проверка на связанность графа
bool is_connected(const std::vector<Job>& jobs) {
    std::unordered_set<int> visited;
    std::unordered_map<int, std::vector<int>> adj_list;

    // Строим список смежности
    for (const auto& job : jobs) {
        adj_list[job.name] = job.depends_on;
    }

    // Начинаем обход с первой джобы, если она есть
    if (!jobs.empty()) {
        std::function<void(int)> dfs = [&](int job_name) {
            if (visited.count(job_name)) return;
            visited.insert(job_name);
            for (int dep : adj_list[job_name]) {
                dfs(dep);
            }
        };

        // Начинаем DFS с первой джобы
        dfs(jobs[0].name);
    }

    // Если количество посещённых джоб равно общему количеству джоб, то граф связан
    return visited.size() == jobs.size();
}

// Механизм для параллельного выполнения с ограничением на количество активных задач
std::mutex job_mutex;
int active_jobs = 0;
const int MAX_PARALLEL_JOBS = 3;  // Максимальное количество параллельных джоб

void execute_job(const Job& job) {
    std::unique_lock<std::mutex> lock(job_mutex);
    
    while (active_jobs >= MAX_PARALLEL_JOBS) {
        // Ждём, пока не освободится место для нового потока
        lock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        lock.lock();
    }

    ++active_jobs;
    lock.unlock();

    // Выполнение джобы
    std::cout << "Executing job " << job.name << ": " << job.command << std::endl;
    if (system(job.command.c_str()) != 0) {
        std::cerr << "Error executing job " << job.name << ": " << job.command << std::endl;
    }

    lock.lock();
    --active_jobs;
    lock.unlock();
}

int main() {
    // Шаг 1: Парсинг файла конфигурации
    std::vector<Job> jobs = parser("new_test.ini");
    std::cout << "Jobs parsed successfully!" << std::endl;

    // Шаг 2: Проверка графа на корректность
    if (has_cycle(jobs)) {
        std::cerr << "Error: The job graph has a cycle!" << std::endl;
        return EXIT_FAILURE;
    }

    if (!validate_start_and_end_jobs(jobs)) {
        std::cerr << "Error: The graph does not contain valid start and end jobs!" << std::endl;
        return EXIT_FAILURE;
    }

    if (!is_connected(jobs)) {
        std::cerr << "Error: The job graph is not connected!" << std::endl;
        return EXIT_FAILURE;
    }

    std::cout << "Graph is valid!" << std::endl;

    // Шаг 3: Выполнение функций из других модулей
    simulate_error();
    run_timer_job();

    // Шаг 4: Выполнение задач
    std::vector<std::thread> threads;
    for (const auto& job : jobs) {
        threads.push_back(std::thread(execute_job, job));
    }
    
    // Ожидание завершения всех потоков
    for (auto& th : threads) {
        th.join();
    }

    std::cout << "All jobs completed successfully!" << std::endl;
    return 0;
}
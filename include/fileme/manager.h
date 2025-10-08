#ifndef MANAGER_H
#define MANAGER_H

#include <filesystem>

namespace fs = std::filesystem;

class Manager {
    private:
        fs::path workspace_path;
        fs::path copy_path;

    public:
        // constructor
        Manager(fs::path _workspace_path): workspace_path(_workspace_path) {};
        Manager(): Manager(fs::current_path()) {};

        // workspace getter/setter
        fs::path get_workspace_path(void);
        void set_workspace_path(fs::path workspace_path);
        
        // file or directory manipulation functions
        int remove(fs::path target_path);
        int rename(fs::path old_path, fs::path new_path);
        int create(fs::path new_path);
        int copy(fs::path target_path);
        int paste(fs::path destination_path);

        // search the current workspace
        fs::path search(fs::path search_directory);

        // navigation to different directories
        int nav_back(void);
        int nav_to(fs::path target_path);
};

#endif // MANAGER_H

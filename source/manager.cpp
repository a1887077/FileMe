#include <fileme/manager.h>

void Manager::buildList(void) {
    auto dir_iter = fs::directory_iterator(this->workspace_path);

    for (auto dir_item : dir_iter) {
        fs::path item_path = dir_item.path();
        std::string item_name = item_path.filename();
        DirEntryType item_type = dir_item.is_directory() ? DIRECTORY_ENTRY : FILE_ENTRY;
        this->entry_list.emplace_back(item_path, item_name, item_type);
    }
}

Manager::Manager(fs::path _workspace_path) {
    this->workspace_path = _workspace_path;

    this->buildList();
}

std::vector<DirEntry> Manager::getEntries(void) {
    return this->entry_list;
}

int Manager::remove(DirEntry entry) {
    int ret = 0;
    ret = FileOperator::remove(entry.entry_path);

    this->buildList();

    return ret;
}

int Manager::create(std::string name, DirEntryType type) {
    fs::path new_item_path = this->workspace_path / name;

    int ret = 0;
    ret = FileOperator::create(new_item_path);

    this->entry_list.emplace_back(new_item_path, name, type);

    return ret;
}

int Manager::rename(DirEntry& entry, std::string new_name) {
    fs::path old_path = entry.entry_path;
    fs::path new_path = entry.entry_path.replace_filename(new_name);
    
    entry.entry_name = new_name;

    int ret = 0;
    ret = FileOperator::rename(old_path, new_path);

    entry.entry_path = new_path;

    return ret;
}

int Manager::copy(DirEntry entry) {
    return FileOperator::copy(entry.entry_path);
}

void searchInFile(const char* filePath, const char* searchString, char* result) {
    FILE* file = fopen(filePath, "r");
    if (file != NULL) {
        char buffer[256];
        while (fgets(buffer, sizeof(buffer), file) != NULL) {
            if (strstr(buffer, searchString) != NULL) {
                LOG_INFO("String found in file - %s", filePath);
                strcat(result, filePath);
                strcat(result, "\n");
                break;
            }
        }
        fclose(file);
    } else {
        LOG_FATAL("Unable to open %s", "file");
    }
}

void searchInDirectory(const char* dirPath, const char* searchString, char* result) {
    DIR* directory = opendir(dirPath);
    if (directory == NULL) {
        LOG_FATAL("Unable to open %s", "directory");
        return;
    }
    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {
        if (entry->d_name[0] != '.' && strcmp(entry->d_name, ".") != 0 && strcmp(entry->d_name, "..") != 0) {
            char fullPath[PATH_MAX];
            snprintf(fullPath, sizeof(fullPath), "%s/%s", dirPath, entry->d_name);
            if (entry->d_type == DT_DIR) {
                if (strcmp(entry->d_name, "/") == 0 || strcmp(entry->d_name, "/proc") == 0 ||
                    strcmp(entry->d_name, "/sys") == 0 || strcmp(entry->d_name, "/dev") == 0)
                    continue;
                searchInDirectory(fullPath, searchString, result);
            } else if (entry->d_type == DT_REG) {
                searchInFile(fullPath, searchString, result);
            }
        }
    }
    closedir(directory);
}

void searchForString(const char* searchString, char* result) {
    LOG_INFO("Searching for the string '%s' in the filesystem", searchString);
    searchInDirectory("/home2/user29/", searchString, result);
}

#ifndef NAMESPACE_DIGITAL_FILE_CONTENT_UTIL_HPP
#define NAMESPACE_DIGITAL_FILE_CONTENT_UTIL_HPP

#include <iostream>
#include <fstream>
#include <iterator>
#include <filesystem>

namespace namespacedigital {
    namespace cppserver {
        namespace util {

            namespace fs = std::filesystem;


            enum FileType {
                REGULAR_FILE,
                DIRECTORY,
                CHARACTER_FILE,
                FIFO,
                SYMLINK,
                BLOCK_DEVICE,
                SOCKET,
                NOT_FOUND
            };

            inline FileType file_type(const fs::path& path, fs::file_status status) {

                if (fs::is_regular_file(status))
                    return REGULAR_FILE;
                if (fs::is_directory(status))
                    return DIRECTORY;
                if (fs::is_block_file(status))
                    return BLOCK_DEVICE;
                if (fs::is_character_file(status))
                    return CHARACTER_FILE;
                if (fs::is_fifo(status))
                    return FIFO;
                if (fs::is_socket(status))
                    return SOCKET;
                if (fs::is_symlink(status))
                    return SYMLINK;
                if (!fs::exists(status))
                    return NOT_FOUND;
            }


            inline std::string get_html_directory_content(const fs::path& path, std::string& target) {
                std::string htmlBody = R"(<!DOCTYPE html>
                                <html>
                                <head>
                                <meta http-equiv="Content-Type" content="text/html; charset=UTF-8" />
                                  <title>Home</title>
                                </head>
                                <body>
                                <ul>)";

                for (const auto& file : fs::directory_iterator(path)) {
                    char fileLink[1000];
                    std::string fileName(file.path().filename());
                    std::string fullUrl = target + fileName;
                    std::string fileUrl = path.c_str() + fileName;
                    FileType fileType = file_type(fileUrl, fs::status(fileUrl));
                    if (fileType == FileType::REGULAR_FILE) {
                        std::sprintf(fileLink, "<li><a href='%s'>%s</a></li>\n", fullUrl.data(), fileName.data());
                    } else {
                        std::sprintf(fileLink, "<li><a href='%s/'>%s</a></li>\n", fullUrl.data(), fileName.data());
                    }
                    htmlBody.append(fileLink);
                }

                htmlBody.append(R"(</ul> </body>
                </html>
                )");
                return htmlBody;
            }

            inline std::vector<std::string> getFilesFromFolder(const fs::path& path, std::vector<std::string>& files) {
                for (const auto& file : fs::directory_iterator(path)) {
                    std::string fileName(file.path().filename());
                    FileType fileType = file_type(file.path(), fs::status(file.path()));
                    if (fileType == FileType::REGULAR_FILE && fileName.rfind("._", 0) != 0 && fileName.rfind(".", 0) != 0) {
                        files.emplace_back(file.path());
                    } else if (fileType == FileType::DIRECTORY) {
                        getFilesFromFolder(file.path(), files);
                    }
                }
                return files;
            }


            inline void writeToFile(const fs::path& path, std::string& content) {
                std::ofstream myfile;
                myfile.open(path);
                myfile << content;
                myfile.close();
            }
        }
    }
}
#endif
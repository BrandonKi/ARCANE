#include "ErrorHandler.h"

ErrorHandler error_log;

ErrorHandler::ErrorHandler():
    buffer()
{
    buffer.reserve(200);
}

ErrorHandler::~ErrorHandler() {
    PROFILE();
    flush();
}

void ErrorHandler::exit(ErrorMessage error) {
    push(std::move(error));
    std::exit(EXIT_FAILURE);
}

void ErrorHandler::push(ErrorMessage error) {
    PROFILE();
    buffer += make_preamble(error);

    switch(error.severity){
        case FATAL:
            buffer += fmt("ERROR ", RED); 
            break;
        case WARN:
            buffer += fmt("WARNING ", YELLOW); 
            break;
        case NOTE:
            buffer += fmt("NOTE ", BLUE); 
            break;
        case MESSAGE:
            buffer += fmt("MESSAGE ", GREEN); 
            break;
    }

    buffer += error.message;
    buffer += '\n';
    buffer += create_graphic(error);
}

std::string ErrorHandler::make_preamble(ErrorMessage& error){
    PROFILE();
    if(error.src_pos.src_line == 0)
        return fmt(error.filename, BLUE) + " : ";
    return fmt(error.filename + '[' + std::to_string(error.src_pos.src_line) + ", " + std::to_string(error.src_pos.src_char) + "]", BLUE) + " : ";
}

std::string ErrorHandler::create_graphic(ErrorMessage& error) {

    // invalid position
    if(error.src_pos.src_line == 0)
        return "";

    std::string result;

    const auto filepath = args.path + "\\\\" + error.filename;
    const auto src = read_file_by_line(filepath);

    const auto line_num = error.src_pos.src_line;
    const auto line_pos = error.src_pos.src_char;
    const auto length = error.src_pos.end_pos - error.src_pos.start_pos;

    auto line_num_string = std::to_string(line_num);
    result.append(4 - line_num_string.size(), ' ');
    result.append(line_num_string);
    result.append(1, '|');
    if (src[line_num - 1].size() > 50)
        result += src[line_num - 1].substr(0, 47) + fmt("...", BLUE);
    else
        result += src[line_num - 1];
    result += '\n';

    result.append("    |");
    result.append(line_pos - 1, ' ');
    std::string temp = "^";
    temp.append(length, '~');

    switch(error.severity) {
        case FATAL:
            temp = fmt(temp, RED);
            break;
        case WARN:
            temp = fmt(temp, YELLOW);
            break;
        case NOTE:
            temp = fmt(temp, BLUE);
            break;
        case MESSAGE:
            temp = fmt(temp, GREEN);
            break;
    }
    result.append(temp);
    return result + '\n';
}

void ErrorHandler::flush() {
    PROFILE();
    if(!buffer.empty())
        println(buffer);
    buffer.clear();
}

[[nodiscard]] std::vector<std::string, arena_allocator<std::string>> ErrorHandler::read_file_by_line(const std::string& filepath) {
    PROFILE();
    std::ifstream file;
    file.open(filepath);
    std::vector<std::string, arena_allocator<std::string>> result;
    std::string line;
    while (std::getline(file, line)) {
        result.push_back(line);
    }
    file.close();
    return result;
}
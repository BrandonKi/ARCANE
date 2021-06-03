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

astring ErrorHandler::make_preamble(ErrorMessage& error){
    PROFILE();
    if(error.src_pos.src_line == 0)
        return strtoastr(fmt(astrtostr(error.filename), BLUE)) + " : ";
    return strtoastr(fmt(astrtostr(error.filename + '[' + to_astring(error.src_pos.src_line) + ", " + to_astring(error.src_pos.src_char) + "]"), BLUE)) + " : ";
}

astring ErrorHandler::create_graphic(ErrorMessage& error) {

    // invalid position
    if(error.src_pos.src_line == 0)
        return "";

    astring result;

    const auto filepath = args.path + "\\\\" + error.filename;
    const auto src = read_file_by_line(filepath);

    const auto line_num = error.src_pos.src_line;
    const auto line_pos = error.src_pos.src_char;
    const auto length = error.src_pos.end_pos - error.src_pos.start_pos;

    auto line_num_string = std::to_string(line_num);
    result.append(4 - line_num_string.size(), ' ');
    result.append(strtoastr(line_num_string));
    result.append(1, '|');
    if (src[line_num - 1].size() > 50)
        result += src[line_num - 1].substr(0, 47) + strtoastr(fmt("...", BLUE));
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
    result.append(strtoastr(temp));
    return result;
}

void ErrorHandler::flush() {
    PROFILE();
    if(!buffer.empty())
        println(astrtostr(buffer));
    buffer.clear();
}

[[nodiscard]] std::vector<astring, arena_allocator<astring>> ErrorHandler::read_file_by_line(const astring& filepath) {
    PROFILE();
    std::ifstream file;
    file.open(filepath);
    std::vector<astring, arena_allocator<astring>> result;
    std::string line;
    while (std::getline(file, line)) {
        result.push_back(strtoastr(line));
    }
    file.close();
    return result;
}
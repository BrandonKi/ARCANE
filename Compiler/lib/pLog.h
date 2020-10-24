/**
 * @file plog.h
 * @author Brandon Kirincich
 * @brief single header library for printing text to the 
 * console with the specified format (color, underline, bold, etc.)
 * 
 * NOT ALL FEATURES ARE SUPPORTED BY ALL TERMINALS
 * 
 * @example 
 * 
 * // Call init_pLog before using any functions. 
 * // This is only needed on Windows, but should be called just in case.
 * init_pLog();
 * 
 * println("test", UNDERLINE, BOLD, RED);
 * 
 * println(fmt("I'm blue", BLUE) + " " + fmt("I'm red and bold", RED, BOLD));
 * 
 * println("r,g,b values are also supported! They can be bold, underlined, etc.", rgb(255, 100, 0), BOLD);
 * 
 * println("to set background color pass a color to the bg() function.", bg(RED));
 * 
 * print("rgb can also be passed to bg", bg(rgb(255, 100, 0)));
 * 
 * print("this also work", bg(255, 100, 0));
 * 
 * @version 0.1
 * @date 2020-10-16
 * 
 * MIT License
 *
 * @copyright (c) 2020 Brandon Kirincich
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 * 
 */

#ifndef _PLOG_INCLUDE_GUARD_
#define _PLOG_INCLUDE_GUARD_
#include <string>
#include <iostream>
#include <cstdint>

#ifdef _WIN32
    #include <windows.h>
    #ifndef ENABLE_VIRTUAL_TERMINAL_PROCESSING
        #define ENABLE_VIRTUAL_TERMINAL_PROCESSING 0x0004
    #endif
#endif


namespace pLog{

    static const std::string _pLog_preamble_ = "\033[";

    // Some macros share values with others
    // this is to give them more common names for ex. "gray/grey" instead of "bright black"

    #ifdef PREFIX_MACROS
    #define PL_CLEAR            "\033[0m"
    #define PL_BOLD             "1;"
    #define PL_FAINT            "2;"
    #define PL_ITALICIZE        "3;"
    #define PL_UNDERLINE        "4;"
    #define PL_SLOW_BLINK       "5;"
    #define PL_FAST_BLINK       "6;"
    #define PL_REVERSE          "7;"
    #define PL_CONCEAL          "8;"
    #define PL_STRIKETHROUGH    "9;"
    #define PL_DEFAULT_FONT     "10;"
    #define PL_ALT_FONT_1       "11;"
    #define PL_ALT_FONT_2       "12;"
    #define PL_ALT_FONT_3       "13;"
    #define PL_ALT_FONT_4       "14;"
    #define PL_ALT_FONT_5       "15;"
    #define PL_ALT_FONT_6       "16;"
    #define PL_ALT_FONT_7       "17;"
    #define PL_ALT_FONT_8       "18;"
    #define PL_ALT_FONT_9       "19;"
    #define PL_FRAKTUR          "20;"

    #define PL_BLACK            "30;"
    #define PL_RED              "31;"
    #define PL_GREEN            "32;"
    #define PL_YELLOW           "33;"
    #define PL_BLUE             "34;"
    #define PL_PINK             "35;"
    #define PL_MAGENTA          "35;"
    #define PL_CYAN             "36;"
    #define PL_WHITE            "37"

    #define PL_FRAME            "51;"
    #define PL_ENCIRCLE         "52;"
    #define PL_OVERLINE         "53;"

    #define PL_BRIGHT_BLACK     "90;"
    #define PL_GRAY             "90;"
    #define PL_GREY             "90;"
    #define PL_BRIGHT_RED       "91;"
    #define PL_BRIGHT_GREEN     "92;"
    #define PL_BRIGHT_YELLOW    "93;"
    #define PL_BRIGHT_BLUE      "94;"
    #define PL_BRIGHT_PINK      "95;"
    #define PL_BRIGHT_MAGENTA   "95;"
    #define PL_BRIGHT_CYAN      "96;"
    #define PL_BRIGHT_WHITE     "97;"
    #else
    #define CLEAR               "\033[0m"
    #define BOLD                "1;"
    #define FAINT               "2;"
    #define ITALICIZE           "3;"
    #define UNDERLINE           "4;"
    #define SLOW_BLINK          "5;"
    #define FAST_BLINK          "6;"
    #define REVERSE             "7;"
    #define CONCEAL             "8;"
    #define STRIKETHROUGH       "9;"
    #define DEFAULT_FONT        "10;"
    #define ALT_FONT_1          "11;"
    #define ALT_FONT_2          "12;"
    #define ALT_FONT_3          "13;"
    #define ALT_FONT_4          "14;"
    #define ALT_FONT_5          "15;"
    #define ALT_FONT_6          "16;"
    #define ALT_FONT_7          "17;"
    #define ALT_FONT_8          "18;"
    #define ALT_FONT_9          "19;"
    #define FRAKTUR             "20;"

    #define BLACK               "30;"
    #define RED                 "31;"
    #define GREEN               "32;"
    #define YELLOW              "33;"
    #define BLUE                "34;"
    #define PINK                "35;"
    #define MAGENTA             "35;"
    #define CYAN                "36;"
    #define WHITE               "37"

    #define FRAME               "51;"
    #define ENCIRCLE            "52;"
    #define OVERLINE            "53;"

    #define BRIGHT_BLACK        "90;"
    #define GRAY                "90;"
    #define GREY                "90;"
    #define BRIGHT_RED          "91;"
    #define BRIGHT_GREEN        "92;"
    #define BRIGHT_YELLOW       "93;"
    #define BRIGHT_BLUE         "94;"
    #define BRIGHT_PINK         "95;"
    #define BRIGHT_MAGENTA      "95;"
    #define BRIGHT_CYAN         "96;"
    #define BRIGHT_WHITE        "97;"
    #endif

    /**
     * @brief Initializes pLog. This is mostly for windows platforms but should be called anyway.
     * 
     * @return a bool representing whether or not pLog could be initialized 
     */
    inline bool init_pLog(){
        #ifdef _WIN32
            HANDLE hOut = GetStdHandle(STD_OUTPUT_HANDLE);
            if (hOut == INVALID_HANDLE_VALUE)
                return false;
            DWORD dwMode = 0;
            if (!GetConsoleMode(hOut, &dwMode))
                return false;
            dwMode |= ENABLE_VIRTUAL_TERMINAL_PROCESSING;
            if (!SetConsoleMode(hOut, dwMode))
                return false;
            return true;
        #endif
        return true;
    }

    /**
     * @brief return a string representing rgb value
     * 
     * @param r value for red (0 - 255)
     * @param g value for green (0 - 255)
     * @param b value for blue (0 - 255)
     * @return formatted string representing rgb value
     */
    inline const std::string rgb(uint8_t r, uint8_t g, uint8_t b){
        return std::move("38;2;" + std::to_string(+r) + ';' + std::to_string(+g) + ';' + std::to_string(+b) + ';');
    }

    /**
     * @brief return string representing a color to set background to
     * 
     * @param color color to set background to can 
     * be a predefined color macro or rgb() string
     * @return formatted string representing background color
     */
    inline std::string bg(const std::string& color){
        if(color.length() > 3)
            return std::move(std::string("48;2;" + color.substr(5)));   // if color is in rgb() format
        return std::move(std::to_string(std::stoi(color) + 10) + ';');
    }

    /**
     * @brief return string representing a color to set background to from r, g, b values
     * 
     * @param r value for red (0 - 255)
     * @param g value for green (0 - 255)
     * @param b value for blue (0 - 255)
     * @return formatted string representing background color
     */
    inline std::string bg(uint8_t r, uint8_t g, uint8_t b){
        return std::string("48;2;" + std::to_string(+r) + ';' + std::to_string(+g) + ';' + std::to_string(+b) + ';');
    }

    /**
     * @brief base case function for unpacking var args
     * 
     * @return an empty string
     */
    inline std::string fstring(){ 
        return std::move(std::string(""));
    }

    /**
     * @brief helper function for unpacking var args
     * this function takes all var args which are assumed to be valid ansii args 
     * and combines them into a single string 
     * 
     * @param var1 first arg in var args
     * @param var2 var args
     * @return all args combined into a single string 
     */
    template <typename T, typename... Types> 
    inline std::string fstring(T var1, Types... var2){     
        return std::move(std::string(var1) + fstring(var2...)); 
    }

    /**
     * @brief function called to correctly format a string with all args given
     * for ex. fmt("test", UNDERLINE, RED, ...)
     * 
     * @param str base string to add ansii escape args onto
     * @param var2 any amount of ansii escape args
     * @return base string with ansii escape args added onto it
     */
    template <typename T, typename... Types> 
    inline std::string fmt(T str, Types... var2){
        std::string&& color = fstring(var2...);
        return std::move(_pLog_preamble_ + color.substr(0,color.length()-1) + 'm' + str + CLEAR);
    }
    

    /**
     * @brief print string to the console
     * 
     * @param str string to print
     */
    inline void print(const std::string& str){
        std::cout << str;
    }

    /**
     * @overload
     * 
     * @param str string to print
     */
    inline void print(const std::string&& str = ""){
        std::cout << str;
    }


    /**
     * @brief print string to the console with specified format as a string
     * 
     * @param str string to print
     * @param fmt format to use
     */
    inline void print(const std::string& str, const std::string& fmt){
        if(fmt == "")
            std::cout << str;
        else{
            std::cout << _pLog_preamble_ << fmt.substr(0, fmt.length()-1) << 'm' << str << CLEAR;
        }
    }

    /**
     * @overload
     * 
     * @param str string to print
     * @param fmt format to use
     */
    inline void print(const std::string& str, const std::string&& fmt){
        if(fmt == "")
            std::cout << str;
        else{
            std::cout << _pLog_preamble_ << fmt.substr(0, fmt.length()-1) << 'm' << str << CLEAR;
        }
    }

    /**
     * @overload
     * 
     * @param str string to print
     * @param fmt format to use
     */
    inline void print(const std::string&& str, const std::string& fmt){
        if(fmt == "")
            std::cout << str;
        else{
            std::cout << _pLog_preamble_ << fmt.substr(0, fmt.length()-1) << 'm' << str << CLEAR;
        }
    }

    /**
     * @overload
     * 
     * @param str string to print
     * @param fmt format to use
     */
    inline void print(const std::string&& str, const std::string&& fmt){
        if(fmt == "")
            std::cout << str;
        else{
            std::cout << _pLog_preamble_ << fmt.substr(0, fmt.length()-1) << 'm' << str << CLEAR;
        }
    }

    /**
     * @brief wrapper of print() that unpacks var args and formats them automatically
     * 
     * @param var1 base string
     * @param var2 list of ansii escape args
     */
    template <typename T, typename... Types> 
    void print(T var1, Types... var2){
        print((std::string)var1, (std::string)fstring(var2...));
    }

    /**
     * @brief print a string to the console with a newline at the end
     * 
     * @param str string to print
     */
    inline void println(const std::string& str = ""){
        std::cout << str << '\n';
    }

    /**
     * @overload
     * 
     * @param str string to print
     */
    inline void println(const std::string&& str){
        std::cout << str << '\n';
    }

    /**
     * @brief print a string with the specified format 
     * to the console with a newline at the end
     * 
     * @param str string to print
     */
    inline void println(const std::string& str, const std::string& fmt){
        if(fmt == "")
            std::cout << str;
        else{
            std::cout << _pLog_preamble_ << fmt.substr(0, fmt.length()-1) << 'm' << str << CLEAR;
        }
    }

    /**
     * @overload
     * 
     * 
     * @param str string to print
     */
    inline void println(const std::string& str, const std::string&& fmt){
        if(fmt == "")
            std::cout << str;
        else{
            std::cout << _pLog_preamble_ << fmt.substr(0, fmt.length()-1) << 'm' << str << CLEAR;
        }
    }

    /**
     * @overload
     * 
     * @param str string to print
     */
    inline void println(const std::string&& str, const std::string& fmt){
        if(fmt == "")
            std::cout << str << '\n';
        else{
            std::cout << _pLog_preamble_ << fmt.substr(0, fmt.length()-1) << 'm' << str << CLEAR << '\n';
        }
    }

    /**
     * @overload
     * 
     * @param str string to print
     */
    inline void println(const std::string&& str, const std::string&& fmt){
        if(fmt == "")
            std::cout << str << '\n';
        else{
            std::cout << _pLog_preamble_ << fmt.substr(0, fmt.length()-1) << 'm' << str << CLEAR << '\n';
        }
    }

    /**
     * @brief wrapper of println() that unpacks var args and formats them automatically
     * 
     * @param var1 base string
     * @param var2 list of ansii escape args
     */
    template <typename T, typename... Types> 
    void println(T var1, Types... var2){ 
        println((std::string)var1, (std::string)fstring(var2...));
    }   
}
#endif
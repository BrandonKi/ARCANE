/**
 * @file small_profiler.h
 * @author Brandon Kirincich
 * @brief small inline profiler that generates a json usable with chrome://tracing
 * @version 0.1
 * @date 2021-03-28
 * 
 * @copyright
 * MIT License
 *
 * Copyright (c) 2021 Brandon Kirincich
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
 *   SOFTWARE.
 */
#ifndef SMALL_PROFILER_H
#define SMALL_PROFILER_H

#include <iostream>
#include <fstream>
#include <thread>
#include <chrono>
#include <string>
#include <sstream>

#if defined(__linux__)
#include <sys/types.h>
#include <unistd.h>
#elif defined(_WIN32)
#include <windows.h>
#endif

#ifndef PROFILE_OUTPUT_FILE
#define PROFILE_OUTPUT_FILE "profile.json"
#endif

#ifdef NO_PROFILE
#define PROFILE()
#else
#define PROFILE() small_profiler::internal_scoped_profiler _small_profiler_temp_{__FUNCTION__}
namespace small_profiler {

    inline unsigned long long get_pid() {
        #if defined(__linux__)
        return getpid();
        #elif defined(_WIN32)
        return GetCurrentProcessId();
        #endif
        return -1;
    }

    class internal_stream_wrapper {
        public:
            std::stringstream stream{PROFILE_OUTPUT_FILE};        

            internal_stream_wrapper() {
                stream << "{ \"traceEvents\": [";
            }
            ~internal_stream_wrapper() {
                stream.seekp(-1, std::ios_base::end);
                stream << "]}";
                std::ofstream out{PROFILE_OUTPUT_FILE};
                out << stream.str();
            }
    };

    inline internal_stream_wrapper file;
    inline auto program_start = std::chrono::high_resolution_clock::now();
    inline unsigned long long tid_counter = 0;

    class internal_scoped_profiler {
        public:
            internal_scoped_profiler(std::string name_p):
                begin(std::chrono::high_resolution_clock::now()), name(name_p)
            {
                
            }

            ~internal_scoped_profiler() {
                auto end = std::chrono::high_resolution_clock::now();

                auto pid = small_profiler::get_pid();
                auto tid = tid_counter++;
                auto ts = std::chrono::duration_cast<std::chrono::microseconds>(begin - program_start).count();
                auto dur = std::chrono::duration_cast<std::chrono::microseconds>(end - begin).count();
                auto ph = "X";
                auto args = std::chrono::duration_cast<std::chrono::milliseconds>(end - begin).count();

                std::string line = std::string(
                    "{ \"pid\":") + std::to_string(pid) + std::string(", \"tid\":") + std::to_string(tid) +
                    std::string(", \"ts\":") + std::to_string(ts) + std::string(", \"dur\":") + std::to_string(dur) +
                    std::string(", \"ph\":\"") + std::string(ph) + std::string("\", \"name\":\"") + name +
                    std::string("\", \"args\":{ \"ms\":") + std::to_string(args) + std::string("} },"
                );
                file.stream << line;
            }
        
        private:
            std::string name;
            std::chrono::time_point<std::chrono::high_resolution_clock> begin;
    };

};
#endif
#endif
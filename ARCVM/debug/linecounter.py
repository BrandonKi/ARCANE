import os
import sys
import re

file_list = []

def main():
    result = ""
    traverse(sys.argv[1])
    for file in file_list:
        try:
            current = open(file, "r")
            result += current.read()
        except UnicodeDecodeError:
            pass
    print(len(re.findall("\n", result)))

def traverse(base_path):
    for x in os.listdir(base_path):
        path = "{}\\{}".format(base_path, x)
        if os.path.isfile(path):
            file_list.append(path)
        elif os.path.isdir(path) and x[0] != '.':
            traverse(path)

def organizeFilesByType():
    filedict = {}
    for file in file_list:
        ext = file[file.rindex('.'):]
        if ext in filedict: 
            filedict.get(ext).append(file)
        else:
            filedict[ext] = [ext]

if __name__ == "__main__":
    main()
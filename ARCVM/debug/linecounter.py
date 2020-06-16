import os
import sys
import re

file_list = []

def main():
    traverse(sys.argv[1])
    files = organizeFilesByType()
    result = ""
    total = 0
    file_counter = 0
    total_files = 0
    print("Extension     Files\t      Lines")
    for ext, ext_list in files.items():
        print("+---------------------------------------+")
        print("|" + ext, end = '\t')
        for file in ext_list:
            try:
                current = open(file, "r")
                result += current.read()
                file_counter += 1
                total_files += 1
            except UnicodeDecodeError:
                pass
        files[ext] = [result, file_counter]
        result = ""
        file_counter = 0
        total += len(re.findall("\n", files[ext][0]))
        print('  |\t{}\t|\t{}\t|'.format(files[ext][1], len(re.findall("\n", files[ext][0])) + files[ext][1]))
    print("+---------------------------------------+")
    print('|Total\t  |\t{}\t|\t{}\t|'.format(total_files, total))
    print("+---------------------------------------+")

def traverse(base_path):
    for x in os.listdir(base_path):
        path = "{}\\{}".format(base_path, x)
        if os.path.isfile(path) and x[0] != '.':
            file_list.append(path)
        elif os.path.isdir(path) and x[0] != '.':
            traverse(path)

def organizeFilesByType():
    filedict = {}
    for file in file_list:
        if '.' in file:
            ext = file[file.rindex('.'):]
            if ext in filedict: 
                filedict.get(ext).append(file)
            else:
                filedict[ext] = [file]
    return filedict

if __name__ == "__main__":
    main()
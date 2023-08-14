from ctypes import cdll

spam = cdll.LoadLibrary("/home/mads/projects/C++/Learning/src/misc/libspam.so")

test = spam.system("ls -l")
print(test)

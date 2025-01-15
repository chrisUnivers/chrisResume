How to use:
    **NOTE: Can only be ran on linux systems. I tested it on Ubuntu 22.04.02 LTS**

# FAT stands for: File Allocation Table

# How to run: 
    In the terminal write: make
# Commands:
   * I will use the file "testNew.IMA" as an example.
    1) ./diskinfo testNew.IMA
    2) ./disklist testNew.IMA
    3) ./diskinfo testNew.IMA INPUT1.TXT

# Additional infomation:  
    1) I will provide two image files: testNew.IMA and disk.ima. These are the files that were provide to use. 
    2) More image files can be created using the program/software winImage which can be downloaded from the web.
    3) The file p3 outlines the assingment requirements with more details.

# Details of the commands:
1) **diskinfo**: Displays information about the file system. The informations are:
    OS Name:
    Label of the disk:
    Total size of the disk:
    Free size of the disk:
    The number of files in the disk including files in all subdirectories: 
    Number of FAT copies:
    Sectors per FAT:

2) **disklist**: Displays content of all directories on the file system.
    Directory name
    F (file size) (file name) (file creation date)
    or 
    D (directory size) (directory name) (directory creation date)

3) **diskget**: Copies a file from the root directory of the file system to the current directory in Linux. My implemention finds a file from an directory and not just the root directory.


# Command lf

`lf` (List Files) is a directory listing command for linux distro, it displays directories and files as a tree. It lists information about the files such as permissions, size, modifications, etc.

`lf` is an alternative to the `ls` command, but has a different perspective. It is much simpler to use thanks to its few options that make it very easy to handle.


## Features

- Display in a tree-like format
- Display by columns
- List by space, comma or semicolon
- Colored output
- Select the files to be displayed
- Select the information to be displayed
- Sorting using different criteria

  
## Usage

```plaintext
Usage:
     lf -[OPTION]=[ARGUMENT],... [FILE]...

Options:
    it's possible to combine those lettre together

    -0  separate files with a space.
    -1  lists one file per line.
    -2  separate files with commas.
    -3  separate files with a semicolon.
    -a  show hidden files.
    -[...]m=[bcdflrsugtrwx],[...]
          File's mode, choose the files to list from the list below:
            b  block device
            c  character device
            d  directory
            p  FIFO/pipe
            l  symlink
            f  regular file
            s  socket
            u  set-user identification (SUID)
            g  set-group identification (SGID)
            t  sticky bit
            r  read by owner
            w  write by owner
            x  execute by owner
          By default, "-m" displays all files and folders except the hidden ones.
          wich has the same effect as not using the "-m" option.
    -[...]l=[inpsugamc],[...]
          File's information, choose the information to display from the list below:
            i  inode number
            n  number of hard links
            p  permissions
            s  size
            u  file owner
            g  file group
            a  last access
            m  last modification
            c  last status change
          By default, "-l" show i,n,p,s,m if no argument is set
    -[...]s=[insugamcte],[...]
          Sort the output
            i  inode number
            n  number of hard links
            s  size
            u  file owner
            g  file group
            a  last access
            m  last modification
            c  last status change
            t  file type
            e  file extension
          By default, lf will sort the output by name,
          But if you invoke "-s" without giving any arguments, you disable sorting of the output.
    -[...]n=[bfqs],[...]
            b  adds a backslash to the string containing spaces
            f  follow link
            q  display the name in quotes
            s  display the folder name with a slash
    -[...]t=[DEPTH]
             DEPTH  tree depth
          By default (without DEPTH), the depth is unlimited.
    -r  make the size readable like 7K, 423M, 2G etc.
    -c  color the output.
    -h  print help.
    -v  print version information.

```

  
## Build

For the moment, there's no installation file yet, but it will be available soon. 

To build and run the project, follow these steps:
```bash
git clone git@github.com:thedevelopr/Command-lf.git
cd Command-lf
make all
./lf

```
    
## Environment Variables

To color the output, you will need the `LS_COLORS` environment variable which is the same one used by `ls`.


  
## Documentation

[Documentation](https://ayoub-dahdouh-etu.pedaweb.univ-amu.fr/extranet/applications/Command-lf/index.php)

  
## Feedback

If you have any comments, please do not hesitate to share them with us via [this forum](https://ayoub-dahdouh-etu.pedaweb.univ-amu.fr/extranet/contact.php).


## Authors

- [@thedevelopr](https://github.com/thedevelopr)

  
## License

The source code is available on [MIT](https://choosealicense.com/licenses/mit/) licence.

  
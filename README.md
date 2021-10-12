
# Command ds

`ds` (List Files) is a directory listing command for linux distro, it displays directories and files as a tree. It lists information about the files such as permissions, size, modifications, etc.

`ds` is an alternative to the `ls` command, but has a different perspective. It is much simpler to use thanks to its few options that make it very easy to handle.


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
Usage:  ds [OPTION]:[PARAMETERS]... [FILE]...

    Display information about the FILEs (the current directory by default).
    without parameters, 'ds' displays all contents except the hidden ones.
    in form of columns, also, the output is not sorted or colored.

    'ds' organise FILEs into FILE'name (-n), FILE's mode(-m) wich represent
    FILE's permissions and types, and FILE's information (-l),

    it's possible to combine those options together

    -1  separate with a newline(\n)

    -2  separate with a space(SPACE)
    
    -3  separate with a commas(,)
    
    -4  separate with a semicolon(;)

    -c  count the number of contents

    -d  don't follow if FILE is directories or dirlinks.

    -[...]m:[hbcdplrsugt123456789]   Mode parameters (file's mode)
          Without parameters, displays everything, the same effect as not using it.

    -[...]t:[DEPTH]   Tree parameters

    -[...]l:[inpsrugamc]   Information parameters (long format)
          i,n,p,r,u,g,m are specified if no argument is given

    -[...]n:[cbfqi]   Name parameters
          c,f,q are specified if no argument is given

    -[...]s:[dinsugamcte]   Sort parameters
           sort by names if no argument is given,

    -h  print help

    -v  print version information
```

  
## Build

For the moment, there's no installation file yet, but it will be available soon. 

To build and run the project, follow these steps:
```bash
git clone git@github.com:thedevelopr/Command-ds.git
cd Command-ds
make all
./ds
```

## Environment Variables

To color the output, you will need the `LS_COLORS` environment variable which is the same one used by `ls`.


  
## Documentation

[Documentation](https://ayoub-dahdouh-etu.pedaweb.univ-amu.fr/extranet/applications/Command-ds/index.php)

  
## Feedback

If you have any comments, please do not hesitate to share them with us via [this forum](https://ayoub-dahdouh-etu.pedaweb.univ-amu.fr/extranet/contact.php).


## Authors

- [@thedevelopr](https://github.com/thedevelopr)

  
## License

The source code is available on [MIT](https://choosealicense.com/licenses/mit/) licence.

  
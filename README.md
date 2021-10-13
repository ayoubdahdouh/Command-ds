
# Command ds

`ds` (Display) is a directory listing command for linux distro, it displays directories and files as a tree. It lists information about the files such as permissions, size, modifications, etc.

`ds` is an alternative to the `ls` command, but using a different perspective. It is very easy to use thanks to its few options.


## Features

- Colored output
- Sorting using different criteria
- Select the files to be displayed
- Select the information to be displayed
- Display using tree-like format
- Display using columns, space, comma or semicolon

  
## Usage

```plaintext
Usage:  ds [OPTION]:[PARAMETERS]... [FILE]...

```
Display information about the FILE (the current directory by default).
without parameters, 'ds' displays all contents except the hidden ones.
in form of columns, also, the output is not sorted or colored.

'ds' organise FILE information into FILE'name (-n), FILE's mode(-m) wich
represent FILE's permissions and types, and FILE's information (-l),

all options can be combined together except:
- combining -l with any of -t, -1, -2, -3 and -4
- also, you cannot combine -t, -1, -2, -3 and -4 together

`-1`  separate with a newline(\n)

`-2`  separate with a space(SPACE)

`-3`  separate with a commas(,)

`-4`  separate with a semicolon(;)

`-c`  count the number of contents

`-d`  don't follow if FILE is directories or dirlinks.

`-[...]m:[hbcdplrsugt123456789]` Mode parameters (file's mode), Without parameters, displays all.

`-[...]t:[DEPTH]` Tree parameters, without parameter, the depth is unlimited

`-[...]l:[inpsrugamc]` Information parameters (long format), without parameters, i,n,p,r,u,g,m are set.

`-[...]n:[cbfqi]` Name parameters, Without parameters, c,f,q are set.

`-[...]s:[dinsugamcte]` Sort parameters, without parameters, sort by names

`-h`  print help

`-v`  print version



## Build and Run

For the moment, there's no installation package yet, but it will be available soon.

To build and run the project, follow these steps:

- Download the repository
```bash
git clone git@github.com:thedevelopr/Command-ds.git
```
- Change current working directory
```bash
cd Command-ds
```
- Compile using any c compiler (i.e `gcc`)
```bash
make all
```
- Execute
```bash
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

  
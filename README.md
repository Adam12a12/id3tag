# id3tag
A simple id3v2 command-line tool.

# Current features
1- Print ID3v2 tags.

2- Append the track number to the name of the file.

# Compiling
First compile the id3v2lib

Then link the id3v2lib while compiling

```bash
$ gcc id3tag.c -o id3tag -lid3v2lib 
```

You could specify the path of the id3v2lib in case it was installed in a place that gcc could not find

```bash
$ gcc id3tag.c -o id3tag -lid3v2lib -I /path/to/id3v2lib.h
```


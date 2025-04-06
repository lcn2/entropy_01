# entropy_01

Determine the entropy from lines of "0" and "1" chars

**NOTE**: We ignore newlines and carriage returns.  We will consider anything not a "0" char to be a "1" character.


# To install

```sh
make clobber all
sudo make install clobber
```


# To use

```sh
/usr/local/bin/entropy_01 [-h] [-V] [-m maxlen] [-i ignsize]

    -h            print help message and exit
    -V            print version string and exit

    -m maxlen     do not scan more than maxlen octets per line (def: BUFSIZ)
    -i ignsize    ignore lines smaller than ignsize octets (def: 0)

Exit codes:
    0         all OK
    1         -o offset beyond arg count
    2         -h and help string printed or -V and version string printed
    3         command line error
 >= 10        internal error

entropy_01 version: 1.2.1 2025-04-05
```


# Examples

```sh
$ sed -e 's/[a-z]/0/g' -e 's/[A-Z]/1/g' -e 's/[^01]//'g < Makefile | ./entropy_01

chi^2 distribution
lines counted: 67
lines ignored: 42
shortest processed line: 1 octets
octets processed: 67 octets

chi^2 [99.50-100.00]%:	0	  0.00%
chi^2 [99.00-99.50]%:	0	  0.00%
chi^2 [98.00-99.00]%:	0	  0.00%
chi^2 [97.50-98.00]%:	0	  0.00%
chi^2 [95.00-97.50]%:	0	  0.00%
chi^2 [90.00-95.00]%:	0	  0.00%
chi^2 [80.00-90.00]%:	0	  0.00%
chi^2 [75.00-80.00]%:	0	  0.00%
chi^2 [70.00-75.00]%:	0	  0.00%
chi^2 [50.00-70.00]%:	0	  0.00%
chi^2 [30.00-50.00]%:	0	  0.00%
chi^2 [25.00-30.00]%:	0	  0.00%
chi^2 [20.00-25.00]%:	0	  0.00%
chi^2 [10.00-20.00]%:	0	  0.00%
chi^2 [5.00-10.00]%:	0	  0.00%
chi^2 [2.50-5.00]%:	0	  0.00%
chi^2 [2.00-2.50]%:	1	100.00%
chi^2 [1.00-2.00]%:	0	  0.00%
chi^2 [0.50-1.00]%:	0	  0.00%
chi^2 [0.10-0.50]%:	0	  0.00%
chi^2 [0.05-0.10]%:	0	  0.00%
chi^2 [0.01-0.05]%:	0	  0.00%
chi^2 [0.00-0.01]%:	0	  0.00%
chi^2 [excess]:		0	  0.00%

entropy bit est: 0 bits	  2.00%
```


# Reporting Security Issues

To report a security issue, please visit "[Reporting Security Issues](https://github.com/lcn2/entropy_01/security/policy)".

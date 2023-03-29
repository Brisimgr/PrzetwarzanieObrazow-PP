# Przetwarzanie Obrazow
## Table of contents
* [General information](#general-information)
* [Technologies](#technologies)
* [Setup](#setup)
* [Additional information](#additional-information)
## General information
This project was made in Podstawy Programowania class during my 1st semsester. It is a simple editor of .ppm and .pgm files. It allows the user to apply filters and edit the files by using terminal commands.
## Technologies
The project is created with:
* gcc
## Setup
To run this project, download its contents and follow the commands listed below:
```
$ make
$ ./przetwarzanieObrazow -i [fileToEdit] -[options] -o [fileToSaveTo]
```
## List of options
* -i -> informs the programme that the next argument is the name of the in file
* -o -> informs the programme that the next argument is the name of the out file
* -p -> thresholding, the next argument must be its value
* -n -> image negative
* -k -> adding outline to the image
* -d -> displaying the image
* -s -> photo weave

The order of the options indicates which of them will be executed first.
